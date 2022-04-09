#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum				// �ϸ��(mnemonic) ����ؼ� Ư�����ڸ� �˱� ���� ǥ�� 
{
	lparen = 0, /* ( ���� ��ȣ            
	���� �ۿ����� ���� ��ȣ�� �켱������ ���� ������, ���� �ȿ����� ���� ��ȣ�� �켱������ ���ٰ� ����Ѵ�.*/
	rparen = 9, /* ) ������ ��ȣ*/
	times = 7,	/* * ���� */
	divide = 6, /* / ������ */
	plus = 5,	/* + ���� */
	minus = 4,	/* - ���� */
	operand = 1 /* �ǿ����� */  
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];	  /* ���� ǥ�⸦ ������ �迭 */
char postfixExp[MAX_EXPRESSION_SIZE]; /* ���� ǥ�⸦ ���� ǥ��� ��ȯ�� ���� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];	  /* ����ǥ����� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];		  /* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1; /* postfixStack�� top,  ������ ������� �� -1 */
int evalStackTop = -1;	  /* evalStack�� top, ������ ������� �� -1 */

int evalResult = 0; /* ��� ����� ������ ���� */

void postfixpush(char x);           // postfixStack�� push 
char postfixPop();					// postfixStack�� pop 
void evalPush(int x);				// evalStack�� push
int evalPop();						// evalStack�� pop 
void getInfix();					// ����ڷκ��� ����(���� ǥ��)�� �Է¹޴´�.
precedence getToken(char symbol);	// symbol�� �ش��ϴ� �������� �켱������ precedence���� ���Ѵ�.
precedence getPriority(char x);		// x�� �켱������ ���Ѵ�.
void charCat(char *c);				// c�� �ִ� ���ڸ� �ϳ��� postfixExp�� �����Ѵ�.
void toPostfix();					// ���� ǥ�⸦ ���� ǥ��� �ٲ۴�.
void debug();						// infixExp, postfixExp, evalResult, postfixStack�� ���¸� �����ش�.
void reset();							
/* infixExp, postfixExp, postfixStack�� ����, postfixStackTop�� evalStackTop�� -1�� �ʱ�ȭ�Ѵ�.
 ��, evalResult�� 0���� �ʱ�ȭ�Ѵ�. */
void evaluation();	// ���� ǥ��� �ٲ� ������ ����Ѵ�.

int main()
{
	char command;			// ��ɾ �Է¹��� char�� command ����

	printf("[----- [����â] [2021040021] -----]\n");

	do
	{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	//��ɾ �Է¹޴´�.

		switch (command)
		{
		case 'i':		// i �Ǵ� I�� �Է� �޴� ���
		case 'I':		
			getInfix(); // ������ �Է� �޴´�.
			break;
		case 'p':		// p �Ǵ� P�� �Է� �޴� ���
		case 'P':
			toPostfix();	// ���� ǥ�⸦ ���� ǥ��� �ٲ۴�.
			break;
		case 'e':		// e �Ǵ� E�� �Է� �޴� ���
		case 'E':
			evaluation();	// ����ǥ��� �ٲ� ������ ����Ѵ�.
			break;
		case 'd':		// d �Ǵ� D�� �Է� �޴� ���
		case 'D':
			debug();	// infixExp, postfixExp, evalResult, postfixStack�� ���¸� �����ش�.
			break;
		case 'r':		// r �Ǵ� R�� �Է� �޴� ���
		case 'R':
			reset();
			/* infixExp, postfixExp, postfixStack�� ����, postfixStackTop�� evalStackTop�� -1�� �ʱ�ȭ�Ѵ�.
 			��, evalResult�� 0���� �ʱ�ȭ�Ѵ�. */
			break;
		case 'q': 		// q �Ǵ� Q�� �Է� �޴� ���
		case 'Q':
			break;		// switch���� ����������.
		default:		// ������ ��ɾ� �̿��� ��ɾ �Է����� ���
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');		// q�� Q�� �Է¹��� ���� ���� �ݺ��Ѵ�.

	return 1;
}

void postfixPush(char x)	// postfixStack�� push 
{
	postfixStack[++postfixStackTop] = x;        
	// postfixStackTop�� 1 ������Ű��, postfixStack[postfixStackTop]�� x�� �ִ´�.
}

char postfixPop()
{
	char x;
	if (postfixStackTop == -1)			
	// postfixStackTop�� -1�̶�� ���� postfixStack�� ����ִ� ���¶�� ���̴�.
		return '\0';					// NULL ��ȯ
	else	// postfixStack�� �����ΰ� ���������
	{
		x = postfixStack[postfixStackTop--];  
		// postfixStack[postfixStackTop]�� x�� �Ҵ��ϰ� postfixStackTop�� 1 ���ҽ�Ų��.
	}
	return x;	// x ��ȯ
}

void evalPush(int x)	// evalStack�� push
{
	evalStack[++evalStackTop] = x;	
	//evalStackTop�� 1 ������Ű��, evalStack[evalStackTop]�� x�� �ִ´�.
}

int evalPop()
{
	if (evalStackTop == -1)
	// evalStackTop�� -1�̶�� ���� evalStack�� ����ִٴ� ���¶�� ���̴�.
		return -1;		// -1 ��ȯ
	else // evalStack�� �����ΰ� ���������
		return evalStack[evalStackTop--]; 
		// evalStack[evalStackTop]�� ��ȯ�ϰ�, evalStackTop�� 1 ���ҽ�Ų��.
}

void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);		// ����ڷκ��� ������ �Է¹޾� infixExp�� �����Ѵ�.
}

