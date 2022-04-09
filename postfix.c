#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum				// 니모닉(mnemonic) 사용해서 특정문자를 알기 쉽게 표현 
{
	lparen = 0, /* ( 왼쪽 괄호            
	스택 밖에서는 왼쪽 괄호의 우선순위가 제일 높지만, 스택 안에서는 왼쪽 괄호의 우선순위가 낮다고 취급한다.*/
	rparen = 9, /* ) 오른쪽 괄호*/
	times = 7,	/* * 곱셈 */
	divide = 6, /* / 나눗셈 */
	plus = 5,	/* + 덧셈 */
	minus = 4,	/* - 뺄셈 */
	operand = 1 /* 피연산자 */  
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];	  /* 중위 표기를 저장할 배열 */
char postfixExp[MAX_EXPRESSION_SIZE]; /* 중위 표기를 후위 표기로 변환한 것을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	  /* 후위표기로의 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		  /* 계산을 위해 필요한 스택 */

int postfixStackTop = -1; /* postfixStack용 top,  스택이 비어있을 때 -1 */
int evalStackTop = -1;	  /* evalStack용 top, 스택이 비어있을 때 -1 */

int evalResult = 0; /* 계산 결과를 저장할 변수 */

void postfixpush(char x);           // postfixStack의 push 
char postfixPop();					// postfixStack의 pop 
void evalPush(int x);				// evalStack의 push
int evalPop();						// evalStack의 pop 
void getInfix();					// 사용자로부터 수식(중위 표기)을 입력받는다.
precedence getToken(char symbol);	// symbol에 해당하는 연산자의 우선순위를 precedence에서 구한다.
precedence getPriority(char x);		// x의 우선순위를 구한다.
void charCat(char *c);				// c에 있는 문자를 하나씩 postfixExp에 복사한다.
void toPostfix();					// 중위 표기를 후위 표기로 바꾼다.
void debug();						// infixExp, postfixExp, evalResult, postfixStack의 상태를 보여준다.
void reset();							
/* infixExp, postfixExp, postfixStack을 비우고, postfixStackTop과 evalStackTop은 -1로 초기화한다.
 또, evalResult를 0으로 초기화한다. */
void evaluation();	// 후위 표기로 바꾼 수식을 계산한다.

int main()
{
	char command;			// 명령어를 입력받을 char형 command 생성

	printf("[----- [복무창] [2021040021] -----]\n");

	do
	{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	//명령어를 입력받는다.

		switch (command)
		{
		case 'i':		// i 또는 I를 입력 받는 경우
		case 'I':		
			getInfix(); // 수식을 입력 받는다.
			break;
		case 'p':		// p 또는 P를 입력 받는 경우
		case 'P':
			toPostfix();	// 중위 표기를 후위 표기로 바꾼다.
			break;
		case 'e':		// e 또는 E를 입력 받는 경우
		case 'E':
			evaluation();	// 후위표기로 바뀐 수식을 계산한다.
			break;
		case 'd':		// d 또는 D를 입력 받는 경우
		case 'D':
			debug();	// infixExp, postfixExp, evalResult, postfixStack의 상태를 보여준다.
			break;
		case 'r':		// r 또는 R을 입력 받는 경우
		case 'R':
			reset();
			/* infixExp, postfixExp, postfixStack을 비우고, postfixStackTop과 evalStackTop은 -1로 초기화한다.
 			또, evalResult를 0으로 초기화한다. */
			break;
		case 'q': 		// q 또는 Q를 입력 받는 경우
		case 'Q':
			break;		// switch문을 빠져나간다.
		default:		// 정해진 명령어 이외의 명령어를 입력했을 경우
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');		// q와 Q를 입력받지 않을 때만 반복한다.

	return 1;
}

void postfixPush(char x)	// postfixStack의 push 
{
	postfixStack[++postfixStackTop] = x;        
	// postfixStackTop을 1 증가시키고, postfixStack[postfixStackTop]에 x를 넣는다.
}

char postfixPop()
{
	char x;
	if (postfixStackTop == -1)			
	// postfixStackTop이 -1이라는 것은 postfixStack이 비어있는 상태라는 것이다.
		return '\0';					// NULL 반환
	else	// postfixStack에 무엇인가 들어있으면
	{
		x = postfixStack[postfixStackTop--];  
		// postfixStack[postfixStackTop]을 x에 할당하고 postfixStackTop을 1 감소시킨다.
	}
	return x;	// x 반환
}

void evalPush(int x)	// evalStack의 push
{
	evalStack[++evalStackTop] = x;	
	//evalStackTop을 1 증가시키고, evalStack[evalStackTop]에 x를 넣는다.
}

int evalPop()
{
	if (evalStackTop == -1)
	// evalStackTop이 -1이라는 것은 evalStack이 비어있다는 상태라는 것이다.
		return -1;		// -1 반환
	else // evalStack에 무엇인가 들어있으면
		return evalStack[evalStackTop--]; 
		// evalStack[evalStackTop]을 반환하고, evalStackTop을 1 감소시킨다.
}

void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);		// 사용자로부터 수식을 입력받아 infixExp에 저장한다.
}

