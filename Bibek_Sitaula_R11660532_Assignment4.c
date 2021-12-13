//********************************************************************************************************//
/*
** Bibek Sitaula | Homework#4 | R11660532 | 10/30/2020
**
** This program validates lexemes and tokens provided by the user in the source code file
*/
//********************************************************************************************************//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* Token codes */	/*Initilizing and defining all required token group*/
#define ASSIGN_OP 1
#define LESSER_OP 2
#define GREATER_OP 3
#define EQUAL_OP 4
#define NEQUAL_OP 5
#define GEQUAL_OP 6
#define LEQUAL_OP 21
#define ADD_OP 7
#define SUB_OP 8
#define MULT_OP 9
#define DIV_OP 10
#define KEY_READ 11
#define KEY_WRITE 12
#define KEY_WHILE 13
#define KEY_DO 14
#define KEY_OD 15
#define IDENT 16
#define INT_LIT 17
#define LEFT_PAREN 18
#define RIGHT_PAREN 19
#define SEMICOLON 20
#define UNKNOWN 99
#define INVALID_TOKEN 44

/*************** Global Variable ***************************/
int nextToken;
int lex();
static void error();
void P();
void S();
void C();
void E();
void T();
void F();
void S1();
//const char *get_name(int nextToken);
/************** Local Variables ****************************/
static int charClass;
static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *fp;

/*************** Local Function declarations *****************/
static void addChar();
static void getChar();
static void getNonBlank();

/***************************************************************************/
/******************** main function driver**********************************/

int main(int argc, char *argv[])
{
	char *file_name;	//Input from filename for example test.danc
	if (argc != 2)		//checking if the filename exist or not in command line. If not, display error message
  {
       printf("Error!!! Missing Filename\n");
       return(2);
  }
  else
 {
       file_name = argv[1];
  }
    /* Open the input data file and process its contents */
    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("ERROR - File doesnot exist. Provide valid input filename. \n");
				return(3);
    } else {
    	printf("DanC Analyzer :: R11660532\n\n");
    	printf("Input file name is: %s\n\n", file_name);
        getChar();
        do {
            lex();
						P();
        } while (nextToken != EOF);
				printf("Syntax Validated\n");
    }
    return 0;
}

/***********************************************************************************/
/** lookup - a function to lookup operators and parentheses and return the token code*****
	if the character is matched, add to the lexeme and print its corresponding token group to consol
*/

static int lookup(char ch) {
    switch (ch) {
    	//Parse Parentheses and operators
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case ':':
        	addChar();
        	getChar();
        	if (nextChar == '='){
        		addChar();
        		nextToken = ASSIGN_OP;
				break;
			}else{
				nextToken = UNKNOWN;
				break;
			}
		case '=':
        	addChar();
        	nextToken = EQUAL_OP;
        	break;
        case ';':
        	addChar();
        	nextToken = SEMICOLON;
        	break;
        case '<':
        	addChar();
        	getChar();
        	if (nextChar == '>'){
        		addChar();
        		nextToken = NEQUAL_OP;
				break;
			}else if(nextChar == '='){
				addChar();
        		nextToken = LEQUAL_OP;
				break;
			}else{
        		nextToken = LESSER_OP;
        		break;
			}
		case '>':
        	addChar();
        	getChar();
        	if (nextChar == '='){
        		addChar();
        		nextToken = GEQUAL_OP;
				break;
			}else{
        		nextToken = GREATER_OP;
        		break;
			}
        //To identify the UNKNOWN TOKEN GROUP
        default:
            addChar();
            break;
    }
    return nextToken;
}

/************************************************************************************/
/**************** addChar - a function to add nextChar to lexeme ********************/

static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */

static void getChar() {
    if ((nextChar = getc(fp)) != EOF) {
        if (isalpha(nextChar)){
            charClass = IDENT;
        }else if (isdigit(nextChar)){
            charClass = INT_LIT;
		}else {
			charClass = UNKNOWN;
		}
	}else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */

static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/***************************************************************/
/********* lex - a simple lexical analyzer for language ********/
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case IDENT:
            addChar();
            getChar();
            while (charClass == IDENT ) {
                addChar();
                getChar();
			}
			/* To check if the identifiers is KEY_READ, KEY_WRITE, KEY_WHILE, KEY_DO, KEY_OD */
			/* If yes, print the corresponding lexeme and token group */
       if(lexeme[0]=='r'&&lexeme[1]=='e'&&lexeme[2]=='a' && lexeme[3]=='d'){
               nextToken = KEY_READ;
               break;
        	}
			if(lexeme[0]=='w'&&lexeme[1]=='r'&&lexeme[2]=='i' && lexeme[3]=='t' && lexeme[4]=='e'){
               nextToken = KEY_WRITE;
               break;
			}
			if(lexeme[0]=='w'&&lexeme[1]=='h'&&lexeme[2]=='i' && lexeme[3]=='l' && lexeme[4]=='e'){
               nextToken = KEY_WHILE;
               break;
			}
			if(lexeme[0]=='d'&&lexeme[1]=='o'){
               nextToken = KEY_DO;
               break;
			}
			if(lexeme[0]=='o'&&lexeme[1]=='d'){
               nextToken = KEY_OD;
               break;
			}
			nextToken = IDENT;
			break;

        /* Parse integer literals */
        case INT_LIT:
            addChar();
            getChar();
            while (charClass == INT_LIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /* EOF- to check the end of file */
        case EOF:
						nextToken = EOF;
            lexeme[0] = '\n';
            break;
    } /* End of switch */
    return nextToken;
} /* End of function lex */

