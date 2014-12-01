// November 5 2014 -- I have fetched this and related files from my dropbox. I plan
// to post them to a new GitHub repo. First I need to clean up the comments and remove
// certain details that only related to the class, or which contain sensitive data such
// as my student ID number. Once these are removed the files can be committed to the repo
// for further editing and development.

//Cleanup step 1: Remove personal id information and names of teachers and TAs -- Done
//Step 2: Remove references to University and courses. -- Done

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//File: myshell.c
//Author: David Ginzberg 
//Last Updated: 7/5/2012
//Last Compiled: 7/5/2012
//Comments:
//Most recent change: Updated function line numbers and removed some personal info
//Project complete, beginning personal development:
//need to add tab completion
// and per-session aliasing
// as well as ^L (FF / 0xC?) functionality (alias to clr)
//want to add colored prompt text
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Program info:
//GinzShell V1.0.1
//Project Files:myshell.c readme makefile
//copyright David Ginzberg, 2014
//beginning command shell project 
//No express warranties, implicit or explicit.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//NOTE: all comments preceeded by //# were originally used for laying down framework of this project based on the 
//project specification.  Most were removed but some remain for easy grading of the source code.

/*The following is a listing of all commands in GinzShell and the line numbers on which they begin.  Each command will
//include references to other line numbers when variables from outside the command are used (such as the current directory).
//Descriptions are not included here, as they are contained within the readme file.  This is simply a reference for developers
//and students who wish to see how these commands are implemented.*/

//These are probably all wrong and not really useful anyway
//main:		Line #90
//cd:		Line #221
//clr:		Line #269
//dir:		Line #290
//envrn:	Line #341
//echo:		Line #364
//help:		Line #390
//pauseShell:	Line #411
//quit:		Line #420
//externalCall	Line #426
//BGCheck	Line #460
//IORedirect	Line #476
//debug:	Line #568
//setPrompt:	Line #575

/*BEGIN include statements for all required files*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
/*END include statements*/

/*BEGIN definitions*/
#define MAX_BUFFER 1024		// max line buffer
#define MAX_ARGS 64		// max # args
#define DELIMITERS " \t\n"	// token separators
#define DEBUG 0			// debugging flag, triggers diagnostic output during execution
/*END definitions*/

/*BEGIN global variable declarations*/
//int batch; //1 if batch file provided, 0 if not. use with if(batch) to determine execution of program
pid_t childPID;	//used in forking and error detection
char *cwd = NULL;
char *newPWD, *shell, *initCall;
char *prompt = "~~>";
int status;
int noWait = 0;
int numArgs;
extern char **environ;
char **out, **env, *readme;
/*END global variable declarations*/

/*BEGIN function prototypes*/
int clr(char **args);
int cd(char **args);
int dir(char **args);
int quit(char **args);
int echo(char **args);
int envrn(char **args);
int pauseShell(char **args);
int help(char **args);
int externalCall(char **args);
void sigHandler(int signo);

void debug(char *err);
void setPrompt();
void BGCheck(char **args);
void IORedirect(char **args, int mode);
/*END function prototypes*/

