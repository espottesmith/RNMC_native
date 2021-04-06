#ifndef SQL_PROGRAMS_H
#define SQL_PROGRAMS_H

#include <stdio.h>
#include "reaction_network.h"

#define SQL_STATEMENT_LENGTH 2048

char *create_tables;

void insert_metadata_command(ReactionNetwork *rnp, char *buffer);
void insert_reaction_command(ReactionNetwork *rnp, int reaction_index, char *command_buffer);



#endif
