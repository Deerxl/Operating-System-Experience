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
#define TRACK 2
#define RECORD 4
#define FREE 0
#define OCCUPIED 1

int bitMap[CYLINDER][CYLINDER];

void Assign()
{
	int need = 0;
	int rest = 0;
	cout << "请输入需要分配的空间大小：" << endl;

	cin >> need;
	if (need <= 0)
	{
		cout << "请按要求输入数据！" << endl;
		exit(0);
	}

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

	if (need <= rest)
	{
		cout << "分配成功！" << endl;
		cout << "柱面\t磁道\t物理记录" << endl;
		for (int i = 0; i < CYLINDER; i++)
		{
			for (int j = 0; j < CYLINDER; j++)
			{
				if (bitMap[i][j] == FREE)
				{
					if (need <= 0)
					{
						break;
					}

					bitMap[i][j] = OCCUPIED;

					cout << i << "\t" << j / RECORD << "\t" << j % RECORD << endl;

					need--;
				}
			}
		}
	}
	else
	{
		cout << "分配失败!" << endl;
	}

}

void Return()
{
	int cylinder, track, record;
	int byte, bit;

	cout << "请依次输入要回收的柱面号(0 - 7)、磁道号(0 - 2)、物理块号(0 - 3)：" << endl;

	cin >> cylinder >> track >> record;
	if (cylinder < 0 || cylinder>7 || track < 0 || track>2 || record < 0 || record>3)
	{
		cout << "请按要求输入数据！" << endl;
		exit(0);
	}

	byte = cylinder;
	bit = track * RECORD + record;

	bitMap[byte][bit] = FREE;

	cout << "归还成功！" << endl;
	cout << "字节号\t" << "位数\t" << endl;
	cout << byte << "\t" << bit << endl;
}

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
	int n;
	while (1)
	{
		Show();
		cout << "输入：\n\t" << "1\t分配" << "\n\t2\t回收" << "\n\t其他\t退出" << endl;
		cin >> n;
		switch (n)
		{
		case 1:
			Assign();
			break;
		case 2:
			Return();
			break;
		default:
			exit(0);
			break;
		}
	}

	return 0;
}