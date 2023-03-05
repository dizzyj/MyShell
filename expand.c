#include <stdio.h>
#include <stdlib.h>
//char *orig, char *new, int newsize
int expand(char *orig, char *new, int newsize){
  char* i = orig;
  char* j = new;
  int counter = 0;
  
  if(*i == 0)
    return -1;
  while(*i != 0){
    //if we are overflowing new return error 
    if(counter >= newsize)
      return -2;
    //if you do not see a variable, just copy
    if(*i != '$'){
      *j++ = *i++;
      counter++;
    }//if you see a variabe, expand.
    else{
      char *temp = j;
      i++;
      if(*i != '{')
	      return -1;
    i++;
    while(*i != '}'){
	    if(*i == 0)
	      return -1;
	    *temp++ = *i++;
    }
    i++;
    *temp = 0;
    char *envar = getenv(j);
    if(envar == NULL)
	return -1;
      else{
	//put envar into newline
	while(*envar != 0){
    //check if overflow and return error
    if(counter >= newsize)
      return -2;
	  *j++ = *envar++;
    counter++;
	}	
      }
    }//end of big else
  }//end while
  *j = 0;
  return 0;
}
