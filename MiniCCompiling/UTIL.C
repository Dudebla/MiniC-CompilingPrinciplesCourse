/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "GLOBALS.H"
#include "UTIL.H"
#include<string>
#include<sstream>
using namespace std;
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

