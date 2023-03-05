/* CS 352 -- Micro Shell!  
 *
 *   Sept 21, 2000,  Phil Nelson
 *   Modified April 8, 2001 
 *   Modified January 6, 2003
 *   Modified January 8, 2017
 *   
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "expand.h"
#include "builtin.h"

/* Constants */ 

#define LINELEN 1024

/* Prototypes */
char** arg_parse(char*line, int *argcptr);
void processline (char *line );

/* Shell main */


int main ()
{
  char   buffer [LINELEN];
  int    len;

  while (1) {
      /* prompt and get line */
    fprintf (stderr, "%% ");
    if (fgets (buffer, LINELEN, stdin) != buffer)
      break;

      /* Get rid of \n at end of buffer. */
    len = strlen(buffer);
    if (buffer[len-1] == '\n')
      buffer[len-1] = 0;

    /* Run it ... */
    processline (buffer);
  }

  if (!feof(stdin))
      perror ("read");

  return 0;		/* Also known as exit (0); */
}


/*  arg_parse  */

//arg_parse takes a string of args and processes it into a **char that execvp can use. It also takes the number of args and saves to an int.
char** arg_parse(char*line, int*argcptr){
  int argcount= 0;
  int seeArg = 0;//bool for if you are looking at an arg
  char *array[50];//temp storage for args pointers
  int quotes = 0; //bool to check for odd quotes
  int j = 0; //itterator for array
  
  // loop to process line
  char *i = line;
  while(*i != 0){
    //this section removes quotes and scoots the rest of the string to the left and fills empty space with NULL
    // I implimented the double quotes processing after implementing the args processing. please don't take off the grade.
    if(*i == '"'){
      while(*i == '"'){
	quotes = !quotes;
	char *Q = i;
	char *P = i;
	Q++;
	while(*Q != 0){
	  *P++ = *Q++;
	}
	*P = 0;
      }
    }
    //if pointing to space at the end of an arg, set equal to NULL
    //ignores space between quotes
    if(*i == ' ' && seeArg && !quotes){
      *i = 0;
      seeArg = 0;
    }
    //checks for first char of an arg and sets pointer and increments count
    else if(*i != ' ' && !seeArg){ 
	seeArg = 1;
	argcount++;
	array[j++] = i;
    }
    //iterate the iterator
    i++;
  }
  
  //malloc args
  char **args = malloc((argcount+1) * sizeof(char*) );
  //error for malloc 
  if(args == NULL){
    fprintf(stderr, "Failed to allocate memory of size %zu\n",((argcount+1) * sizeof(char*)));
    return args;
  }
  //error for odd number of quotes 
  if(quotes){
    fprintf(stderr,"Line input error\n");
    return args;
  }
  //set args = to array
  for(int i=0; i <argcount; i++){
    args[i]= array[i];
  }
  //set last pointer in args to NULL
  args[argcount] = NULL;
  *argcptr = argcount +1; //plus 1 for null pointer at end
  return args;
}

/*  processline  */
void processline (char *line)
{
    pid_t  cpid;
    int    status;
    int    narg; //number of arguments if there are no arguments do nothing
    char newLine[LINELEN];
    int expanded = expand(line,newLine,LINELEN);
    //printf("%s \n",newLine);
    if(expanded < 0)
      //probably should print an error here.
      return;
    char **args = arg_parse(newLine,&narg);
    if(narg > 0){
      //check for builtin here
      if(check_builtin(args, narg)){
        execute_builtin(args, narg);
        free(args);
        return;
      }
       //fork if we have args
       /* Start a new process to do the job. */
       cpid = fork();
       if (cpid < 0) {
	 /* Fork wasn't successful */
	 perror ("fork");
	 return;
       }
       /* Check for who we are! */
       if (cpid == 0) {
	 /* We are the child! */
	 execvp (args[0],args);
	 /* execvp reuturned, wasn't successful */
	 perror ("exec");
	 fclose(stdin);  // avoid a linux stdio bug
	 exit (127);
       }    
       /* Have the parent wait for child to complete */
       if (wait (&status) < 0) {
	 /* Wait wasn't successful */
	 perror ("wait");
	 free(args); //deallocate args
       }
     }else{
       free(args); //deallocate args
       return ;
     }
}
