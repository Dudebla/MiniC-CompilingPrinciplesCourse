
// MFCApplication1Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include<iostream>
#include<fstream>
#include<string>
#include <sstream>

/* BUFLEN = length of the input buffer for
source code lines */
#define BUFLEN 256


/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
in hash function  */
#define SHIFT 4

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN + 1];

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;



using namespace std;

TreeNode * parse(void);

TreeNode * syntaxTree;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editControl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE3, &CMFCApplication1Dlg::OnEnChangeMfceditbrowse3)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication1Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CEdit* m_Edit = (CEdit*)GetDlgItem(IDC_MFCEDITBROWSE3);
	m_Edit->SetWindowText(_T(""));



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CString m_temp = "";
	errno_t err;  //判断此文件流是否存在 存在返回1
	CString selectedPath;
	//存储打印的语法树
	string result;

	GetDlgItemText(IDC_MFCEDITBROWSE3, selectedPath);

	if (selectedPath == "") {
		MessageBox("选择的文件为空！");
	}
	else {
		//打开文件
		err = fopen_s(&source, selectedPath, "r");

		if (syntaxTree != NULL) {
			syntaxTree = NULL;
		}

		//初始化全局数据
		lineno = 0;
		memset(lineBuf, '\0', sizeof(lineBuf));
		linepos = 0; 
		bufsize = 0;  
		EOF_flag = FALSE;

		//生成语法树
		syntaxTree = parse();
		//打印语法树
		result = printTree(syntaxTree);
		fclose(source);

		//输出到显示框
		CEdit* m_Edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
		m_Edit2->SetWindowText(result.c_str());

	}
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	CString selectedPath;
	CString content;

	GetDlgItemText(IDC_MFCEDITBROWSE3, selectedPath);
	
	if (selectedPath != "") {
		CEdit* m_Edit = (CEdit*)GetDlgItem(IDC_EDIT1);
		m_Edit->GetWindowText(content);

		content.Replace('\n', ' ');

		//处理保存的文件名
		string tempPath = selectedPath.GetBuffer(0);
		int position = tempPath.find_last_of(".");
		tempPath = tempPath.substr(0, position);
		tempPath += ".TNY";

		selectedPath = tempPath.c_str();

		ofstream outfile(selectedPath);

		if (!outfile.is_open())
		{
			MessageBox("文件打开错误，保存失败!" );
			outfile.close();
			return;
		}
		outfile << content;
		outfile.close();
		MessageBox("创建并写入成功!" );
	}
	else {
		MessageBox("路径为空！请选择或新建源程序！");
	}

}




void CMFCApplication1Dlg::OnEnChangeMfceditbrowse3()
{
	CString selectedPath;
	CString content;

	GetDlgItemText(IDC_MFCEDITBROWSE3, selectedPath);

	if (selectedPath != "") {
		selectedPath.Replace('\\','/');
		errno_t err;  //判断此文件流是否存在 存在返回1
		err = fopen_s(&source,selectedPath ,"r");
		char nextline[256];
		CString temp_str = "";
		CString m_temp = "";
		while (fgets(nextline, sizeof(nextline), source))
		{
			temp_str = nextline;
			m_temp += temp_str;
			m_temp += "\r\n";
		}
		CEdit* m_Edit = (CEdit*)GetDlgItem(IDC_EDIT1);
		m_Edit->SetWindowText(m_temp);
		fclose(source);
	}
}


void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString selectedPath;
	GetDlgItemText(IDC_MFCEDITBROWSE3, selectedPath);

	if (selectedPath != "") {
		selectedPath.Replace('\\', '/');

		string tempPath = selectedPath.GetBuffer(0);
		int position = tempPath.find_last_of(".");
		tempPath = tempPath.substr(0, position);
		tempPath += ".tm";

		//打开文件，写方式，若不存在则创建
		int err1,err2;
		const char* codefile = tempPath.c_str();
		err1 = fopen_s(&code, codefile, "w");
		err2 = fopen_s(&source, selectedPath, "r");


		if (code == NULL)
		{
			//printf("Unable to open %s\n", codefile);
			MessageBox("无法打开文件!");
			exit(1);
		}

		//char* codefileTemp = new char[(sizeof(codefile)+4)*sizeof(char)];
		char *codefileTemp = (char*)tempPath.c_str();

		//初始化全局数据
		lineno = 0;
		memset(lineBuf, '\0', sizeof(lineBuf));
		linepos = 0;
		bufsize = 0;
		EOF_flag = FALSE;

		//生成语法树
		syntaxTree = parse();
		//解析语法树成为.tm文件
		codeGen(syntaxTree, codefileTemp);

		MessageBox("生成.tm文件成功！");

		fclose(source);
		fclose(code);
	}	
}



