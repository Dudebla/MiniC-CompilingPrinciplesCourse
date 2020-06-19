/**
  * SYMTAB.C
  *
  * @brief Symbol table implementation for the CMINUS compiler
  * (allows only one symbol table)
  * Symbol table is implemented as a chained
  * hash table
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/5/16
  */

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
    BucketList l = st_bucket(name);
    if (l != NULL) return l->memloc;
    return -1;

}

/**
 * @brief st_lookup_top
 * @param name
 * @return the location of var or funtion
 */
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
/**
 * @brief sc_top
 * @return the last scope
 */
Scope sc_top( void )
{
    return scopeStack[nScopeStack - 1];
}

/**
 * @brief sc_pop back to the last scope
 */
void sc_pop( void )
{
    --nScopeStack;
}

/**
 * @brief addLocation
 * @return current scope
 */
int addLocation( void )
{
    return location[nScopeStack - 1]++;
}

/**
 * @brief sc_push push the scope into stack
 * @param scope
 */
void sc_push( Scope scope )
{
    scopeStack[nScopeStack] = scope;
    location[nScopeStack++] = 0;
}

/**
 * @brief sc_create create a new scope due to the function name
 * @param funcName
 * @return scope
 */
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
// TODO
} /* printSymTab */

