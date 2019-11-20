#include <stdlib.h>
#include <stdio.h>

#include "pid_parser.h"


/* Defines? */


/* Global Variables? */


/* Initialize? */


/*
 * Minimum requirements for the Process Information tab
 *   - Table with options to:
 *       - Display list of all running processes
 *       - Display processes only owned by current user
 *       - Display the list in tree format
 *   - This list should be updated at a fixed interval OR by using a refresh button
 *   
 *   - For each individual process you should have the ability to:
 *       - Stop
 *       - Continue
 *       - Kill
 *       - List memory maps
 *       - List open files 
 *   
 *   - Each individual process should have a window that shows:
 *       - Process Name
 *       - User
 *       - State
 *       - Memory
 *       - Virtual memory
 *       - Resident memory
 *       - Shared memory
 *       - CPU time
 *       - Date/time started
 */



/* -----Process Listing Functions----- */








 
/* ------Individual Process Parsing Functions----- */

/* Add comment */
char *get_name(int pid){
  char buf[50];
  sprintf(buf, "%d", pid);

  char path[50];
  sprintf(path, "/proc/%s/stat", buf);

  FILE * fp = fopen(path, "r");

  /* If fp is NULL, file could not be opened */
  if(fp == NULL){
    printf("Could not open file\n");
    return NULL;
  }

  char *name = malloc(sizeof(char) * 100);
  fscanf(fp, "%*d %s", name);

  fclose(fp);

  return name;
}

/* Add comment */
char *get_user(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_state(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_mem(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_vmem(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_rmem(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_smem(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_cputime(char *pid){
  //TODO
  return NULL;
}

/* Add comment */
char *get_start(char *pid){
  //TODO
  return NULL;
}




















/* End pid_parser.c */
