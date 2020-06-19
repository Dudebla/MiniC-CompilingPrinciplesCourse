/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

/**
  * GlOBALS.H
  *
  * @brief Based on the TINY compiler file. Handling the global types and vars.
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/6/10
  */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <map>
#include <QString>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 6

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
    ASSIGN, EQ, NEQ, LT, LTEQ, GT, GTEQ, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, SEMI, COMMA, ADDITIVE,POWER
} TypeToken;
/*
 * END
 */

extern FILE* source; /* source code t ext file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */
extern QString Last_fileName;

extern int lineno; /* source line number for listing */
extern int EOF_flag; /* corrects ungetNextChar behavior on EOF */

extern std::string lexicalMessage;
extern std::string errorMessage;

extern TypeToken token;

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum { StmtK, ExpK } NodeKind;
/*miniC*/
typedef enum { IfK, ReadK, WriteK , WhileK, VarDclK, FunDclK, CompndK, ReturnK , ParamK} StmtKind;

typedef enum { OpK, ConstK, IdK, AssignK, CallK , ArgsK } ExpKind;

/* ExpType is used for type checking */
typedef enum { Void, Integer, IntList, VoidList, Boolean } ExpType;

#define MAXCHILDREN 3

struct ScopeRec;

/* ArrayAttr is used for attributes for array variables */
typedef struct arrayAttr {
    TypeToken type;
    char * name;
    int size;
} ArrayAttr;

typedef struct treeNode
{
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union {
        TypeToken op;
        TypeToken type;
        int val;
        char * name;
        ArrayAttr arr;
        struct ScopeRec * scope;
    } attr;
    ExpType type; /* for type checking of exps */
} TreeNode;

/*  var type in function, declare in param or in comp*/
typedef enum { PARAM_VAR, COMN_VAR } Func_var;

/*
* miniC存储变量结构体
*/
typedef struct varStruct
{
    ExpType type; //变量类型
    std::string name;      //变量名
    int memloc;          //在内存中的相对位置
    int size; //length, default 1
    int scope; //scope index, default -1
    Func_var varType;
} VarStruct;

/*
* miniC存储函数结构体
*/
typedef struct funStruct
{
    ExpType returnType; //函数返回类型
    std::string name;      //函数名
    int paramsNum;    //参数个数
    std::map<std::string, VarStruct> params;   //函数内部定义的参数及变量
} FunStruct;

/*
* miniC存储全局变量及函数的vector容器
*/

extern std::map<std::string, VarStruct> VarStructMap;
extern std::map<std::string, FunStruct> FunStructMap;


typedef enum{
    VarDcl, FunDcl, VarFunDcl
}VarFunDclType;// declaration type

typedef enum{
    InCompound, InFunDcl, GlobalVarDcl
}ManageMapState;// node parsing status

extern ManageMapState manageMapState;
extern std::string lastDeclaredFunName;
extern std::string lastDeclaredVarName;


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
