/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <vector>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 14

//typedef enum
// /* book-keeping tokens */
//{
//    ENDFILE, ERRO,
//    /* reserved words */
//    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE, WHILE,DO, ENDWHILE,FOR,TO, DOWNTO, ENDDO,
//    /* multicharacter tokens */
//    ID, NUM,
//    /* special symbols */
//    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI,ADDITIVE,POWER
//} TypeToken;

/*
 * miniC修改部分
 */
typedef enum
/* book-keeping tokens */
{
    ENDFILE, ERRO,
    /* reserved words */
    ELSE, IF, RETURN, WHILE, INT, VOID,
    /* multicharacter tokens */
    ID, NUM,
    /* special symbols*//* lparen: '(', lbracket: '[', lbrace: '{' */
    ASSIGN, EQ, NEQ, LT, LTEQ, GT, GTEQ, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, LBRACKET, RBRAKET, LBRACE, RBRACE, SEMI,ADDITIVE,POWER
} TypeToken;
/*
 * END
 */

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

                   /**************************************************/
                   /***********   Syntax tree for parsing ************/
                   /**************************************************/

typedef enum { StmtK, ExpK } NodeKind;
//typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK , WhileK, DowhileK, ForIncK, ForDecK, AdditiveK} StmtKind;
/*miniC*/
typedef enum { IfK, AssignK, ReadK, WriteK , WhileK, ForIncK, VarDclK, FunDclK, CompndK} StmtKind;

typedef enum { OpK, ConstK, IdK } ExpKind;

/* ExpType is used for type checking */
typedef enum { Void, Integer, Boolean, IntList, VoidList } ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union {
        TypeToken op;
        int val;
        char * name;
    } attr;
    ExpType type; /* for type checking of exps */
} TreeNode;


/*
* miniC存储全局变量结构体
*/
typedef struct varStruct
{
    ExpType type; //变量类型
    char * name;      //变量名
} VarStruct;

/*
* miniC存储函数结构体
*/
typedef struct funStruct
{
    ExpType returnType; //函数返回类型
    char * name;      //函数名
    std::vector<VarStruct> params;   //函数内部定义的参数及变量
} FunStruct;

/*
* miniC存储全局变量及函数的vector容器
*/

/*
* miniC修改部分，添加vector容器装载定义的全局变量和函数
*/
extern std::vector<VarStruct> VarVector;
extern std::vector<FunStruct> FunVector;



/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
* be echoed to the listing file with line numbers
* during parsing
*/
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
* printed to the listing file as each token is
* recognized by the scanner
*/
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
* printed to the listing file in linearized form
* (using indents for children)
*/
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
* and lookups to be reported to the listing file
*/
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
* to the TM code file as code is generated
*/
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;

#endif
