#include "reaction_network.h"

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



ReactionNetwork *new_reaction_network(char *directory) {
  // currently segfaults if files don't exist.
  ReactionNetwork *rnp = malloc(sizeof(ReactionNetwork));
  char *end;
  char path[2048];
  FILE* file;

  int return_code;
  int i, j;
  int step = 2; // hard coding reactions having <= 2 reactants and products

  rnp->dir = directory;

  // read number_of_species
  end = stpcpy(path, directory);
  stpcpy(end, number_of_species_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }
  return_code = fscanf(file, "%d\n", &rnp->number_of_species);
  fclose(file);

  // read number_of_reactions
  end = stpcpy(path, directory);
  stpcpy(end, number_of_reactions_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }
  return_code = fscanf(file, "%d\n", &rnp->number_of_reactions);
  fclose(file);

  // read number_of_reactants
  rnp->number_of_reactants = malloc(sizeof(int) * rnp->number_of_reactions);
  end = stpcpy(path, directory);
  stpcpy(end, number_of_reactants_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  for (i = 0; i < rnp->number_of_reactions; i++) {
    return_code = fscanf(file, "%d\n", rnp->number_of_reactants + i);
  }
  fclose(file);

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
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  for (i = 0; i < rnp->number_of_reactions; i++) {
    for (j = 0; j < rnp->number_of_reactants[i]; j++) {
      return_code = fscanf(file, "%d ", rnp->reactants[i] + j);
    }
    return_code = fscanf(file, "\n");
  }
  fclose(file);

  // read number_of_products
  rnp->number_of_products = malloc(sizeof(int) * rnp->number_of_reactions);
  end = stpcpy(path, directory);
  stpcpy(end, number_of_products_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  for (i = 0; i < rnp->number_of_reactions; i++) {
    return_code = fscanf(file, "%d\n", rnp->number_of_products + i);
  }
  fclose(file);

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
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  for (i = 0; i < rnp->number_of_reactions; i++) {
    for (j = 0; j < rnp->number_of_products[i]; j++) {
      return_code = fscanf(file, "%d ", rnp->products[i] + j);
    }
    return_code = fscanf(file, "\n");
  }
  fclose(file);


  // read factor_zero
  end = stpcpy(path, directory);
  stpcpy(end, factor_zero_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  return_code = fscanf(file, "%lf\n", &rnp->factor_zero);
  fclose(file);

  // read factor_two
  end = stpcpy(path, directory);
  stpcpy(end, factor_two_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  return_code = fscanf(file, "%lf\n", &rnp->factor_two);
  fclose(file);

  // read factor_duplicate
  end = stpcpy(path, directory);
  stpcpy(end, factor_duplicate_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  return_code = fscanf(file, "%lf\n", &rnp->factor_duplicate);
  fclose(file);

  // read rates
  rnp->rates = malloc(sizeof(double) * rnp->number_of_reactions);
  end = stpcpy(path, directory);
  stpcpy(end, rates_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  for (i = 0; i < rnp->number_of_reactions; i++) {
    return_code = fscanf(file, "%lf\n", rnp->rates + i);
  }
  fclose(file);

  // read initial_state
  rnp->initial_state = malloc(sizeof(int) * rnp->number_of_species);
  end = stpcpy(path, directory);
  stpcpy(end, initial_state_postfix);
  file = fopen(path, "r");
  if (!file) {
    printf("new_reaction_network: cannot open %s",path);
    return NULL;
  }

  for (i = 0; i < rnp->number_of_species; i++) {
    return_code = fscanf(file, "%d\n", rnp->initial_state + i);
  }
  fclose(file);

  initialize_propensities(rnp);
  build_dependency_graph(rnp);
  return rnp;
}

void free_reaction_network(ReactionNetwork *rnp) {
  free(rnp->number_of_reactants);
  free(rnp->reactants[0]);
  free(rnp->reactants);
  free(rnp->number_of_products);
  free(rnp->products[0]);
  free(rnp->products);
  free(rnp->rates);
  free(rnp->initial_state);


  if (rnp->initial_propensities)
    free(rnp->initial_propensities);

  if (rnp->number_of_dependents)
    free(rnp->number_of_dependents);

  if (rnp->dependents) {

    free(rnp->dependents[0]);
    free(rnp->dependents);

  }
  free(rnp);

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
}



void build_dependency_graph(ReactionNetwork *rnp) {

  // allocate number_of_dependents array
  rnp->number_of_dependents = malloc(rnp->number_of_reactions * sizeof(int));

  // i,j store the numbers of reactants and products of reactions
  // m and n are reaction indices
  // mspecies and nspecies are species indices
  int i, j, m, n, mspecies, nspecies;

  // for a reaction m, flag[n] == 1 if reaction n depends on m
  //                   flag[n] == 0 if reaction n does not depend on m
  int *flag = malloc(rnp->number_of_reactions * sizeof(int));

  for (m = 0; m < rnp->number_of_reactions; m++) {

    // initialize flag for reaction m
    for (n = 0; n < rnp->number_of_reactions; n++) flag[n] = 0;

    // find all reactions n with reactant a reactant of m
    for (i = 0; i < rnp->number_of_reactants[m]; i++) {
      mspecies = rnp->reactants[m][i];
      for (n = 0; n < rnp->number_of_reactions; n++) {
        for (j = 0; j < rnp->number_of_reactants[n]; j++) {
          nspecies = rnp->reactants[n][j];
          if (mspecies == nspecies) flag[n] = 1;
        }
      }
    }


    // find all reactions n with reactant a product of m
    for (i = 0; i < rnp->number_of_products[m]; i++) {
      mspecies = rnp->products[m][i];
      for (n = 0; n < rnp->number_of_reactions; n++) {
        for (j = 0; j < rnp->number_of_reactants[n]; j++) {
          nspecies = rnp->reactants[n][j];
          if (mspecies == nspecies) flag[n] = 1;
        }
      }
    }

    rnp->number_of_dependents[m] = 0;
    for (n = 0; n < rnp->number_of_reactions; n++)
      if (flag[n]) rnp->number_of_dependents[m]++;
  }

  free(flag);

  // computing the maximum number of dependents over all reactions
  int max_dependents = 0;
  for (m = 0; m < rnp->number_of_reactions; m++) {
    if( rnp->number_of_dependents[m] > max_dependents )
      max_dependents = rnp->number_of_dependents[m];
  }

  rnp->max_number_of_dependents = max_dependents;

  // allocating dependents array
  rnp->dependents = malloc(sizeof(int *) * rnp->number_of_reactions);
  int *dependents_values = malloc(sizeof(int)
                                 * max_dependents
                                 * rnp->number_of_reactions);

  for (int d = 0; d < max_dependents * rnp->number_of_reactions; d++)
    dependents_values[d] = -1;

  for (m = 0; m < rnp->number_of_reactions; m++) {
    rnp->dependents[m] = dependents_values + m * max_dependents;
  }

  // we zero out the number_of_dependents because we use it
  // as an array of counters while initializing dependents

  for (m = 0; m < rnp->number_of_reactions; m++)
    rnp->number_of_dependents[m] = 0;

  // k loop ensures dependency was not already stored

  int k;

  for (m = 0; m < rnp->number_of_reactions; m++) {
    for (i = 0; i < rnp->number_of_reactants[m]; i++) {
      mspecies = rnp->reactants[m][i];
      for (n = 0; n < rnp->number_of_reactions; n++) {
        for (j = 0; j < rnp->number_of_reactants[n]; j++) {
          nspecies = rnp->reactants[n][j];
          if (mspecies == nspecies) {
            for (k = 0; k < rnp->number_of_dependents[m]; k++)
              if (n == rnp->dependents[m][k]) break;
            if (k == rnp->number_of_dependents[m])
              rnp->dependents[m][rnp->number_of_dependents[m]++] = n;
          }
        }
      }
    }

    for (i = 0; i < rnp->number_of_products[m]; i++) {
      mspecies = rnp->products[m][i];
      for (n = 0; n < rnp->number_of_reactions; n++) {
        for (j = 0; j < rnp->number_of_reactants[n]; j++) {
          nspecies = rnp->reactants[n][j];
          if (mspecies == nspecies) {
            for (k = 0; k < rnp->number_of_dependents[m]; k++)
              if (n == rnp->dependents[m][k]) break;
            if (k == rnp->number_of_dependents[m])
              rnp->dependents[m][rnp->number_of_dependents[m]++] = n;
          }
        }
      }
    }
  }
}


int reaction_network_to_file(ReactionNetwork *rnp, char *directory) {
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

  // save rates
  end = stpcpy(path, directory);
  stpcpy(end, initial_state_postfix);
  file = fopen(path, "w");
  for (i = 0; i < rnp->number_of_species; i++) {
    fprintf(file, "%d\n", rnp->initial_state[i]);
  }
  fclose(file);

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
