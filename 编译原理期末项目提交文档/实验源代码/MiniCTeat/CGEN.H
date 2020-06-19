/**
  * CGEN.H
  *
  * @brief The code generator interface to the CMINUS compiler
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/6/5
  */

#ifndef _CGEN_H_
#define _CGEN_H_
#include"GLOBALS.H"


/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, const char * codefile);

#endif
