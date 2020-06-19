/**
  * ANALYZE.H
  *
  * @brief Semantic analyzer implementation for the CMINUS compiler
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/6/5
  */

#ifndef _ANALYZE_H_
#define _ANALYZE_H_
#include"GLOBALS.H"

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *);

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);

#endif
