#include "reaction_network.h"
#include "sql_programs.h"



char *reaction_network_db_postix = "/rn.sqlite";
char *number_of_species_postfix = "/number_of_species";
char *number_of_reactions_postfix = "/number_of_reactions";
char *number_of_reactants_postfix = "/number_of_reactants";
char *reactants_postfix = "/reactants";
char *number_of_products_postfix = "/number_of_products";
char *products_postfix = "/products";
char *factor_zero_postfix = "/factor_zero";
char *factor_two_postfix = "/factor_two";
char *factor_duplicate_postfix = "/factor_duplicate";
char *rates_postfix = "/rates";
char *initial_state_postfix = "/initial_state";

void initialize_dependents_node(DependentsNode *dnp) {
  dnp->number_of_dependents = -1;
  dnp->dependents = NULL;
  pthread_mutex_init(&dnp->mutex, NULL);
  dnp->first_observed = -1;
}

void free_dependents_node(DependentsNode *dnp) {
  // we don't free dnp because they get initialized as a whole chunk
  if (dnp->dependents)
    free(dnp->dependents);
  pthread_mutex_destroy(&dnp->mutex);
}

ReactionNetwork *new_reaction_network_from_files(char *directory, bool logging) {
  // currently segfaults if files don't exist.
  ReactionNetwork *rnp = malloc(sizeof(ReactionNetwork));
  char *end;
  char path[2048];
  FILE* file;

  int return_code;
  int i, j;
  int step = 2; // hard coding reactions having <= 2 reactants and products

  rnp->dir = directory;
  rnp->db = NULL;
  rnp->logging = logging;


  // read number_of_species
  end = stpcpy(path, directory);
  stpcpy(end, number_of_species_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    return_code = fscanf(file, "%d\n", &rnp->number_of_species);
    fclose(file);
  }

  // read number_of_reactions
  end = stpcpy(path, directory);
  stpcpy(end, number_of_reactions_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    return_code = fscanf(file, "%d\n", &rnp->number_of_reactions);
    fclose(file);
  }

  // read number_of_reactants
  rnp->number_of_reactants = malloc(sizeof(int) * rnp->number_of_reactions);
  end = stpcpy(path, directory);
  stpcpy(end, number_of_reactants_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    for (i = 0; i < rnp->number_of_reactions; i++) {
      return_code = fscanf(file, "%d\n", rnp->number_of_reactants + i);
    }
    fclose(file);
  }


  // read reactants
  int *reactants_values = malloc(sizeof(int) * 2 * rnp->number_of_reactions);
  for (i = 0; i < 2 * rnp->number_of_reactions; i++)
    reactants_values[i] = -1;

  rnp->reactants = malloc(sizeof(int *) * rnp->number_of_reactions);
  for (i = 0; i < rnp->number_of_reactions; i++) {
    rnp->reactants[i] = reactants_values + step * i;
  }

  end = stpcpy(path, directory);
  stpcpy(end, reactants_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    for (i = 0; i < rnp->number_of_reactions; i++) {
      for (j = 0; j < rnp->number_of_reactants[i]; j++) {
        return_code = fscanf(file, "%d ", rnp->reactants[i] + j);
      }
      return_code = fscanf(file, "\n");
    }
    fclose(file);
  }


  // read number_of_products
  rnp->number_of_products = malloc(sizeof(int) * rnp->number_of_reactions);
  end = stpcpy(path, directory);
  stpcpy(end, number_of_products_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    for (i = 0; i < rnp->number_of_reactions; i++) {
      return_code = fscanf(file, "%d\n", rnp->number_of_products + i);
    }
    fclose(file);
  }

  // read products
  int *products_values = malloc(sizeof(int) * 2 * rnp->number_of_reactions);
  for (i = 0; i < 2 * rnp->number_of_reactions; i++)
    products_values[i] = -1;


  rnp->products = malloc(sizeof(int *) * rnp->number_of_reactions);
  for (i = 0; i < rnp->number_of_reactions; i++) {
    rnp->products[i] = products_values + step * i;
  }

  end = stpcpy(path, directory);
  stpcpy(end, products_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    for (i = 0; i < rnp->number_of_reactions; i++) {
      for (j = 0; j < rnp->number_of_products[i]; j++) {
        return_code = fscanf(file, "%d ", rnp->products[i] + j);
      }
      return_code = fscanf(file, "\n");
    }
    fclose(file);
  }

  // read factor_zero
  end = stpcpy(path, directory);
  stpcpy(end, factor_zero_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    return_code = fscanf(file, "%lf\n", &rnp->factor_zero);
    fclose(file);
  }

  // read factor_two
  end = stpcpy(path, directory);
  stpcpy(end, factor_two_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    return_code = fscanf(file, "%lf\n", &rnp->factor_two);
    fclose(file);
  }

  // read factor_duplicate
  end = stpcpy(path, directory);
  stpcpy(end, factor_duplicate_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    return_code = fscanf(file, "%lf\n", &rnp->factor_duplicate);
    fclose(file);
  }

  // read rates
  rnp->rates = malloc(sizeof(double) * rnp->number_of_reactions);
  end = stpcpy(path, directory);
  stpcpy(end, rates_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {
    for (i = 0; i < rnp->number_of_reactions; i++) {
      return_code = fscanf(file, "%lf\n", rnp->rates + i);
    }
    fclose(file);
  }


  // read initial_state
  rnp->initial_state = malloc(sizeof(int) * rnp->number_of_species);
  end = stpcpy(path, directory);
  stpcpy(end, initial_state_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s\n",path);
    return NULL;
  } else {

    for (i = 0; i < rnp->number_of_species; i++) {
      return_code = fscanf(file, "%d\n", rnp->initial_state + i);
    }
    fclose(file);
  }

  rnp->start_time = time(NULL);

  initialize_dependency_graph(rnp);
  initialize_propensities(rnp);

  return rnp;
}

void free_reaction_network(ReactionNetwork *rnp) {
  // terminate database connection if it exists
  if (rnp->db) {
    sqlite3_close(rnp->db);
  }

  free(rnp->number_of_reactants);
  free(rnp->reactants[0]);
  free(rnp->reactants);
  free(rnp->number_of_products);
  free(rnp->products[0]);
  free(rnp->products);
  free(rnp->rates);
  free(rnp->initial_state);
  free(rnp->initial_propensities);

  int i; // reaction index
  for (i = 0; i < rnp->number_of_reactions; i++)
    free_dependents_node(rnp->dependency_graph + i);

  free(rnp->dependency_graph);

  free(rnp);

}

DependentsNode *get_dependency_node(ReactionNetwork *rnp, int index) {
    DependentsNode *node = rnp->dependency_graph + index;

    pthread_mutex_lock(&node->mutex);
    if (! node->dependents)
      compute_dependency_node(rnp, index);

    pthread_mutex_unlock(&node->mutex);
    return node;
}

void compute_dependency_node(ReactionNetwork *rnp, int index) {

  DependentsNode *node = rnp->dependency_graph + index;

  int number_of_dependents_count = 0;
  int j; // reaction index
  int l, m, n; // reactant and product indices

  for (j = 0; j < rnp->number_of_reactions; j++) {
    bool flag = false;

    for (l = 0; l < rnp->number_of_reactants[j]; l++) {
      for (m = 0; m < rnp->number_of_reactants[index]; m++) {
        if (rnp->reactants[j][l] == rnp->reactants[index][m])
          flag = true;
      }

      for (n = 0; n < rnp->number_of_products[index]; n++) {
        if (rnp->reactants[j][l] == rnp->products[index][n])
          flag = true;
      }
    }

    if (flag)
      number_of_dependents_count++;
  }

  node->number_of_dependents = number_of_dependents_count;
  node->dependents = malloc(sizeof(int)
                            * number_of_dependents_count);
  int dependents_counter = 0;
  int current_reaction = 0;
  while (dependents_counter < number_of_dependents_count) {
    bool flag = false;
    for (l = 0; l < rnp->number_of_reactants[current_reaction]; l++) {
      for (m = 0; m < rnp->number_of_reactants[index]; m++) {
        if (rnp->reactants[current_reaction][l] == rnp->reactants[index][m])
          flag = true;
      }

      for (n = 0; n < rnp->number_of_products[index]; n++) {
        if (rnp->reactants[current_reaction][l] == rnp->products[index][n])
          flag = true;
      }
    }

    if (flag) {
      node->dependents[dependents_counter] = current_reaction;
      dependents_counter++;
    }
    current_reaction++;
  }
  node->first_observed = time(NULL) - rnp->start_time;

  if (rnp->logging)
    printf("dependency node: time = %ld , reaction = %d\n",
           node->first_observed, index);
}

void initialize_dependency_graph(ReactionNetwork *rnp) {

 if (rnp->logging)
    puts("started initializing dependency graph");

  int i; // reaction index
  rnp->dependency_graph = malloc(sizeof(DependentsNode)
                                 * rnp->number_of_reactions);

  for (i = 0; i < rnp->number_of_reactions; i++) {
    initialize_dependents_node(rnp->dependency_graph + i);
  }

  // this is how you would precompute the dependency graph.
  // Doing so is completely inpractical for large networks.
  // for (i = 0; i < rnp->number_of_reactions; i++) {
  //   compute_dependency_node(rnp, i);
  // }

 if (rnp->logging)
    puts("finished initializing dependency graph");


}

double compute_propensity(ReactionNetwork *rnp,
                         int *state,
                         int reaction) {
  double p;
  // zero reactants
  if (rnp->number_of_reactants[reaction] == 0)
    p = rnp->factor_zero
      * rnp->rates[reaction];

  // one reactant
  else if (rnp->number_of_reactants[reaction] == 1)
    p = state[rnp->reactants[reaction][0]]
      * rnp->rates[reaction];


  // two reactants
  else {
    if (rnp->reactants[reaction][0] == rnp->reactants[reaction][1])
      p = rnp->factor_duplicate
        * rnp->factor_two
        * state[rnp->reactants[reaction][0]]
        * (state[rnp->reactants[reaction][0]] - 1)
        * rnp->rates[reaction];

    else
      p = rnp->factor_two
        * state[rnp->reactants[reaction][0]]
        * state[rnp->reactants[reaction][1]]
        * rnp->rates[reaction];

  }

  return p;
}

void initialize_propensities(ReactionNetwork *rnp) {
  rnp->initial_propensities = malloc(rnp->number_of_reactions * sizeof(double));
  for (int reaction = 0; reaction < rnp->number_of_reactions; reaction++) {
    rnp->initial_propensities[reaction] =
      compute_propensity(rnp, rnp->initial_state, reaction);
  }
  if (rnp->logging) {
    puts("finished computing initial propensities");
  }
}



int reaction_network_to_files(ReactionNetwork *rnp, char *directory) {
  char *end;
  char path[2048];
  FILE* file;
  if (strlen(directory) > 1024) {
    puts("reaction_network_to_file: directory path too long");
    return -1;
  }

  DIR *dir = opendir(directory);
  if (dir) {
    puts("reaction_network_to_file: directory already exists");
    closedir(dir);
    return -1;
  }

  if (mkdir(directory, 0777)) {
    puts("reaction_network_to_file: failed to make directory");
    return -1;
  }

  int i, j;

  // save number_of_species
  end = stpcpy(path, directory);
  stpcpy(end, number_of_species_postfix);
  file = fopen(path, "w");
  fprintf(file, "%d\n", rnp->number_of_species);
  fclose(file);

  // save number_of_reactions
  end = stpcpy(path, directory);
  stpcpy(end, number_of_reactions_postfix);
  file = fopen(path, "w");
  fprintf(file, "%d\n", rnp->number_of_reactions);
  fclose(file);

  // save number_of_reactants
  end = stpcpy(path, directory);
  stpcpy(end, number_of_reactants_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_reactions; i++) {
    fprintf(file, "%d\n", rnp->number_of_reactants[i]);
  }
  fclose(file);

  // save reactants
  end = stpcpy(path, directory);
  stpcpy(end, reactants_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_reactions; i++) {
    for (j = 0; j < rnp->number_of_reactants[i]; j++) {
      fprintf(file, "%d ", rnp->reactants[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);

  // save number_of_products
  end = stpcpy(path, directory);
  stpcpy(end, number_of_products_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_reactions; i++) {
    fprintf(file, "%d\n", rnp->number_of_products[i]);
  }
  fclose(file);

  // save products
  end = stpcpy(path, directory);
  stpcpy(end, products_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_reactions; i++) {
    for (j = 0; j < rnp->number_of_products[i]; j++) {
      fprintf(file, "%d ", rnp->products[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);

  // save factor_zero
  end = stpcpy(path, directory);
  stpcpy(end, factor_zero_postfix);
  file = fopen(path, "w");
  fprintf(file, "%e\n", rnp->factor_zero);
  fclose(file);

  // save factor_two
  end = stpcpy(path, directory);
  stpcpy(end, factor_two_postfix);
  file = fopen(path, "w");
  fprintf(file, "%e\n", rnp->factor_two);
  fclose(file);


  // save factor_duplicate
  end = stpcpy(path, directory);
  stpcpy(end, factor_duplicate_postfix);
  file = fopen(path, "w");
  fprintf(file, "%e\n", rnp->factor_duplicate);
  fclose(file);

  // save rates
  end = stpcpy(path, directory);
  stpcpy(end, rates_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_reactions; i++) {
    fprintf(file, "%e\n", rnp->rates[i]);
  }
  fclose(file);

  // save initial_state
  end = stpcpy(path, directory);
  stpcpy(end, initial_state_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_species; i++) {
    fprintf(file, "%d\n", rnp->initial_state[i]);
  }
  fclose(file);


  return 0;
}


int reaction_network_to_db(ReactionNetwork *rnp, char *directory) {
  char *end;
  char path[2048];
  char *error = NULL;
  char sql_command[SQL_STATEMENT_LENGTH];
  FILE* file;
  sqlite3 *db;
  if (strlen(directory) > 1024) {
    puts("reaction_network_to_db: directory path too long");
    return -1;
  }

  DIR *dir = opendir(directory);
  if (dir) {
    puts("reaction_network_to_db: directory already exists");
    closedir(dir);
    return -1;
  }

  if (mkdir(directory, 0777)) {
    puts("reaction_network_to_db: failed to make directory");
    return -1;
  }

  end = stpcpy(path, directory);
  stpcpy(end, reaction_network_db_postix);

  // TODO: check error code here
  sqlite3_open(path, &db);

  // TODO: check errors here
  sqlite3_exec(db, create_tables, NULL, NULL, NULL);

  insert_metadata_command(rnp, sql_command);
  // TODO: check errors here
  sqlite3_exec(db, sql_command, NULL, NULL, NULL);

  int i;

  for (i = 0; i < rnp->number_of_reactions; i++) {
    insert_reaction_command(rnp, i, sql_command);
    sqlite3_exec(db, sql_command, NULL, NULL, NULL);
  }

  // save factor_zero
  end = stpcpy(path, directory);
  stpcpy(end, factor_zero_postfix);
  file = fopen(path, "w");
  fprintf(file, "%e\n", rnp->factor_zero);
  fclose(file);

  // save factor_two
  end = stpcpy(path, directory);
  stpcpy(end, factor_two_postfix);
  file = fopen(path, "w");
  fprintf(file, "%e\n", rnp->factor_two);
  fclose(file);


  // save factor_duplicate
  end = stpcpy(path, directory);
  stpcpy(end, factor_duplicate_postfix);
  file = fopen(path, "w");
  fprintf(file, "%e\n", rnp->factor_duplicate);
  fclose(file);

  // save initial_state
  end = stpcpy(path, directory);
  stpcpy(end, initial_state_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_species; i++) {
    fprintf(file, "%d\n", rnp->initial_state[i]);
  }
  fclose(file);


  sqlite3_close(db);
  return 0;
}

bool reaction_networks_differ(ReactionNetwork *rnpa, ReactionNetwork *rnpb) {
  int i, j;

  if (rnpa->number_of_species != rnpb->number_of_species)
    return true;

  if (rnpa->number_of_reactions != rnpb->number_of_reactions)
    return true;

  for (i = 0; i < rnpa->number_of_reactions; i++) {
    if (rnpa->number_of_reactants[i] != rnpb->number_of_reactants[i])
      return true;
  }

  for (i = 0; i < rnpa->number_of_reactions; i++) {
    for (j = 0; j < rnpa->number_of_reactants[i]; j++) {
      if (rnpa->reactants[i][j] != rnpb->reactants[i][j])
        return true;
    }
  }

  for (i = 0; i < rnpa->number_of_reactions; i++) {
    if (rnpa->number_of_products[i] != rnpb->number_of_products[i])
      return true;
  }

  for (i = 0; i < rnpa->number_of_reactions; i++) {
    for (j = 0; j < rnpa->number_of_products[i]; j++) {
      if (rnpa->products[i][j] != rnpb->products[i][j])
        return true;
    }
  }

  if (rnpa->factor_zero != rnpb->factor_zero)
    return true;

  if (rnpa->factor_two != rnpb->factor_two)
    return true;

  if (rnpa->factor_duplicate != rnpb->factor_duplicate)
    return true;

  for (i = 0; i < rnpa->number_of_reactions; i++) {
    if (rnpa->rates[i] != rnpb->rates[i])
      return true;
  }

  for (i = 0; i < rnpa->number_of_species; i++) {
    if (rnpa->initial_state[i] != rnpb->initial_state[i])
      return true;
  }

  return false;
}
