By-Ashit Rustagi
Roll no. 20171022
1. main.c contains the main program and calls functions included in other files to do the operations. It also checks if a child process exited abnormally.
2. built-in.c contains the functions for echo and cd
3. ls.c contains function to implement ls along with the flag -a and -l ONLY. If any other flag is provided, then only normal ls runs. You can also do ls by typing the directory name(both relative and absolute path can be specified). Flags and directory name can come in any order. 
4. fore-back-proc.c is used to manage foreground and background processes. When any background process exits, a message will be generated displaying the PID of the process that finished. It is understood that & comes at the end of the command.
5. pinfo.c can show info about the current shell program or the PID specified.
6. clock.c will display time dynamically at regular intervals of time specified as the second argument. When Ctrl+C is pressed then the clock stops displaying time. No flag is specified. To run the clock, type clock <duration of each interval>
7. remindme.c will be used to generate a reminder after the time specified on the screen.
8. env.c contains functions to set or unset environment variables and generate suitable error messages.
9. jobs.c contains functions to implement the job command in my shell, displaying all background processes with their names and pid in order of creation. kjob function is used to send a signal to a valid job id whereas overkill functions kills all background processes.
10. fg-bg.c has two functions. fg will bring a process with a valid Job ID to the foreground whereas bg will cause a stopped background process to start running again.
11. Ctrl-C and Ctrl-Z signals have been handled appropriately. Ctrl-C will kill the current foreground process and not disturb any background process whereas Ctrl-Z will push the current foregrouund process to the background and stop it.
12. Commands can be semi-colon separated, their outputs will apear one after the other. Do not use & and ; together. They don't function on regular terminal as well.
13. Type make to compile all the files, and then ./shell to run the program.