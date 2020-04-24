/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "GLOBALS.H"
#include "UTIL.H"
#include "SCAN.H"
#include "PARSE.H"
#include<string>
using namespace std;

static TypeToken token; /* holds current token */

                        /* function prototypes for recursive calls */
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

/*TINY的文法扩充*/
//static TreeNode * while_stmt(void);
//static TreeNode * dowhile_stmt(void);
//static TreeNode * for_stmt(void);

typedef enum{
    VarDcl, FunDcl, VarFunDcl
}VarFunDclType;

/*miniC functions*/
static TreeNode * declaration_list(void);
static TreeNode * declaration(void);
static TreeNode * var_fun_declaration(VarFunDclType dclType);
static TreeNode * compound_stmt(void);
static TreeNode * params(void);
static TreeNode * local_declarations(void);
static TreeNode * statement_list(void);
static TreeNode * statement(void);
static TreeNode * selection_stmt(void);
static TreeNode * expression_stmt(void);
static TreeNode * var(void);
static TreeNode * var_call(void);
static TreeNode * iteration_stmt(void);
static TreeNode * return_stmt(void);
static TreeNode * expression(void);
static TreeNode * simple_expression(void);
static TreeNode * addtive_expression(void);
static TreeNode * term(void);
static TreeNode * factor(void);
static TreeNode * call(void);

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
    strcpy(r, result.c_str());
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