/*PARSE.C*/

static TypeToken token; /* holds current token */

						/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * exp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);

/*TINY的文法扩充*/
static TreeNode * while_stmt(void);
static TreeNode * dowhile_stmt(void);
static TreeNode * for_stmt(void);


//static void syntaxError(char * message)
static char* syntaxError(char * message)
{
	string result;
	//fprintf(listing, "\n>>> ");
	//fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	string temp = message;
	result += "\r\n>>>Syntax error at line " + lineno + temp;
	Error = TRUE;

	char* r = new char[result.size() + 1];
	strcpy_s(r, result.size() + 1, result.c_str());
	return r;
}

static void match(TypeToken expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "      ");
	}
}

TreeNode * stmt_sequence(void)
{
	TreeNode * t = statement();
	TreeNode * p = t;
	while ((token != ENDFILE) && (token != END) && 
		(token != ELSE) && (token != UNTIL) && (token != WHILE)&& (token != ENDDO) && (token != ENDWHILE))
	{
		TreeNode * q;
		if (token == SEMI) {
			match(SEMI);
		}
		q = statement();
		if (q != NULL) {
			if (t == NULL) t = p = q;
			else /* now p cannot be NULL either */
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}


//P394 
//lineno: 961
TreeNode * statement(void)
{
	TreeNode * t = NULL;
	switch (token) {
	case IF: t = if_stmt(); break;
	case REPEAT: t = repeat_stmt(); break;
	case ID: t = assign_stmt(); break;
	case READ: t = read_stmt(); break;
	case WRITE: t = write_stmt(); break;
	case WHILE: t = while_stmt(); break;
	case DO: t = dowhile_stmt(); break;
	case FOR: t = for_stmt(); break;
	case ENDDO: break;
	case ENDWHILE: break;
	default: syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	} /* end case */
	return t;
}

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

//更改后的if
TreeNode * if_stmt(void)
{
	TreeNode * t = newStmtNode(IfK);
	match(IF);
	if (t != NULL) t->child[0] = exp();
	if (t != NULL) t->child[1] = stmt_sequence();
	if (token == ELSE) {
		match(ELSE);
		if (t != NULL) t->child[2] = stmt_sequence();
	}
	return t;
}


//P394 
//lineno:991
TreeNode * repeat_stmt(void)
{
	TreeNode * t = newStmtNode(RepeatK);
	match(REPEAT);
	if (t != NULL) t->child[0] = stmt_sequence();
	match(UNTIL);
	if (t != NULL) t->child[1] = exp();
	return t;
}

TreeNode * assign_stmt(void)
{
	TreeNode * t = newStmtNode(AssignK);
	if ((t != NULL) && (token == ID))
		t->attr.name = copyString(tokenString);
	match(ID);
	if (token == ASSIGN) {
		match(ASSIGN);
	}
	else if( token == ADDITIVE){  //加法赋值号的文法扩充
		match(ADDITIVE);
		t->kind.stmt = AdditiveK;
	}
	if (t != NULL) t->child[0] = exp();
	return t;
}

TreeNode * read_stmt(void)
{
	TreeNode * t = newStmtNode(ReadK);
	match(READ);
	if ((t != NULL) && (token == ID))
		t->attr.name = copyString(tokenString);
	match(ID);
	return t;
}

TreeNode * write_stmt(void)
{
	TreeNode * t = newStmtNode(WriteK);
	match(WRITE);
	if (t != NULL) t->child[0] = exp();
	return t;
}

TreeNode * exp(void)
{
	TreeNode * t = simple_exp();
	if ((token == LT) || (token == EQ)) {
		TreeNode * p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t != NULL)
			t->child[1] = simple_exp();
	}
	return t;
}

TreeNode * simple_exp(void)
{
	TreeNode * t = term();
	while ((token == PLUS) || (token == MINUS))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
			t->child[1] = term();
		}
	}
	return t;
}