/*BEGIN main driver function*/
int main(int argc, char **argv){
	
	/*BEGIN variable declarations*/
	char buf[MAX_BUFFER];		//line buffer
	char *args[MAX_ARGS];		//pointers to arg strings
	char **arg;			//working pointer thru args
	int noPrompt = 0;
	/*END variable declarations*/

	/*Register the signal handler function*/
	if (signal(SIGINT, sigHandler) == SIG_ERR)
		printf("\nSomething went wrong; can't catch SIGINT\n");

	/*Set the prompt.  Also puts a value in cwd that is used in several other operations*/
	setPrompt();
	
	//set the literal path for the readme file
	readme = (char*)malloc(sizeof(char) * (strlen(cwd) + 8));
	strcat(readme, cwd);
	strcat(readme, "/readme");

	/*Set the SHELL environment variable*/
	shell= (char *)malloc(sizeof(char) * (strlen(cwd)+strlen(argv[0])+8));
	strcat(shell, "SHELL=");
	strcat(shell, cwd);
	strcat(shell, "/");
	strcat(shell, argv[0]);
	putenv(shell);

	/*Set the initial call to the shell.  Used in setting PARENT environment variable for child processes*/
	initCall = (char *)malloc(sizeof(char) * (strlen(cwd) + strlen(argv[0]) + 2));
	strcat(initCall, cwd);
	strcat(initCall, "/");
	strcat(initCall, argv[0]);

	/*check for arguments to program, assume first argument after program name is batchfile, ignore all others(for now)*/
	if(argc > 1){
		/*Check for existance of file*/
		if(access(argv[1], F_OK)==-1){
			puts("Bad filename or path");
		}else
			/*Check for read/execute access to file(commands being executed = execute access should be honored)*/
			if((access(argv[1], R_OK)==0)&&(access(argv[1], X_OK)==0)){
				freopen(argv[1], "r", stdin);
				noPrompt = 1;
			}else
				puts("Cannot open that file, you may not have access permissions to it.");
	}

	/*BEGIN infinite loop*///Looped input reading code based on strtokeg.c
	while(!feof(stdin))
	{
		/*display current PID if debug is set*/
		if(DEBUG)
			printf("Process: %d \n", (int)getpid());

		/*get user input unless batchfile used*/
		if(!noPrompt)
			fputs(prompt, stdout);
	


		if(fgets(buf, MAX_BUFFER, stdin))	//read a line
		{
			noWait = 0;	//reset background flag in case it was used in last command
			/*tokenize input into args array*/

			arg = args;
			*arg++ = strtok(buf, DELIMITERS);	//tokenize first argument
			while((*arg++ = strtok(NULL, DELIMITERS)));	//last entry will be null and loop will exit
			

			//alias implementation needs an alias list and a way to push aliased commands to args[]


			if(args[0])
			{
				
				debug("First Arg:");
				debug(args[0]);
				if(!strcmp(args[0],"clr")){
					if(clr(args) == -1)
						debug("Clear error");
				}else{	//else1
				if(!strcmp(args[0], "cd")){
					if(cd(args) == -1)
						debug("Cd error");
				}else{	//else2
				if(!strcmp(args[0], "dir")){
					if(dir(args) == -1)
						debug("Dir error");
				}else{	//else3
				if(!strcmp(args[0], "environ")){
					if(envrn(args) == -1)
						debug("Env error");
				}else{	//else4
				if(!strcmp(args[0], "echo")){
					if(echo(args) == -1)
						debug("Echo error");
				}else{	//else5
				if(!strcmp(args[0], "help")){
					if(help(args) == -1)
						debug("Help error");
				}else{	//else6
				if(!strcmp(args[0], "pause")){
					if(pauseShell(args) == -1)
						debug("Pause error");
				}else{	//else7
				if(!strcmp(args[0], "quit")){
					if(quit(args) == -1)
						debug("Quit error");
				}else{  //else8
				if(!strcmp(args[0], "alias")){
					fputs("Sorry, alias has not yet been implemented\n", stdout);
					debug("alias not implemented yet");
				}else{	//else9
				if(externalCall(args)== -1)
					debug("Externalcall error");
				}//else9
				}//else8
				}//else7
				}//else6
				}//else5
				}//else4
				}//else3
				}//else2
				}//else1
				
			}//if(args[0])
		}//if(fgets(buf, MAX_BUFFER, stdin))
	}
	/*END infinite loop*/
	return -1;	//this point should not be reachable
}
/*END main driver function*/


/*BEGIN command functions*/

