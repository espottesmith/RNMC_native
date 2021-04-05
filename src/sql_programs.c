#include "sql_programs.h"


char *create_tables =
  "CREATE TABLE metadata ("
  "        number_of_species   INTEGER NOT NULL,"
  "        number_of_reactions INTEGER NOT NULL,"
  "        factor_duplicate    REAL NOT NULL,"
  "        factor_two          REAL NOT NULL,"
  "        factor_zero         REAL NOT NULL"
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

char *insert_metadata(ReactionNetwork *rnp) {
  char *sql_statement = malloc(sizeof(char) * SQL_STATEMENT_LENGTH);
  return sql_statement;
}
