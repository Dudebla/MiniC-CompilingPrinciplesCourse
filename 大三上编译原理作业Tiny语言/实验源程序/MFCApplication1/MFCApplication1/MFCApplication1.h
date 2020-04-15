// MFCApplication1.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CMFCApplication1App:
// 有关此类的实现，请参阅 MFCApplication1.cpp
//

class CMFCApplication1App : public CWinApp
{
public:
	CMFCApplication1App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()


};

extern CMFCApplication1App theApp;



///*GLOBALS.H*/
//
///* MAXRESERVED = the number of reserved words */
//#define MAXRESERVED 8
//
//typedef enum
///* book-keeping tokens */
//{
//	ENDFILE, ERRO,
//	/* reserved words */
//	IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
//	/* multicharacter tokens */
//	ID, NUM,
//	/* special symbols */
//	ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
//} TypeToken;
//
//extern FILE* source; /* source code text file */
//extern FILE* listing; /* listing output text file */
//extern FILE* code; /* code text file for TM simulator */
//
//extern int lineno; /* source line number for listing */
//
//				   /**************************************************/
//				   /***********   Syntax tree for parsing ************/
//				   /**************************************************/
//
//typedef enum { StmtK, ExpK } NodeKind;
//typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK } StmtKind;
//typedef enum { OpK, ConstK, IdK } ExpKind;
//
///* ExpType is used for type checking */
//typedef enum { Void, Integer, Boolean } ExpType;
//
//#define MAXCHILDREN 3
//
//typedef struct treeNode
//{
//	struct treeNode * child[MAXCHILDREN];
//	struct treeNode * sibling;
//	int lineno;
//	NodeKind nodekind;
//	union { StmtKind stmt; ExpKind exp; } kind;
//	union {
//		TypeToken op;
//		int val;
//		char * name;
//	} attr;
//	ExpType type; /* for type checking of exps */
//} TreeNode;
//
///**************************************************/
///***********   Flags for tracing       ************/
///**************************************************/
//
///* EchoSource = TRUE causes the source program to
//* be echoed to the listing file with line numbers
//* during parsing
//*/
//extern int EchoSource;
//
///* TraceScan = TRUE causes token information to be
//* printed to the listing file as each token is
//* recognized by the scanner
//*/
//extern int TraceScan;
//
///* TraceParse = TRUE causes the syntax tree to be
//* printed to the listing file in linearized form
//* (using indents for children)
//*/
//extern int TraceParse;
//
///* TraceAnalyze = TRUE causes symbol table inserts
//* and lookups to be reported to the listing file
//*/
//extern int TraceAnalyze;
//
///* TraceCode = TRUE causes comments to be written
//* to the TM code file as code is generated
//*/
//extern int TraceCode;
//
///* Error = TRUE prevents further passes if an error occurs */
//extern int Error;
//
//
///* MAXTOKENLEN is the maximum size of a token */
//#define MAXTOKENLEN 40
//
///* tokenString array stores the lexeme of each token */
//extern char tokenString[MAXTOKENLEN + 1];
//
///* function getToken returns the
//* next token in source file
//*/
//TypeToken getToken(void);
//
///* Procedure printToken prints a token
//* and its lexeme to the listing file
//*/
//char* printToken(TypeToken, const char*);
//
///* Function newStmtNode creates a new statement
//* node for syntax tree construction
//*/
//TreeNode * newStmtNode(StmtKind);
//
///* Function newExpNode creates a new expression
//* node for syntax tree construction
//*/
//TreeNode * newExpNode(ExpKind);
//
///* Function copyString allocates and makes a new
//* copy of an existing string
//*/
//char * copyString(char *);
//
///* procedure printTree prints a syntax tree to the
//* listing file using indentation to indicate subtrees
//*/
//char* printTree(TreeNode *);




