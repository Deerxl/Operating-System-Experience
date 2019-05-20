/*
�ɱ��������ʽ�²����״���Ӧ�㷨ʵ���������ͻ���
[��ʾ]��
(1)	�ɱ������ʽ�ǰ���ҵ��Ҫ������ռ��С���ָ�����ġ�
��Ҫװ��һ����ҵʱ��������ҵ��Ҫ�����������鿴�Ƿ����㹻�Ŀ��пռ䣬
���У�������䣬������ҵ�޷�װ�롣
�ٶ��ڴ��СΪ128K��������˵�����ʽΪ��
�������š�����ʾ�ǵڼ������з�����
����ʼ��ַ����ָ������������ʼ��ַ��
�����ȡ���һ�������������ĳ��ȣ�
(2)	�����״���Ӧ�㷨��������ڴ�ռ䡣����ʱ������һϵ�з�������ͻ�������
Ҫ���ܽ������Լ��̵Ŀռ����뼰�ͷ���������ʾ�������估���պ���ڴ沼�������
*/

#include<iostream>
using namespace std;

#define MAXSIZE 128
#define UNASSIGNED 0
#define ASSIGNED 1

struct Partition
{
	int num = NULL;           //������
	int startAddr;     //��ʼ��ַ
	int length;		   //��������
	int state;		   //״̬         0 δ���� 1 �ѷ���
	int tasknum = NULL;       //��ҵ��
	struct Partition* next;
};

bool Insert(Partition*& head, Partition*& newTast);
bool Recycle(Partition*& head, Partition*& deleteTask);


//��ʾ�ڴ沼����� �����ţ�0��ʾ�ǿ�������1...n Ϊ������
void Show(Partition*& head)
{
	Partition* pHead = head;
	Partition* p = new Partition;
	cout << "************************���з�����****************************" << endl;
	cout << "������\t" << "��ʼ��ַ\t" << "����\t" << "״̬\t" << "��ҵ��" << endl;
	
	
	while (pHead->next != NULL)
	{
		p = pHead->next;
		cout << p->num << "\t" << p->startAddr << "\t\t" << p->length << "\t" << p->state << "\t" << p->tasknum << endl;
		
		pHead = pHead->next;
	}
	cout << "************************���з�����****************************" << endl;
}

//����ռ�
void NewTask(Partition*& head)
{
	Partition* newTask = new Partition;
	cout << "��Ϊ����ռ���������� ��������int�� �� �ռ��С��int����" << endl;

	cin >> newTask->tasknum >> newTask->length;
	if (newTask->tasknum < 0 || newTask->length < 0)
	{
		cout << "�밴��Ҫ����������!" << endl;
		exit(0);
	}
	if (Insert(head, newTask))
	{
		cout << "����ɹ���" << endl;
	}
	else
	{
		cout << "����ʧ�ܣ�" << endl;
	}

}

//���뵽������
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

//���տռ�
void DeleteTask(Partition*& head)
{
	Partition* deleteTask = new Partition;
	cout << "������Ҫ�ͷſռ����������" << endl;

	cin >> deleteTask->tasknum;
	if (deleteTask->tasknum < 0)
	{
		cout << "�밴��Ҫ����������!" << endl;
		exit(0);
	}
	if (Recycle(head, deleteTask))
	{
		cout << "�ͷųɹ���" << endl;
	}
	else
	{
		cout << "�ͷ�ʧ�ܣ�" << endl;
	}
}

bool Recycle(Partition*& head, Partition*& deleteTask)
{
	Partition* phead = head;
	Partition* p = new Partition;
	p = phead->next;
	Partition* pre = NULL;
	Partition* pre2 = new Partition;
	int flag = 0;
	while (p != NULL)
	{
		if (p->tasknum == deleteTask->tasknum && p->state == ASSIGNED)
		{
			(pre == NULL) ? (p->num = 1) : (p->num = pre->num + 1);
			p->state = UNASSIGNED;
			p->tasknum = 0;
			while ((phead->next != NULL) && (phead->next->next != NULL) && (phead->next->next->state == UNASSIGNED))
			{
				phead->next = phead->next->next;
				p = phead->next;
				p->startAddr = pre->startAddr;
				p->length = pre->length + p->length;
			}
			flag = 1;
		}
		if (!flag)
		{
			pre = p;
		}
		phead = phead->next;
		p = phead->next;
	}
	Partition* phead1 = head;
	Partition* q = new Partition;
	q = phead1->next;
	while (q != NULL)
	{
		if ((pre != NULL) && (pre->state == UNASSIGNED) && (q == pre))
		{
			phead1->next = phead1->next->next;
			q = phead1->next;
			q->startAddr = pre->startAddr;
			q->length = pre->length + q->length;
			q->tasknum = 0;
			return true;
		}
		phead1 = phead1->next;
		q = phead1->next;
	}	
	return true;
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
		cout << "���룺\n\t" << "1\t����ռ�" << "\n\t2\t�ͷſռ�"<<"\n\t����\t�˳�" << endl;
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