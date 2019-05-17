#include<iostream>
using namespace std;

#define PRODUCER "producer"
#define CONSUMER "consumer"
#define PRODUCTS 10

#define PRODUCE 0
#define P 1
#define PUT 2
#define V 3
#define GOTO 4
#define GET 5
#define CONSUME 6
#define NOP 7

struct PCB
{
	string name;	//进程名
	string state;		//状态  运行态、就绪态、等待态和完成态
	string reason;	//等待原因  等待信号量s1或s2
	int breakPoint;	//断点信息  一旦进程再度占有处理器则从断点位置继续运行；处于完成状态，进程执行结束
	struct PCB* next;
};

string states[4] = { "run", "ready", "block", "done" };
int in = 0, out = 0;		//生产者指针 消费者指针
int x = 1;          //取出生产/消费的字符
int products[PRODUCTS];	 //10件产品
int product = 0;
int s1, s2;		 //信号量
int PC = 0;		//指令计数器，假设模拟的指令长度为1，每执行一条模拟指令后，PC加1，提取出下一条指令地址。

int PA[5] = { PRODUCE, P, PUT, V, GOTO };	//PA[i] PA[PC] 存放 生产者 程序中的一条模拟指令执行的入口地址
int SA[5] = { P, GET, V, CONSUME, GOTO };	//SA[i] SA[PC] 存放 消费者 程序中的一条模拟指令执行的入口地址
int i = -1;      //当前指令

PCB* producer, * consumer, * pcbNow; //生产者 消费者 现行进程

//初始化工作包括对信号量s1、s2赋初值，对生产者、消费者进程的PCB初始化。初始化后转向处理调度程序
void Init()
{
	cout << "init" << endl;

	s1 = 10, s2 = 0;

	producer = new PCB;
	producer->name = PRODUCER;
	producer->breakPoint = -1;
	producer->state = states[1];
	producer->next = NULL;

	consumer = new PCB;
	consumer->name = CONSUMER;
	consumer->breakPoint = -1;
	consumer->state = states[1];
	consumer->next = NULL;

	pcbNow = new PCB;
	PC = 0;
}

//将信号量s减去1，若结果小于0，则执行原语的进程被置成等待信号量s的状态。
void p()
{
	if (pcbNow == producer)
	{
		cout << "PRODUCER - P" << endl;
		if (--s1 < 0)
		{
			pcbNow->state = states[2];
			pcbNow->reason = "s1";
			pcbNow->breakPoint = i;
		}
		else
		{
			pcbNow->state = states[1];
		}
	}
	else
	{
		cout << "CONSUMER - P" << endl;
		if (--s2 < 0)
		{
			pcbNow->state = states[2];
			pcbNow->reason = "s2";
			pcbNow->breakPoint = i;
		}
		else
		{
			pcbNow->state = states[1];
		}
	}
}

//将信号量s加1，若结果不大于0，则释放一个等待信号量s的进程。
void v()
{
	if (pcbNow == producer)
	{
		cout << "PRODUCER - V" << endl;
		if (++s2 < 0 && consumer->state == states[3])
		{
			consumer->state = states[1];
		}
		pcbNow->state = states[1];
	}
	else
	{
		cout << "CONSUMER - V" << endl;
		if (++s1 < 0 && producer->state == states[3])
		{
			producer->state = states[1];
		}
		pcbNow->state = states[1];
	}
}

// x:=B[out];  out: =(out+1) mod 10
void Put()
{
	if (product >= 10)
	{
		cout << "PRODUCER - Put: CANNOT PUT" << endl;
		pcbNow->state = states[3];
		i--;
		return;
	}
	cout << "PRODUCER - Put" << endl;
	product += 1;
	products[in] = x;
	in = (in + 1) % PRODUCTS;
}

// x:=B[out];  out: =(out+1) mod 10
void Get()
{
	if (product <= 0)
	{
		cout << "CONSUMER - Get: CANNOT Get " << endl;
		i--;
		return;
	}
	else
	{
		cout << "CONSUMER - Get" << endl;
		product -= 1;
		x = products[out];
		out = (out + 1) % PRODUCTS;
	}

}

//输入一个字符放入C中
void Produce()
{
	cout << "PRODUCER - Produce " << x << endl;
	in += 1;
}

//打印或显示x中的字符
void Consume()
{
	cout << "CONSUMER - Consume " << x << endl;
}

//GOTO L	PC: =L
void Goto()
{
	if (pcbNow == producer)
	{
		cout << "PRODUCER - Goto" << endl;
	}
	else
	{
		cout << "CONSUMER - Goto" << endl;
	}

	i = -1;
}

//NOP	空操作
void Nop()
{
	;
}

//模拟处理器指令执行程序：按“指令计数器”PC之值执行指定的指令，且PC加1指向下一条指令。
void Excute()
{
	i++;
	int j = (pcbNow == producer) ? PA[i] : SA[i];
	switch (j)
	{
	case 0:
		Produce(); 
		pcbNow->state = states[1];
		break;
	case 1:
		p();
		break;
	case 2:
		Put(); 
		pcbNow->state = states[1];
		break;
	case 3:
		v();
		break;
	case 4:
		Goto();
		pcbNow->state = states[1];
		break;
	case 5:
		Get(); 
		pcbNow->state = states[1];
		break;
	case 6:
		Consume();
		pcbNow->state = states[1];
		break;
	case 7:
		Nop(); pcbNow->state = states[1];
		break;
	}
}

/*处理器调度程序：
在计算机系统中，进程并发执行时，任一进程占用处理器执行完一条指令后就有可能被打断而让出处理器由其它进程运行。
故在模拟系统中也类似处理，每当执行一条模拟的指令后，保护当前进程的现场，让它成为非运行态，
由处理器调度程序按随机数再选择一个就绪进程占用处理器运行。
*/
void Processor()
{
	while (1)
	{
		PC++;
		pcbNow->breakPoint = i;
		if (product == 10)
		{
			producer->state = states[3];
			cout << "end" << endl;
			exit(0);
		}
		if (producer->state == states[1] && consumer->state == states[1])
		{
			pcbNow = ((rand() % 2) == 0) ? producer : consumer;

		}
		pcbNow->state = states[0];
		i = pcbNow->breakPoint;

		Excute();

	}
}

int main()
{

	Init();
	Processor();

	return 0;
}
