Wrote my own shell program, Mav shell (msh), similar tobourne shell (bash), c-shell (csh), or korn shell (ksh). It will accept commands, fork a child process and execute those commands. The shell, like csh or bash, will run and accept commands until the user exits the shell.

- This shell is implemented using fork(), wait() and one of the exec family of functions
- Shell support and execute any command entered. Any command in /bin, /usr/bin/, /usr/local/bin/ and the current working directory works.
Shell searches in the following PATH order:
1. Current working directory,
2. /usr/local/bin
3. /usr/bin
4. /bin
- Parameters may also be combined. For example, ps may be executed as: ps –aef or ps –a –e -f.
- Shell supports the 'history' command which will list the last 15 commands entered by the user. Typing !n, where n is a number between 1 and 15 will result in the shell re-running the nth command. If the nth command does not exist then shell states “Command not in history.”. 