TreeNode * term(void)
{
	TreeNode * t = factor();
	while ((token == TIMES) || (token == OVER) || (token == POWER))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
			p->child[1] = factor();
		}
	}
	return t;
}

TreeNode * factor(void)
{
	TreeNode * t = NULL;
	switch (token) {
	case NUM:
		t = newExpNode(ConstK);
		if ((t != NULL) && (token == NUM))
			t->attr.val = atoi(tokenString);
		match(NUM);
		break;
	case ID:
		t = newExpNode(IdK);
		if ((t != NULL) && (token == ID))
			t->attr.name = copyString(tokenString);
		match(ID);
		break;
	case LPAREN:
		match(LPAREN);
		t = exp();
		match(RPAREN);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}


/*TINY的文法扩充*/
static TreeNode * while_stmt(void) {
	TreeNode * t = newStmtNode(WhileK);
	match(WHILE);
	if (t != NULL) {
		t->child[0] = exp();
	}
	match(DO);
	if (t != NULL) {
		t->child[1] = stmt_sequence();
	}
	match(ENDWHILE);
	return t;
}

static TreeNode * dowhile_stmt(void) {
	TreeNode * t = newStmtNode(DowhileK);
	match(DO);
	if (t != NULL) {
		t->child[0] = stmt_sequence();
	}
	match(WHILE);
	if (t != NULL) {
		t->child[1] = exp();
	}
	return t;
}


static TreeNode * for_stmt(void) {
	TreeNode * t = newStmtNode(ForIncK);
	match(FOR);
	if (t != NULL) {
		t->child[0] = assign_stmt();
		if (token == TO) {
			match(TO);
		}
		else if (token == DOWNTO) {
			match(DOWNTO);
			t->kind.stmt = ForDecK;
		}
		t->child[1] = simple_exp();
		match(DO);
		t->child[2] = stmt_sequence();
		match(ENDDO);
	}
	return t;
}



/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
* constructed syntax tree
*/
TreeNode * parse(void)
{
	TreeNode * t;
	token = getToken();
	t = stmt_sequence();
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}



/*SCAN.C*/
/* states in scanner DFA */
typedef enum
{
	START, INASSIGN, INCOMMENT, INNUM, INID, DONE , INPOWER
}
StateType;



/* getNextChar fetches the next non-blank character
from lineBuf, reading in a new line if lineBuf is
exhausted */
static int getNextChar(void)
{
	if (!(linepos < bufsize))
	{
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			if (EchoSource) fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else
		{
			EOF_flag = TRUE;
			return EOF;
		}
	}
	else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
in lineBuf */
static void ungetNextChar(void)
{
	if (!EOF_flag) linepos--;
}

/* lookup table of reserved words */
static struct
{
	char* str;
	TypeToken tok;
} reservedWords[MAXRESERVED]
= { { "if",IF },{ "then",THEN },{ "else",ELSE },{ "end",END },
{ "repeat",REPEAT },{ "until",UNTIL },{ "read",READ },
{ "write",WRITE },{ "do" , DO },{"while",WHILE} , {"for",FOR},{"downto",DOWNTO},{"enddo",ENDDO},{"endwhile",ENDWHILE} };

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TypeToken reservedLookup(char * s)
{
	int i;
	for (i = 0; i<MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
* next token in source file
*/
TypeToken getToken(void)
{  /* index for storing into tokenString */
	int tokenStringIndex = 0;
	/* holds current token to be returned */
	TypeToken currentToken;
	/* current state - always begins at START */
	StateType state = START;
	/* flag to indicate save to tokenString */
	int save;
	while (state != DONE)
	{
		int c = getNextChar();
		save = TRUE;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			else if (c == ':')
				state = INASSIGN;
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;
			else if (c == '{')
			{
				save = FALSE;
				state = INCOMMENT;
			}
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '=':
					currentToken = EQ;
					break;
				case '<':
					currentToken = LT;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '^':
					currentToken = POWER;
					break;
				case '/':
					currentToken = OVER;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case ';':
					currentToken = SEMI;
					break;
				default:
					currentToken = ERRO;
					break;
				}
			}
			break;
		case INCOMMENT:
			save = FALSE;
			if (c == EOF)
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			else if (c == '}') state = START;
			break;
		case INASSIGN:
			state = DONE;
			if (c == '=')
				currentToken = ASSIGN;
			else
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				currentToken = ERRO;
			}
			break;
		case INNUM:
			if (!isdigit(c))
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c))
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
		default: /* should never happen */
			fprintf(listing, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERRO;
			break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = (char)c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
				currentToken = reservedLookup(tokenString);
		}
	}
	if (TraceScan) {
		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
} /* end getToken */


  /*UTIL.C*/

  /* Procedure printToken prints a token
  * and its lexeme to the listing file
  */
