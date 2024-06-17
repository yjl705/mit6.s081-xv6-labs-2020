#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  if(argc != 2) {
    fprintf(2, "Usage: sleep for x ticks...\n");
    exit(1);
  }
  char *s = argv[1];
  if('0' <= *s && *s <= '9') {
    int n = atoi(argv[1]);
    sleep(n);
  } else {
    fprintf(2, "Need to input an integer\n");
    exit(1);
  }

  exit(0);
}