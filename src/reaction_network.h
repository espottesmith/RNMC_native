#ifndef REACTION_NETWORK_H
#define REACTION_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

// struct for storing the static reaction network state which
// will be shared across all simulation instances


typedef struct reactionNetwork {

  // filled out by reaction_network_from_file
  char *dir; // directory where reaction network serialization files are
  int number_of_species;
  int number_of_reactions;
  // we assume that each reaction has zero, one or two reactants
  int *number_of_reactants; // array storing the number of reactants of each reaction
  int **reactants; // array storing the reactants of each reaction
  int *number_of_products; // array storing the number of products of each reaction
  int **products; // array storing the products of each reaction


  double factor_zero; // rate modifer for reactions with zero reactants
  double factor_two; // rate modifier for reactions with two reactants
  double factor_duplicate; // rate modifier for reactions of form A + A -> ...
  double *rates; // array storing the rates for each reaction


  int *initial_state; // initial state for all the simulations


  // allocated and filled out by initializePropensities. Initially set to 0
  double *initial_propensities; // initial propensities for all the reactions


  // allocated filled out by buildDependencyGraph. Initially set to 0
  int *number_of_dependents; // number of reactions that depend on each reaction
  int max_number_of_dependents;
  int **dependents;
  // dependents[i] is the list of reactions whose propensities
  // need to be updated after reaction i occours


} ReactionNetwork;

ReactionNetwork *new_reaction_network(char *directory);
void free_reaction_network(ReactionNetwork *rnp);

double compute_propensity(ReactionNetwork *rnp, int *state, int reaction);
void initialize_propensities(ReactionNetwork *rnp);
void build_dependency_graph(ReactionNetwork *rnp);

// serialize a reaction network to the disk (not including initial propensities
// and dependency graph). Each attribute is stored in directory/attribute_name
// directory is an argument so we can serialize into another location
int reaction_network_to_file(ReactionNetwork *rnp, char *directory);
bool reaction_networks_differ(ReactionNetwork *rnpa, ReactionNetwork *rnpb);

#endif
