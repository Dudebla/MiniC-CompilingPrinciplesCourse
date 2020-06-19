/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SYMTAB.H"

/* SHIFT is the power of two used as multiplier
in hash function  */
#define SHIFT 4

#define MAX_SCOPE 1000

/* the hash table */
static BucketList hashTable[SIZE];

static Scope scopes[MAX_SCOPE];
static int nScope = 0;
static Scope scopeStack[MAX_SCOPE];
static int nScopeStack = 0;
static int location[MAX_SCOPE];


/* the hash function */
static int Hash(char * key)
{
    int temp = 0;
    int i = 0;
    while (key[i] != '\0')
    {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

/* Procedure st_insert inserts line numbers and
* memory locations into the symbol table
* loc = memory location is inserted only the
* first time, otherwise ignored
*/
/*void st_insert(char * name, int lineno, int loc)
{
    int h = Hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL)
    {
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList)malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->memloc = loc;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    }
    else
    {
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
    }
}*/ /* st_insert */

//MiniC st_insert function
void st_insert( char * name, int lineno, int loc, TreeNode * treeNode )
{ int h = Hash(name);
  Scope top = sc_top();
  BucketList l =  top->hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->treeNode = treeNode;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = top->hashTable[h];
    top->hashTable[h] = l; }
  else /* found in table, so just add line number */
  { // ERROR!
  }
} /* st_insert */

  /* Function st_lookup returns the memory
  * location of a variable or -1 if not found
  */
int st_lookup(char * name)
{
//    int h = Hash(name);
//    BucketList l = hashTable[h];
//    while ((l != NULL) && (strcmp(name, l->name) != 0))
//        l = l->next;
//    if (l == NULL) return -1;
//    else return l->memloc;
    BucketList l = st_bucket(name);
    if (l != NULL) return l->memloc;
    return -1;

}

int st_lookup_top (char * name)
{ int h = Hash(name);
  Scope sc = sc_top();
  while(sc) {
    BucketList l = sc->hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0))
      l = l->next;
    if (l != NULL) return l->memloc;
    break;
  }
  return -1;
}

int st_add_lineno(char * name, int lineno)
{ BucketList l = st_bucket(name);
  LineList ll = l->lines;
  while (ll->next != NULL) ll = ll->next;
  ll->next = (LineList) malloc(sizeof(struct LineListRec));
  ll->next->lineno = lineno;
  ll->next->next = NULL;
}


//MiniC添加部分

Scope sc_top( void )
{
    return scopeStack[nScopeStack - 1];
}

void sc_pop( void )
{
    --nScopeStack;
}

int addLocation( void )
{
    return location[nScopeStack - 1]++;
}

void sc_push( Scope scope )
{
    scopeStack[nScopeStack] = scope;
    location[nScopeStack++] = 0;
}

Scope sc_create(char *funcName)
{
    Scope newScope;

    newScope = (Scope) malloc(sizeof(struct ScopeRec));
    newScope->funcName = funcName;
    newScope->nestedLevel = nScopeStack;
    newScope->parent = sc_top();
    int i = 0;
    while(i < SIZE){
        newScope->hashTable[i++] = NULL;
    }

    scopes[nScope++] = newScope;
    return newScope;
}

BucketList st_bucket( char * name )
{
    int h = Hash(name);
    Scope sc = sc_top();
    while(sc) {
    BucketList l = sc->hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0))
      l = l->next;
    if (l != NULL) return l;
    sc = sc->parent;
    }
    return NULL;
}



/* Procedure printSymTab prints a formatted
* listing of the symbol table contents
* to the listing file
*/
void printSymTab(FILE * listing)
{
//    int i;
//    fprintf(listing, "Variable Name  Location   Line Numbers\n");
//    fprintf(listing, "-------------  --------   ------------\n");
//    for (i = 0; i<SIZE; ++i)
//    {
//        if (hashTable[i] != NULL)
//        {
//            BucketList l = hashTable[i];
//            while (l != NULL)
//            {
//                LineList t = l->lines;
//                fprintf(listing, "%-14s ", l->name);
//                fprintf(listing, "%-8d  ", l->memloc);
//                while (t != NULL)
//                {
//                    fprintf(listing, "%4d ", t->lineno);
//                    t = t->next;
//                }
//                fprintf(listing, "\n");
//                l = l->next;
//            }
//        }
//    }
} /* printSymTab */

