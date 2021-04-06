#include "sql_programs.h"

// if reactants and products don't exist, we use the value -1
char *create_tables =
  "CREATE TABLE metadata ("
  "        number_of_species   INTEGER NOT NULL,"
  "        number_of_reactions INTEGER NOT NULL"
  ");"

  "CREATE TABLE reactions ("
  "        reaction_id         INTEGER NOT NULL PRIMARY KEY,"
  "        reaction_string     TEXT NOT NULL,"
  "        number_of_reactants INTEGER NOT NULL,"
  "        number_of_products  INTEGER NOT NULL,"
  "        reactant_1          INTEGER NOT NULL,"
  "        reactant_2          INTEGER NOT NULL,"
  "        product_1           INTEGER NOT NULL,"
  "        product_2           INTEGER NOT NULL,"
  "        rate                REAL NOT NULL"
  ");"

  "CREATE UNIQUE INDEX reaction_string_idx ON reactions (reaction_string)";

void insert_metadata_command(ReactionNetwork *rnp, char *command_buffer) {
  char *header =
    "INSERT INTO metadata ("
    "        number_of_species,"
    "        number_of_reactions)";

  sprintf(command_buffer,
          "%s\nVALUES (%d, %d);",
          header,
          rnp->number_of_species,
          rnp->number_of_reactions);

}

void insert_reaction_command(ReactionNetwork *rnp,
                             int reaction_index,
                             char *command_buffer) {

  char *header =
    "INSERT INTO reactions ("
    "        reaction_id,"
    "        reaction_string,"
    "        number_of_reactants,"
    "        number_of_products,"
    "        reactant_1,"
    "        reactant_2,"
    "        product_1,"
    "        product_2,"
    "        rate)";

  char reactants_string[1024];
  char products_string[1024];
  char reaction_string[2048];

  switch (rnp->number_of_reactants[reaction_index])
  {
    case 0: {
      reactants_string[0] = 0;
      break;
    }
    case 1: {
      sprintf(reactants_string, "%d",rnp->reactants[reaction_index][0]);
      break;
    }
    default: {
      sprintf(reactants_string, "%d+%d",
              rnp->reactants[reaction_index][0],
              rnp->reactants[reaction_index][1]);
      break;
    }
  }

  switch (rnp->number_of_products[reaction_index])
  {
    case 0: {
      products_string[0] = 0;
      break;
    }
    case 1: {
      sprintf(products_string, "%d",rnp->products[reaction_index][0]);
      break;
    }
    default: {
      sprintf(products_string, "%d+%d",
              rnp->products[reaction_index][0],
              rnp->products[reaction_index][1]);
      break;
    }
  }

  sprintf(reaction_string, "%s->%s", reactants_string, products_string);

  // for reactants and products which don't exist, the value -1 is used.
  sprintf(command_buffer,
          "%s\nVALUES (%d, \"%s\", %d, %d, %d, %d, %d, %d, %1.9e);",
          header,
          reaction_index,
          reaction_string,
          rnp->number_of_reactants[reaction_index],
          rnp->number_of_products[reaction_index],
          rnp->reactants[reaction_index][0],
          rnp->reactants[reaction_index][1],
          rnp->products[reaction_index][0],
          rnp->products[reaction_index][1],
          rnp->rates[reaction_index]);

}
