
// MFCApplication1.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///* allocate global variables */
//int lineno = 0;
//FILE * source;
//FILE * listing;
//FILE * code;
//
///* allocate and set tracing flags */
//int EchoSource = FALSE;
//int TraceScan = FALSE;
//int TraceParse = TRUE;
//int TraceAnalyze = TRUE;
//int TraceCode = TRUE;
//
//int Error = FALSE;
//

// CMFCApplication1App

BEGIN_MESSAGE_MAP(CMFCApplication1App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCApplication1App 构造

CMFCApplication1App::CMFCApplication1App()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMFCApplication1App 对象

CMFCApplication1App theApp;


// CMFCApplication1App 初始化

BOOL CMFCApplication1App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CMFCApplication1Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//
///*PARSE.C*/
//
//static TypeToken token; /* holds current token */
//
//						/* function prototypes for recursive calls */
//static TreeNode * stmt_sequence(void);
//static TreeNode * statement(void);
//static TreeNode * if_stmt(void);
//static TreeNode * repeat_stmt(void);
//static TreeNode * assign_stmt(void);
//static TreeNode * read_stmt(void);
//static TreeNode * write_stmt(void);
//static TreeNode * exp(void);
//static TreeNode * simple_exp(void);
//static TreeNode * term(void);
//static TreeNode * factor(void);
//
//static void syntaxError(char * message)
//{
//	fprintf(listing, "\n>>> ");
//	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
//	Error = TRUE;
//}
//
//static void match(TypeToken expected)
//{
//	if (token == expected) token = getToken();
//	else {
//		syntaxError("unexpected token -> ");
//		printToken(token, tokenString);
//		fprintf(listing, "      ");
//	}
//}
//
//TreeNode * stmt_sequence(void)
//{
//	TreeNode * t = statement();
//	TreeNode * p = t;
//	while ((token != ENDFILE) && (token != END) &&
//		(token != ELSE) && (token != UNTIL))
//	{
//		TreeNode * q;
//		match(SEMI);
//		q = statement();
//		if (q != NULL) {
//			if (t == NULL) t = p = q;
//			else /* now p cannot be NULL either */
//			{
//				p->sibling = q;
//				p = q;
//			}
//		}
//	}
//	return t;
//}
//
//
////P394 
////lineno: 961
//TreeNode * statement(void)
//{
//	TreeNode * t = NULL;
//	switch (token) {
//	case IF: t = if_stmt(); break;
//	case REPEAT: t = repeat_stmt(); break;
//	case ID: t = assign_stmt(); break;
//	case READ: t = read_stmt(); break;
//	case WRITE: t = write_stmt(); break;
//	default: syntaxError("unexpected token -> ");
//		printToken(token, tokenString);
//		token = getToken();
//		break;
//	} /* end case */
//	return t;
//}
//
//
////P394 
////lineno: 977
//TreeNode * if_stmt(void)
//{
//	TreeNode * t = newStmtNode(IfK);
//	match(IF);
//	if (t != NULL) t->child[0] = exp();
//	match(THEN);
//	if (t != NULL) t->child[1] = stmt_sequence();
//	if (token == ELSE) {
//		match(ELSE);
//		if (t != NULL) t->child[2] = stmt_sequence();
//	}
//	match(END);
//	return t;
//}
//
////P394 
////lineno:991
//TreeNode * repeat_stmt(void)
//{
//	TreeNode * t = newStmtNode(RepeatK);
//	match(REPEAT);
//	if (t != NULL) t->child[0] = stmt_sequence();
//	match(UNTIL);
//	if (t != NULL) t->child[1] = exp();
//	return t;
//}
//
//TreeNode * assign_stmt(void)
//{
//	TreeNode * t = newStmtNode(AssignK);
//	if ((t != NULL) && (token == ID))
//		t->attr.name = copyString(tokenString);
//	match(ID);
//	match(ASSIGN);
//	if (t != NULL) t->child[0] = exp();
//	return t;
//}
//
//TreeNode * read_stmt(void)
//{
//	TreeNode * t = newStmtNode(ReadK);
//	match(READ);
//	if ((t != NULL) && (token == ID))
//		t->attr.name = copyString(tokenString);
//	match(ID);
//	return t;
//}
//
//TreeNode * write_stmt(void)
//{
//	TreeNode * t = newStmtNode(WriteK);
//	match(WRITE);
//	if (t != NULL) t->child[0] = exp();
//	return t;
//}
//
//TreeNode * exp(void)
//{
//	TreeNode * t = simple_exp();
//	if ((token == LT) || (token == EQ)) {
//		TreeNode * p = newExpNode(OpK);
//		if (p != NULL) {
//			p->child[0] = t;
//			p->attr.op = token;
//			t = p;
//		}
//		match(token);
//		if (t != NULL)
//			t->child[1] = simple_exp();
//	}
//	return t;
//}
//
//TreeNode * simple_exp(void)
//{
//	TreeNode * t = term();
//	while ((token == PLUS) || (token == MINUS))
//	{
//		TreeNode * p = newExpNode(OpK);
//		if (p != NULL) {
//			p->child[0] = t;
//			p->attr.op = token;
//			t = p;
//			match(token);
//			t->child[1] = term();
//		}
//	}
//	return t;
//}
//
//TreeNode * term(void)
//{
//	TreeNode * t = factor();
//	while ((token == TIMES) || (token == OVER))
//	{
//		TreeNode * p = newExpNode(OpK);
//		if (p != NULL) {
//			p->child[0] = t;
//			p->attr.op = token;
//			t = p;
//			match(token);
//			p->child[1] = factor();
//		}
//	}
//	return t;
//}
//
//TreeNode * factor(void)
//{
//	TreeNode * t = NULL;
//	switch (token) {
//	case NUM:
//		t = newExpNode(ConstK);
//		if ((t != NULL) && (token == NUM))
//			t->attr.val = atoi(tokenString);
//		match(NUM);
//		break;
//	case ID:
//		t = newExpNode(IdK);
//		if ((t != NULL) && (token == ID))
//			t->attr.name = copyString(tokenString);
//		match(ID);
//		break;
//	case LPAREN:
//		match(LPAREN);
//		t = exp();
//		match(RPAREN);
//		break;
//	default:
//		syntaxError("unexpected token -> ");
//		printToken(token, tokenString);
//		token = getToken();
//		break;
//	}
//	return t;
//}
//
///****************************************/
///* the primary function of the parser   */
///****************************************/
///* Function parse returns the newly
//* constructed syntax tree
//*/
//TreeNode * parse(void)
//{
//	TreeNode * t;
//	token = getToken();
//	t = stmt_sequence();
//	if (token != ENDFILE)
//		syntaxError("Code ends before file\n");
//	return t;
//}
//
//
//
///*SCAN.C*/
///* states in scanner DFA */
//typedef enum
//{
//	START, INASSIGN, INCOMMENT, INNUM, INID, DONE
//}
//StateType;
//
///* lexeme of identifier or reserved word */
//char tokenString[MAXTOKENLEN + 1];
//
//
///* BUFLEN = length of the input buffer for
//source code lines */
//#define BUFLEN 256
//
//static char lineBuf[BUFLEN]; /* holds the current line */
//static int linepos = 0; /* current position in LineBuf */
//static int bufsize = 0; /* current size of buffer string */
//static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */
//
//							 /* getNextChar fetches the next non-blank character
//							 from lineBuf, reading in a new line if lineBuf is
//							 exhausted */
//static int getNextChar(void)
//{
//	if (!(linepos < bufsize))
//	{
//		lineno++;
//		if (fgets(lineBuf, BUFLEN - 1, source))
//		{
//			if (EchoSource) fprintf(listing, "%4d: %s", lineno, lineBuf);
//			bufsize = strlen(lineBuf);
//			linepos = 0;
//			return lineBuf[linepos++];
//		}
//		else
//		{
//			EOF_flag = TRUE;
//			return EOF;
//		}
//	}
//	else return lineBuf[linepos++];
//}
//
///* ungetNextChar backtracks one character
//in lineBuf */
//static void ungetNextChar(void)
//{
//	if (!EOF_flag) linepos--;
//}
//
///* lookup table of reserved words */
//static struct
//{
//	char* str;
//	TypeToken tok;
//} reservedWords[MAXRESERVED]
//= { { "if",IF },{ "then",THEN },{ "else",ELSE },{ "end",END },
//{ "repeat",REPEAT },{ "until",UNTIL },{ "read",READ },
//{ "write",WRITE } };
//
///* lookup an identifier to see if it is a reserved word */
///* uses linear search */
//static TypeToken reservedLookup(char * s)
//{
//	int i;
//	for (i = 0; i<MAXRESERVED; i++)
//		if (!strcmp(s, reservedWords[i].str))
//			return reservedWords[i].tok;
//	return ID;
//}
//
///****************************************/
///* the primary function of the scanner  */
///****************************************/
///* function getToken returns the
//* next token in source file
//*/
//TypeToken getToken(void)
//{  /* index for storing into tokenString */
//	int tokenStringIndex = 0;
//	/* holds current token to be returned */
//	TypeToken currentToken;
//	/* current state - always begins at START */
//	StateType state = START;
//	/* flag to indicate save to tokenString */
//	int save;
//	while (state != DONE)
//	{
//		int c = getNextChar();
//		save = TRUE;
//		switch (state)
//		{
//		case START:
//			if (isdigit(c))
//				state = INNUM;
//			else if (isalpha(c))
//				state = INID;
//			else if (c == ':')
//				state = INASSIGN;
//			else if ((c == ' ') || (c == '\t') || (c == '\n'))
//				save = FALSE;
//			else if (c == '{')
//			{
//				save = FALSE;
//				state = INCOMMENT;
//			}
//			else
//			{
//				state = DONE;
//				switch (c)
//				{
//				case EOF:
//					save = FALSE;
//					currentToken = ENDFILE;
//					break;
//				case '=':
//					currentToken = EQ;
//					break;
//				case '<':
//					currentToken = LT;
//					break;
//				case '+':
//					currentToken = PLUS;
//					break;
//				case '-':
//					currentToken = MINUS;
//					break;
//				case '*':
//					currentToken = TIMES;
//					break;
//				case '/':
//					currentToken = OVER;
//					break;
//				case '(':
//					currentToken = LPAREN;
//					break;
//				case ')':
//					currentToken = RPAREN;
//					break;
//				case ';':
//					currentToken = SEMI;
//					break;
//				default:
//					currentToken = ERRO;
//					break;
//				}
//			}
//			break;
//		case INCOMMENT:
//			save = FALSE;
//			if (c == EOF)
//			{
//				state = DONE;
//				currentToken = ENDFILE;
//			}
//			else if (c == '}') state = START;
//			break;
//		case INASSIGN:
//			state = DONE;
//			if (c == '=')
//				currentToken = ASSIGN;
//			else
//			{ /* backup in the input */
//				ungetNextChar();
//				save = FALSE;
//				currentToken = ERRO;
//			}
//			break;
//		case INNUM:
//			if (!isdigit(c))
//			{ /* backup in the input */
//				ungetNextChar();
//				save = FALSE;
//				state = DONE;
//				currentToken = NUM;
//			}
//			break;
//		case INID:
//			if (!isalpha(c))
//			{ /* backup in the input */
//				ungetNextChar();
//				save = FALSE;
//				state = DONE;
//				currentToken = ID;
//			}
//			break;
//		case DONE:
//		default: /* should never happen */
//			fprintf(listing, "Scanner Bug: state= %d\n", state);
//			state = DONE;
//			currentToken = ERRO;
//			break;
//		}
//		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
//			tokenString[tokenStringIndex++] = (char)c;
//		if (state == DONE)
//		{
//			tokenString[tokenStringIndex] = '\0';
//			if (currentToken == ID)
//				currentToken = reservedLookup(tokenString);
//		}
//	}
//	if (TraceScan) {
//		fprintf(listing, "\t%d: ", lineno);
//		printToken(currentToken, tokenString);
//	}
//	return currentToken;
//} /* end getToken */
//
//
///*UTIL.C*/
//
///* Procedure printToken prints a token
//* and its lexeme to the listing file
//*/
//void printToken(TypeToken token, const char* tokenString)
//{
//	switch (token)
//	{
//	case IF:
//	case THEN:
//	case ELSE:
//	case END:
//	case REPEAT:
//	case UNTIL:
//	case READ:
//	case WRITE:
//		fprintf(listing,
//			"reserved word: %s\n", tokenString);
//		break;
//	case ASSIGN: fprintf(listing, ":=\n"); break;
//	case LT: fprintf(listing, "<\n"); break;
//	case EQ: fprintf(listing, "=\n"); break;
//	case LPAREN: fprintf(listing, "(\n"); break;
//	case RPAREN: fprintf(listing, ")\n"); break;
//	case SEMI: fprintf(listing, ";\n"); break;
//	case PLUS: fprintf(listing, "+\n"); break;
//	case MINUS: fprintf(listing, "-\n"); break;
//	case TIMES: fprintf(listing, "*\n"); break;
//	case OVER: fprintf(listing, "/\n"); break;
//	case ENDFILE: fprintf(listing, "EOF\n"); break;
//	case NUM:
//		fprintf(listing,
//			"NUM, val= %s\n", tokenString);
//		break;
//	case ID:
//		fprintf(listing,
//			"ID, name= %s\n", tokenString);
//		break;
//	case ERRO:
//		fprintf(listing,
//			"ERROR: %s\n", tokenString);
//		break;
//	default: /* should never happen */
//		fprintf(listing, "Unknown token: %d\n", token);
//	}
//}
//
///* Function newStmtNode creates a new statement
//* node for syntax tree construction
//*/
//TreeNode * newStmtNode(StmtKind kind)
//{
//	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
//	int i;
//	if (t == NULL)
//		fprintf(listing, "Out of memory error at line %d\n", lineno);
//	else {
//		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
//		t->sibling = NULL;
//		t->nodekind = StmtK;
//		t->kind.stmt = kind;
//		t->lineno = lineno;
//	}
//	return t;
//}
//
///* Function newExpNode creates a new expression
//* node for syntax tree construction
//*/
//TreeNode * newExpNode(ExpKind kind)
//{
//	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
//	int i;
//	if (t == NULL)
//		fprintf(listing, "Out of memory error at line %d\n", lineno);
//	else {
//		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
//		t->sibling = NULL;
//		t->nodekind = ExpK;
//		t->kind.exp = kind;
//		t->lineno = lineno;
//		t->type = Void;
//	}
//	return t;
//}
//
///* Function copyString allocates and makes a new
//* copy of an existing string
//*/
//char * copyString(char * s)
//{
//	int n;
//	char * t;
//	if (s == NULL) return NULL;
//	n = strlen(s) + 1;
//	t = (char*)malloc(n);
//	if (t == NULL)
//		fprintf(listing, "Out of memory error at line %d\n", lineno);
//	else strcpy_s(t,sizeof(t), s);
//	return t;
//}
//
///* Variable indentno is used by printTree to
//* store current number of spaces to indent
//*/
//static int indentno = 0;
//
///* macros to increase/decrease indentation */
//#define INDENT indentno+=2
//#define UNINDENT indentno-=2
//
///* printSpaces indents by printing spaces */
//static void printSpaces(void)
//{
//	int i;
//	for (i = 0; i<indentno; i++)
//		fprintf(listing, " ");
//}
//
///* procedure printTree prints a syntax tree to the
//* listing file using indentation to indicate subtrees
//*/
//void printTree(TreeNode * tree)
//{
//	int i;
//	INDENT;
//	while (tree != NULL) {
//		printSpaces();
//		if (tree->nodekind == StmtK)
//		{
//			switch (tree->kind.stmt) {
//			case IfK:
//				fprintf(listing, "If\n");
//				break;
//			case RepeatK:
//				fprintf(listing, "Repeat\n");
//				break;
//			case AssignK:
//				fprintf(listing, "Assign to: %s\n", tree->attr.name);
//				break;
//			case ReadK:
//				fprintf(listing, "Read: %s\n", tree->attr.name);
//				break;
//			case WriteK:
//				fprintf(listing, "Write\n");
//				break;
//			default:
//				fprintf(listing, "Unknown ExpNode kind\n");
//				break;
//			}
//		}
//		else if (tree->nodekind == ExpK)
//		{
//			switch (tree->kind.exp) {
//			case OpK:
//				fprintf(listing, "Op: ");
//				printToken(tree->attr.op, "\0");
//				break;
//			case ConstK:
//				fprintf(listing, "Const: %d\n", tree->attr.val);
//				break;
//			case IdK:
//				fprintf(listing, "Id: %s\n", tree->attr.name);
//				break;
//			default:
//				fprintf(listing, "Unknown ExpNode kind\n");
//				break;
//			}
//		}
//		else fprintf(listing, "Unknown node kind\n");
//		for (i = 0; i<MAXCHILDREN; i++)
//			printTree(tree->child[i]);
//		tree = tree->sibling;
//	}
//	UNINDENT;
//}

