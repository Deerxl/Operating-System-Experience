/*
可变分区管理方式下采用首次适应算法实现主存分配和回收
[提示]：
(1)	可变分区方式是按作业需要的主存空间大小来分割分区的。
当要装入一个作业时，根据作业需要的主存容量查看是否有足够的空闲空间，
若有，则按需分配，否则，作业无法装入。
假定内存大小为128K，空闲区说明表格式为：
·分区号——表示是第几个空闲分区；
·起始地址——指出空闲区的起始地址；
·长度——一个连续空闲区的长度；
(2)	采用首次适应算法分配回收内存空间。运行时，输入一系列分配请求和回收请求。
要求能接受来自键盘的空间申请及释放请求，能显示分区分配及回收后的内存布局情况。
*/

#include<iostream>
using namespace std;

#define MAXSIZE 128
#define UNASSIGNED 0
#define ASSIGNED 1

struct Partition
{
	int num = NULL;           //分区号
	int startAddr;     //起始地址
	int length;		   //分区长度
	int state;		   //状态         0 未分配 1 已分配
	int tasknum = NULL;       //作业名
	struct Partition* next;
};

bool Insert(Partition*& head, Partition*& newTast);
bool Recycle(Partition*& head, Partition*& deleteTask);


//显示内存布局情况 分区号：0表示非空闲区，1...n 为空闲区
void Show(Partition*& head)
{
	Partition* pHead = head;
	Partition* p = new Partition;
	cout << "************************空闲分区表****************************" << endl;
	cout << "分区号\t" << "起始地址\t" << "长度\t" << "状态\t" << "作业名" << endl;
	
	
	while (pHead->next != NULL)
	{
		p = pHead->next;
		cout << p->num << "\t" << p->startAddr << "\t\t" << p->length << "\t" << p->state << "\t" << p->tasknum << endl;
		
		pHead = pHead->next;
	}
	cout << "************************空闲分区表****************************" << endl;
}

//分配空间
void NewTask(Partition*& head)
{
	Partition* newTask = new Partition;
	cout << "请为申请空间的任务设置 任务名（int） 和 空间大小（int）：" << endl;

	cin >> newTask->tasknum >> newTask->length;
	if (newTask->tasknum < 0 || newTask->length < 0)
	{
		cout << "请按照要求输入数据!" << endl;
		exit(0);
	}
	if (Insert(head, newTask))
	{
		cout << "申请成功！" << endl;
	}
	else
	{
		cout << "申请失败！" << endl;
	}

}

//插入到分区表
bool Insert(Partition*& head, Partition*& newTask)
{
	Partition* phead = head;
	Partition* p = new Partition;
	p = phead->next;
	while (p != NULL)
	{
		if (p->length > newTask->length && (p->state == UNASSIGNED))
		{
			newTask->next = p;
			phead->next = newTask;

			newTask->startAddr = p->startAddr;
			newTask->state = ASSIGNED;
			newTask->num = NULL;

			p->startAddr += newTask->length;
			p->length -= newTask->length;

			return true;
		}
		else if (p->length == newTask->length && (p->state == UNASSIGNED))
		{
			p->state = ASSIGNED;
			p->tasknum = newTask->tasknum;

			while (phead->next->next != NULL)
			{
				phead = phead->next;
				p = head->next;
				if (p->state == UNASSIGNED)
				{
					p->num -= 1;
				}
				
			}

			return true;
		}
		phead = phead->next;
		p = phead->next;
	}

	return false;
}

//回收空间
void DeleteTask(Partition*& head)
{
	Partition* deleteTask = new Partition;
	cout << "请输入要释放空间的任务名：" << endl;

	cin >> deleteTask->tasknum;
	if (deleteTask->tasknum < 0)
	{
		cout << "请按照要求输入数据!" << endl;
		exit(0);
	}
	if (Recycle(head, deleteTask))
	{
		cout << "释放成功！" << endl;
	}
	else
	{
		cout << "释放失败！" << endl;
	}
}

bool Recycle(Partition*& head, Partition*& deleteTask)
{
	Partition* phead = head;
	Partition* p = new Partition;
	p = phead->next;
	Partition* pre = NULL;
	while (p != NULL)
	{
		if (p->tasknum == deleteTask->tasknum && p->state == ASSIGNED)
		{
			if (pre == NULL)
			{
				p->num = 1;
			}
			else
			{
				p->num = pre->num + 1;
			}

			p->state = UNASSIGNED;
			while (phead->next->next != NULL)
			{
				phead = phead->next;
				p = phead->next;
				if (p->state == UNASSIGNED)
				{
					p->num += 1;
				}
				
			}
			return true;
		}
		if (p->state == UNASSIGNED)
		{
			pre = p;
		}		
		phead = phead->next;
		p = phead->next;
	}
	return false;
}

int main()
{
	Partition* head = new Partition;
	Partition* p = new Partition;
	p->num = 1;
	p->length = MAXSIZE;
	p->startAddr = 0;
	p->state = UNASSIGNED;
	head->next = p;
	p->next = NULL;

	int n = 0;
	while (1)
	{
		Show(head);
		cout << "输入：\n\t" << "1\t申请空间" << "\n\t2\t释放空间"<<"\n\t其他\t退出" << endl;
		cin >> n;
		switch (n)
		{
		case 1:
			NewTask(head);
			break;
		case 2:
			DeleteTask(head);
			break; 
		default:
			delete p, head;
			exit(0);
			break;
		}
	}
	return 0;
}