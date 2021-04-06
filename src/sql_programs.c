#include "sql_programs.h"


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
  "        reactant_1          INTEGER,"
  "        reactant_2          INTEGER,"
  "        product_1           INTEGER,"
  "        product_2           INTEGER,"
  "        rate                REAL NOT NULL"
  ");"

  "CREATE UNIQUE INDEX reaction_string_idx ON reactions (reaction_string)";

void insert_metadata_command(ReactionNetwork *rnp, char *command_buffer) {
  char *header =
    "INSERT INTO metadata ("
    "        number_of_species,"
    "        number_of_reactions)";

  sprintf(command_buffer,
          "%s\n VALUES (%d, %d);",
          header,
          rnp->number_of_species,
          rnp->number_of_reactions);

}