precedence getToken(char symbol) // symbol에 해당하는 연산자의 우선순위를 precedence에서 구한다.
{
	switch (symbol) 
	{
	case '(':				// '('의 경우
		return lparen;		// lparen 반환 즉, 0 반환
	case ')':				// ')'의 경우
		return rparen;		// rparen 반환 즉, 9 반환
	case '+':				// '+'의 경우
		return plus;		// plus 반환 즉, 5 반환
	case '-':				// '-'의 경우
		return minus;		// minus 반환 즉, 4 반환
	case '/':				// '/' 경우
		return divide;		// divide 반환 즉, 6 반환
	case '*':				// '*'의 경우
		return times;		// times 반환 즉, 7 반환
	default:				// 그 밖의 경우
		return operand;		// operand 반환 즉, 1 반환
	}
}

precedence getPriority(char x)	//  x의 우선순위를 구한다.
{
	return getToken(x);			// x에 해당하는 연산자의 우선순위 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char *c)	// c에 있는 문자 하나를 postfixExp에 복사한다.
{
	if (postfixExp == '\0')			// postfixExp에 널문자가 들어있다면
		strncpy(postfixExp, c, 1);	// c의 문자 하나를 postfixExp에 복사한다.
	else // postfixExp 수식이 들어있다면
		strncat(postfixExp, c, 1);	// c의 문자 하나를 postfixExp에 복사한다.
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()		// 중위 표기를 후위 표기로 바꾼다.
{ 
	char *exp = infixExp; // infixExp의 문자 하나씩을 읽기위한 포인터
	char x; // 문자하나를 임시로 저장하기 위한 변수

	// exp를 증가시켜가면서, 문자를 읽고 postfix로 변경한다.
	while (*exp != '\0')	// exp가 exp가 가리키는 infixExp의 널문자를 가리키지 않을 때만 반복
	{
		if (getPriority(*exp) == operand)	//  exp가 가리키는 것의 우선순위가 피연산자에 해당한다면
		{
			x = *exp;		// x에 exp가 가리키는 것을 저장한다.
			charCat(&x);	// postfixExp에 x를 복사한다.
		}
		else if (getPriority(*exp) == lparen)	// exp가 가리키는 것의 우선순위가 왼쪽 괄호라면
		{
			postfixPush(*exp);	// postfixStack에 push
		}
		else if (getPriority(*exp) == rparen)	// exp가 가리키는 것의 우선순위가 오른쪽 괄호라면
		{
			while ((x = postfixPop()) != '(')	
			// postfixStack에서 pop한 것을 x에 할당하고, 값을 가지게 된 x가 '(' 아닐 때만 반복
			{
				charCat(&x);	// postfixExp에 x를 복사한다.
			}
		}
		else 
		// exp가 가리키는 것의 우선순위가 피연산자, 왼쪽 괄호, 오른쪽 괄호를 제외한 그 밖의 연산자라면
		{
			while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
			// postfixStack[postfixStackTop]의 우선순위가 exp가 가리키는 것의 우선순위 이상일 때만 반복
			{
				x = postfixPop();	// x에 postfixStack에서 pop한 것을 할당한다.
				charCat(&x);	// postfixExp에 x를 복사한다.
			}
			postfixPush(*exp);	// postfixStakc에 exp가 가리키는 것을 push한다.
		}
		exp++; // exp가 다음을 가리키도록 한다.
	}

	while (postfixStackTop != -1)	
	// postfixStackTop이 -1이 아닐 때만 반복 즉, postfixStack에 값이 들어있을 때만 반복
	{
		x = postfixPop();	// postfixStack에서 pop한 것을 x에 할당한다.
		charCat(&x);	//  postfixExp에에 x를 복사한다.
	}
}
void debug()	// infixExp, postfixExp, evalResult, postfixStack의 상태를 보여준다.
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);   	// infixExp 출력
	printf("postExp =  %s\n", postfixExp);		// postfixExp 출력
	printf("eval result = %d\n", evalResult);	// evalResult 출력

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)	// MAX_STACK_SIZE번 반복
		printf("%c  ", postfixStack[i]);		// postfixStack의 요소를 0번부터 MAX_STACK_SIZE-1 까지 출력

	printf("\n");
}

