// Trabalho ED1 BSI 2023
// Vinicius Person RA: 221022503; 
// Natan Alcantara RA: 221020179;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 512

//FILA

typedef struct Node {
    char data;
    struct Node *next;
} Node;
typedef struct Queue {
    Node *head;
    Node *tail;
} Queue;
void init(Queue *q) {
    q->head = q->tail = NULL;
}
int is_empty(Queue *q) {
    return q->head == NULL;
}
void enqueue(Queue *q, char data) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    if (is_empty(q)) {
        q->head = q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
}
int dequeue(Queue *q) {
    if (is_empty(q)) {
        return -1;
    } else {
        Node *node = q->head;
        char data = node->data;
        q->head = node->next;
        free(node);
        if (q->head == NULL) {
            q->tail = NULL;
        }
        return data;
    }
}
void print_queue(Queue *q) {
    Node *node = q->head;
    while (node != NULL) {
        printf("%c", node->data);
        node = node->next;
    }
}

Queue fila;

//PILHA 

char pilha[MAX];

int top = -1;
int vazio(){
   return top == -1;
}
void push(char data) {
   top++;
   pilha[top] = data;
}
int pop() {
   int data;
   if (vazio()) {
      return -1;
   }
   data = pilha[top];
   top--;
   return data;
}

//CONTROLE

char operadores[] = {'^', '*', '/', '+', '-', '<', '>', '=', '#', '.', '|', 255, 255, 255};

char input[MAX];

int parcount, type, last, err_flag, nopd, nopr;


//OPD = OPERANDO; OPR = OPERADOR; APAR = ABRE PAR.; FPAR = FECHA PAR.;
enum inputs{
	none, opd, opr, apar, fpar
};

enum cod{
	syntax, lex
};

//FUNCOES

void error(int cod) {

	err_flag = 1;

	parcount = 0;

	if(!cod) {
	
		printf("Syntax Error!");
		
	} else {
	
		printf("Lexical Error!");
		
	}
	
}

int verifica(char c) {    
    
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
			nopd++;
			return opd;
		}	
    
    for (int i = 0; i < 13; i++) {
        if (operadores[i] == c) {
            return opr;
        }
    }
    return none;
}

int prio(char c) {

	if(c == '^') return 6;
	if(c == '*' || c == '/') return 5;
	if(c == '+' || c == '-') return 4;
	if(c == '>' || c == '<' || c == '=' || c == '#') return 3;
	if(c == '.') return 2;
	if(c == '|') return 1;
	return 0;
}

int main() {
	
	while(scanf("%s", input) != EOF) {
		memset(pilha, 0, MAX);
		top = -1;
		err_flag = 0;
		parcount = 0;
		char *p = input;
		init(&fila);
		type = none;
		last = none;
		nopd = 0;
		nopr = 0;
		while(*p) {
			
			//VERIFICACAO
			
			type = verifica(*p);		
			if(*p == '(') {
				parcount++;
				type = apar;	
			}
			if(*p == ')') {
				parcount--;
				type = fpar;
				if(vazio()) {
				
					error(syntax);
					break;
					
				}
			}			
			if(parcount < 0) {
			
				error(syntax);
				break;
			}			
			if(type == none) {			
				error(lex);
				break;				
			} else {
				if(type != apar && type != fpar) {
				
					if(type == last) {
					
						error(syntax);
						break;
						
					}
					// (B case
					if(last == fpar && type == opd) {
					
						error(syntax);
						break;
						
					}

					// (+ case
					if(last == apar && type == opr) {
					
						error(syntax);
						break;
						
					}

				}	
				// +) case
				if(type == fpar && last == opr) {
					
					error(syntax);
					break;
					
				}

				// )( case
				if(type == apar && last == fpar) {
					
					error(syntax);
					break;
					
				}

							
			}

			
			if(type == opd) {
			
				enqueue(&fila, *p);
				
			}
			
			if(type == opr) {
				
				if(prio(pilha[top]) >= prio(*p)) {
				
					while(prio(pilha[top]) >= prio(*p)) {
						
						enqueue(&fila, pop());
						
					}
					
					push(*p);
					
				} else {
				
					push(*p);
					
				}
				
			}
			
			if(type == apar) {
			
				push(*p);
				
			}
			
			if(type == fpar) {
				
				char foo;
				
				while(1) {
					
					foo = pop();
					if(foo == '(') break;
					enqueue(&fila, foo);
					
				}
					
				
			}
					
					
			last = type;
			type = none;
			p++;			
		}
		if(!err_flag && (parcount != 0 || nopd < 2)) {
			error(syntax);
		}
		if(!err_flag) {
			while(!vazio()) {
				enqueue(&fila, pop());
			};
			
			print_queue(&fila);
			
		}
		printf("\n");
	}
	
	
}
