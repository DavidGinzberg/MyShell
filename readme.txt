#!/bin/gedit
David Ginzberg

NAME
	myshell  -  shell  command  interpreter   project

SYNOPSIS
	myshell [batchfile]

DESCRIPTION
	myshell  is a  command  interpreter,  with a very
	minimal  syntax,  supporting  only the very basic
	terminal activities. It is intended as a learning
	exercise.

  Usage Modes:
	myshell can be used in one of two modes:
	batch execution, and interactive execution.

  Batch execution:
	When  called  with  the  name  of  a  file  as an
	argument   ('myshell filename'),   that  file  is
	interpreted as a batch file, and myshell attempts
	to  execute each line of the  file as a  command.
	Command execution  continues  automatically until
	a 'quit' command is reached.  Batchfiles must end
	with a 'quit' command or execution will not stop.

  Interactive execution:
	When called without any arguments, myshell
	begins running as an interactive command
	interpretation shell (command shell or command
	prompt). Input is read from the user one line
	at a time, and commands are executed as they are
	received. The user can leave execution at any
	time by typing quit. External commands can also
	be used and will be passed to the parent system
	to be executed.

FEATURES

  I/O Redirection
	[command] [< infile] [[>][>>] outfile]
	I/O redirection is the process of redirecting the
	input or  output of the  shell to and from files.
	If an infile is  specified,  the command receives
	input from that file instead of the user.   If an
	outfile is  specified,  output  is  redirected to
	that file instead of to the terminal.   There are
	two ways to redirect output.  The  '>'  character
	truncates the outfile and  overwrites it with the
	output of the command. The '>>' character appends
	the output of the command to the end of the file.
	Output  redirection is supported for  dir,  echo,
	environ,  and help commands,  as well as external
	commands.  Input redirection is only supported by
	external commands at this time.

  Background Execution
	[command] [&]
	Background  execution  allows the shell to return
	immediately to the  prompt to  receive  the  next
	command before the  previous command has finished
	executing.   The previous command continues until
	completion as a  separate process from the shell.
	Background  execution is currently only supported
	for external commands.

COMMANDS

  cd	[directory]
	Change Directory -- Changes the current directory
	to the directory  specified.   If no directory is
	given, prints the current directory.   Reports an
	error if the specified directory is not available
	and changes the  PWD  environment variable if the
	current directory is successfully changed.

  clr	
	Clear terminal--clears all text from the terminal
	and returns to prompt.

  dir	[directory] [[>][>>] outfile]
	Directory Listing--Lists all files and folders in
	the specified  directory in a fashion  equivalent
	to that of "ls -al" in most Linux/Unix shells. If
	no directory is specified,  lists contents of the
	current directory.  Output redirection supported.

  echo	[comment] [[>][>>] outfile]
	Print Comment -- Prints the comment followed by a
	newline.  All whitespace (space, tab, newline) is
	reduced to a single space.  Output redirection is
	supported.

  environ	[[>][>>] outfile]
	Environment  Variables -- Prints the  environment
	variables for the current  system state,  each on
	a separate line. Output redirection is supported.

  help
	Displays this help file

  pause
	Pauses the shell until ENTER is pressed.

  quit
	Quits the shell.

ENVIRONMENT

	myshell dynamically updates the folliwng 
	environment variables under the given conditions

  PWD
	Changed to the current directory whenever cd
	is called to change the current directory

  SHELL
	at the beginning of execution

  PARENT
	when spawning a new process via an external
	command
