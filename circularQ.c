/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */
// insert : enQueue delete : deQueue print : printQ debug queue에서 front와 rear를 출력하는 함수 debugQ

#include <stdio.h>  
#include <stdlib.h> // malloc, free 함수 사용을 위한 헤더파일

#define MAX_QUEUE_SIZE 4 // queue의 최대 크기를 4로 정의

typedef char element; // element를 char로 정의
typedef struct 
{
	element queue[MAX_QUEUE_SIZE]; // queue를 MAX_QUEUE_SIZE(4)의 크기로 정의
	int front, rear; // front와 rear를 정의
}QueueType;


QueueType *createQueue(); // queue를 생성하는 함수
int freeQueue(QueueType *cQ); // queue를 free하는 함수
int isEmpty(QueueType *cQ); // queue가 비어있는지 확인하는 함수
int isFull(QueueType *cQ); // queue가 가득 차있는지 확인하는 함수
void enQueue(QueueType *cQ, element item); // queue에 요소를 넣는 함수
void deQueue(QueueType *cQ, element* item); // queue에서 요소를 빼는 함수
void printQ(QueueType *cQ); // queue를 출력하는 함수
void debugQ(QueueType *cQ); // queue의 front와 rear를 출력하는 함수
element getElement(); // 요소를 입력받는 함수

int main(void)
{
	QueueType *cQ = createQueue(); // queue를 생성 후 cQ에 저장
	element data;  // 요소(element)를 저장할 변수
	char command; // command를 입력받을 변수

	printf("2021041002 오원석\n");	

	// 사용자가 q를 입력할 때까지 원형큐를 사용할 수 있는 명령어를 출력
	do
	{
		// 사용자가 입력할 수 있는 명령어 출력
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		// command 입력
		switch(command) 
		{
		case 'i': case 'I':
			data = getElement(); //i, I를 입력받으면 요소를 입력받아 data에 저장
			enQueue(cQ, data); // data를 queue에 넣음
			break; // switch문 탈출
		case 'd': case 'D':
			deQueue(cQ, &data); //d, D를 입력받으면 queue에서 요소를 빼 data에 저장
			break; // switch문 탈출
		case 'p': case 'P':
			printQ(cQ); //p, P를 입력받으면 queue를 출력
			break; // switch문 탈출
		case 'b': case 'B': 
			debugQ(cQ); //b, B를 입력받으면 queue의 front와 rear를 출력
			break; // switch문 탈출
		case 'q': case 'Q':
   	        freeQueue(cQ); //q, Q를 입력받으면 queue를 free(동적할당 해제)
			break; // switch문 탈출
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 잘못된 명령어 입력 시 출력
			break;
		}

	}while(command != 'q' && command != 'Q'); // q, Q를 입력받으면 종료

	return 1;
}

QueueType *createQueue() // queue를 생성하는 함수
{
	QueueType *cQ; // QueueType 구조체 포인터 cQ 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); // cQ에 QueueType 크기만큼 동적할당
	cQ->front = 0; // front를 0으로 초기화
	cQ->rear = 0; // rear를 0으로 초기화
	return cQ; // cQ 반환
}

int freeQueue(QueueType *cQ) // queue를 free하는 함수
{
    if(cQ == NULL) return 1; // cQ가 NULL이면 함수 종료
    free(cQ); // cQ 동적할당 해제
    return 1; // 함수 종료
}

element getElement() // 요소를 입력받는 함수
{
	element item; // 요소를 저장할 변수 item 선언
	printf("Input element = "); 
	scanf(" %c", &item); // item에 요소 입력
	return item; // item 반환
}

void printQ(QueueType *cQ) // queue를 출력하는 함수
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // front의 다음 위치를 first에 저장
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // rear의 다음 위치를 last에 저장

	printf("Circular Queue : [");

	i = first; // i에 first값 저장
	while(i != last) // i가 last(rear)와 같아질 때까지 반복
	{ 
		printf("%3c", cQ->queue[i]); // queue의 i번째 요소 출력
		i = (i+1)%MAX_QUEUE_SIZE; // i를 다음 위치로 이동

	}
	printf(" ]\n");
}

void debugQ(QueueType *cQ) // queue의 front와 rear를 출력하는 함수
{

	printf("\n---DEBUG\n"); 
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // i가 0부터 MAX_QUEUE_SIZE까지 반복 
	{
		if(i == cQ->front) { 
			printf("  [%d] = front\n", i); //i가 front와 같으면 front 출력
			continue; 
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //i가 front와 다르면 queue의 i번째 요소 출력
	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front와 rear 출력
}

int isEmpty(QueueType *cQ) // queue가 비어있는지 확인하는 함수
{
    if(cQ->front == cQ->rear) // front와 rear가 같으면 비어있는 상태
        return 1; // 1 반환
    else
        return 0; // 아니면 0 반환
}

int isFull(QueueType *cQ) // queue가 가득 차있는지 확인하는 함수
{
    if((cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front) // rear의 다음이 front와 같으면 가득 찬 상태
        return 1; // 1 반환
    else
        return 0; // 아니면 0 반환
}


void enQueue(QueueType *cQ, element item) // queue에 요소를 넣는 함수 
{
    if(isFull(cQ)) // isFull 함수를 통해 1이 반환되면 함수 종료 0이 반환되면 계속 진행
        return;
    else 
	{
        cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 다음 위치로 이동 (원형큐에서 마지막 위치에 도달하면 처음으로 이동하기 위해 MAX_QUEUE_SIZE로 나눈 나머지를 저장)
        cQ->queue[cQ->rear] = item; // rear 위치에 item 저장 
    }
}


void deQueue(QueueType *cQ, element *item) // queue에서 요소를 빼는 함수
{
    if(isEmpty(cQ)) // isEmpty 함수를 통해 1이 반환되면 함수 종료 0이 반환되면 계속 진행
        return; 
    else 
	{
        cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front를 다음 위치로 이동 (원형큐에서 마지막 위치에 도달하면 처음으로 이동하기 위해 MAX_QUEUE_SIZE로 나눈 나머지를 저장)
        *item = cQ->queue[cQ->front]; // front 위치의 요소를 item에 저장
    }
}