char* printToken(TypeToken token, const char* tokenString)
{
	string result;
	string temp = tokenString;
	switch (token)
	{
	case IF:
	case THEN:
	case ELSE:
	case END:
	case REPEAT:
	case UNTIL:
	case READ:
	case WRITE:
	case DO:
	case WHILE:
	case ENDWHILE:
	case FOR:
	case TO:
	case DOWNTO:
	case ENDDO:

		//fprintf(listing,"reserved word: %s\n", tokenString);
		/*strcat_s(result,15, "reserved word: ");
		strcat_s(result,sizeof(tokenString), tokenString);
		strcat_s(result,4, "\r\n");*/
		result += "reserved word: " + temp + "\r\n";
		break;
	case ASSIGN:
		//fprintf(listing, ":=\n");
		//strcat_s(result, 6,":=\r\n");
		result += ":=\r\n";
		break;
	case LT:
		//fprintf(listing, "<\n"); 
		//strcat_s(result,5, "<\r\n"); 
		result += "<\r\n";
		break;
	case EQ:
		//fprintf(listing, "=\n");
		//strcat_s(result,5, "=\r\n"); 
		result += "=\r\n";
		break;
	case LPAREN:
		//fprintf(listing, "(\n");
		//strcat_s(result,5, "(\r\n"); 
		result += "(\r\n";
		break;
	case RPAREN:
		//fprintf(listing, ")\n");
		//strcat_s(result, 5,")\r\n"); 
		result += ")\r\n";
		break;
	case SEMI:
		//fprintf(listing, ";\n");
		//strcat_s(result,5, ";\r\n");
		result += ";\r\n";
		break;
	case PLUS:
		//fprintf(listing, "+\n");
		//strcat_s(result,5, "+\r\n");
		result += "+\r\n";
		break;
	case MINUS:
		//fprintf(listing, "-\n");
		//strcat_s(result,5, "-\r\n");
		result += "-\r\n";
		break;
	case TIMES:
		//fprintf(listing, "*\n"); 
		//strcat_s(result,5, "*\r\n");
		result += "*\r\n";
		break;
	case POWER:
		result += "^\r\n";
		break;
	case OVER:
		//fprintf(listing, "/\n");
		//strcat_s(result,5, "/\r\n");
		result += "/\r\n";
		break;
	case ENDFILE:
		//fprintf(listing, "EOF\n");
		//strcat_s(result, 7,"EOF\r\n");
		result += "EOF\r\n";
		break;
	case NUM:
		//fprintf(listing,"NUM, val= %s\n", tokenString);
		/*strcat_s(result, 14,"NUM, val= \r\n");
		strcat_s(result, sizeof(tokenString), tokenString);
		strcat_s(result, 4,"\r\n");*/
		result += "NUM, val= " + temp + "\r\n";
		break;
	case ID:
		//fprintf(listing,"ID, name= %s\n", tokenString);
		/*strcat_s(result, sizeof("ID, name= \r\n"), "ID, name= \r\n");
		strcat_s(result,sizeof(tokenString), tokenString);
		strcat_s(result,4, "\r\n");*/
		result += "ID, name= " + temp + "\r\n";
		break;
	case ERRO:
		//fprintf(listing,"ERROR: %s\n", tokenString);
		/*strcat_s(result,7, "ERROR: ");
		strcat_s(result,sizeof(tokenString), tokenString);
		strcat_s(result, 4,"\r\n");*/
		result += "ERROR: " + temp + "\r\n";
		break;
	case ADDITIVE:
		result += "+=\r\n";
		break;
	default: /* should never happen */
		//fprintf(listing, "Unknown token: %d\n", token);
		//strcat_s(result, sizeof("Unknown token: "), "Unknown token: ");
		result += "Unknown token: \r\n";
		//strcat(result, token);
		//strcat_s(result,4, "\r\n");
	}
	char*r = new char[result.size() + 1];
	strcpy_s(r, result.size() + 1, result.c_str());
	return r;
}

