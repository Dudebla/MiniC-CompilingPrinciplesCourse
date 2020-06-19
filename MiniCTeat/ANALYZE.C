/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

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

//static void insertIOFunc(void)
//{ TreeNode *func;
//  TreeNode *typeSpec;
//  TreeNode *param;
//  TreeNode *compStmt;


//  func = newStmtNode(FunDclK);
//  //int input(void)
//  compStmt = newStmtNode(CompndK);
//  compStmt->child[0] = NULL;      // no local var
//  compStmt->child[1] = NULL;      // no stmt
//  func->lineno = 0;
//  func->type = Integer;
//  func->attr.name = copyString("input");
//  func->child[0] = NULL; //no param
//  func->child[1] = compStmt;
//  st_insert("input", -1, addLocation(), func);

//  //void output(int)
//  string paramName = "arg";
//  func = newStmtNode(FunDclK);
//  compStmt = newStmtNode(CompndK);
//  param = newStmtNode(ParamK);
//  param->attr.name = copyString("arg");
//  param->type = Integer;
//  param->child[0] = newExpNode(IdK);
//  param->child[0]->type = Integer;
//  param->child[0]->attr.name = copyString("arg");
//  compStmt->child[0] = NULL;      // no local var
//  compStmt->child[1] = NULL;      // no stmt
//  func->lineno = 0;
//  func->type = Void;
//  func->attr.name = copyString("output");
//  func->child[0] = param;
//  func->child[1] = compStmt;
//  st_insert("output", -1, addLocation(), func);

//}

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

static char* symbolError(TreeNode * t, char * message)
{
    string result = "Symbol error at line " + to_string(t->lineno) + ": " + message + "\n";
//    fprintf(listing,"Symbol error at line %d: %s\n",t->lineno,message);
    Error = TRUE;
    return const_cast<char*>(result.c_str());
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
                errorMessage = symbolError(t,"variable should have non-void type");
                break;
            }
            if(st_lookup_top(t->attr.name)<0){
                st_insert(t->attr.name, t->lineno, addLocation(), t);
            }else{
                errorMessage = symbolError(t,"symbol already declared for current scope");
                break;
            }
            break;
        }
        case FunDclK:{
            funcName = t->attr.name;
//            if (st_lookup_top(funcName) >= 0) {
//            /* already in table, so it's an error */
//              symbolError(t,"function already declared");
//              break;
//            }
            st_insert(funcName,t->lineno,addLocation(),t);
            sc_push(sc_create(funcName));
            preserveLastScope = TRUE;
//          switch (t->child[0]->attr.type){
//          case INT:
//            t->type = Integer;
//            break;
//          case VOID:
//            default:
//            t->type = Void;
//            break;
//          }
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
                errorMessage =  symbolError(t->child[0],"void type parameter is not allowed");
            }
            if (st_lookup(t->attr.name) == -1) {
                st_insert(t->attr.name,t->lineno,addLocation(),t);
//                if (t->kind.param == NonArrParamK)
//                    t->type = Integer;
//                else
//                    symbolError(t,"symbol already declared for current scope");
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
                  symbolError(t, "undelcared symbol");
              else
              /* already in table, so ignore location,
               add line number of use only */
                  st_add_lineno(t->attr.name,t->lineno);
              break;
          case AssignK:
          case CallK:
              if (st_lookup(t->child[0]->attr.name) == -1)
              /* not yet in table, error */
                  symbolError(t, "undelcared symbol");
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
//        case IdK:{
//            const char *symbolName = t->attr.name;
//            const BucketList bucket = st_bucket(const_cast<char*>(symbolName));
//            TreeNode *symbolDecl = NULL;
//            if (bucket == NULL) break;
//            symbolDecl = bucket->treeNode;
//            if (t->type == IntList) {
//                if (symbolDecl->type != IntList && symbolDecl->kind.param != ArrParamK){
//                    errorMessage += typeError(t,"expected array symbol");
//                } else if (t->child[0]->type != Integer){
//                    errorMessage += typeError(t,"index expression should have integer type");
//                }else{
//                    t->type = Integer;
//                }
//            } else {
//                t->type = symbolDecl->type;
//            }
//            break;

//        }
        case AssignK:
            if (t->child[0]->type == IntList || t->child[0]->type == Void){
                errorMessage += typeError(t->child[0],"assignment of non-integer value");
            } else {
                t->type = t->child[0]->type;
            }
            break;
//        case CallK:{
//            const char *callingFuncName = t->attr.name;
//            const TreeNode * funcDecl = st_bucket(const_cast<char*>(callingFuncName))->treeNode;
//            TreeNode *arg;
//            TreeNode *param;
//            if (funcDecl == NULL)
//                break;
//            arg = t->child[0];
//            param = funcDecl->child[1];
//            if (funcDecl->kind.stmt != FunDclK){
//                errorMessage += typeError(t,"expected function symbol");
//                break;
//            }
//            while (arg != NULL) {
//                if (param == NULL) /* the number of arguments does not match to that of parameters */
//                    errorMessage += typeError(arg,"the number of parameters is wrong");
//                else if (arg->type == Void)
//                    errorMessage += typeError(arg,"void value cannot be passed as an argument");
//                else {  // no problem!
//                    arg = arg->sibling;
//                    param = param->sibling;
//                    continue;
//                }
//              /* any problem */
//                break;
//            }
//            if (arg == NULL && param != NULL)  /* the number of arguments does not match to
//               that of parameters */
//                errorMessage += typeError(t->child[0],"the number of parameters is wrong");
//            t->type = funcDecl->type;
//        }break;
        default:
            break;
        }
        break;
    case StmtK:
         switch (t->kind.stmt){
//         case IfK:
//         case ReadK:
//         case WriteK:
         case WhileK:{
             if (t->child[0]->type == Void)/* while test should be void function call */
                 errorMessage += typeError(t->child[0],"while test has void value");
             break;
         }
//         case VarDclK:
//         case FunDclK:
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
//         case ParamK:
//         case ArgsK:

//         case IfK:
//            if (t->child[0]->type == Integer || t->child[0]->type == IntList)
//                typeError(t->child[0],"if test is not Boolean");
//            break;
//         case WriteK:
//            if (t->child[0]->type != Integer || t->child[0]->type != IntList)
//                typeError(t->child[0],"write of non-integer value");
//            break;
//         case WhileK:
//            if (t->child[0]->type == Integer || t->child[0]->type == IntList)
//                typeError(t->child[0],"while test is not Boolean");
//            break;
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
