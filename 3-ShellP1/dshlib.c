#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
char *token;
char *cmd_copy;
char *saveptr1,*saveptr2;
int cmd_count=0;
memset(clist,0,sizeof(command_list_t));

cmd_copy=strdup(cmd_line);
if (cmd_copy==NULL)
{
return ERR_CMD_OR_ARGS_TOO_BIG;
}

token=strtok_r(cmd_copy,PIPE_STRING,&saveptr1);
while (token !=NULL) {
char *cmd_start=token;
char *next_token;
while (*cmd_start && isspace(*cmd_start))
{
cmd_start++;
}

if (cmd_count>=CMD_MAX)
{
free(cmd_copy);
return ERR_TOO_MANY_COMMANDS;
}
next_token=strtok_r(cmd_start," \t",&saveptr2);
if (next_token)
{
if (strlen(next_token)>=EXE_MAX) 
{
free(cmd_copy);
return ERR_CMD_OR_ARGS_TOO_BIG;
}
strcpy(clist->commands[cmd_count].exe, next_token);
char args_buffer[ARG_MAX] = "";
next_token=strtok_r(NULL, " \t", &saveptr2);
while (next_token)
{
if (strlen(args_buffer)+ strlen(next_token) + 2 >= ARG_MAX) { 
free(cmd_copy);
return ERR_CMD_OR_ARGS_TOO_BIG;
}
                
if (args_buffer[0] !='\0')
{
strcat(args_buffer," ");
}
strcat(args_buffer,next_token);
next_token=strtok_r(NULL," \t",&saveptr2);
}
strcpy(clist->commands[cmd_count].args,args_buffer);
cmd_count++;
}
token=strtok_r(NULL,PIPE_STRING,&saveptr1);
}

free(cmd_copy);

clist->num =cmd_count;
    
return OK;
}
