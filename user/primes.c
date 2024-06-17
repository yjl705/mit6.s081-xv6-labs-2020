#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes(int left[2]){
    int p;
    if(read(left[0], &p, sizeof(p)) == 0) {
        exit(0);
    }
    // 1st read number must be prime
    printf("prime %d\n",p);

    int right[2];
    pipe(right);

    if(fork()==0) {
        close(left[0]);
        close(right[1]);
        primes(right);
    }else{
        close(right[0]);
        int buf;
        while(read(left[0], &buf, sizeof(buf)) != 0) {
            if (buf % p != 0) {
                write(right[1], &buf, sizeof(buf));
            }
        }
        close(right[1]);
        wait(0);
        exit(0);
    }
    
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if(fork() == 0) {
        close(p[1]);
        primes(p);
        exit(0);
    } else {
        close(p[0]);
        for(int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }
    wait(0);
    exit(0);
}