/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 20
#define MAX_EXPRESSION_SIZE 40

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x); // postfix 스택을 push하는 함수 
char postfixPop(); // postfix 스택을 pop하는 함수
void evalPush(int x); // eval 스택을 push하는 함수
int evalPop(); // eval 스택을 pop하는 함수
void getInfix(); // infix 표현을 입력받는 함수
precedence getToken(char symbol); // 문자를 받아서 연산자 우선순위를 반환하는 함수
precedence getPriority(char x); // 문자를 받아서 연산자 우선순위를 반환하는 함수
void charCat(char* c); // 문자를 받아서 postfixExp에 추가하는 함수
void toPostfix(); // infixExp를 postfixExp로 변환하는 함수
void debug(); // infixExp, postfixExp, evalResult, postfixStack을 출력하는 함수
void reset(); // infixExp, postfixExp, postfixStack, evalStack, evalResult를 초기화하는 함수
void evaluation(); // postfixExp를 계산하는 함수

int main()
{
	char command; 
    printf("2021041002 오원석\n");

	do // 사용자가 q를 입력할 때까지 반복
    {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix(); // i, I를 입력받으면 infixExp를 입력받음
			break; // switch문 탈출
		case 'p': case 'P': 
			toPostfix(); // p, P를 입력받으면 infixExp를 postfixExp로 변환
			break; // switch문 탈출
		case 'e': case 'E':
			evaluation(); // e, E를 입력받으면 postfixExp를 계산
			break; // switch문 탈출
		case 'd': case 'D':
			debug(); // d, D를 입력받으면 infixExp, postfixExp, evalResult, postfixStack을 출력
			break; // switch문 탈출
		case 'r': case 'R':
			reset(); // r, R을 입력받으면 infixExp, postfixExp, postfixStack, evalStack, evalResult를 초기화
			break; // switch문 탈출
		case 'q': case 'Q':
			break; // switch문 탈출
		default: // 잘못된 명령어 입력 시 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q, Q를 입력받으면 종료

	return 1;
}

void postfixPush(char x) // postfix 스택을 푸시하는 함수
{
    postfixStack[++postfixStackTop] = x; // 스택에 푸시
}

char postfixPop()  // postfix 스택을 팝하는 함수
{
    char x; // 팝한 값을 저장할 변수
    if(postfixStackTop == -1) // 스택이 비어있는 경우
        return '\0'; // NULL 반환
    else {
    	x = postfixStack[postfixStackTop--]; // 스택에서 팝
    }
    return x; // 팝한 값 반환
}

void evalPush(int x) // eval 스택을 푸시하는 함수
{
    evalStack[++evalStackTop] = x; // 스택에 푸시
}

int evalPop() // eval 스택을 팝하는 함수
{
    if(evalStackTop == -1) // 스택이 비어있는 경우
        return -1; // -1 반환
    else 
        return evalStack[evalStackTop--]; // 스택에서 팝
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> "); // infix 표현을 입력받음
    scanf("%s",infixExp); // infixExp에 저장
}

precedence getToken(char symbol) // 문자를 받아서 연산자 우선순위를 반환하는 함수
{
	switch(symbol) {
	case '(' : return lparen; // symbol이 '('인 경우 lparen 반환
	case ')' : return rparen; // symbol이 ')'인 경우 rparen 반환
	case '+' : return plus; // symbol이 '+'인 경우 plus 반환
	case '-' : return minus; // symbol이 '-'인 경우 minus 반환
	case '/' : return divide; // symbol이 '/'인 경우 divide 반환
	case '*' : return times; // symbol이 '*'인 경우 times 반환 
	default : return operand; // 나머지 경우 operand 반환
	}
}

precedence getPriority(char x) // 문자를 받아서 연산자 우선순위를 반환하는 함수
{
	return getToken(x); // getToken 함수 호출
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) // 문자를 받아서 postfixExp에 추가하는 함수
{
	if (postfixExp == '\0') 
		strncpy(postfixExp, c, 1); // postfixExp가 비어있는 경우 문자를 복사
	else
		strncat(postfixExp, c, 1); // postfixExp가 비어있지 않은 경우 문자를 추가
}

void debug()
{
	printf("\n---DEBUG\n"); 
	printf("infixExp =  %s\n", infixExp); // infixExp 출력
	printf("postExp =  %s\n", postfixExp); // postfixExp 출력
	printf("eval result = %d\n", evalResult); // evalResult 출력
	printf("postfixStack : "); // postfixStack 출력
	for(int i = 0; i < MAX_STACK_SIZE; i++) 
		printf("%c  ", postfixStack[i]); // postfixStack 출력
	printf("\n");
}

void reset()
{
	infixExp[0] = '\0'; // infixExp 초기화
	postfixExp[0] = '\0'; // postfixExp 초기화

	for(int i = 0; i < MAX_STACK_SIZE; i++) 
		postfixStack[i] = '\0'; // int형 배열 postfixStack 초기화
         
	postfixStackTop = -1; // postfixStackTop -1로 초기화
	evalStackTop = -1; // evalStackTop -1로 초기화
	evalResult = 0; // evalResult 0으로 초기화
}

void toPostfix() // infixExp를 postfixExp로 변환하는 함수
{
    char *exp = infixExp; // infixExp를 가리키는 포인터
    char x; // infixExp의 문자를 저장할 변수

    while(*exp != '\0') // infixExp의 끝까지 반복
    {
        if(*exp == '(') // 왼쪽 괄호인 경우 스택에 푸시
        {
            postfixPush(*exp); // 스택에 푸시
        }
        else if(*exp == ')') // 오른쪽 괄호인 경우 왼쪽 괄호가 나올 때까지 스택에서 팝
        {
            while((x = postfixPop()) != '(') // 왼쪽 괄호가 나올 때까지 반복
            {
                char buffer[2] = {x, '\0'}; // buffer에 문자와 NULL 추가 -> (가 나올 때까지 스택에서 팝한 문자를 postfixExp에 추가
                charCat(buffer); 
            }
        }
        else if(*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/') // 연산자인 경우
        {
            while(postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) // 스택의 top이 현재 연산자보다 우선순위가 높거나 같은 경우
            {
                x = postfixPop(); // 스택에서 팝
                char buffer[2] = {x, '\0'}; // buffer에 문자와 NULL 추가
                charCat(buffer); // postfixExp에 추가
            }
            postfixPush(*exp); // 현재 연산자를 스택에 푸시
        }
        else // 피연산자인 경우
        {
            char buffer[2] = {*exp, '\0'}; // buffer에 문자와 NULL 추가 
            charCat(buffer); // postfixExp에 추가
        }
        exp++;
    }

    while(postfixStackTop != -1) // 스택에 남아있는 연산자를 모두 팝
    {
        x = postfixPop();
        char buffer[2] = {x, '\0'};
        charCat(buffer);
    }
}

void evaluation() // postfixExp를 계산하는 함수
{
    char *exp = postfixExp; // postfixExp를 가리키는 포인터
    int op1, op2, value; // 피연산자와 연산 결과를 저장할 변수
    char symbol; // 연산자를 저장할 변수
    while(*exp != '\0') // postfixExp의 끝까지 반복
    {
        symbol = getToken(*exp); // 연산자인지 피연산자인지 판별
        if(symbol == operand) // 피연산자인 경우
        {
            evalPush(*exp - '0'); // 문자를 숫자로 변환하여 스택에 푸시
        }
        else if(symbol == plus || symbol == minus || symbol == times || symbol == divide) // 연산자인 경우
        {
            if(evalStackTop < 1) // 에러: 연산자에 대한 피연산자가 부족한 경우
            {
                printf("연산자 부족!!! %c\n", *exp); // 에러 메시지 출력
                return; // 함수 종료
            }
            op2 = evalPop(); // 스택에서 피연산자를 팝
            op1 = evalPop(); // 스택에서 피연산자를 팝
            switch(symbol) // 연산자에 따라 연산 수행
            {
                case plus: value = op1 + op2; break; // 덧셈
                case minus: value = op1 - op2; break; // 뺄셈
                case times: value = op1 * op2; break; // 곱셈
                case divide: // 나눗셈
                    if(op2 == 0) // Error: division by zero
                    {
                        printf("분모가 0이여서 divide 불가\n");
                        return; // 함수 종료
                    }
                    value = op1 / op2; break; // 나눗셈
            }
            evalPush(value); // 연산 결과를 스택에 푸시
        }
        else // 에러: 잘못된 문자가 있는 경우
        {
            printf("잘못된 문자 있음!! %c\n", *exp); // 에러 메시지 출력
            return;
        }
        exp++;
    }
    if(evalStackTop > 0) // 에러: 피연산자가 너무 많은 경우
    {
        printf("피연산자가 너무 많음!!!\n"); // 에러 메시지 출력
        return;
    }
    evalResult = evalPop(); // 최종 결과를 스택에서 팝
}