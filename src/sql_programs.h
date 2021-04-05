#ifndef SQL_PROGRAMS_H
#define SQL_PROGRAMS_H

#include "reaction_network.h"

#define SQL_STATEMENT_LENGTH 2048

char *create_tables;

char *insert_metadata(ReactionNetwork *rnp);

#endif
