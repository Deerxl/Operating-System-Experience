/*
设计一个按优先数调度算法实现处理器调度的程序。
[提示]：
(1)假定系统有5个进程，每个进程用一个PCB来代表。PCB的结构为：
·进程名——如P1~P5。
·指针——按优先数的大小把5个进程连成队列，用指针指出下一个进程PCB的首地址。
·要求运行时间——假设进程需要运行的单位时间数。
·优先数——赋予进程的优先数，调度时总是选取优先数大的进程先执行。
·状态——假设两种状态：就绪和结束，用R表示就绪，用E表示结束。初始状态都为就绪状态。
(2) 开始运行之前，为每个进程确定它的“优先数”和“要求运行时间”。通过键盘输入这些参数。
(3) 处理器总是选择队首进程运行。采用动态改变优先数的办法，进程每运行1次，优先数减1，要求运行时间减1。
(4) 进程运行一次后，若要求运行时间不等于0，则将它加入就绪队列，否则，将状态改为“结束”，退出就绪队列。
(5) 若就绪队列为空，结束，否则转到(3)重复。
要求能接受键盘输入的进程优先数及要求运行时间，能显示每次进程调度的情况，如哪个进程在运行，哪些进程就绪，就绪进程的排列情况。
*/

#include<iostream>
#include<string>
using namespace std;

#define PCBNUM 5

struct PCB
{
	string name;      //进程名
	int time;         //要求运行时间
	int priority;     //优先级
	char status;      //状态 R 就绪 E 结束
	struct PCB* next;
};

void SetPCB(PCB*& pcb, int i) 
{
	pcb->name = "P" + to_string(i);
	
	cout << "请为进程P" << i << "依次输入 进程优先数 和 要求运行时间：" << endl;
	try
	{
		cin >> pcb->priority >> pcb->time;
		if (pcb->priority < 0 || pcb->time < 0)
		{
			cout << "请按照要求输入数据！" << endl;
			exit(0);
		}
		cout << endl;
	}
	catch(exception e)
	{
		cout << "请按照要求输入数据！" << endl;
		exit(0);
	}

	pcb->status = 'R';
}

void SortPriority(PCB *&head)
{
	PCB* p, * pre, * q;
	p = head->next->next;
	head->next->next = NULL;
	while (p != NULL)
	{
		q = p->next;
		pre = head;
		while (pre->next != NULL && pre->next->priority > p->priority)
			pre = pre->next;
		p->next = pre->next;
		pre->next = p;
		p = q;
	}
}

bool Run(PCB*& head)
{
	PCB* pcb = new PCB;
	pcb = head->next;
	
	if (pcb->priority != 0)
	{
		pcb->priority = pcb->priority - 1;
	}
	
	pcb->time = pcb->time - 1;

	return pcb->time;
}

void Show(PCB*& head)
{
	PCB* h = head;
	PCB* pcb = h->next;
	cout << "Run:" << endl;
	cout << "\tName\t" << "Time\t" << "Priority\t" << "Status\t" << endl;
	cout << '\t' << pcb->name << '\t' << pcb->time << '\t' << pcb->priority << "\t\t" << pcb->status << '\t' << endl;

	cout << "Ready:" << endl;
	h = h->next;
	pcb = h->next;
	while (pcb != NULL)
	{
		cout << '\t' << pcb->name << '\t' << pcb->time << '\t' << pcb->priority << "\t\t" << pcb->status << '\t' << endl;
		h = h->next;
		pcb = h->next;
	}
	cout << endl;
}

int main()
{
	PCB* head = new PCB;
	head->next = NULL;
	
	int count = 0;
	
	for (int i = 1; i <= PCBNUM; i++)
	{
		PCB* pcb = new PCB;
		SetPCB(pcb, i);
		pcb->next = head->next;
		head->next = pcb;
	}
	
	while( head->next != NULL)
	{

		SortPriority(head);

		cout << "第" << count << "次运行" << endl;
		Show(head);

		if (head->next == NULL)
			return NULL;

		if (!Run(head))
		{
			head->next->status = 'E';
			head->next = head->next->next;
		}
		
		count++;
	}
	

	return 0;
}