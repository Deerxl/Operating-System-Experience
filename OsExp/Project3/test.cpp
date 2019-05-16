/*
用位示图管理磁盘存储空间
[提示]：
(1) 为了提高磁盘存储空间的利用率，可在磁盘上组织成链接文件、索引文件，
这类文件可以把逻辑记录存放在不连续的存储空间。
为了表示哪些磁盘空间已被占用，哪些磁盘空间是空闲的，可用位示图来指出。
位示图由若干字节构成，每一位与磁盘上的一块对应，“1”状态表示相应块已占用，“0”状态表示该块为空闲。
位示图的形式与实习二中的位示图一样，但要注意，对于主存储空间和磁盘存储空间应该用不同的位示图来管理，绝不可混用。
(2) 申请一块磁盘空间时，由分配程序查位示图，找出一个为“0”的位，计算出这一位对应块的磁盘物理地址，且把该位置成占用状态“1”。

假设现在有一个盘组共8个柱面，每个柱面有2个磁道（盘面），每个磁道分成4个物理记录。

那么，当在位示图中找到某一字节的某一位为“0”时，这个空闲块对应的磁盘物理地址为：
柱面号=字节号
磁道号= 位数 / 4
物理记录号= 位数 % 4
(3) 归还一块磁盘空间时，由回收程序根据归还的磁盘物理地址计算出归还块在位示图中的对应位，把该位置成“0”。
按照（2）中假设的盘组，归还块在位示图中的位置计算如下：
字节号=柱面号
位数=磁道号 *4+物理记录号
(4) 设计申请磁盘空间和归还磁盘空间的程序。
要求能接受来自键盘的空间申请及释放请求，要求能显示或打印程序运行前和运行后的位示图；
分配时把分配到的磁盘空间的物理地址显示或打印出来，归还时把归还块对应于位示图的字节号和位数显示或打印出来。
*/

#include <iostream>
using namespace std;

#define CYLINDER 8
#define MaxSize 64
#define RECORD 4
#define FREE 0
#define OCCUPIED 1

int bitMap[CYLINDER][CYLINDER];

struct Task
{
	int name = -1;	//任务名
	int need = -1;	//请求数
	int x[MaxSize];	//存储柱面号（byte）
	int y[MaxSize];	//存储磁道号和物理地址（bit）
	struct Task* next;
};

//获取剩余磁盘空间数量
int GetRest()  
{
	int rest = 0;
	for (int i = 0; i < CYLINDER; i++)
	{
		for (int j = 0; j < CYLINDER; j++)
		{
			if (bitMap[i][j] == FREE)
			{
				rest++;
			}
		}
	}
	return rest;
} 

//检查分配的任务名是否重名
bool checkTaskName(Task*& head, int name)
{
	Task* phead = head;
	Task* task = new Task;
	task = phead->next;
	if (task != NULL)
	{
		if (task->name == name)
		{
			return false;
		}
		phead = phead->next;
		task = phead->next;
	}
	return true;
}

//分配磁盘空间
bool Assign(Task *&head, int name, int need)
{
	if (need <= GetRest() && checkTaskName(head, name))
	{
		Task* task = new Task;
		task->next = head->next;
		head->next = task;

		task->name = name;
		task->need = need;

		int m = 0;
		cout << "柱面\t磁道\t物理记录" << endl;
		for (int i = 0; i < CYLINDER; i++)
		{
			for (int j = 0; j < CYLINDER; j++)
			{
				if (bitMap[i][j] == FREE)
				{
					if (need <= 0)
					{
						return true;
					}

					bitMap[i][j] = OCCUPIED;
					task->x[m] = i;
					task->y[m] = j;
					cout << i << "\t" << j / RECORD << "\t" << j % RECORD << endl;
					m++;
					need--;
				}
			}
		}
	}
	return false;
}

//回收磁盘空间
bool Return(Task *&head, int name)
{
	Task* phead = head;
	Task* task = new Task;
	task = phead->next;

	int byte, bit;

	while (task != NULL)
	{
		if (name == task->name)
		{
			cout << "字节号\t" << "位数\t" << endl;
			for (int i = 0; i < task->need; i++)
			{
				byte = task->x[i];
				bit = task->y[i];
				bitMap[byte][bit] = FREE;
				cout << byte << "\t" << bit << endl;
			}
			phead->next = task->next;
			return true;
		}
		phead = phead->next;
		task = phead->next;
	}
	return false;
}

//显示位视图
void Show()
{
	cout << "********************************位视图********************************" << endl;
	for (int i = 0; i < CYLINDER; i++)
	{
		cout << "\t" << i;
	}
	cout << "\n********************************************************************" << endl;

	for (int i = 0; i < CYLINDER; i++)
	{
		cout << i << "  *"<<"\t";
		for (int j = 0; j < CYLINDER; j++)
		{
			cout << bitMap[i][j] << "\t";
		}
		cout << endl;
	}
	cout<< "********************************************************************" << endl;
}

int main()
{
	Task* head = new Task;
	head->next = NULL;

	int n;
	int assignName = 0, need = 0;
	int returnName = 0;

	while (1)
	{
		Show();
		cout << "输入：\n\t" << "1\t分配" << "\n\t2\t回收" << "\n\t其他\t退出" << endl;
		cin >> n;
		switch (n)
		{
		case 1:
			
			cout << "请依次输入需要分配的任务名（int）、空间大小：" << endl;
			cin >> assignName >> need;
			if (assignName < 0 || need <= 0 || need > MaxSize)
			{
				cout << "请按要求输入数据！" << endl;
				exit(0);
			}
			if (Assign(head, assignName, need))
			{
				cout << "分配成功！" << endl;
			}
			else
			{
				cout << "分配失败！" << endl;
			}
			break;
		case 2:
			
			cout << "请输入要释放磁盘空间的任务名：" << endl;
			cin >> returnName;
			if (Return(head, returnName))
			{
				cout << "归还成功！" << endl;
			}
			else
			{
				cout << "归还失败！" << endl;
			}
			break;
		default:
			exit(0);
			break;
		}
	}

	return 0;
}