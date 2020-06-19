/**
  * main.C
  *
  * @brief The main file..
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/4/10
  */

#include "mainwindow.h"
#include <QApplication>


#include "GLOBALS.H"
/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

#include "UTIL.H"
#if NO_PARSE
#include "SCAN.H"
#else
#include "PARSE.H"
#if !NO_ANALYZE
#include "ANALYZE.H"
#if !NO_CODE
#include "CGEN.H"
#endif
#endif
#endif

/* allocate global variables */
//int lineno = 0;
//FILE * source;
//FILE * listing;
//FILE * code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
