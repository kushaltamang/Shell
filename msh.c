/*Mohit Tamang
1001552822
*/
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size
#define MAX_HISTORY 15 			// max number of commands that the history command will list
#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports five arguments

int main()
{
	int hsize =0;
	int numpid =0;
  int pids[15];
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  while( 1 )
  {
  	// Print out the msh prompt
		printf ("msh> ");
    // Read the command from the commandline.  The
  	// maximum command that will be read is MAX_COMMAND_SIZE
  	// This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];
    
    pids[numpid++] = getpid();
    int   token_count = 0;                                                                                       
    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;                                                                                                
    char *working_str  = strdup( cmd_str );                
    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      	token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      	if( strlen( token[token_count] ) == 0 )
      	{
        	token[token_count] = NULL;
      	}
        token_count++;
    }
    //if user enters with blank input, re-run the while loop
  	if(token[0]==NULL)
  	{
  	  	continue;
  	}
    char hist[MAX_HISTORY][30];
    strcpy(hist[hsize++],cmd_str);
    //handling !n command, where n is the nth command in history
    int n=0;
    if(token[1]==NULL && token[0][0]=='!')
    {
      if((int)(token[0][2])==0)
    	{
    		n = (int)(token[0][1] - 48);//in ASCII, numbers start from 48
    	}
    	else//handles if n>=10(two char numbers)
    	{
    		n= atoi(strcat(&token[0][1],"\0"));
    	}
    	
    	if(n>=0 && n<=(hsize-2))
    	{
    		char *arg;
	  		int count = 0;
	  		char* separr = hist[n];
	  		while ((( (arg = strsep(&separr, WHITESPACE )) != NULL) && 
              (count<MAX_NUM_ARGUMENTS)))
	  		{
	  			token[count] = strndup( arg, MAX_COMMAND_SIZE );
	  			if( strlen( token[count] ) == 0 )
      			{
        			token[count] = NULL;
      			}
        		count++;
	  		}
    	}		
    }
  	if(strcmp(token[0],"showpids")==0)
    {
      int p;
      for(p=0;p<numpid;p++)
      {
        printf("%d: %d\n",p,pids[p]);
      }
      continue;
    }
  	//exit with zero status if "quit" or "exit" entered in shell
  	if(strcmp(token[0],"quit")==0 || strcmp(token[0],"exit")==0)
  	{
	  	exit(0);
  	} 
  	if(strcmp(token[0],"cd")==0)
    {
		  if(token[1]==NULL)
		  {
			 chdir(getenv("HOME"));
		  }
		  chdir(token[1]);
		  continue;
	 }
	 int k;
	 if(strcmp(token[0],"history")==0)
	 {
    for(k=0;k<hsize;k++)
		{
			printf("%d: %s",k,hist[k]);
			//printf("\n");
		}
		continue;
	}
  	//fork a child and a parent process
  	//child executes the commands in a different process
	pid_t pid = fork();
	if(pid < 0)
	{
	  	printf("Fork error");
	}
	//child process
	if(pid == 0)
	{
    //pids[numpid++] = getpid();
	  int res = execvp(token[0], token);
	  if(n>(hsize-2))
	  {
	  	printf("Command not in history.\n");
	  }
	  else
	  {
	  	if(res==-1)
	  	{
	  		printf("%s: Command not found.\n",token[0]);
	  	}	
	  }
	 exit(0);        
	}
	wait(NULL);
  free( working_root );
  }
  return 0;
}