//#i. cd <directory> - change current default directory to <directory>
//#If the <directory> argument is not present, report the current
//#directory. If the directory does not exist an appropriate error
//#should be reported. This command should also change the PWD 
//#environment variable.
int cd(char **args){

	/*check for additional arguments, if none, print current directory*/
	if(args[1] == NULL){
		 /*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
		switch(childPID = fork()){
			case -1:
				debug("Fork error");
				break;
			case 0:
				execlp("pwd", "pwd", NULL);
				exit(-1);
			default:
				if(!noWait)
					waitpid(childPID, &status, WUNTRACED);
		}
	return childPID;	//return success or failure
	}
	/*Execution should not reach this point unless other arguments were passed*/

	/*check target directory*/
	if(chdir(args[1]) == 0){
		/*set PWD variable to new directory*/
		setPrompt();

		debug("Allocating space for newPWD");
		debug("Current cwd:");
		debug(cwd);

		/*allocate memory needed for newPWD, store full string, then push to environment variables*/
		/*cwd should contain five fewer characters than required for the new PWD at this point*/
		newPWD = (char *)malloc(sizeof(char) * (strlen(cwd) +5));
		debug("Concatenating \'PWD=\' to newPWD");
		strcat(newPWD, "PWD=");
		debug("Concatenating cwd to newPWD");
		strcat(newPWD, cwd);
		putenv(newPWD);		//oh god i think this is a memory leak...gotta fix that someday
	}else{
		/*Executes if target directory does not exist or is not accessible*/
		puts("That directory does not exist or cannot be accessed.");
		return -1;	//return error
	}
	
	return 0;	//return success

}

//#ii. clr - clear the screen
int clr(char **args){
	debug("Clr called");
	debug("Begin clr fork");
	
	/*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
	switch(childPID = fork()){
		case -1:
			debug("Fork Error");
			break;
		case 0:
			debug("Clr fork case 0");
			execlp("clear", "clear", NULL);
		default:
			if(!noWait)
				waitpid(childPID, &status, WUNTRACED);
	}
	debug("End of clr fork");
	return childPID;
}

//#iii. dir <directory> - list the contents of <directory>
int dir(char **args){
	int i = 2;
	numArgs = 0;
	debug("Dir called");
	
	/*count the arguments*/
	while(args[++numArgs] != NULL);

	/*allocate space for, and build the new set of arguments*/
	out = (char**)malloc(sizeof(char*) * (numArgs + 1));
	out[0] = (char *)malloc(sizeof(char) * 3);
	strcpy(out[0], "ls");
	out[1] = (char *)malloc(sizeof(char) * 4);
	strcpy(out[1], "-al");
	out[2] = NULL;	//in case of no additional arguments
	
	debug("Setting Args. List to follow:");
	debug(out[0]);
	debug(out[1]);
	/*set the new list of arguments, terminated with a NULL pointer*/
	while(i < numArgs+1){
		out[i] = (char *)malloc(sizeof(char) * ((strlen(args[i-1]))+1));
		strcpy(out[i], args[i-1]);
		debug(out[i]);
		++i;
		out[i] = NULL;
	}

	 /*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
	switch(childPID = fork()){
		case -1:
			debug("Fork error");
			break;
		case 0:
			debug("Dir fork case 0");
			debug("Checking for I/O Redirection");
			IORedirect(out, 1);
			debug("I/O Redirection check complete. Executing call.");
			execvp("ls", out);
			exit(-1);
		default:
			if(!noWait){
				debug("Dir wait");
				waitpid(childPID, &status, WUNTRACED);
			}
	}
	free(out);
	return childPID;
}

//#iv. environ - list all the environment strings
int envrn(char **args){
	debug("Environ called");
	env = environ;

	 /*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
	switch(childPID = fork()){
		case -1:
			debug("Fork Error");
			break;
		case 0:
			IORedirect(args, 1);
			while(*env)
				puts(*env++);
			exit(0);
		default:
			if(!noWait)
				waitpid(childPID, &status, WUNTRACED);
	}
	return childPID;
}

//#v. echo <comment> - display <comment> on the display followed by
//#a new line (multiple spaces/tabs may be reduced to a single space)
int echo(char **args){
	int i = 1;
	debug("Echo called");

	 /*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
	switch(childPID = fork()){
		case -1:
			debug("Fork error");
			break;
		case 0:
			IORedirect(args, 1);
			/*print all arguments on a single line, each followed by a single space.*/
			while(args[i] != NULL){
				printf("%s ", args[i]);
				++i;
			}
			printf("\n");	//end the line with a newline character
			exit(0);
		default:
			if(!noWait)
				waitpid(childPID, &status, WUNTRACED);
	}
	return childPID;
}

