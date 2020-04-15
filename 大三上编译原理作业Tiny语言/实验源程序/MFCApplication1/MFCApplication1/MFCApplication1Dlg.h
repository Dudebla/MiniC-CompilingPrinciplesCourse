
// MFCApplication1Dlg.h: 头文件
//

#pragma once


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeMfceditbrowse3();
	CEdit m_editControl;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton3();
};


/*GLOBALS.H*/

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 14

typedef enum
/* book-keeping tokens */
{
	ENDFILE, ERRO,
	/* reserved words */
	IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE, WHILE,DO, ENDWHILE,FOR,TO, DOWNTO, ENDDO,
	/* multicharacter tokens */
	ID, NUM,
	/* special symbols */
	ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI,ADDITIVE,POWER
} TypeToken;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

				   /**************************************************/
				   /***********   Syntax tree for parsing ************/
				   /**************************************************/

typedef enum { StmtK, ExpK } NodeKind;
typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK , WhileK, DowhileK, ForIncK, ForDecK, AdditiveK} StmtKind;
typedef enum { OpK, ConstK, IdK } ExpKind;

/* ExpType is used for type checking */
typedef enum { Void, Integer, Boolean } ExpType;

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


/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN + 1];

/* function getToken returns the
* next token in source file
*/
TypeToken getToken(void);

/* Procedure printToken prints a token
* and its lexeme to the listing file
*/
char* printToken(TypeToken, const char*);

/* Function newStmtNode creates a new statement
* node for syntax tree construction
*/
TreeNode * newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
* node for syntax tree construction
*/
TreeNode * newExpNode(ExpKind);

/* Function copyString allocates and makes a new
* copy of an existing string
*/
char* copyString(char *);

/* procedure printTree prints a syntax tree to the
* listing file using indentation to indicate subtrees
*/
char* printTree(TreeNode *);


/*CGEN.H*/
/* Procedure codeGen generates code to a code
* file by traversal of the syntax tree. The
* second parameter (codefile) is the file name
* of the code file, and is used to print the
* file name as a comment in the code file
*/
void codeGen(TreeNode * syntaxTree, char * codefile);


/*CODE.H*/

/* pc = program counter  */
#define  pc 7

/* mp = "memory pointer" points
* to top of memory (for temp storage)
*/
#define  mp 6

/* gp = "global pointer" points
* to bottom of memory for (global)
* variable storage
*/
#define gp 5

/* accumulator */
#define  ac 0

/* 2nd accumulator */
#define  ac1 1

/* code emitting utilities */

/* Procedure emitComment prints a comment line
* with comment c in the code file
*/
void emitComment(char * c);

/* Procedure emitRO emits a register-only
* TM instruction
* op = the opcode
* r = target register
* s = 1st source register
* t = 2nd source register
* c = a comment to be printed if TraceCode is TRUE
*/
void emitRO(char *op, int r, int s, int t, char *c);

/* Procedure emitRM emits a register-to-memory
* TM instruction
* op = the opcode
* r = target register
* d = the offset
* s = the base register
* c = a comment to be printed if TraceCode is TRUE
*/
void emitRM(char * op, int r, int d, int s, char *c);

/* Function emitSkip skips "howMany" code
* locations for later backpatch. It also
* returns the current code position
*/
int emitSkip(int howMany);

/* Procedure emitBackup backs up to
* loc = a previously skipped location
*/
void emitBackup(int loc);

/* Procedure emitRestore restores the current
* code position to the highest previously
* unemitted position
*/
void emitRestore(void);

/* Procedure emitRM_Abs converts an absolute reference
* to a pc-relative reference when emitting a
* register-to-memory TM instruction
* op = the opcode
* r = target register
* a = the absolute location in memory
* c = a comment to be printed if TraceCode is TRUE
*/
void emitRM_Abs(char *op, int r, int a, char * c);



/*SYMTAB.H*/

/* Procedure st_insert inserts line numbers and
* memory locations into the symbol table
* loc = memory location is inserted only the
* first time, otherwise ignored
*/
void st_insert(char * name, int lineno, int loc);

/* Function st_lookup returns the memory
* location of a variable or -1 if not found
*/
int st_lookup(char * name);

/* Procedure printSymTab prints a formatted
* listing of the symbol table contents
* to the listing file
*/
void printSymTab(FILE * listing);
