1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: fgets() is a good choice for this shell program because it safely reads input one line at a time, won't overflow our buffer, and properly handles when the user ends the input. This makes it perfect for both normal shell usage and for running the test cases that check if our program works correctly.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:We use malloc() instead of a regular array because command lines can be pretty big. By putting big arrays directly in the program's memory could cause crashes on computers with limited memory. malloc() puts the array in a different type of memory heap that has more space and it also lets us check if we got the memory we needed before we try to use it.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: Removing extra spaces from the beginning and end of commands is important because computers are very picky about exact names since if someone types "  name" with spaces at the start, the computer would look for a program literally named "  name" with the spaces so instead of just "name". This would cause the program to not be found. The same problem happens with piped commands since if we type "ls |  sort" with extra space the computer would end up lookign for a program called sort with the spaces instead of "sort" so by trimming these extra spaces we make sure the shell can always find and run the right programs.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Our shell needs to handle three important ways of redirecting input and output.So we can implement Output redirection using > which lets users save a commands output to a file instead of showing it on the screen, like when typing ls > files.txt we can also implement Input redirection using < which lets commands read from a file instead of the keyboard, like sort < input.txt and etc. another feature would be Pipe redirectionusing | which connects two commands together so the output of the first command becomes the input of the second command, like ls | grep .txt. and more

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection and piping are two different ways to control where data goes in a shell, but they work quite differently. Redirection  connects a program with files like saving what a program shows to a file instead of your screen or making a program read from a file instead of your keyboard whereas Pipes  are different since they connect two programs directly together so what the first program produces goes straight into the second program

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: It's important for the shell to handle error messages using STDERR separately from normal output using STDOUT because it helps us work with commands more effectively. By keeping errors separate the shell can show you error messages on the screen while only sending the real results to files or other commands. For example if we are looking for files and saving the list to a file then we would want the file list in our saved file but would still want to see any error messages that could pop up on our screen to let us know if something went wrong

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  To do this we could make it so our shell supports the special notation 2>&1 which means sending errors to the same place as normal output this would save both normal output and any errors to the output log file 