void reset()
{
	infixExp[0] = '\0';			// infixExp를 널문자로 초기화
	postfixExp[0] = '\0';		// postfixExp를 널문자로 초기화

	for (int i = 0; i < MAX_STACK_SIZE; i++)	// MAX_STACK_SIZE번 반복
		postfixStack[i] = '\0';		// postfixStack의 요소를 0번부터 MAX_STACK_SIZE-1 까지 널문자로 초기화

	postfixStackTop = -1;	//	postfixStackTop을 -1로 초기화
	evalStackTop = -1;		// evalStackTop을 -1로 초기화
	evalResult = 0;			// evalResult를 0으로 초기화
}

void evaluation()	// 후위 표기로 바꾼 수식을 계산한다.
{
	int opr1, opr2, i;	// 피연산자를 할당할 opr1과 opr2, 반복문에 쓰일 i를 생성한다.

	int length = strlen(postfixExp);	// length에 postfixExp의 길이를 할당한다.
	char symbol;	// 연산자를 할당할 symbol 생성
	evalStackTop = -1;	// evalStackTop을 -1로 초기화

	for (i = 0; i < length; i++)	// length번 반복
	{
		symbol = postfixExp[i];	// postfixExp의 i번째 요소를 symbol에 할당
		if (getToken(symbol) == operand) // symbol의 우선순위가 operand와 같다면
		{
			evalPush(symbol - '0'); 	
			// 계산을 위한 숫자로 바꾸기 위해 '0'의 아스키 코드 값만큼 빼준다.
			// 그 값을 evalStack에 push
		}
		else	// symbol의 우선순위가 연산자라면
		{
			opr2 = evalPop(); //evalStack에서 pop한 것을 opr2 할당한다.
			opr1 = evalPop(); //evalStack에서 pop한 것을 opr1 할당한다.
			switch (getToken(symbol))
			{
			case plus:					// plus의 경우
				evalPush(opr1 + opr2);	// opr1+opr2를 연산 후 그 결과를 evalStack에 push 
				break;					// switch문 빠져나간다.
			case minus:					// minus의 경우
				evalPush(opr1 - opr2);	// opr1-opr2를 연산 후 그 결과를 evalStack에 push
				break;					// switch문 빠져나간다.
			case times:					// times의 경우
				evalPush(opr1 * opr2);	// opr1*opr2를 연산 후 그 결과를 evalStack에 push
				break;					// switch문 빠져나간다.
			case divide:				// divide의 경우
				evalPush(opr1 / opr2);	// opr1/opr2를 연산 후 그 결과를 evalStack에 push
				break;					// switch문 빠져나간다.
			default:					// 그 밖의 경우
				break;					// switch문 빠져나간다.
			}
		}
	}
	evalResult = evalPop(); 
	// 연산이 완료되면 하나의 값이 남게 되는데, 그 값을 evalStack에서 pop해 evalResult에 할당한다.
}