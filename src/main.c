
#include <stdlib.h>
#include <stdio.h>

#include "sys_info.h"
#include "pid_parser.h"


int main() {

  int err = init_sys_info();

  if (err != 0) {
    printf("Its fucked");
    exit(0);
  }


  double version = get_disk_storage();

  printf("\n%f\n\n", version);
  //free(version);
  

  //-----pid_parser tests-----
  printf("Start of pid_parser tests\n");
  char * buf[50];
  *buf = get_name(197020);
  printf("%s\n", *buf);
}
