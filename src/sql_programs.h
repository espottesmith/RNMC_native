#ifndef SQL_PROGRAMS_H
#define SQL_PROGRAMS_H

// if reactants and products don't exist, we use the value -1
static char create_tables[] =
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


static char insert_metadata[] =
  "INSERT INTO metadata ("
  "        number_of_species,"
  "        number_of_reactions)"
  "VALUES (?1, ?2);";


static char insert_reaction[] =
  "INSERT INTO reactions ("
  "        reaction_id,"
  "        reaction_string,"
  "        number_of_reactants,"
  "        number_of_products,"
  "        reactant_1,"
  "        reactant_2,"
  "        product_1,"
  "        product_2,"
  "        rate) "
  "VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9);";

static char get_metadata[] =
  "SELECT * FROM metadata;";

static char get_reactions[] =
  "SELECT reaction_id,"
  "       number_of_reactants,"
  "       number_of_products,"
  "       reactant_1,"
  "       reactant_2,"
  "       product_1,"
  "       product_2,"
  "       rate "
  "FROM reactions;";


#endif
