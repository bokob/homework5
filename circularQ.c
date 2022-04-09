#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;	// char�� element��� �� �����Ѵ�.

typedef struct	// ����ü���� 
{	
	element queue[MAX_QUEUE_SIZE]; 
	// MAX_QUEUE_SIZE ũ���� elelment�� queue �迭�� �����.
	int front, rear;	// queue���� ����� front�� rear ����
}QueueType;	// ������� ����ü�� QueueType��� ����


QueueType *createQueue();	// QueueType�� ����ü�� �������� �����Ѵ�.
int freeQueue(QueueType *cQ);	// cq�� ����Ű�� ������ �����Ѵ�.
int isEmpty(QueueType *cQ);		// cQ�� ����Ű�� ���� ����ִ��� �˻��Ѵ�.
int isFull(QueueType *cQ);		// cQ�� ����Ű�� ���� ���� á���� �˻��Ѵ�.
void enQueue(QueueType *cQ, element item);	// item�� cQ�� ����Ű�� ���� queue�� �ִ´�.
void deQueue(QueueType *cQ, element* item);	// item�� cQ�� ����Ű�� ���� queue���� ������.
void printQ(QueueType *cQ);	// cQ ����Ű�� ���� queue���� ���� ����ִ� �ε��� ���
void debugQ(QueueType *cQ);	
// cQ�� ����Ű�� ���� queue����
//�� �� �ε����� ����ְ�, front���� rear����, ��, � ���� ����ִ��� ���
element getElement();	// ����ڷκ��� �����͸� �Է¹޴´�.

int main(void)
{
	QueueType *cQ = createQueue();	
	// �޸��� Heap �������� ����ť�� �������� �����Ѵ�.
	// ������ ���� �ּҸ� cQ�� ����Ű���� �Ѵ�.
	element data; // elelment�� ���� data ��, char�� ���� data�� �޸��� Stack ������ �����Ѵ�.
	char command;	// ��ɾ ���� command ����

	printf("[----- [����â] [2021040021] -----]\n");
	
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	// ��ɾ �Է¹޴´�.

		switch(command) {
		case 'i': case 'I':			// i �Ǵ� I�� ���
			data = getElement();	// �����͸� �Է¹޾� data�� �Ҵ�
			enQueue(cQ, data);		// cQ�� ����Ű�� ���� queue�� enqueue�Ѵ�.
			break;					// switch�� Ż��
		case 'd': case 'D':			// d �Ǵ� D�� ���
			deQueue(cQ, &data);		// cQ�� ����Ű�� ���� queue�� dequeue�Ѵ�.
			break;					// switch�� Ż��
		case 'p': case 'P':			// p �Ǵ� P�� ���
			printQ(cQ);				// cQ�� ����Ű�� ���� queue���� ���� ����ִ� �ε��� ���
			break;					// switch�� Ż��
		case 'b': case 'B':			// b �Ǵ� B�� ���
			debugQ(cQ);
			// cQ�� ����Ű�� ���� queue���� 
			//�� �� �ε����� ����ְ�, front���� rear����, ��, � ���� ����ִ��� ���
			break;					// switch�� Ż��
		case 'q': case 'Q':			// q �Ǵ� Q�� ���
   	        freeQueue(cQ);			// cQ�� ����Ű�� ���� ���� ����
			break;					// switch�� Ż��
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;					// switch�� Ż��
		}

	}while(command != 'q' && command != 'Q');	// q�� Q�� �Է¹��� ���� ���� �ݺ��Ѵ�.


	return 1;
}

QueueType *createQueue() // QueueType�� ����ü�� �������� �����Ѵ�.
{
	QueueType *cQ;	// �޸��� Stack ������ QueueType �� ���� cQ ����
	cQ = (QueueType *)malloc(sizeof(QueueType)); 
	// �޸��� Heap ������ QueueType��ŭ�� ũ���� ������ ���� �Ŀ� cq�� �� ������ �ּ� �Ҵ� 
	cQ->front = 0;	// cQ�� ����Ű�� ���� front�� 0���� �ʱ�ȭ
	cQ->rear = 0;	// cQ�� ����Ű�� ���� rear�� 0���� �ʱ�ȭ
	return cQ;	// cq�� �ּ� ��ȯ
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // cQ�� �ƹ��͵� ����Ű�� ���� �ʴٸ� ���� ����
    free(cQ);	//  cQ�� ����Ű�� ���� ���� ����
    return 1;   
}

