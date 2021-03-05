#include "simulation.h"

Chunk *new_chunk() {
  Chunk *chunkp = malloc(sizeof(Chunk));
  int i;
  for (i = 0; i < CHUNK_SIZE; i++) {
    chunkp->data[i].reaction = -1;
    chunkp->data[i].time = 0.0;
  }
  chunkp->next_free_index = 0;
  chunkp->next_chunk = NULL;
  return chunkp;
}

SimulationHistory *new_simulation_history() {
  SimulationHistory *simulation_historyp = malloc(sizeof(SimulationHistory));
  Chunk *chunkp = new_chunk();
  simulation_historyp->first_chunk = chunkp;
  simulation_historyp->last_chunk = chunkp;

  return simulation_historyp;
}

void free_simulation_history(SimulationHistory *simulation_historyp) {
  Chunk *chunkp = simulation_historyp->first_chunk;
  Chunk *next_chunkp;

  while (chunkp) {
    next_chunkp = chunkp->next_chunk;
    free(chunkp);
    chunkp = next_chunkp;
  }

  free(simulation_historyp);
}

void insert_history_element(SimulationHistory *simulation_historyp, int reaction, double time) {

  Chunk *last_chunkp = simulation_historyp->last_chunk;
  if (last_chunkp->next_free_index == CHUNK_SIZE) {
    Chunk *new_chunkp = new_chunk();
    last_chunkp->next_chunk = new_chunkp;
    simulation_historyp->last_chunk = new_chunkp;
    new_chunkp->data[0].reaction = reaction;
    new_chunkp->data[0].time = time;
    new_chunkp->next_free_index++;
  } else {
    last_chunkp->data[last_chunkp->next_free_index].reaction = reaction;
    last_chunkp->data[last_chunkp->next_free_index].time = time;
    last_chunkp->next_free_index++;
  }
}

int simulation_history_length(SimulationHistory *shp) {
  int length = 0;
  Chunk *chunk = shp->first_chunk;
  while (chunk) {
    length += chunk->next_free_index;
    chunk = chunk->next_chunk;
  }
  return length;
}


Simulation *new_simulation(ReactionNetwork *rnp,
                           unsigned long int seed,
                           SolveType type) {
  int i;


  Simulation *sp = malloc(sizeof(Simulation));
  sp->rn = rnp;
  sp->seed = seed;
  sp->state = malloc(sizeof(int) * rnp->number_of_species);

  for (i = 0; i < rnp->number_of_species; i++)
    sp->state[i] = rnp->initial_state[i];

  sp->time = 0.0;
  sp->step = 0;
  sp->solver = new_solve(type,
                         seed,
                         rnp->number_of_reactions,
                         rnp->initial_propensities);

  sp->history = new_simulation_history();
  sp->propensity_buffer = malloc(sizeof(double) * rnp->number_of_reactions);

  return sp;
}

void free_simulation(Simulation *sp) {
  // don't free the reaction network as it is shared by all simulations
  free(sp->state);
  free_solve(sp->solver);
  free_simulation_history(sp->history);
  free(sp->propensity_buffer);
  free(sp);
}

bool step(Simulation *sp) {
  int m;
  double dt;
  bool dead_end = false;
  int next_reaction = sp->solver->event(sp->solver, &dt);
  int reaction_index;

  if (next_reaction < 0) dead_end = true;
  else {
    // update steps and time
    sp->step++;
    sp->time += dt;

    // record reaction
    insert_history_element(sp->history, next_reaction, sp->time);

    // update state
    for (m = 0; m < sp->rn->number_of_reactants[next_reaction]; m++)
      sp->state[sp->rn->reactants[next_reaction][m]]--;
    for (m = 0; m < sp->rn->number_of_products[next_reaction]; m++)
      sp->state[sp->rn->products[next_reaction][m]]++;

    // update propensities
    DependentsNode *dnp = get_dependency_node(sp->rn, next_reaction);
    int number_of_updates = dnp->number_of_dependents;
    int *dependents = dnp->dependents;

    for (m = 0; m < number_of_updates; m++) {
      reaction_index = dependents[m];
      sp->propensity_buffer[reaction_index] = compute_propensity(sp->rn,
                                                                 sp->state,
                                                                 reaction_index);
    }

    sp->solver->update_many(sp->solver,
                            number_of_updates,
                            dependents,
                            sp->propensity_buffer);

  }

  return dead_end;
}

void run_until(Simulation *sp, double time_cutoff) {
  while (!step(sp)) {
    if (sp->time > time_cutoff)
      break;
  }
}

void run_for(Simulation *sp, int step_cutoff) {
  while (!step(sp)) {
    if (sp->step > step_cutoff)
      break;
  }
}

void simulation_history_to_file(Simulation *sp) {
  char *base_directory = sp->rn->dir;
  char *folder = "simulation_histories";
  char path[2048];
  char *end = stpcpy(path, base_directory);
  sprintf(end,"/");
  end += 1;
  end = stpcpy(end, folder);

  DIR* dir = opendir(path);
  if (dir) {
      /* Directory exists. */
      closedir(dir);
  } else {
    mkdir(path,0777);
  }

  char reaction_path[2048];
  char time_path[2048];
  char* reaction_end = stpcpy(reaction_path, path);
  char* time_end = stpcpy(time_path, path);

  sprintf(reaction_end, "/reactions_%ld", sp->seed);
  sprintf(time_end, "/times_%ld", sp->seed);
  FILE *reaction_file = fopen(reaction_path,"w");
  FILE *time_file = fopen(time_path,"w");
  int i;
  Chunk *chunk = sp->history->first_chunk;
  while (chunk) {
    for (i = 0; i < chunk->next_free_index; i++) {
      fprintf(reaction_file, "%d\n", chunk->data[i].reaction);
      fprintf(time_file, "%1.13e\n", chunk->data[i].time);
    }
    chunk = chunk->next_chunk;
  }
  fclose(reaction_file);
  fclose(time_file);
}