//#vi. help - display the user manual using the more filter
int help(char **args){
	debug("Help called");

	 /*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
	switch(childPID = fork()){
		case -1:
			debug("Fork error");
			break;
		case 0:
			IORedirect(args, 1);
			/*Display the readme file using the more filter*/
			execlp("more", "more", readme, NULL);
			exit(-1);
		default:
			if(!noWait)
				waitpid(childPID, &status, WUNTRACED);
	}
	return childPID;
}

//#vii. pause - pause operation of the shell until 'Enter' is pressed
int pauseShell(char **args){
	debug("Pause called");

	puts("Execution paused, press ENTER to resume");
	while(getchar() != '\n');
	return 0;
}

//#viii. quit - quit the shell
int quit(char **args){
	debug("Quit called");
	exit(0);
}

/*BEGIN externalCall function--passes user input to external shell. Used only if user inputs a command not implemented in this shell*/
int externalCall(char **args){
	
	/*check for the background execution argument '&'*/
	BGCheck(args);

	 /*fork/exec statement--case 0 is the child process, default is the parent, -1 is an error*/
	switch(childPID = fork()){
		case -1:
			debug("Fork error");
			break;
		case 0:
			debug("Externalcall fork case 0");
			
			/*allocate memory for, build, and push PARENT environment variable (used a previously made char* to save space*/
			shell = (char*)malloc(sizeof(char) *(strlen(initCall) + 8));
			strcat(shell, "PARENT=");
			strcat(shell, initCall);
			putenv(shell);

			/*check for I/O Redirect, both in and out(all other calls currently used only check for output redirection)*/
			IORedirect(args, 2);
			execvp(args[0], args);
			exit(-1);	//exit with error code, shouldn't ever reach this line
		default:
			if(!noWait){
				debug("Externalcall wait");
				waitpid(childPID, &status, WUNTRACED);
			}
	}
	return childPID;
}
/*END externalCall function*/

/*BEGIN BGCheck function-- check for background execution symbol '&'*/
void BGCheck(char **args){
	int i, j;
	for(i = 0; args[i] != NULL; i++){
		if(strcmp(args[i], "&") == 0){
			noWait = 1;
			/*remove the '&' from the arguments*/
			for(j = i; args[j] != NULL; j++)
				args[j] = args[j+1];
		}
	}
}
/*END BGCheck function*/