/*miniC parse begin */
TreeNode * declaration_list(void){
    TreeNode * t = declaration();
    TreeNode * p = t;
    while ((token != ENDFILE))
    {
        TreeNode * q;
        q = declaration();
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

TreeNode * declaration(void){
    TreeNode * t = NULL;
    switch (token) {
    case INT:
    case VOID: t = var_fun_declaration(VarFunDcl); break;
    case LBRACE: t = compound_stmt(); break;
    default:
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
        break;
    }
    return t;
}

TreeNode * var_fun_declaration(VarFunDclType dclType){
    TypeToken beforeT;
    TreeNode * t = NULL;
    beforeT = token;
    token = getToken();
    t = newStmtNode(VarDclK);
    if ((t != NULL) && (token == ID))
        t->attr.name = copyString(tokenString);
    match(ID);
    switch (dclType) {
    case VarFunDcl:{
        switch (token) {
        case SEMI:
            t->type = beforeT==INT?Integer:Void;
            match(SEMI);
            break;
        case LBRACKET:
            t->type = beforeT==INT?IntList:VoidList;
            match(LBRACE);
            t->attr.val = atoi(tokenString);//length of list
            match(NUM);
            match(RBRACKET);
            match(SEMI);
            break;
        case LPAREN:
            t->kind.stmt = FunDclK;
            t->type = beforeT==INT?Integer:Void;
            match(LPAREN);
            t->child[0] = params();
            match(RPAREN);
            break;
        default:
            syntaxError("unexpected token -> ");
            printToken(token, tokenString);
            token = getToken();
            break;
        }
        break;
    }
    case VarDcl:{
        switch (token) {
        case SEMI:
            t->type = beforeT==INT?Integer:Void;
            match(SEMI);
            break;
        case LBRACKET:
            t->type = beforeT==INT?IntList:VoidList;
            match(LBRACE);
            t->attr.val = atoi(tokenString);//length of list
            match(NUM);
            match(RBRACKET);
            match(SEMI);
            break;
        default:
            syntaxError("unexpected token -> ");
            printToken(token, tokenString);
            token = getToken();
            break;
        }
        break;
    }
    case FunDcl:{
        t->kind.stmt = FunDclK;
        t->type = beforeT==INT?Integer:Void;
        match(LPAREN);
        t->child[0] = params();
        match(RPAREN);
        break;
    }
    }
    return t;
}

TreeNode * compound_stmt(void){
    TreeNode * t = newStmtNode(CompndK);
    match(LBRACE);
    if(t!=NULL) t->child[0] = local_declarations();
    if(t!=NULL) t->child[1] = statement_list();
    match(RBRACE);
    return t;
}

TreeNode * params(void){




}

TreeNode * local_declarations(void){
    TreeNode * t = NULL;
    TreeNode * current = t;
    TreeNode * newT = NULL;
    while(token==INT || token==VOID){
        newT = var_fun_declaration(VarDcl);
        current->sibling = newT;
        current = newT;
    }
    return t;
}

treeNode * statement_list(void){
    TreeNode * t = NULL;
    TreeNode * p = t;
    while (token==SEMI || token==ID || token==LPAREN || token==NUM || token==LBRACE || token==IF || token==WHILE ||token==RETURN) {
        TreeNode * q = statement();
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



//TreeNode * stmt_sequence(void)
//{
//    TreeNode * t = statement();
//    TreeNode * p = t;
//    while ((token != ENDFILE) && (token != END) &&
//        (token != ELSE) && (token != UNTIL) && (token != WHILE)&& (token != ENDDO) && (token != ENDWHILE))
//    {
//        TreeNode * q;
//        if (token == SEMI) {
//            match(SEMI);
//        }
//        q = statement();
//        if (q != NULL) {
//            if (t == NULL) t = p = q;
//            else /* now p cannot be NULL either */
//            {
//                p->sibling = q;
//                p = q;
//            }
//        }
//    }
//    return t;
//}

//P394
//lineno: 961
TreeNode * statement(void)
{
    TreeNode * t = NULL;
    switch (token) {
    case IF: t = selection_stmt(); break;
    case ID: t = expression_stmt(); break;
    case WHILE: t = iteration_stmt(); break;
    case LBRACE: t = compound_stmt(); break;
    case RETURN: t = return_stmt(); break;
    default: syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
        break;
    } /* end case */
    return t;
}

TreeNode * selection_stmt(void){
    TreeNode * t = newStmtNode(IfK);
    match(IF);
    match(LPAREN);
    if (t != NULL) t->child[0] = expression();
    match(RPAREN);
    if (t != NULL) t->child[1] = statement();
    if (token == ELSE) {
        match(ELSE);
        if (t != NULL) t->child[2] = statement();
    }
    return t;
}

TreeNode * expression_stmt(void){
    TreeNode * t = NULL;
    if(token!=SEMI){
        t = expression();
    }
    match(SEMI);
    return t;
}

TreeNode * expression(void){
    TreeNode * t = NULL;
    if(token==ID){
        t = newStmtNode(AssignK);
        if(t!=NULL){
            t->child[0] = var();
        }
        match(ASSIGN);
        t->attr.op = ASSIGN;
        if(t!=NULL){
            t->child[1] = expression();
        }
    }else{
        t = simple_expression();
    }
    return t;
}

TreeNode * var(void){
    TreeNode * t = newExpNode(IdK);
    if(t!=NULL){
        t->attr.name = copyString(tokenString);
    }
    match(ID);
    if(t!=NULL && token==LBRACKET){
        match(LBRACKET);
        t->child[0] = expression();
        match(RBRACKET);
    }
    return t;
}

TreeNode * simple_expression(void){
    TreeNode * t = addtive_expression();
    while(token==LT || token==GT || token==EQ || token==NEQ || token==LTEQ || token ==GTEQ){
        TreeNode * p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = addtive_expression();
        }
    }
    return t;
}

TreeNode * addtive_expression(void){
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

TreeNode * iteration_stmt(void){
    TreeNode * t = newStmtNode(WhileK);
    match(WHILE);
    match(LPAREN);
    if (t != NULL) {
        t->child[0] = expression();
    }
    match(RPAREN);
    if (t != NULL) {
        t->child[1] = statement();
    }
    return t;
}

TreeNode * return_stmt(void){
    TreeNode * t = newStmtNode(ReturnK);
    match(RETURN);
    if(t!=NULL && token!=SEMI){
        t->child[0] = expression();
    }
    match(SEMI);
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
//TreeNode * if_stmt(void)
//{
//    TreeNode * t = newStmtNode(IfK);
//    match(IF);
//    if (t != NULL) t->child[0] = exp();
//    if (t != NULL) t->child[1] = stmt_sequence();
//    if (token == ELSE) {
//        match(ELSE);
//        if (t != NULL) t->child[2] = stmt_sequence();
//    }
//    return t;
//}


//P394
//lineno:991
//TreeNode * repeat_stmt(void)
//{
//    TreeNode * t = newStmtNode(RepeatK);
//    match(REPEAT);
//    if (t != NULL) t->child[0] = stmt_sequence();
//    match(UNTIL);
//    if (t != NULL) t->child[1] = exp();
//    return t;
//}

//TreeNode * assign_stmt(void)
//{
//    TreeNode * t = newStmtNode(AssignK);
//    if ((t != NULL) && (token == ID))
//        t->attr.name = copyString(tokenString);
//    match(ID);
//    if (token == ASSIGN) {
//        match(ASSIGN);
//    }
//    else if( token == ADDITIVE){  //加法赋值号的文法扩充
//        match(ADDITIVE);
//        t->kind.stmt = AdditiveK;
//    }
//    if (t != NULL) t->child[0] = exp();
//    return t;
//}

//TreeNode * read_stmt(void)
//{
//    TreeNode * t = newStmtNode(ReadK);
//    match(READ);
//    if ((t != NULL) && (token == ID))
//        t->attr.name = copyString(tokenString);
//    match(ID);
//    return t;
//}

//TreeNode * write_stmt(void)
//{
//    TreeNode * t = newStmtNode(WriteK);
//    match(WRITE);
//    if (t != NULL) t->child[0] = exp();
//    return t;
//}

//TreeNode * exp(void)
//{
//    TreeNode * t = simple_exp();
//    if ((token == LT) || (token == EQ)) {
//        TreeNode * p = newExpNode(OpK);
//        if (p != NULL) {
//            p->child[0] = t;
//            p->attr.op = token;
//            t = p;
//        }
//        match(token);
//        if (t != NULL)
//            t->child[1] = simple_exp();
//    }
//    return t;
//}

//TreeNode * simple_exp(void)
//{
//    TreeNode * t = term();
//    while ((token == PLUS) || (token == MINUS))
//    {
//        TreeNode * p = newExpNode(OpK);
//        if (p != NULL) {
//            p->child[0] = t;
//            p->attr.op = token;
//            t = p;
//            match(token);
//            t->child[1] = term();
//        }
//    }
//    return t;
//}

TreeNode * term(void)
{
    TreeNode * t = factor();
    while ((token == TIMES) || (token == OVER))
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
    case ID://call or var
        t = var_call();
        break;
    case LPAREN:
        match(LPAREN);
        t = expression();
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

TreeNode * var_call(void){
    TreeNode * t = newExpNode(ConstK);
    if(t != NULL){
        t->attr.name = copyString(tokenString);
    }
    match(ID);
    if(t!=NULL && token==LPAREN){
        t->child[0] = call();
    }else{
        t->child[0] = var();
    }
    return t;
}

TreeNode * call(void){



}

///*TINY的文法扩充*/
//static TreeNode * while_stmt(void) {
//    TreeNode * t = newStmtNode(WhileK);
//    match(WHILE);
//    if (t != NULL) {
//        t->child[0] = exp();
//    }
//    match(DO);
//    if (t != NULL) {
//        t->child[1] = stmt_sequence();
//    }
//    match(ENDWHILE);
//    return t;
//}

//static TreeNode * dowhile_stmt(void) {
//    TreeNode * t = newStmtNode(DowhileK);
//    match(DO);
//    if (t != NULL) {
//        t->child[0] = stmt_sequence();
//    }
//    match(WHILE);
//    if (t != NULL) {
//        t->child[1] = exp();
//    }
//    return t;
//}


//static TreeNode * for_stmt(void) {
//    TreeNode * t = newStmtNode(ForIncK);
//    match(FOR);
//    if (t != NULL) {
//        t->child[0] = assign_stmt();
//        if (token == TO) {
//            match(TO);
//        }
//        else if (token == DOWNTO) {
//            match(DOWNTO);
//            t->kind.stmt = ForDecK;
//        }
//        t->child[1] = simple_exp();
//        match(DO);
//        t->child[2] = stmt_sequence();
//        match(ENDDO);
//    }
//    return t;
//}



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
    //t = stmt_sequence();
    t = declaration_list();
    if (token != ENDFILE)
        syntaxError("Code ends before file\n");
    return t;
}