//***********************************************
// P() - Program or root of the grammar, P -> S
void P(){
	 printf("Enter <P>\n");
	 S();
	 if(nextToken == UNKNOWN || nextToken != EOF){
		 error();
    }
	printf("Exit <P>\n");
 }
//********************************************
//S() - Statement of grammar
//S -> V:=E S1 | read(V) S1 | write(V) S1 | while C do S od S1
void S(){
	printf("Enter <S>\n");
	//S1();
	if(nextToken == IDENT){
			lex();
			if(nextToken == ASSIGN_OP){
					lex();
					E();
					S1();
			}else{
				error();
			}
	}else	if(nextToken == KEY_WRITE || nextToken == KEY_READ){
			lex();
			if(nextToken == LEFT_PAREN){
					lex();
					if(nextToken == IDENT){
							lex();
							if(nextToken == RIGHT_PAREN){
									lex();
									S1();
							}else{
								// printf("ERROR Encountered - 2.\n");
								error();
							}
					}else{
						// printf("ERROR Encountered - 3.\n");
						error();
					}
			}else{
				// printf("ERROR Encountered - 4.\n");
				error();
			}
	}else if(nextToken == KEY_WHILE){
				 lex();
				 C();
				 if(nextToken == KEY_DO){
						 lex();
						 S();
						 if(nextToken == KEY_OD){
								 lex();
 							 	 S1();
						 }else{
							 // printf("ERROR Encountered - 5.\n");
							 error();
						 }
				 }else{
					 // printf("ERROR Encountered - 6.\n");
					 error();
				 }
		 }
	printf("Exit <S>\n");
}
//S1() - Statement_prime function of grammar
//S1 ->  ; S S1 | e

void S1(){
	printf("Enter <S1>\n");
	while(nextToken == SEMICOLON ){
			 lex();
			 if(nextToken == EOF){
				 error();
			 }
			 S();
			 S1();
		}
	printf("Exit <S1>\n");
}

//******************************************************
//C() - comparison function of grammar
//C -> E {(< | > | = | <> | <= | >=) E}
 void C(){
	 printf("Enter <C>\n");
	 /* Determine which RHS */
	 E();
	 if(nextToken == GREATER_OP || nextToken == LESSER_OP || nextToken == EQUAL_OP ||
	 		nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP)	{
		 lex();
		 E();
	 }else{
 		error();
 	}
	 printf("Exit <C>\n");
}
//E() - Expression of grammar
// E -> T {(+ | *) T}
void E() {
	printf("Enter <E>\n");
	T();
	while(nextToken == ADD_OP || nextToken == SUB_OP) {
		/* code */
		lex();
		T();
	}
	printf("Exit <E>\n");
}
//T() - Term function of grammar
//T -> T{(* | /) T}
void T(){
	printf("Enter <T>\n");
	F();
	while (nextToken == MULT_OP || nextToken == DIV_OP) {
		/* code */
		lex();
		F();
	}
	printf("Exit <T>\n");
}
//F() - Factor function of grammar
//F -> (E) | N | V
void F()
{
    printf("Enter <F>\n");
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex(); /* Get the next token */
    } else {
        /* If the RHS is (<expr>), call lex to pass over the
        left parenthesis, call expr, and check for the right
        parenthesis */
        if (nextToken == LEFT_PAREN) {
            lex();
            E();
            if (nextToken == RIGHT_PAREN) {
                lex();
            } else {
						//	printf("ERROR Encountered - 8.\n");
              error();
            }
        } /* End of if (nextToken == ... */
        /* It was not an id, an integer literal, or a left parenthesis */
        else
        {
				//	printf("ERROR Encountered - 9.\n");
          error();
        }
    } /* End of else */
    printf("Exit <F>\n");
}

static void error()
{
    printf("Error Encountere: The next lexeme is %s and the next token (number) is %d\n", lexeme, nextToken);
    exit(1);
}
