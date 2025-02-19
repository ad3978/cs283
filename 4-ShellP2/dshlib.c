#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

Built_In_Cmds match_command(const char *input)
{
if (strcmp(input, "cd") == 0)
{
return BI_CMD_CD;
}
return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
Built_In_Cmds type = match_command(cmd->argv[0]);

if (type == BI_CMD_CD)
{
// If cd has an argument, change to that directory
if (cmd->argc > 1)
{
if (chdir(cmd->argv[1]) != 0)
{
perror("cd");
}
}
// If no argument, do nothing (per requirements)
return BI_EXECUTED;
}

return type;
}
/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int exec_local_cmd_loop()
{
char *cmd_buff;
int rc = 0;
cmd_buff_t cmd;
// TODO IMPLEMENT MAIN LOOP
cmd_buff = malloc(SH_CMD_MAX);
if (cmd_buff==NULL)
{
return ERR_MEMORY;
}

while(1) {
printf("%s", SH_PROMPT);
if (fgets(cmd_buff, ARG_MAX, stdin)==NULL)
{
printf("\n");
break;
}
//remove the trailing \n from cmd_buff
cmd_buff[strcspn(cmd_buff,"\n")] ='\0';

// TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
// Initialize/clear command buffer
cmd.argc=0;
cmd._cmd_buffer=malloc(strlen(cmd_buff)+1);
if (cmd._cmd_buffer==NULL)
{
free(cmd_buff);
return ERR_MEMORY;
}

// Copy command manually
int i;
for (i = 0;cmd_buff[i];i++)
{
cmd._cmd_buffer[i] =cmd_buff[i];
}
cmd._cmd_buffer[i] ='\0';

// Skip leading spaces
char *current = cmd._cmd_buffer;
while (*current && (*current == ' ' || *current == '\t')) current++;

// Parse into argv array
char *token =current;
int in_quotes =0;

while (*current)
{
if (*current == '"')
{
// Shift remaining string left by one to remove quote
char *src = current + 1;
char *dst = current;
while ((*dst++ = *src++));
in_quotes = !in_quotes;
continue;
}

if (!in_quotes && (*current == ' ' || *current == '\t'))
{
*current ='\0';
if (strlen(token) > 0) {
if (cmd.argc >= CMD_ARGV_MAX - 1)
{
break;
}
cmd.argv[cmd.argc++] =token;
}
current++;
while (*current && (*current == ' ' || *current == '\t')) current++;
token=current;
continue;
}
current++;
}

// Add the last token if it exists
if (strlen(token) > 0 && cmd.argc < CMD_ARGV_MAX - 1)
{
cmd.argv[cmd.argc++] = token;
}

cmd.argv[cmd.argc] =NULL;

if (cmd.argc==0)
{
printf(CMD_WARN_NO_CMD);
free(cmd._cmd_buffer);
continue;
}

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing
if (cmd.argc>0)
{
Built_In_Cmds result = match_command(cmd.argv[0]);
if (result==BI_CMD_CD)
{
if (cmd.argc > 1)
{
if (chdir(cmd.argv[1]) != 0)
{
printf("cannot change directory\n");
}
}
free(cmd._cmd_buffer);
continue;
}
}

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
pid_t pid = fork();
if (pid<0)
{
printf("fork error\n");
free(cmd._cmd_buffer);
continue;
}
else if (pid==0)
{
// Child process
execvp(cmd.argv[0], cmd.argv);
// If execvp returns, it means there was an error
printf("execvp error\n");
exit(1);
}
else
{
// Parent process
int status;
waitpid(pid, &status, 0);
if (WIFEXITED(status)) {
rc= WEXITSTATUS(status);
}
}

free(cmd._cmd_buffer);
}

free(cmd_buff);
return rc;
}