/* Function newStmtNode creates a new statement
* node for syntax tree construction
*/
TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function newExpNode creates a new expression
* node for syntax tree construction
*/
TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

/* Function copyString allocates and makes a new
* copy of an existing string
*/
char* copyString(char * s)
{
	int n;
	char * t ;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = new char[n];
	//t = (char*)malloc(n);
	//if (t == NULL) {
	//	//fprintf(listing, "Out of memory error at line %d\n", lineno);
	// 	MessageBox(NULL, TEXT("Out of memory error at line "), TEXT("骗人了吧!"), MB_OK);
	//}
	//else
	//	strcpy_s(t, sizeof(s), s);
	strcpy_s(t, n, s);

	return t;
}

/* Variable indentno is used by printTree to
* store current number of spaces to indent
*/
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static char* printSpaces(void)
{
	string result ;
	int i;
	for (i = 0; i < indentno; i++) {
		//fprintf(listing, " ");
		result += " ";
	}
	
	char* r = new char[result.size()+1];
	strcpy_s(r, result.size() + 1, result.c_str());
	return r;
}

/* procedure printTree prints a syntax tree to the
* listing file using indentation to indicate subtrees
*/
char* printTree(TreeNode * tree)
{
	string result = "";
	char* temp;
	int i;
	INDENT;
	while (tree != NULL) {
		temp = printSpaces();
		string name;
		string val;
		//strcat_s(result, sizeof(temp), temp);
		result += temp;
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt) {
			case IfK:
				//fprintf(listing, "If\n");
				//strcat_s(result, 6,"If\r\n");
				result += "If\r\n";
				break;
			case RepeatK:
				//fprintf(listing, "Repeat\n");
				//strcat_s(result,10, "Repeat\r\n");
				result += "Repeat\r\n";
				break;
			case AssignK:
				//fprintf(listing, "Assign to: %s\n", tree->attr.name);
				/*strcat_s(result,sizeof("Assign to: "), "Assign to: ");
				strcat_s(result,sizeof(tree->attr.name), tree->attr.name);
				strcat_s(result,4, "\r\n");*/
				name = tree->attr.name;
				result += "Assign to: " + name + "\r\n";
				break;
			case ReadK:
				//fprintf(listing, "Read: %s\n", tree->attr.name);
				/*strcat_s(result,6, "Read: ");
				strcat_s(result, sizeof(tree->attr.name),tree->attr.name);
				strcat_s(result,4, "\r\n");*/
				name = tree->attr.name;
				result += "Read: " + name + "\r\n";
				break;
			case WriteK:
				//fprintf(listing, "Write\n");
				//strcat_s(result,sizeof("Write\r\n"), "Write\r\n");
				result += "Write\r\n";
				break;
			case ForDecK:
				result += "for\r\n"  ;
				break;
			case ForIncK:
				result += "for\r\n";
				break;
			case DowhileK:
				result += "do\r\n";
				break;
			case WhileK:
				result += "while\r\n";
				break;
			case AdditiveK:
				name = tree->attr.name;
				result += "Add and assign to: " + name + "\r\n";
			default:
				//fprintf(listing, "Unknown ExpNode kind\n");
				//strcat_s(result, sizeof("Unknown ExpNode kind\r\n"),"Unknown ExpNode kind\r\n");
				result += "Unknown ExpNode kind\r\n";
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{

			stringstream ss;
			ss << tree->attr.val;
			string temp;
			string val;
			string name;


			switch (tree->kind.exp) {
			case OpK:
				//fprintf(listing, "Op: ");
				//strcat_s(result,4, "Op: ");
				result += "Op: ";
				temp = printToken(tree->attr.op, "\0");
				//strcat_s(result, sizeof(temp),temp);
				result += temp;
				break;
			case ConstK:
				//fprintf(listing, "Const: %d\n", tree->attr.val);
				/*strcat_s(result,sizeof("Const: "), "Const: ");
				strcat_s(result, sizeof(ss.str().c_str()),ss.str().c_str());
				strcat_s(result, 4,"\r\n");*/
				val = to_string(tree->attr.val);
				result += "Const: " + val + "\r\n";
				break;
			case IdK:
				//fprintf(listing, "Id: %s\n", tree->attr.name);
				/*strcat_s(result,4, "Id: ");
				strcat_s(result, sizeof(tree->attr.name),tree->attr.name);
				strcat_s(result,4, "\r\n");*/
				name = tree->attr.name;
				result += "Id: " + name + "\r\n";
				break;
			default:
				//fprintf(listing, "Unknown ExpNode kind\n");
				//strcat_s(result,sizeof("Unknown ExpNode kind\r\n"), "Unknown ExpNode kind\r\n");
				result += "Unknown ExpNode kind\r\n";
				break;
			}
		}
		else {
			//fprintf(listing, "Unknown node kind\n");
			//strcat_s(result, sizeof("Unknown node kindr\n"),"Unknown node kindr\n");
			result += "Unknown node kindr\r\n";
		}
		for (i = 0; i < MAXCHILDREN; i++) {
			string temp;
			temp = printTree(tree->child[i]);
			//strcat_s(result, sizeof(temp),temp);
			result += temp;
		}
		tree = tree->sibling;
	}
	UNINDENT;
	char * r = new char[result.size() + 1];
	strcpy_s(r, result.size() + 1, result.c_str());
	return r;
}


