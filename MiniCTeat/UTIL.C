/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "GLOBALS.H"
#include "UTIL.H"
#include "PARSE.H"
#include<string>
#include<sstream>
#include<QTextCodec>

using namespace std;

extern std::map<std::string, FunStruct> FunStructMap;
extern TreeNode *syntaxTree;

extern ManageMapState manageMapState;
extern std::string lastDeclaredFunName;

void initMap(void){

    /*
     ** C语言打开文件并存为FILE类型，保存到source变量中
    */
    //可打开含中文路径文件
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(Last_fileName).data();
    source=fopen(name.c_str(),"rb");
    //init the global var
    lineno = 0;
    lexicalMessage = "词法分析输出，格式：行号. 识别的ID \r\n";
    errorMessage = "";
    syntaxTree = NULL;
    if(!VarStructMap.empty()){
        VarStructMap.clear();
    }
    if(!FunStructMap.empty()){
        FunStructMap.clear();
    }
    EOF_flag = FALSE;
    FunStruct f;
    VarStruct v;
    //int input(void){...}
    f.name = "input";
    f.returnType = Integer;
    f.paramsNum = 0;
    FunStructMap[f.name] = f;

    //void output(int x){...}
    f.name = "output";
    f.returnType = Void;
    v.name = "x";
    v.type = Integer;
    f.params[v.name] = v;
    f.paramsNum =1 ;
    FunStructMap[f.name] = f;

    manageMapState = GlobalVarDcl;
    lastDeclaredFunName = "output";

    return;
}

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
   case ELSE:
   case WHILE:
   case RETURN:
   case INT:
   case VOID:
       result += "reserved word: " + temp ;
       result += "\r\n";
       break;
   case ASSIGN:
       result += ":=\r\n";
       break;
   case LT:
       result += "<\r\n";
       break;
   case EQ:
       result += "==\r\n";
       break;
   case LTEQ:
       result += "<=\r\n";
       break;
   case GTEQ:
       result += ">=\r\n";
       break;
   case LPAREN:
       result += "(\r\n";
       break;
   case RPAREN:
       result += ")\r\n";
       break;
   case LBRACKET:
       result += "[\r\n";
       break;
   case RBRACKET:
       result += "]\r\n";
       break;
   case LBRACE:
       result += "{\r\n";
       break;
   case RBRACE:
       result += "}\r\n";
       break;
   case COMMA:
       result += ",\r\n";
   case SEMI:
       result += ";\r\n";
       break;
   case PLUS:
       result += "+\r\n";
       break;
   case MINUS:
       result += "-\r\n";
       break;
   case TIMES:
       result += "*\r\n";
       break;
   case POWER:
       result += "^\r\n";
       break;
   case OVER:
       result += "/\r\n";
       break;
   case ENDFILE:
       result += "EOF\r\n";
       break;
   case NUM:
       result += "NUM, val= " + temp ;
       result += "\r\n";
       break;
   case ID:
       result += "ID, name= " + temp ;
       result += "\r\n";
       break;
   case ERRO:
       result += "ERROR: " + temp ;
       result += "\r\n";
       break;
   case ADDITIVE:
       result += "+=\r\n";
       break;
   default: /* should never happen */
       result += "Unknown token: \r\n";
   }
   char*r = new char[result.size() + 1];
   strcpy(r, result.c_str());
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
   strcpy(t, s);

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
       result += " ";
   }

   char* r = new char[result.size()+1];
   strcpy(r, result.c_str());
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
       result += temp;
       if (tree->nodekind == StmtK)
       {
           switch (tree->kind.stmt) {
           case IfK:
               result += "If\r\n";
               break;
           case AssignK:
               name = tree->child[0]->attr.name;
               result += "Assign to: " + name + "\r\n";
               break;
           case ReadK:
               name = tree->attr.name;
               result += "Read: " + name + "\r\n";
               break;
           case WriteK:
               result += "Write\r\n";
               break;
           case WhileK:
               result += "while\r\n";
               break;
           case VarDclK:
               name = tree->attr.name;
               result += "declare var: " + name + "\r\n";
               break;
           case FunDclK:
               name = tree->attr.name;
               result += "declare fun: " + name + "\r\n";
               break;
           case CompndK:
               result += "function body\r\n";
               break;
           case ReturnK:
               result += "return\r\n";
               break;
           case CallK:
               name = tree->child[0]->attr.name;
               result += "call function: " + name + "\r\n";
               break;
           default:
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
               result += "Op: ";
               temp = printToken(tree->attr.op, "\0");
               result += temp;
               break;
           case ConstK:
               val = to_string(tree->attr.val);
               result += "Const: " + val + "\r\n";
               break;
           case IdK:
               name = tree->attr.name;
               result += "Id: " + name + "\r\n";
               break;
           default:
               result += "Unknown ExpNode kind\r\n";
               break;
           }
       }
       else {
           result += "Unknown node kindr\r\n";
       }
       for (i = 0; i < MAXCHILDREN; i++) {
           string temp;
           temp = printTree(tree->child[i]);
           result += temp;
       }
       tree = tree->sibling;
   }
   UNINDENT;
   char * r = new char[result.size() + 1];
   strcpy(r, result.c_str());
   return r;
}

