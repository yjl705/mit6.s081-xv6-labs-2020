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
        close(left[0]); // the read is for parent only, no use in child
        close(right[1]); // the right is for parent only, no use in child
        primes(right);
    }else{
        close(right[0]); // the read is for child only
        int buf;
        while(read(left[0], &buf, sizeof(buf)) != 0) {
            if (buf % p != 0) {
                write(right[1], &buf, sizeof(buf));
            }
        }
        close(right[1]); // end the write
        wait(0);
        exit(0);
    }
    
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    // 差别对待要在if分支里做，如果在外面就关闭了某个文件描述符，会导致child也受到同样影响
    if(fork() == 0) {
        close(p[1]); // the write is for parent only
        primes(p);
        exit(0);
    } else {
        close(p[0]); // the read is for child only
        for(int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }
    wait(0);
    exit(0);
}