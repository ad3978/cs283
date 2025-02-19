1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: We use fork() before execvp() because execvp() completely replaces the current program with the new program, and never returns. If we called execvp() directly in our shell, it would replace our shell program with the command being run, and our shell would terminate after running just one command. By using fork(), we create a child process that is a copy of our shell, and then only that child process gets replaced by execvp(). This allows our shell to continue running, wait for the child to finish, and then be ready to accept and execute the next command. This pattern lets us maintain our shell's continuous operation while still being able to run other programs.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  In my implementation, when fork() fails it returns a negative value, and we handle this by printing "fork error" and then continuing the loop after freeing the command buffer's memory

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The execvp() function finds commands to execute by searching through the directories listed in the PATH environment variable. When you type a command like "ls" or "grep", execvp() looks through each directory in PATH until it finds an executable file with that name, it will use this PATH variable for searching. If the command is specified with a complete path then execvp() will try to execute that file directly without searching PATH. If the command isn't found in any of the PATH directories then execvp() fails and returns an error.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The parent process calls wait() or in our case, waitpid() to prevent any processes from being left in the system. When a child process finishes executing, it doesn't completely disappear it instead enters a state where it holds onto some resources and its exit status until the parent process retrieves this information through wait(). If we didn't call wait() with each command we ran would leave behind a process eventually using up all the system's process table slots.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() extracts the actual exit code from the status value returned by waitpid(). When a child process ends, it returns an exit code like when we call exit1 on error or the program naturally returns a value. However, this exit code is packed into a larger status value along with other information about how the process ended. WEXITSTATUS() specifically pulls out just the 8-bit exit code from this status. This is important because it lets our shell know if the command succeeded or failed, which we can then use to inform the user or make decisions about continuing execution. In our implementation, we store this value in our rc variable to track the success or failure of the last executed command.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  In my code, I use a flag called in_quotes to track whether we're currently inside a quoted string. When we encounter a quote mark, we flip this flag and remove the quote character by shifting the rest of the string left. Any spaces encountered while in_quotes is true are preserved instead of being treated as argument separators.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In refactoring from the previous assignment, I made significant changes to move from the command_list_t structure to the new cmd_buff_t structure. The main difference is that instead of splitting commands by pipes and storing them in an array, now we're just handling a single command at a time. The biggest challenge was adapting the string parsing logic previously we used strtok_r() to split commands by pipes and then by spaces, but now we needed to implement a more careful character by character parsing to properly handle quoted strings while preserving spaces within quotes.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are simple, immediate notifications between processes in Linux, used to trigger specific actions like program termination. Unlike other forms of IPC that can transfer data, signals only communicate a signal number, making them lightweight but limited to basic process control and event notification.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGINT is Sent when user presses Ctrl+C. Used to interrupt a program for a graceful termination, allowing it to clean up before exiting.
SIGTERM is the default signal sent by 'kill' command. A polite request for a program to terminate, giving it a chance to save data and clean up.
SIGKILL forcefully terminates a process immediately with no cleanup. Used as a last resort when a program won't respond to SIGTERM, but can leave resources in an inconsistent state.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP immediately freezes a process, and unlike SIGINT, it cannot be caught or ignored by the process. This signal is special because it's one of the few "mandatory" signals which means that the operating system enforces its behavior and doesn't allow processes to override it. This design ensures that system administrators always have a way to suspend processes, even if they're misbehaving or trying to block signals. The suspended process can only be resumed by receiving a SIGCONT signal.
