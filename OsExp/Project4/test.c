/*
编制一段程序，使用系统调用fork( )创建两个子进程，这样在此程序运行时，在系统中就有一个父进程和两个子进程在活动。
每一个进程在屏幕上显示一个字符，其中父进程显示字符A，子进程分别显示字符 B和字符C。
试观察、记录并分析屏幕上进程调度的情况。
[提示]：
（1）可用fork()系统调用来创建一个新进程。
系统调用格式：pid=fork()
fork()返回值意义如下：
=0：若返回值为0，表示当前进程是子进程。
>0：若返回值大于0，表示当前进程是父进程，返回值为子进程的pid值。
-1：若返回值小于0，表示进程创建失败。
如果fork()调用成功，它向父进程返回子进程的pid，并向子进程返回0，即fork()被调用了一次，但返回了两次。
此时OS在内存中建立一个新进程，所建的新进程是调用fork()父进程的副本，称为子进程。
子进程继承了父进程的许多特性，并具有与父进程完全相同的用户级上下文。父进程与子进程并发执行。
（2）编译和执行的方法：
gcc  源文件名 -o 执行文件名
最后，在shell提示符下输入：   ./执行文件名
 就可执行该文件。
*/

#include<stdio.h>
#include<unistd.h>

int main()
{
	pid_t pid1, pid2;
	if ((pid1 = fork()) < 0)
	{
		printf("fork error\n");
		return -1;
	}
	else if (pid1 == 0)     //子进程1
	{
		printf("child1: B\n");
		return 0;
	}
	if ((pid2 = fork()) < 0)
	{
		printf("fork error\n");
		return -1;
	}
	else if (pid2 == 0)
	{
		printf("child2: C\n");
		return 0;
	}

	wait(pid1, NULL, 0);
	wait(pid2, NULL, 0);
	printf("parent: A\n");
	return 0;
}

