// Program to evaluate infix expressions
// Supported operators: + - x * / ^ ( )
// Multiplication can be done with * or x

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

// simple stack with macros
#define stackSize 1024
double stack[stackSize] = {0};
int top = -1;
#define peek stack[top]
#define isEmpty top == -1
#define push(val) stack[++top] = val
#define pop --top

// whether to print postfix parse for debugging
#define PRINT_POSTFIX 1

#define MAXLEN 1024 // max length of strings

int isOperator(int c)
{
	return (c == '+' || c == '-' || c == '*' || c == 'x' || c == '/' || c == '^');
}

double calc(double a, double b, char op)
{
	switch(op) {
		case '+':
			return a + b;
		case '-':
			return a - b;
		case 'x':
		case '*':
			return a * b;
		case '^':
			return pow(a,b);
		default: // case '/':
			return a / b;
	}
}

void postfix_eval(char *expr)
{
	for(char *token = strtok(expr, " "); token; token = strtok(NULL, " ")) {
		size_t j = strlen(token);
		// handling positive numbers
		if(isdigit(token[0]) || token[0] == '.') {
			while(--j > 0)
				if(!(isdigit(token[j]) || token[j] == '.')) {
					goto error;
				}
			push(atof(token));
		}
		// handling negative numbers
		else if(j > 1 && token[0] == '-') {
			while(--j > 0)
				if(!isdigit(token[j])) {
					goto error;
				}
			push(atof(token));
		}
		// handling operators
		else if(j == 1 && isOperator(token[0])) {
			if(isEmpty) {
				goto error;
			}
			double b = peek;
			pop;
			if(isEmpty) {
				goto error;
			}
			double a = peek;
			pop;
			push(calc(a, b, *token));
		}
		else {
			goto error;
		}
	}
	
	double res;
	if(!(isEmpty)) {
		res = peek;
		pop;
	}
	else
		goto error;
	if(!(isEmpty)) {
		goto error;
	}
	
	printf("\nResult: %f\n", res);
	return;
	
	error:
	puts("Invalid expression");
}

int prec(char c)
{
	if(c == '+' || c == '-')
		return 0;
	if(c == 'x' || c == '*' || c == '/')
		return 1;
	if(c == '^')
		return 2;
	else
		return -1;
}

char *infix2postfix(char *expr)
{
	int len = strlen(expr);
	// allocated memory is freed in main later
	char *output = calloc(len * 2, sizeof(char));
	char *oPtr = output;
	
	for(int c = 0; c < len; c++) {
		// handling numbers
		if(isdigit(expr[c]) || expr[c] == '.') {
			*oPtr++ = expr[c];
			if(!(isdigit(expr[c + 1]) || expr[c + 1] == '.'))
				*oPtr++ = ' ';
		}
		// handling operators
		else if(expr[c] != '(' && expr[c] != ')') {
			// handling negative numbers
			if(expr[c] == '-' && (c == 0 || !isdigit(expr[c - 1]))) 
				*oPtr++ = expr[c];
			
			// handling regular operators
			else {
				while(!(isEmpty) && (prec(peek) >= prec(expr[c]))) {
					*oPtr++ = peek;
					*oPtr++ = ' ';
					pop;
				}
				push(expr[c]);
			}
		}
		// handling parenthesis open
		else if(expr[c] == '(') {
			push(expr[c]);
		}
		// handling parenthesis close
		else if(expr[c] == ')') {
			while(!(isEmpty) && peek != '(') {
				*oPtr++ = peek;
				*oPtr++ = ' ';
				pop;
			}
			if(!(isEmpty))
				pop;
		}
	}
	while(!(isEmpty)) {
		*oPtr++ = peek;
		*oPtr++ = ' ';
		pop;
	}
	
#if PRINT_POSTFIX
	puts("\nAfter converting to postfix:");
	puts(output);
#endif
	
	return output;
}

int main()
{
	puts("Enter expression without spaces:");
	char expr[MAXLEN + 1], tmp[MAXLEN + 1];
	fgets(tmp, MAXLEN, stdin);
	tmp[strcspn(tmp, "\n")] = 0;
	
	// copy tmp to expr, replacing negative signs
	// ... with '-1*'
	int i=0	, j=0;
	while(i <= MAXLEN) {
		if(tmp[i] == '-') {
			if(i == 0 || !isdigit(tmp[i-1])) {
				strncpy(&expr[j], "-1*", 3);
				j+=3, i++;
				continue;
			}
		}
		expr[j++] = tmp[i++];
	}
	
	char *converted = infix2postfix(expr);
	postfix_eval(converted);
	free(converted);
}