precedence getToken(char symbol) // symbol�� �ش��ϴ� �������� �켱������ precedence���� ���Ѵ�.
{
	switch (symbol) 
	{
	case '(':				// '('�� ���
		return lparen;		// lparen ��ȯ ��, 0 ��ȯ
	case ')':				// ')'�� ���
		return rparen;		// rparen ��ȯ ��, 9 ��ȯ
	case '+':				// '+'�� ���
		return plus;		// plus ��ȯ ��, 5 ��ȯ
	case '-':				// '-'�� ���
		return minus;		// minus ��ȯ ��, 4 ��ȯ
	case '/':				// '/' ���
		return divide;		// divide ��ȯ ��, 6 ��ȯ
	case '*':				// '*'�� ���
		return times;		// times ��ȯ ��, 7 ��ȯ
	default:				// �� ���� ���
		return operand;		// operand ��ȯ ��, 1 ��ȯ
	}
}

precedence getPriority(char x)	//  x�� �켱������ ���Ѵ�.
{
	return getToken(x);			// x�� �ش��ϴ� �������� �켱���� ��ȯ
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char *c)	// c�� �ִ� ���� �ϳ��� postfixExp�� �����Ѵ�.
{
	if (postfixExp == '\0')			// postfixExp�� �ι��ڰ� ����ִٸ�
		strncpy(postfixExp, c, 1);	// c�� ���� �ϳ��� postfixExp�� �����Ѵ�.
	else // postfixExp ������ ����ִٸ�
		strncat(postfixExp, c, 1);	// c�� ���� �ϳ��� postfixExp�� �����Ѵ�.
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()		// ���� ǥ�⸦ ���� ǥ��� �ٲ۴�.
{ 
	char *exp = infixExp; // infixExp�� ���� �ϳ����� �б����� ������
	char x; // �����ϳ��� �ӽ÷� �����ϱ� ���� ����

	// exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� �����Ѵ�.
	while (*exp != '\0')	// exp�� exp�� ����Ű�� infixExp�� �ι��ڸ� ����Ű�� ���� ���� �ݺ�
	{
		if (getPriority(*exp) == operand)	//  exp�� ����Ű�� ���� �켱������ �ǿ����ڿ� �ش��Ѵٸ�
		{
			x = *exp;		// x�� exp�� ����Ű�� ���� �����Ѵ�.
			charCat(&x);	// postfixExp�� x�� �����Ѵ�.
		}
		else if (getPriority(*exp) == lparen)	// exp�� ����Ű�� ���� �켱������ ���� ��ȣ���
		{
			postfixPush(*exp);	// postfixStack�� push
		}
		else if (getPriority(*exp) == rparen)	// exp�� ����Ű�� ���� �켱������ ������ ��ȣ���
		{
			while ((x = postfixPop()) != '(')	
			// postfixStack���� pop�� ���� x�� �Ҵ��ϰ�, ���� ������ �� x�� '(' �ƴ� ���� �ݺ�
			{
				charCat(&x);	// postfixExp�� x�� �����Ѵ�.
			}
		}
		else 
		// exp�� ����Ű�� ���� �켱������ �ǿ�����, ���� ��ȣ, ������ ��ȣ�� ������ �� ���� �����ڶ��
		{
			while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
			// postfixStack[postfixStackTop]�� �켱������ exp�� ����Ű�� ���� �켱���� �̻��� ���� �ݺ�
			{
				x = postfixPop();	// x�� postfixStack���� pop�� ���� �Ҵ��Ѵ�.
				charCat(&x);	// postfixExp�� x�� �����Ѵ�.
			}
			postfixPush(*exp);	// postfixStakc�� exp�� ����Ű�� ���� push�Ѵ�.
		}
		exp++; // exp�� ������ ����Ű���� �Ѵ�.
	}

	while (postfixStackTop != -1)	
	// postfixStackTop�� -1�� �ƴ� ���� �ݺ� ��, postfixStack�� ���� ������� ���� �ݺ�
	{
		x = postfixPop();	// postfixStack���� pop�� ���� x�� �Ҵ��Ѵ�.
		charCat(&x);	//  postfixExp���� x�� �����Ѵ�.
	}
}
void debug()	// infixExp, postfixExp, evalResult, postfixStack�� ���¸� �����ش�.
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);   	// infixExp ���
	printf("postExp =  %s\n", postfixExp);		// postfixExp ���
	printf("eval result = %d\n", evalResult);	// evalResult ���

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)	// MAX_STACK_SIZE�� �ݺ�
		printf("%c  ", postfixStack[i]);		// postfixStack�� ��Ҹ� 0������ MAX_STACK_SIZE-1 ���� ���

	printf("\n");
}