/*BEGIN IORedirect function -- when called from within the 0 case of switch(fork()), sets up proper I/O redirection*/
//if mode integer is not 2, function will not check for redirection of stdin.  more modes can be added,
//but that functionality will not be implemented until after final submission of project.
void IORedirect(char **args, int mode){
	int i, j;
	int inFound = 0;
	int outFound = 0;

	/*iterate through all args looking for redirection tokens.  Remove tokens and their arguments when found*/
        for(i = 0; (args[i+1] != NULL)&&(args[i]!=NULL); i++){

		/*Input Redirection, only used if mode 2 is passed to function*/
                if((strcmp(args[i], "<") == 0)&& (!inFound) && (mode == 2)){
			//set instream (should not occur for dir, environ, echo, or help)
			if(access(args[i+1], R_OK) == 0){
				freopen(args[i+1], "r", stdin);
				/*remove input redirection token and its argument*/
				for(j = i; args[j+1] != NULL; j++)
					args[j] = args[j+2];
			}else{
				/*remove bogus input redirection token*/
				for(j = i; args[j] != NULL; j++)
					args[j] = args[j+1];
				puts("Could not open specified input file");
			}
			inFound = 1;
		}
		
		/*Prevent attempt to access null pointers...it's an ugly solution, i know, but i was pressed for time and this works for now.*/
		if((args[i] != NULL)&&(args[i+1]!=NULL)){

			/*Truncated output redirection*/
               		if((strcmp(args[i], ">") == 0)&&(!outFound)){
				//Check for existance of file.  If file does not exist, create it
				debug("Truncate found. Checking file existance.");
				if(access(args[i+1], F_OK)==-1){
					fopen(args[i+1], "w");
					debug("File does not exist. Creating File");
				}

				//truncate file and open it for writing
				if(access(args[i+1], W_OK) ==0){
					debug("Truncating file and appending to empty file.");
					freopen(args[i+1], "w", stdout);
					debug("Begin appending to file");
					/*remove truncate token and filename*/
					for(j = i; args[j+1] != NULL; j++)
						args[j] = args[j+2];
					debug("End for statement.");
				}else{
					/*remove bogus truncate token*/
					for(j = i; args[j] != NULL; j++)
						args[j] = args[j+1];
					puts("Could not open specified output file");
				}
				outFound = 1;
				debug("outFound = 1, proceeding with execution");
			}
		}

		/*again...ugly but it works, same solution as above (line 502)*/
		if((args[i] != NULL)&&(args[i+1]!=NULL)){
			/*Appended output redirection*/
			if((strcmp(args[i], ">>") == 0)&&(!outFound)){
				debug("Append found. Checking file existance.");
			
				/*Check for existance of file. If file does not exist, create it*/
				if(access(args[i+1], F_OK) ==-1){
					fopen(args[i+1], "w");
					debug("File does not exist. Creating File");
				}
			
				/*open file for writing at end of file.*/
				if(access(args[i+1], W_OK) ==0){
					debug("Appending to end of file.");
					freopen(args[i+1], "a", stdout);
					/*remove append token and its argument*/
					for(j = i; args[j+1] != NULL; j++)
						args[j] = args[j+2];
				}else{
					/*remove bogus append token*/
					for(j = i; args[j] != NULL; j++)
						args[j] = args[j+1];
					puts("Could not open specified output file");
				}
				outFound = 1;
			}
		}
		debug("Past final if statement");
        }
	return;
}
/*END IORedirect function*/

/*BEGIN debug function--my personal function used in debuging unusual behavior, only prints if DEBUG flag is non-zero*/
void debug(char *err){	//consider adding priority flags as an argument, only print if priority <= DEBUG
	if(DEBUG)
		puts(err);
}
/*END debug function*/

/*BEGIN setPrompt function--sets the prompt variable to contain the prompt that will print to the command line to request input*/
void setPrompt(){
        size_t size = 25;       //half of starting buffer size for cwd...50 seems to work pretty well
        char *buf = NULL;       //buffer for calling getcwd
	debug("SetPrompt called");

        /*double size, try to store cwd in size characters, repeat until successful*/
        do{
                size *= 2;
                buf = (char *)malloc(sizeof(char) * size);
                cwd = getcwd(buf, size);
        }while(cwd == NULL);

	debug("Allocating space for prompt");
        /*store prompt, begin by allocating necessary space, then cat ~~> onto cwd. easily customized*/
        prompt = (char *)malloc(sizeof(char) *(strlen(cwd) + 4));       //cwd + prompt chars + null char
        strcat(prompt, cwd);
        strcat(prompt, "~~>");	//change this to change the prompt style
        return;
}
/*END setPrompt function*/

/*BEGIN sigHandler function -- catches various signals. we'll start with just SIGINT (CTRL+C)*/
void sigHandler(int signo){
	//cosider a switch statement instead
	if(signo == SIGINT){
		/*Bash SIGINT behavior: display ^C, go to  new line, re-print prompt.  does not kill bg procs*/
		debug("SIGINT Caught but not doing anything with it yet.\n");
		fputs("\n", stdout);
		fputs(prompt, stdout);
		fflush(stdout);
	}

}
/*END sigHandler function*/