element getElement()	// ����ڷκ��� �����͸� �Է¹޴´�. 
{
	element item;	// element�� ��, char�� ���� item ����
	printf("Input element = ");
	scanf(" %c", &item);	// �����͸� �Է¹޾� item�� �Ҵ�
	return item;	// item ��ȯ
}


int isEmpty(QueueType *cQ)	// cQ�� ����Ű�� ���� ������ ����ִ��� �˻�
{
	if(cQ->front ==cQ->rear)	// cq�� ����Ű�� ���� front�� rear�� ���ٸ�
	{
		printf("Circular Queue is empty!");	// ����ť�� ����ִٴ� ���� ���
		return 1;
	}

    return 0;
}

int isFull(QueueType *cQ)	// cQ�� ����Ű�� ���� ������ ���� ���ִ��� �˻�
{
	if(((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front)	
	// cQ�� ����Ű�� ���� rear�� 1�� ���� ���� MAX_QUEUE_SIZE�� ������ �� �������� front�� ���ٸ�
	{
		printf(" Circular Queue is full");	// ����ť�� ���� ���ִٴ� ���� ���
		return 1;
	}
	return 0;
}

void enQueue(QueueType *cQ, element item) // cQ�� ����Ű�� ���� queue�� item �Ҵ�
{
	if(isFull(cQ))	// cQ�� ����Ű�� ���� ���� ���ִ��� �˻��Ѵ�. ���� á�ٸ�
		return;	// �ƹ��͵� ��ȯ���� �ʴ´�.
	else	// cQ�� ����Ű�� ���� ���� ���� �ʾҴٸ�
	{
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		// rear�� (rear+1)%MAX_QUEUE_SIZE ������ �� �� �� ����� �Ҵ��Ѵ�.
		cQ->queue[cQ->rear] = item;
		// cQ->queue[cQ->rear]�� item �Ҵ�
	}
}

void deQueue(QueueType *cQ, element *item)	
// cQ�� ����Ű�� ������ queue[front] ������. 
{
    if(isEmpty(cQ))	// cQ�� ����Ű�� ���� ����ִ��� �˻��Ѵ�. ����ִٸ�
		return;	// �ƹ��͵� ��ȯ���� �ʴ´�.
	else	// cQ�� ����Ű�� ���� ������� �ʴٸ�
	{
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;
		// cQ�� ����Ű�� ���� front�� (front+1) % MAX_QUEUE_SIZE ������ �� �� �� ����� �Ҵ��Ѵ�.
		*item = cQ->queue[cQ->front];
		// item�� ����Ű�� ���� cQ->queue[cQ->front] �Ҵ�
		return;
	}
	
}


void printQ(QueueType *cQ)	// cQ ����Ű�� ���� queue���� ���� ����ִ� �ε��� ���
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // (cQ->front+1)%MAX_QUEUE_SIZE ���� cQ->first�� �Ҵ�
	// front ���ĺ��� ���� ����ֱ� ����
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	// (cQ->rear+1)%MAX_QUEUE_SIZE�� ���� last�� �Ҵ�
	// rear�� ������ ���� �ִ� �ε��� ��ȣ�� ���� ����

	printf("Circular Queue : [");

	i = first;	// i�� first�� �� �Ҵ�
	while(i != last)	// i�� last�� �ƴ� �� ���
	{	
		printf("%3c", cQ->queue[i]);	// queue�� ��� ���
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)		// MAX_QUEUE_SIZE ��ŭ �ݺ�
	{
		if(i == cQ->front) {	// i�� cQ�� front�� ��ġ�Ѵٸ�
			printf("  [%d] = front\n", i);	// i ���
			continue; // ���� �ڵ�� �������� �ʰ�, �ٷ� ���� ������ �Ѿ��.
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);	// queue�� ��ҵ� ���

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	// cQ�� front�� rear ���
}