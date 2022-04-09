#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;	// char를 element라고 형 정의한다.

typedef struct	// 구조체선언 
{	
	element queue[MAX_QUEUE_SIZE]; 
	// MAX_QUEUE_SIZE 크기의 elelment형 queue 배열을 만든다.
	int front, rear;	// queue에서 사용할 front와 rear 생성
}QueueType;	// 만들어진 구조체를 QueueType라고 정의


QueueType *createQueue();	// QueueType형 구조체를 동적으로 생성한다.
int freeQueue(QueueType *cQ);	// cq가 가리키는 공간을 해제한다.
int isEmpty(QueueType *cQ);		// cQ가 가리키는 곳이 비어있는지 검사한다.
int isFull(QueueType *cQ);		// cQ가 가리키는 곳이 가득 찼는지 검사한다.
void enQueue(QueueType *cQ, element item);	// item을 cQ가 가리키는 곳의 queue에 넣는다.
void deQueue(QueueType *cQ, element* item);	// item을 cQ가 가리키는 곳의 queue에서 빼낸다.
void printQ(QueueType *cQ);	// cQ 가리키는 곳의 queue에서 값이 들어있는 인덱스 출력
void debugQ(QueueType *cQ);	
// cQ가 가리키는 곳의 queue에서
//몇 번 인덱스가 비어있고, front인지 rear인지, 또, 어떤 값이 들어있는지 출력
element getElement();	// 사용자로부터 데이터를 입력받는다.

int main(void)
{
	QueueType *cQ = createQueue();	
	// 메모리의 Heap 영역에서 원형큐를 동적으로 생성한다.
	// 생성된 곳의 주소를 cQ가 가리키도록 한다.
	element data; // elelment형 변수 data 즉, char형 변수 data를 메모리의 Stack 영역에 생성한다.
	char command;	// 명령어를 받을 command 생성

	printf("[----- [복무창] [2021040021] -----]\n");
	
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	// 명령어를 입력받는다.

		switch(command) {
		case 'i': case 'I':			// i 또는 I의 경우
			data = getElement();	// 데이터를 입력받아 data에 할당
			enQueue(cQ, data);		// cQ가 가리키는 곳의 queue에 enqueue한다.
			break;					// switch문 탈출
		case 'd': case 'D':			// d 또는 D의 경우
			deQueue(cQ, &data);		// cQ가 가리키는 곳의 queue에 dequeue한다.
			break;					// switch문 탈출
		case 'p': case 'P':			// p 또는 P의 경우
			printQ(cQ);				// cQ가 가리키는 곳의 queue에서 값이 들어있는 인덱스 출력
			break;					// switch문 탈출
		case 'b': case 'B':			// b 또는 B의 경우
			debugQ(cQ);
			// cQ가 가리키는 곳의 queue에서 
			//몇 번 인덱스가 비어있고, front인지 rear인지, 또, 어떤 값이 들어있는지 출력
			break;					// switch문 탈출
		case 'q': case 'Q':			// q 또는 Q의 경우
   	        freeQueue(cQ);			// cQ가 가리키는 곳의 공간 해제
			break;					// switch문 탈출
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;					// switch문 탈출
		}

	}while(command != 'q' && command != 'Q');	// q와 Q를 입력받지 않을 때만 반복한다.


	return 1;
}

QueueType *createQueue() // QueueType형 구조체를 동적으로 생성한다.
{
	QueueType *cQ;	// 메모리의 Stack 영역에 QueueType 형 변수 cQ 생성
	cQ = (QueueType *)malloc(sizeof(QueueType)); 
	// 메모리의 Heap 영역에 QueueType만큼의 크기의 공간을 생성 후에 cq에 그 공간의 주소 할당 
	cQ->front = 0;	// cQ가 가리키는 곳의 front를 0으로 초기화
	cQ->rear = 0;	// cQ가 가리키는 곳의 rear를 0으로 초기화
	return cQ;	// cq의 주소 반환
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // cQ가 아무것도 가르키고 있지 않다면 정상 종료
    free(cQ);	//  cQ가 가리키는 곳의 공간 해제
    return 1;   
}

element getElement()	// 사용자로부터 데이터를 입력받는다. 
{
	element item;	// element형 즉, char형 변수 item 생성
	printf("Input element = ");
	scanf(" %c", &item);	// 데이터를 입력받아 item에 할당
	return item;	// item 반환
}


int isEmpty(QueueType *cQ)	// cQ가 가리키는 곳의 공간이 비어있는지 검사
{
	if(cQ->front ==cQ->rear)	// cq가 가리키는 곳의 front와 rear가 같다면
	{
		printf("Circular Queue is empty!");	// 원형큐가 비어있다는 문구 출력
		return 1;
	}

    return 0;
}

int isFull(QueueType *cQ)	// cQ가 가리키는 곳의 공간이 가득 차있는지 검사
{
	if(((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front)	
	// cQ가 가리키는 곳의 rear에 1을 더한 값을 MAX_QUEUE_SIZE로 나눴을 때 나머지가 front와 같다면
	{
		printf(" Circular Queue is full");	// 원형큐가 가득 차있다는 문구 출력
		return 1;
	}
	return 0;
}

void enQueue(QueueType *cQ, element item) // cQ가 가리키는 곳의 queue에 item 할당
{
	if(isFull(cQ))	// cQ가 가리키는 곳이 가득 차있는지 검사한다. 가득 찼다면
		return;	// 아무것도 반환하지 않는다.
	else	// cQ가 가리키는 곳이 가득 차지 않았다면
	{
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		// rear에 (rear+1)%MAX_QUEUE_SIZE 연산을 한 후 그 결과를 할당한다.
		cQ->queue[cQ->rear] = item;
		// cQ->queue[cQ->rear]에 item 할당
	}
}

void deQueue(QueueType *cQ, element *item)	
// cQ가 가리키는 곳에서 queue[front] 빼낸다. 
{
    if(isEmpty(cQ))	// cQ가 가리키는 곳이 비어있는지 검사한다. 비어있다면
		return;	// 아무것도 반환하지 않는다.
	else	// cQ가 가리키는 곳이 비어있지 않다면
	{
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;
		// cQ가 가리키는 곳의 front에 (front+1) % MAX_QUEUE_SIZE 연산을 한 후 그 결과를 할당한다.
		*item = cQ->queue[cQ->front];
		// item이 가리키는 곳에 cQ->queue[cQ->front] 할당
		return;
	}
	
}


void printQ(QueueType *cQ)	// cQ 가리키는 곳의 queue에서 값이 들어있는 인덱스 출력
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // (cQ->front+1)%MAX_QUEUE_SIZE 값을 cQ->first에 할당
	// front 이후부터 값이 들어있기 때문
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	// (cQ->rear+1)%MAX_QUEUE_SIZE의 값을 last에 할당
	// rear가 마지막 값이 있는 인덱스 번호와 같기 때문

	printf("Circular Queue : [");

	i = first;	// i에 first의 값 할당
	while(i != last)	// i가 last가 아닐 때 출력
	{	
		printf("%3c", cQ->queue[i]);	// queue의 요소 출력
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)		// MAX_QUEUE_SIZE 만큼 반복
	{
		if(i == cQ->front) {	// i가 cQ의 front와 일치한다면
			printf("  [%d] = front\n", i);	// i 출력
			continue; // 밑의 코드는 진행하지 않고, 바로 다음 루프로 넘어간다.
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);	// queue의 요소들 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	// cQ의 front와 rear 출력
}