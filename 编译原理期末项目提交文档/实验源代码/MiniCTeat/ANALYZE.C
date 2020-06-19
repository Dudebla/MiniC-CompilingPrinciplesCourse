/**
  * ANALYZE.C
  *
  * @brief Semantic analyzer implementation for the CMINUS compiler
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/6/5
  */

#include "GLOBALS.H"
#include "SYMTAB.H"
#include "ANALYZE.H"
#include "UTIL.H"
#include<string>
using namespace std;

//MininC中间代码生成
static char * funcName;
static int preserveLastScope = FALSE;

/* counter for variable memory locations */
static int location = 0;

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

static void insertIOFunc(void)
{
    TreeNode *func;
    TreeNode *typeSpec;
    TreeNode *param;
    TreeNode *compStmt;

    func = newStmtNode(FunDclK);

    typeSpec = newStmtNode(FunDclK);
    typeSpec->attr.type = INT;
    func->type = Integer;

    compStmt = newStmtNode(CompndK);
    compStmt->child[0] = NULL;      // no local var
    compStmt->child[1] = NULL;      // no stmt

    func->lineno = 0;
    func->attr.name = "input";
    func->type = Integer;
    func->child[0] = NULL ;              //无参数
    func->child[1] = compStmt;

    st_insert("input", -1, addLocation(), func);

    func = newStmtNode(FunDclK);

    typeSpec = newStmtNode(FunDclK);
    func->type = Void;

    param = newExpNode(IdK);
    param->attr.name = "arg";
    param->type = Integer;

    compStmt = newStmtNode(CompndK);
    compStmt->child[0] = NULL;      // no local var
    compStmt->child[1] = NULL;      // no stmt

    func->lineno = 0;
    func->attr.name = "output";
    func->child[0] = param;
    func->child[1] = compStmt;

    st_insert("output", -1, addLocation(), func);
}



/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

static string symbolError(TreeNode * t, char * message)
{
    string result = "Symbol error at line " + to_string(t->lineno) + ": " + message + "\n";
//    fprintf(listing,"Symbol error at line %d: %s\n",t->lineno,message);
    Error = TRUE;
    return result;
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  {
    case StmtK:
        switch (t->kind.stmt){
        case VarDclK:{
            char * name;
            if(t->child[0]->type == Void){
                errorMessage += symbolError(t,"variable should have non-void type");
                break;
            }
            if(st_lookup_top(t->attr.name)<0){
                st_insert(t->attr.name, t->lineno, addLocation(), t);
            }else{
                errorMessage += symbolError(t,"symbol already declared for current scope");
                break;
            }
            break;
        }
        case FunDclK:{
            funcName = t->attr.name;
            st_insert(funcName,t->lineno,addLocation(),t);
            sc_push(sc_create(funcName));
            preserveLastScope = TRUE;
            break;
        }
        case CompndK:{
            if(preserveLastScope){
                preserveLastScope = FALSE;
            }else{
                Scope scope = sc_create(funcName);
                sc_push(scope);
            }
            t->attr.scope = sc_top();
            break;
        }
        case ParamK:{
            if (t->child[0]->attr.type == VOID){
                errorMessage +=  symbolError(t->child[0],"void type parameter is not allowed");
            }
            if (st_lookup(t->attr.name) == -1) {
                st_insert(t->attr.name,t->lineno,addLocation(),t);
            }
            break;
        }
        default:
            break;
        }
        break;
      case ExpK:
          switch (t->kind.exp) {
          case IdK:
              if (st_lookup(t->attr.name) == -1)
              /* not yet in table, error */
                 errorMessage += symbolError(t, "undelcared symbol");
              else
              /* already in table, so ignore location,
               add line number of use only */
                  st_add_lineno(t->attr.name,t->lineno);
              break;
          case AssignK:
          case CallK:
              if (st_lookup(t->child[0]->attr.name) == -1)
              /* not yet in table, error */
                  errorMessage += symbolError(t, "undelcared symbol");
              else
              /* already in table, so ignore location,
               add line number of use only */
                  st_add_lineno(t->child[0]->attr.name,t->lineno);
              break;
          case ArgsK:
              break;
          default:
              break;
          }
      }
}

/**
 * @brief afterInsertNode stack the last scope
 * @param t
 */
static void afterInsertNode( TreeNode * t ){
    switch (t->nodekind){
    case StmtK:
        switch (t->kind.stmt){
        case CompndK:
            sc_pop();
            break;
        default:
            break;
        }
        break;
        default:
            break;
        }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree){
    globalScope = sc_create(NULL);
    sc_push(globalScope);
    insertIOFunc();
    traverse(syntaxTree,insertNode,afterInsertNode);
    sc_pop();
    if (TraceAnalyze){
        //fprintf(listing,"\nSymbol table:\n\n");
        printSymTab(listing);
    }
}

static char * typeError(TreeNode * t, char * message){
    string result = "Type error at line " + to_string(t->lineno) + ": " + message + "\n";
//    fprintf(listing,"Symbol error at line %d: %s\n",t->lineno,message);
    Error = TRUE;
    return const_cast<char*>(result.c_str());
}

/**
 * @brief beforeCheckNode prepare for node check
 * @param t
 */
static void beforeCheckNode(TreeNode * t){
    switch (t->nodekind){
    case StmtK:
        switch (t->kind.stmt){
        case FunDclK:
            funcName = t->attr.name;
            break;
        case CompndK:
            sc_push(t->attr.scope);
            break;
        default:
            break;
        }
    default:
        break;
    }
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t){
    switch (t->nodekind){
    case ExpK:
        switch (t->kind.exp){
        case OpK:{
            if ((t->child[0]->type != Integer) || (t->child[1]->type != Integer) ||
              (t->child[0]->type != IntList) || (t->child[1]->type != IntList))
                errorMessage += typeError(t,"Op applied to non-integer");
            if ((t->attr.op == EQ) || (t->attr.op == LT) || (t->attr.op == LTEQ) || (t->attr.op == GT) || (t->attr.op == GTEQ) || (t->attr.op == NEQ))
                t->type = Boolean;
            else
                t->type = t->child[0]->type;
            break;
        }
        case ConstK:{
            t->type = Integer;
            break;
        }
        case AssignK:
            if (t->child[0]->type == IntList || t->child[0]->type == Void){
                errorMessage += typeError(t->child[0],"assignment of non-integer value");
            } else {
                t->type = t->child[0]->type;
            }
            break;
        default:
            break;
        }
        break;
    case StmtK:
         switch (t->kind.stmt){
         case WhileK:{
             if (t->child[0]->type == Void)/* while test should be void function call */
                 errorMessage += typeError(t->child[0],"while test has void value");
             break;
         }
         case CompndK:{
             sc_pop();
             break;
         }
         case ReturnK:{
             const TreeNode * funcDecl = st_bucket(funcName)->treeNode;
             const ExpType funcType = funcDecl->type;
             const TreeNode * expr = t->child[0];
             if (funcType == Void && (expr != NULL && expr->type != Void)) {
                errorMessage += typeError(t,"expected no return value");
                //ValueReturned = TRUE;
             } else if (funcType == Integer && (expr == NULL || expr->type == Void)) {
                typeError(t,"expected return value");
             }
             break;
         }
         default:
            break;
        }
        break;
    default:
        break;
  }
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree){ 
    sc_push(globalScope);
    traverse(syntaxTree,beforeCheckNode,checkNode);
    sc_pop();
}
