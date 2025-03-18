#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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

Built_In_Cmds match_command(const char *input)
{
if (strcmp(input, "cd") == 0)
{
return BI_CMD_CD;
}
else if (strcmp(input, EXIT_CMD) == 0)
{
return BI_CMD_EXIT;
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

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
char *token, *saveptr;
int cmd_count = 0;

clist->num = 0;

if (strlen(cmd_line) == 0) {
return WARN_NO_CMDS;
}

token = strtok_r(cmd_line, PIPE_STRING, &saveptr);

while (token != NULL) {
if (cmd_count >= CMD_MAX) {
return ERR_TOO_MANY_COMMANDS;
}

if (alloc_cmd_buff(&clist->commands[cmd_count]) != OK) {
return ERR_MEMORY;
}

if (build_cmd_buff(token, &clist->commands[cmd_count]) != OK) {
return ERR_MEMORY;
}

cmd_count++;
token = strtok_r(NULL, PIPE_STRING, &saveptr);
}

clist->num = cmd_count;

if (cmd_count == 0) {
return WARN_NO_CMDS;
}

return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
cmd_buff->argc = 0;
cmd_buff->_cmd_buffer = NULL;

for (int i = 0; i < CMD_ARGV_MAX; i++) {
cmd_buff->argv[i] = NULL;
}

return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
cmd_buff->_cmd_buffer = malloc(strlen(cmd_line) + 1);
if (cmd_buff->_cmd_buffer == NULL) {
return ERR_MEMORY;
}

strcpy(cmd_buff->_cmd_buffer, cmd_line);

char *current = cmd_buff->_cmd_buffer;
while (*current && (*current == ' ' || *current == '\t')) current++;

char *token = current;
int in_quotes = 0;

while (*current) {
if (*current == '"') {
char *src = current + 1;
char *dst = current;
while ((*dst++ = *src++));
in_quotes = !in_quotes;
continue;
}

if (!in_quotes && (*current == ' ' || *current == '\t')) {
*current = '\0';
if (strlen(token) > 0) {
if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
break;
}
cmd_buff->argv[cmd_buff->argc++] = token;
}
current++;
while (*current && (*current == ' ' || *current == '\t')) current++;
token = current;
continue;
}
current++;
}

if (strlen(token) > 0 && cmd_buff->argc < CMD_ARGV_MAX - 1) {
cmd_buff->argv[cmd_buff->argc++] = token;
}

cmd_buff->argv[cmd_buff->argc] = NULL;

return OK;
}

int free_cmd_list(command_list_t *cmd_list)
{
for (int i = 0; i < cmd_list->num; i++) {
free(cmd_list->commands[i]._cmd_buffer);
cmd_list->commands[i]._cmd_buffer = NULL;
}

return OK;
}

int execute_pipeline(command_list_t *clist)
{
int num_commands = clist->num;
int status;
pid_t pids[CMD_MAX];
int pipes[CMD_MAX-1][2];

// Create all necessary pipes
for (int i = 0; i < num_commands - 1; i++) {
if (pipe(pipes[i]) < 0) {
perror("pipe");
return ERR_EXEC_CMD;
}
}

// Create a process for each command
for (int i = 0; i < num_commands; i++) {
pids[i] = fork();

if (pids[i] < 0) {
perror("fork");
return ERR_EXEC_CMD;
}

if (pids[i] == 0) {



if (i > 0) {
if (dup2(pipes[i-1][0], STDIN_FILENO) < 0) {
perror("dup2");
exit(EXIT_FAILURE);
}
}

if (i < num_commands - 1) {
if (dup2(pipes[i][1], STDOUT_FILENO) < 0) {
perror("dup2");
exit(EXIT_FAILURE);
}
}

for (int j = 0; j < num_commands - 1; j++) {
close(pipes[j][0]);
close(pipes[j][1]);
}


execvp(clist->commands[i].argv[0], clist->commands[i].argv);


perror("execvp");
exit(EXIT_FAILURE);
}
}


for (int i = 0; i < num_commands - 1; i++) {
close(pipes[i][0]);
close(pipes[i][1]);
}


for (int i = 0; i < num_commands; i++) {
waitpid(pids[i], &status, 0);
}

return OK;
}

int exec_local_cmd_loop()
{
char *cmd_buff;
int rc = 0;
command_list_t cmd_list;

cmd_buff = malloc(SH_CMD_MAX);
if (cmd_buff == NULL)
{
return ERR_MEMORY;
}

while(1) {
printf("%s", SH_PROMPT);
if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
{
printf("\n");
break;
}

cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

if (strcmp(cmd_buff, EXIT_CMD) == 0) {
free(cmd_buff);
return OK_EXIT;
}

rc = build_cmd_list(cmd_buff, &cmd_list);

if (rc == WARN_NO_CMDS) {
printf(CMD_WARN_NO_CMD);
continue;
} else if (rc == ERR_TOO_MANY_COMMANDS) {
printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
continue;
} else if (rc == ERR_MEMORY) {
free(cmd_buff);
return ERR_MEMORY;
}

if (cmd_list.num == 1) {
Built_In_Cmds result = match_command(cmd_list.commands[0].argv[0]);
if (result == BI_CMD_EXIT) {
free_cmd_list(&cmd_list);
free(cmd_buff);
return OK_EXIT;
} else if (result == BI_CMD_CD) {
exec_built_in_cmd(&cmd_list.commands[0]);
free_cmd_list(&cmd_list);
continue;
}
}

rc = execute_pipeline(&cmd_list);

free_cmd_list(&cmd_list);
}

free(cmd_buff);
return rc;
}
