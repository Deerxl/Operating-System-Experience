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
	string name;	//������
	string state;		//״̬  ����̬������̬���ȴ�̬�����̬
	string reason;	//�ȴ�ԭ��  �ȴ��ź���s1��s2
	int breakPoint;	//�ϵ���Ϣ  һ�������ٶ�ռ�д�������Ӷϵ�λ�ü������У��������״̬������ִ�н���
	struct PCB* next;
};

string states[4] = { "run", "ready", "block", "done" };
int in = 0, out = 0;		//������ָ�� ������ָ��
int x = 1;          //ȡ������/���ѵ��ַ�
int products[PRODUCTS];	 //10����Ʒ
int product = 0;
int s1, s2;		 //�ź���
int PC = 0;		//ָ�������������ģ���ָ���Ϊ1��ÿִ��һ��ģ��ָ���PC��1����ȡ����һ��ָ���ַ��

int PA[5] = { PRODUCE, P, PUT, V, GOTO };	//PA[i] PA[PC] ��� ������ �����е�һ��ģ��ָ��ִ�е���ڵ�ַ
int SA[5] = { P, GET, V, CONSUME, GOTO };	//SA[i] SA[PC] ��� ������ �����е�һ��ģ��ָ��ִ�е���ڵ�ַ
int i = -1;      //��ǰָ��

PCB* producer, * consumer, * pcbNow; //������ ������ ���н���

//��ʼ�������������ź���s1��s2����ֵ���������ߡ������߽��̵�PCB��ʼ������ʼ����ת������ȳ���
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

//���ź���s��ȥ1�������С��0����ִ��ԭ��Ľ��̱��óɵȴ��ź���s��״̬��
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

//���ź���s��1�������������0�����ͷ�һ���ȴ��ź���s�Ľ��̡�
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

//����һ���ַ�����C��
void Produce()
{
	cout << "PRODUCER - Produce " << x << endl;
	in += 1;
}

//��ӡ����ʾx�е��ַ�
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

//NOP	�ղ���
void Nop()
{
	;
}

//ģ�⴦����ָ��ִ�г��򣺰���ָ���������PCִֵ֮��ָ����ָ���PC��1ָ����һ��ָ�
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

/*���������ȳ���
�ڼ����ϵͳ�У����̲���ִ��ʱ����һ����ռ�ô�����ִ����һ��ָ�����п��ܱ���϶��ó��������������������С�
����ģ��ϵͳ��Ҳ���ƴ���ÿ��ִ��һ��ģ���ָ��󣬱�����ǰ���̵��ֳ���������Ϊ������̬��
�ɴ��������ȳ����������ѡ��һ����������ռ�ô��������С�
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
