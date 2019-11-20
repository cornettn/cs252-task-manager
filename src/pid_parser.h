#ifndef PID_PARSER_H
#define PID_PARSER_H


/* Initialization */



/* Functions */

char *get_name(int pid);
char *get_user(char *pid);
char *get_state(char *pid);
char *get_mem(char *pid);
char *get_vmem(char *pid);
char *get_rmem(char *pid);
char *get_smem(char *pid);
char *get_cputime(char *pid);
char *get_start(char *pid);



#endif
