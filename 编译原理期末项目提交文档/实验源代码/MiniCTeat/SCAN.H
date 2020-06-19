/**
  * SCAN.H
  *
  * @brief The scanner interface for the CMINUS compiler
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/4/20
  */


#ifndef _SCAN_H_
#define _SCAN_H_
#include"GLOBALS.H"


/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN + 1];

/* function getToken returns the
* next token in source file
*/
TypeToken getToken(void);
/* function getToken rock balck
 * to the last token
*/
void ungetToken(void);
#endif
