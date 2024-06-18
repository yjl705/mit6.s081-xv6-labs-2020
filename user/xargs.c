#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


// 这道题首先要明白main只是看xargs后面的参数，也就是管道符号|之前的都属于标准输入但是不属于xargs的命令
// xargs做的事情就是把管道的输出作为自己的输入的参数的一部分来执行
// 做这道题一开始一直没懂，以为其他行的命令比如echo也需要我来执行，但实际上这里默认的是其他行已经执行完了
// 只需要把每一行读出来，然后把每一行的参数划分好，把参数和xargs的参数加起来给xargs的命令执行就行
// 每行执行一次

void run(char *func, char **args)
{
    if(fork() == 0) {
        exec(func, args);
        exit(0);
    }
    return;
}

int
main(int argc, char *argv[])
{
    char buf[2048];
    char *p = buf, *last_p = buf;
    char *argsbuf[128]; // 存储的是所有参数，包括xargs本身的和其他地方输入的
    char **args = argsbuf; // 用来负责参数的交互，其最终指向会是第一个从标准输入读取的参数


    for (int i = 1; i < argc; i++) {
        // printf(argv[i]);
        *args = argv[i]; // 先把xargs的参数存入args，同时也就是存到了argsbuf中
        args++;
    }
    // args最终指向的永远是第一个std读取的参数，不会变，所以需要有另一个可以变化的指针指向所有参数的末尾

    char **param_std = args; // 这个参数指向所有参数的末尾
    while(read(0, p, 1) != 0) {
        if(*p == ' ' || *p == '\n') {
            // while(*last_p == ' ' || *last_p == '\n') last_p++; //  跳过空格来读取下一个参数
            // 这里默认每个空格只有一个字符，不会出现连续多个空格或换行
            *p = '\0'; // 用'\0'分隔参数，程序读到这个标志就会认定是参数的结束
            *(param_std++) = last_p; // 这里相当于把新的参数读进去
            // param_std++; // 下一个位置用来存储下一个参数

            last_p = p+1;

			if(*p == '\n') {
				// 读入一行完成
				*param_std = 0; // 参数列表末尾用 null 标识列表结束
				run(argv[1], argsbuf); // 执行最后一行指令
				param_std = args; // 重置读入参数指针，准备读入下一行
			}
        }
        p++;
    }
    if (param_std != args) { // 意味着最后一行没有换行符
        *p = '\0'; // 用'\0'分隔参数，程序读到这个标志就会认定是参数的结束
        *(param_std++) = last_p; // 这里相当于把新的参数读进去
        // param_std++; // 下一个位置用来存储下一个参数
        *param_std = 0; // null 标志结束

        run(argv[1], argsbuf);

    }
    while(wait(0) != -1) {}; // 循环等待所有子进程完成，每一次 wait(0) 等待一个，-1代表没有子进程在运行
    exit(0);
}