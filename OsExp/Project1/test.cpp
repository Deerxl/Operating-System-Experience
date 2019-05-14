/*
���һ���������������㷨ʵ�ִ��������ȵĳ���
[��ʾ]��
(1)�ٶ�ϵͳ��5�����̣�ÿ��������һ��PCB������PCB�ĽṹΪ��
��������������P1~P5��
��ָ�롪�����������Ĵ�С��5���������ɶ��У���ָ��ָ����һ������PCB���׵�ַ��
��Ҫ������ʱ�䡪�����������Ҫ���еĵ�λʱ������
������������������̵�������������ʱ����ѡȡ��������Ľ�����ִ�С�
��״̬������������״̬�������ͽ�������R��ʾ��������E��ʾ��������ʼ״̬��Ϊ����״̬��
(2) ��ʼ����֮ǰ��Ϊÿ������ȷ�����ġ����������͡�Ҫ������ʱ�䡱��ͨ������������Щ������
(3) ����������ѡ����׽������С����ö�̬�ı��������İ취������ÿ����1�Σ���������1��Ҫ������ʱ���1��
(4) ��������һ�κ���Ҫ������ʱ�䲻����0����������������У����򣬽�״̬��Ϊ�����������˳��������С�
(5) ����������Ϊ�գ�����������ת��(3)�ظ���
Ҫ���ܽ��ܼ�������Ľ�����������Ҫ������ʱ�䣬����ʾÿ�ν��̵��ȵ���������ĸ����������У���Щ���̾������������̵����������
*/

#include<iostream>
#include<string>
using namespace std;

#define PCBNUM 5

struct PCB
{
	string name;      //������
	int time;         //Ҫ������ʱ��
	int priority;     //���ȼ�
	char status;      //״̬ R ���� E ����
	struct PCB* next;
};

void SetPCB(PCB*& pcb, int i) 
{
	pcb->name = "P" + to_string(i);
	
	cout << "��Ϊ����P" << i << "�������� ���������� �� Ҫ������ʱ�䣺" << endl;
	try
	{
		cin >> pcb->priority >> pcb->time;
		if (pcb->priority < 0 || pcb->time < 0)
		{
			cout << "�밴��Ҫ���������ݣ�" << endl;
			exit(0);
		}
		cout << endl;
	}
	catch(exception e)
	{
		cout << "�밴��Ҫ���������ݣ�" << endl;
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

		cout << "��" << count << "������" << endl;
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