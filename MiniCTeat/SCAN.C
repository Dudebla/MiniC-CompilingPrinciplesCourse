/****************************************************/
/* File: scan.c                                     */
/* The scanner implementation for the TINY compiler */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "GLOBALS.H"
#include "UTIL.H"
#include "SCAN.H"
#include<string>

using namespace std;

/* states in scanner DFA */
typedef enum
{
    START, INASSIGN, INCOMMENT, INNUM, INID, DONE , INPOWER
}
StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN + 1];
char *lastTokenString;


/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */
static int tokenLength = 0; //当前token长度，用于ungetToken
static TypeToken lastToken;
TypeToken currentToken;



/* getNextChar fetches the next non-blank character
from lineBuf, reading in a new line if lineBuf is
exhausted */
static int getNextChar(void)
{
    if (!(linepos < bufsize))
    {
        lineno++;
        memset(lineBuf,0,BUFLEN);
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
//static struct
//{
//    char* str;
//    TypeToken tok;
//} reservedWords[MAXRESERVED]
//= { { "if",IF },{ "then",THEN },{ "else",ELSE },{ "end",END },
//{ "repeat",REPEAT },{ "until",UNTIL },{ "read",READ },
//{ "write",WRITE },{ "do" , DO },{"while",WHILE} , {"for",FOR},{"downto",DOWNTO},{"enddo",ENDDO},{"endwhile",ENDWHILE} };
/*
 * miniC修改部分
 */
static struct
{
    char* str;
    TypeToken tok;
} reservedWords[MAXRESERVED]
= { { "else",ELSE },{ "if",IF },{ "return",RETURN },{ "while" ,WHILE },{"int",INT},{"void", VOID} };
/*
 * END
 */

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
{
    //保存上一个token的信息
    lastToken = token;
    lastTokenString = copyString(tokenString);
    tokenLength = 0;
    /* index for storing into tokenString */
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
        ++tokenLength;
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
            /*
            * miniC修改部分
            */
            else if (c == '/')   //扫描的当前字符为“/”，表示可能为除法也可能为注释的第一个字符
            {
                c = getNextChar(); //获取下一个字符进一步判断
                ++tokenLength;
                if (c == '*') {
                    save = FALSE;
                    state = INCOMMENT;
                }
                else {
                    save = TRUE;
                    ungetNextChar();
                    --tokenLength;
                    c = '/';
                    state = DONE;
                    currentToken = OVER;
                }
            }
            /*
            * END
            */
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
                    c = getNextChar();
                    ++tokenLength;
                    if (c == '=') {
                        currentToken = EQ;
                    }
                    else {
                        ungetNextChar();
                        --tokenLength;
                        //c = '=';
                        currentToken = ASSIGN;
                    }
                    break;
                case '!':
                    c = getNextChar();
                    ++tokenLength;
                    if (c == '=') {
                        currentToken = NEQ;
                    }
                    else {
                        currentToken = ERRO;
                    }
                case '<':
                    c = getNextChar();
                    ++tokenLength;
                    if (c == '=') { //manage token < and token <=
                        currentToken = LTEQ;
                    }
                    else {
                        ungetNextChar();
                        --tokenLength;
                        currentToken = LT;
                    }
                    break;
                case '>':
                    c = getNextChar();
                    ++tokenLength;
                    if (c == '=') {//manage token < and token <=
                        currentToken = GTEQ;
                    }
                    else {
                        ungetNextChar();
                        --tokenLength;
                        currentToken = GT;
                    }
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
                case '[':
                    currentToken = LBRACKET;
                    break;
                case ']':
                    currentToken = RBRACKET;
                    break;
                case '{':
                    currentToken = LBRACE;
                    break;
                case '}':
                    currentToken = RBRACE;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                case ',':
                    currentToken = COMMA;
                    break;
                default:
                    currentToken = ERRO;
                    break;
                }
            }
            break;
            //        case INCOMMENT:
            //            save = FALSE;
            //            if (c == EOF)
            //            {
            //                state = DONE;
            //                currentToken = ENDFILE;
            //            }
            //            else if (c == '}') state = START;
            //            break;
            /*
            * miniC修改部分
            */
        case INCOMMENT: //当前状态为注释内部状态
            save = FALSE;
            if (c == EOF) //是否读到文件末
            {
                state = DONE;
                currentToken = ENDFILE;
            }
            else {        //判读是否为注释结束符号
                if (c == '*') {
                    c = getNextChar();   //提前读取下一个字符
                    ++tokenLength;
                    if (c == '/') {     //连续两个字符为'*/'，退出注释状态
                        state = START;
                    }
                    else {
                        ungetNextChar();   //还原预读的字符
                        --tokenLength;
                    }

                }
            }
            break;
            /*
            * END
            */
        case INASSIGN:
            state = DONE;
            if (c == '=')
                currentToken = ASSIGN;
            else
            { /* backup in the input */
                ungetNextChar();
                --tokenLength;
                save = FALSE;
                currentToken = ERRO;
            }
            break;
        case INNUM:
            if (!isdigit(c))
            { /* backup in the input */
                ungetNextChar();
                --tokenLength;
                save = FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if (!isalpha(c))
            { /* backup in the input */
                ungetNextChar();
                --tokenLength;
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
        if ((save) && (tokenStringIndex <= MAXTOKENLEN)) {
            tokenString[tokenStringIndex++] = (char)c;
        }
        if (state == DONE)
        {
            tokenString[tokenStringIndex] = '\0';
            //tokenLength = tokenStringIndex ;
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (currentToken == ID) {
        //fprintf(listing, "\t%d: ", lineno);   //行号
        char * tokenMessage = printToken(currentToken, tokenString); //token
        //fprintf(listing, tokenMessage);

        //lexicalMessage.append(to_string(lineno) + ' ' + tokenString + '\n');
        lexicalMessage += to_string(lineno);
        lexicalMessage.append(" ");
        lexicalMessage +=   tokenString ;
        lexicalMessage.append("\r\n");
    }
    return currentToken;
}
 /* end getToken */

void ungetToken(void) {
    while (tokenLength > 0) {
        ungetNextChar();
        --tokenLength;
    }
    token = lastToken;
    strcpy(tokenString, lastTokenString);
}
