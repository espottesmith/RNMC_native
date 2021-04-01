char create_tables[] =
  "CREATE TABLE metadata ("
  "        number_of_species   INTEGER NOT NULL,"
  "        number_of_reactions INTEGER NOT NULL,"
  "        factor_duplicate    REAL NOT NULL,"
  "        factor_two          REAL NOT NULL,"
  "        factor_zero         REAL NOT NULL"
  ");"

  "CREATE TABLE reactions ("
  "        reaction_string     TEXT PRIMARY KEY,"
  "        number_of_reactants INTEGER NOT NULL,"
  "        number_of_products  INTEGER NOT NULL,"
  "        reactant_1          INTEGER,"
  "        reactant_2          INTEGER,"
  "        product_1           INTEGER,"
  "        product_2           INTEGER,"
  "        rate                REAL NOT NULL"
  ");";