void reset()
{
	infixExp[0] = '\0';			// infixExp�� �ι��ڷ� �ʱ�ȭ
	postfixExp[0] = '\0';		// postfixExp�� �ι��ڷ� �ʱ�ȭ

	for (int i = 0; i < MAX_STACK_SIZE; i++)	// MAX_STACK_SIZE�� �ݺ�
		postfixStack[i] = '\0';		// postfixStack�� ��Ҹ� 0������ MAX_STACK_SIZE-1 ���� �ι��ڷ� �ʱ�ȭ

	postfixStackTop = -1;	//	postfixStackTop�� -1�� �ʱ�ȭ
	evalStackTop = -1;		// evalStackTop�� -1�� �ʱ�ȭ
	evalResult = 0;			// evalResult�� 0���� �ʱ�ȭ
}

void evaluation()	// ���� ǥ��� �ٲ� ������ ����Ѵ�.
{
	int opr1, opr2, i;	// �ǿ����ڸ� �Ҵ��� opr1�� opr2, �ݺ����� ���� i�� �����Ѵ�.

	int length = strlen(postfixExp);	// length�� postfixExp�� ���̸� �Ҵ��Ѵ�.
	char symbol;	// �����ڸ� �Ҵ��� symbol ����
	evalStackTop = -1;	// evalStackTop�� -1�� �ʱ�ȭ

	for (i = 0; i < length; i++)	// length�� �ݺ�
	{
		symbol = postfixExp[i];	// postfixExp�� i��° ��Ҹ� symbol�� �Ҵ�
		if (getToken(symbol) == operand) // symbol�� �켱������ operand�� ���ٸ�
		{
			evalPush(symbol - '0'); 	
			// ����� ���� ���ڷ� �ٲٱ� ���� '0'�� �ƽ�Ű �ڵ� ����ŭ ���ش�.
			// �� ���� evalStack�� push
		}
		else	// symbol�� �켱������ �����ڶ��
		{
			opr2 = evalPop(); //evalStack���� pop�� ���� opr2 �Ҵ��Ѵ�.
			opr1 = evalPop(); //evalStack���� pop�� ���� opr1 �Ҵ��Ѵ�.
			switch (getToken(symbol))
			{
			case plus:					// plus�� ���
				evalPush(opr1 + opr2);	// opr1+opr2�� ���� �� �� ����� evalStack�� push 
				break;					// switch�� ����������.
			case minus:					// minus�� ���
				evalPush(opr1 - opr2);	// opr1-opr2�� ���� �� �� ����� evalStack�� push
				break;					// switch�� ����������.
			case times:					// times�� ���
				evalPush(opr1 * opr2);	// opr1*opr2�� ���� �� �� ����� evalStack�� push
				break;					// switch�� ����������.
			case divide:				// divide�� ���
				evalPush(opr1 / opr2);	// opr1/opr2�� ���� �� �� ����� evalStack�� push
				break;					// switch�� ����������.
			default:					// �� ���� ���
				break;					// switch�� ����������.
			}
		}
	}
	evalResult = evalPop(); 
	// ������ �Ϸ�Ǹ� �ϳ��� ���� ���� �Ǵµ�, �� ���� evalStack���� pop�� evalResult�� �Ҵ��Ѵ�.
}