/*CGEN.C*/

/* tmpOffset is the memory offset for temps
It is decremented each time a temp is
stored, and incremeted when loaded again
*/
static int tmpOffset = 0;

/* prototype for internal recursive code generator */
static void cGen(TreeNode * tree);

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode * tree)
{
	TreeNode * p1, *p2, *p3;
	int savedLoc1, savedLoc2, currentLoc;
	int loc;
	switch (tree->kind.stmt) {

	case IfK:
		if (TraceCode) emitComment("-> if");
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		/* generate code for test expression */
		cGen(p1);
		savedLoc1 = emitSkip(1);
		emitComment("if: jump to else belongs here");
		/* recurse on then part */
		cGen(p2);
		savedLoc2 = emitSkip(1);
		emitComment("if: jump to end belongs here");
		currentLoc = emitSkip(0);
		emitBackup(savedLoc1);
		emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
		emitRestore();
		/* recurse on else part */
		cGen(p3);
		currentLoc = emitSkip(0);
		emitBackup(savedLoc2);
		emitRM_Abs("LDA", pc, currentLoc, "jmp to end");
		emitRestore();
		if (TraceCode)  emitComment("<- if");
		break; /* if_k */

	case RepeatK:
		if (TraceCode) emitComment("-> repeat");
		p1 = tree->child[0];
		p2 = tree->child[1];
		savedLoc1 = emitSkip(0);
		emitComment("repeat: jump after body comes back here");
		/* generate code for body */
		cGen(p1);
		/* generate code for test */
		cGen(p2);
		emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
		if (TraceCode)  emitComment("<- repeat");
		break; /* repeat */

	case AssignK:
		if (TraceCode) emitComment("-> assign");
		/* generate code for rhs */
		cGen(tree->child[0]);
		/* now store value */
		loc = st_lookup(tree->attr.name);
		emitRM("ST", ac, loc, gp, "assign: store value");
		if (TraceCode)  emitComment("<- assign");
		break; /* assign_k */
		/*具体的不知道写啥*/
	case AdditiveK:
		if (TraceCode)
			emitComment("-> add and assign");
		cGen(tree->child[0]);
		loc = st_lookup(tree->attr.name);
		emitRM("ST", ac, loc, gp, "add and assign: store value");
		if (TraceCode)  emitComment("<- add and assign");
		break; 

	case ReadK:
		emitRO("IN", ac, 0, 0, "read integer value");
		loc = st_lookup(tree->attr.name);
		emitRM("ST", ac, loc, gp, "read: store value");
		break;
	case WriteK:
		/* generate code for expression to write */
		cGen(tree->child[0]);
		/* now output it */
		emitRO("OUT", ac, 0, 0, "write ac");
		break;
	default:
		break;
	}
} /* genStmt */

  /* Procedure genExp generates code at an expression node */
