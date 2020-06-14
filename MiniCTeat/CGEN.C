/****************************************************/
/* File: cgen.c                                     */
/* The code generator implementation                */
/* for the TINY compiler                            */
/* (generates code for the TM machine)              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "SYMTAB.H"
#include "CODE.H"
#include "CGEN.H"
#include<string>
using namespace std;

#define ofpFO 0
#define retFO -1
#define initFO -2

/* tmpOffset is the memory offset for temps
   It is decremented each time a temp is
   stored, and incremeted when loaded again
*/
static int tmpOffset = 0;
//全局变量的偏移量
static int globalOffset = 0;
static int localOffset = initFO;

/*
 * 判断当前是否在函数体内
*/
static int isInFunc = FALSE;

/* prototype for internal recursive code generator */
static void cGen (TreeNode * tree);

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode * tree)
{
    TreeNode * p1, *p2, *p3;
    int savedLoc1, savedLoc2, currentLoc;
    int loc;
    int size;  //偏移量
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
        //待修改
        case AssignK:
            if (TraceCode) emitComment("-> assign");

            p1 = tree->child[0];
            p2 = tree->child[1];

            cGen(tree->child[0]);
            /* now store value */
//            loc = st_lookup(tree->attr.name);
//            emitRM("ST", ac, loc, gp, "assign: store value");
            emitRM("ST", ac, tmpOffset--, mp, "assign: push left(address)");
            /* gen code for ac = right operand */
            cGen(p2);
            /* now load left operand */
            emitRM("LD", ac1, ++tmpOffset, mp, "assign: load left(value)");
            emitRM("ST", ac, 0, ac1, "assign: store value");

            if (TraceCode)  emitComment("<- assign");
            break; /* assign_k */
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
        case WhileK:
            if (TraceCode) emitComment("-> while");
            p1 = tree->child[0];
            p2 = tree->child[1];
            savedLoc1 = emitSkip(0);
            emitComment("while: jump after body comes back here");
            /* generate code for test */
            cGen(p1);
            /* generate code for body */
            savedLoc2 = emitSkip(1);
            emitComment("while: jump to end belongs here");
            cGen(p2);
            emitRM_Abs("LDA",pc,savedLoc1,"while: jmp back to test");

            currentLoc = emitSkip(0);
            emitBackup(savedLoc2);
            emitRM_Abs("JEQ", ac, currentLoc, "while: jmp to end");
            emitRestore();

            if (TraceCode)  emitComment("<- while");
            break; /* while */
        case VarDclK:   //变量声明
            if (TraceCode) emitComment("-> varDcl");

            if (tree->type == IntList || tree->type == VoidList)   //数组变量声明
                  size = tree->child[0]->attr.val;      //数组大小
                else
                  size = 1;                             //变量大小

                if (isInFunc == TRUE)             //在函数内
                  tmpOffset -= size;
                else                              //在函数外
                  globalOffset -= size;

            if (TraceCode)  emitComment("<- varDcl");
            break;
        case FunDclK:
            if (TraceCode) emitComment("-> funDcl");

            if (TraceCode)  emitComment("<- funDcl");
            break;
        case CompndK:
            if (TraceCode) emitComment("-> Compnd");

            if (TraceCode)  emitComment("<- Compnd");
            break;
        case ReturnK:
            if (TraceCode) emitComment("-> return");
            p1 = tree->child[0];

            cGen(p1);
            emitRM("LD",pc,retFO,mp,"return: to caller");

            if (TraceCode)  emitComment("<- return");
            break;
        case CallK:
            if (TraceCode) emitComment("-> call");

            if (TraceCode)  emitComment("<- call");
            break;
        case ParamK:
            if (TraceCode) emitComment("-> param");

            if (TraceCode)  emitComment("<- param");
            break;
        case ArgsK:
            if (TraceCode) emitComment("-> args");

            if (TraceCode)  emitComment("<- args");
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
        case LTEQ:
            emitRO("SUB",ac,ac1,ac,"op <=");
            emitRM("JLE",ac,2,pc,"br if true");
            emitRM("LDC",ac,0,ac,"false case");
            emitRM("LDA",pc,1,pc,"unconditional jmp");
            emitRM("LDC",ac,1,ac,"true case");
            break;
        case GT:
            emitRO("SUB",ac,ac1,ac,"op >");
            emitRM("JGT",ac,2,pc,"br if true");
            emitRM("LDC",ac,0,ac,"false case");
            emitRM("LDA",pc,1,pc,"unconditional jmp");
            emitRM("LDC",ac,1,ac,"true case");
            break;
        case GTEQ:
            emitRO("SUB",ac,ac1,ac,"op >=");
            emitRM("JGE",ac,2,pc,"br if true");
            emitRM("LDC",ac,0,ac,"false case");
            emitRM("LDA",pc,1,pc,"unconditional jmp");
            emitRM("LDC",ac,1,ac,"true case");
            break;
        case EQ:
            emitRO("SUB", ac, ac1, ac, "op ==");
            emitRM("JEQ", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        case NEQ:
            emitRO("SUB",ac,ac1,ac,"op !=");
            emitRM("JNE",ac,2,pc,"br if true");
            emitRM("LDC",ac,0,ac,"false case");
            emitRM("LDA",pc,1,pc,"unconditional jmp");
            emitRM("LDC",ac,1,ac,"true case");
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
void codeGen(TreeNode * syntaxTree, const char * codefile)
{
    char * s = (char*)malloc(strlen(codefile) + 7);
    string temp = codefile;
    string str = "File: " + temp;
    emitComment("MiniC Compilation to CM Code");
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