static void genExp(TreeNode * tree)
{
	int loc;
	TreeNode * p1, *p2;
	switch (tree->kind.exp) {

	case ConstK:
		if (TraceCode) emitComment("-> Const");
		/* gen code to load integer constant using LDC */
		emitRM("LDC", ac, tree->attr.val, 0, "load const");
		if (TraceCode)  emitComment("<- Const");
		break; /* ConstK */

	case IdK:
		if (TraceCode) emitComment("-> Id");
		loc = st_lookup(tree->attr.name);
		emitRM("LD", ac, loc, gp, "load id value");
		if (TraceCode)  emitComment("<- Id");
		break; /* IdK */

	case OpK:
		if (TraceCode) emitComment("-> Op");
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* gen code for ac = left arg */
		cGen(p1);
		/* gen code to push left operand */
		emitRM("ST", ac, tmpOffset--, mp, "op: push left");
		/* gen code for ac = right operand */
		cGen(p2);
		/* now load left operand */
		emitRM("LD", ac1, ++tmpOffset, mp, "op: load left");
		switch (tree->attr.op) {
		case PLUS:
			emitRO("ADD", ac, ac1, ac, "op +");
			break;
		case MINUS:
			emitRO("SUB", ac, ac1, ac, "op -");
			break;
		case TIMES:
			emitRO("MUL", ac, ac1, ac, "op *");
			break;
		case POWER:
			emitRO("POWER", ac, ac1, ac, "op ^");
			break;
		case OVER:
			emitRO("DIV", ac, ac1, ac, "op /");
			break;
		case LT:
			emitRO("SUB", ac, ac1, ac, "op <");
			emitRM("JLT", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case EQ:
			emitRO("SUB", ac, ac1, ac, "op ==");
			emitRM("JEQ", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		default:
			emitComment("BUG: Unknown operator");
			break;
		} /* case op */
		if (TraceCode)  emitComment("<- Op");
		break; /* OpK */

	default:
		break;
	}
} /* genExp */

  /* Procedure cGen recursively generates code by
  * tree traversal
  */
static void cGen(TreeNode * tree)
{
	if (tree != NULL)
	{
		switch (tree->nodekind) {
		case StmtK:
			genStmt(tree);
			break;
		case ExpK:
			genExp(tree);
			break;
		default:
			break;
		}
		cGen(tree->sibling);
	}
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
* file by traversal of the syntax tree. The
* second parameter (codefile) is the file name
* of the code file, and is used to print the
* file name as a comment in the code file
*/
void codeGen(TreeNode * syntaxTree, char * codefile)
{
	char * s = (char*)malloc(strlen(codefile) + 7);
	//memset(s, 0, strlen(codefile) + 7);
	//strcpy_s(s,sizeof("File: "), "File: ");
	//strcat_s(s,sizeof(codefile), codefile);
	string temp = codefile;
	string str = "File: " + temp;
	emitComment("TINY Compilation to TM Code");
	emitComment((char*)str.c_str());
	/* generate standard prelude */
	emitComment("Standard prelude:");
	emitRM("LD", mp, 0, ac, "load maxaddress from location 0");
	emitRM("ST", ac, 0, ac, "clear location 0");
	emitComment("End of standard prelude.");
	/* generate code for TINY program */
	cGen(syntaxTree);
	/* finish */
	emitComment("End of execution.");
	emitRO("HALT", 0, 0, 0, "");
}

/*CODE.C*/

/* TM location number for current instruction emission */
static int emitLoc = 0;

/* Highest TM location emitted so far
For use in conjunction with emitSkip,
emitBackup, and emitRestore */
static int highEmitLoc = 0;

/* Procedure emitComment prints a comment line
* with comment c in the code file
*/
void emitComment(char * c)
{
	if (TraceCode)
		fprintf(code, "* %s\n", c);
}

/* Procedure emitRO emits a register-only
* TM instruction
* op = the opcode
* r = target register
* s = 1st source register
* t = 2nd source register
* c = a comment to be printed if TraceCode is TRUE
*/
void emitRO(char *op, int r, int s, int t, char *c)
{
	fprintf(code, "%3d:  %5s  %d,%d,%d ", emitLoc++, op, r, s, t);
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
} /* emitRO */

  /* Procedure emitRM emits a register-to-memory
  * TM instruction
  * op = the opcode
  * r = target register
  * d = the offset
  * s = the base register
  * c = a comment to be printed if TraceCode is TRUE
  */
void emitRM(char * op, int r, int d, int s, char *c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op, r, d, s);
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
} /* emitRM */

  /* Function emitSkip skips "howMany" code
  * locations for later backpatch. It also
  * returns the current code position
  */
int emitSkip(int howMany)
{
	int i = emitLoc;
	emitLoc += howMany;
	if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
	return i;
} /* emitSkip */

  /* Procedure emitBackup backs up to
  * loc = a previously skipped location
  */
void emitBackup(int loc)
{
	if (loc > highEmitLoc) emitComment("BUG in emitBackup");
	emitLoc = loc;
} /* emitBackup */

  /* Procedure emitRestore restores the current
  * code position to the highest previously
  * unemitted position
  */
void emitRestore(void)
{
	emitLoc = highEmitLoc;
}

/* Procedure emitRM_Abs converts an absolute reference
* to a pc-relative reference when emitting a
* register-to-memory TM instruction
* op = the opcode
* r = target register
* a = the absolute location in memory
* c = a comment to be printed if TraceCode is TRUE
*/
void emitRM_Abs(char *op, int r, int a, char * c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ",
		emitLoc, op, r, a - (emitLoc + 1), pc);
	++emitLoc;
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
} /* emitRM_Abs */


/*SYMTAB.C*/

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
in hash function  */
#define SHIFT 4

/* the hash function */
static int Hash(char * key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

/* the list of line numbers of the source
* code in which a variable is referenced
*/
typedef struct LineListRec
{
	int lineno;
	struct LineListRec * next;
} *LineList;

/* The record in the bucket lists for
* each variable, including name,
* assigned memory location, and
* the list of line numbers in which
* it appears in the source code
*/
typedef struct BucketListRec
{
	char * name;
	LineList lines;
	int memloc; /* memory location for variable */
	struct BucketListRec * next;
} *BucketList;

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
* memory locations into the symbol table
* loc = memory location is inserted only the
* first time, otherwise ignored
*/
void st_insert(char * name, int lineno, int loc)
{
	int h = Hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) /* variable not yet in table */
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->next = hashTable[h];
		hashTable[h] = l;
	}
	else /* found in table, so just add line number */
	{
		LineList t = l->lines;
		while (t->next != NULL) t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
} /* st_insert */

  /* Function st_lookup returns the memory
  * location of a variable or -1 if not found
  */
int st_lookup(char * name)
{
	int h = Hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return -1;
	else return l->memloc;
}

/* Procedure printSymTab prints a formatted
* listing of the symbol table contents
* to the listing file
*/
void printSymTab(FILE * listing)
{
	int i;
	fprintf(listing, "Variable Name  Location   Line Numbers\n");
	fprintf(listing, "-------------  --------   ------------\n");
	for (i = 0; i<SIZE; ++i)
	{
		if (hashTable[i] != NULL)
		{
			BucketList l = hashTable[i];
			while (l != NULL)
			{
				LineList t = l->lines;
				fprintf(listing, "%-14s ", l->name);
				fprintf(listing, "%-8d  ", l->memloc);
				while (t != NULL)
				{
					fprintf(listing, "%4d ", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
} /* printSymTab */
