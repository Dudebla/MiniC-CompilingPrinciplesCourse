/**
  * mainwindow.C
  *
  * @brief The program's UI control file. Handle user event.
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/4/10
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QTreeWidget>

#include<string>
/*
 * include编译工具文件
 */
#include"GLOBALS.H"
#include"ANALYZE.H"
#include"CGEN.H"
#include"CODE.H"
#include"SYMTAB.H"
#include"UTIL.H"
#include"PARSE.H"

using namespace std;


TypeToken token;// using for souce content's scan
std::map<string, VarStruct> VarStructMap;// using for analyzation, storing the vars and params
std::map<string, FunStruct> FunStructMap;// using for analyzation, storeing the functions

int Flag_isOpen; // mask: flag for open or create a file
int Flag_isNew; // mask: create a new file if 1
int Flag_parserResultStyle; //mask: parse result style is tree list if 1, tree graph other
QString Last_fileName; // name of file have saved last time
QString Last_fileContent; // content of file have saved last time
QString lexicalResult;// storing the lex result as QString for displaying
QString parserResult;// storing the syntaxtree as QString for displaying
//保存生成的语法树
TreeNode *syntaxTree;// storing the synextree as tree for compiling
//保存打开的源文件
FILE * source;// storing the source file
int lineno = 0;// mask for scaning lines of source
FILE * listing;// listing output text file
FILE * code;// code text file for TM simulator
int EOF_flag = false;// corrects ungetNextChar behavior on EOF


string lexicalMessage = "词法分析输出，格式：行号. 识别的ID";// lexial message for display
string errorMessage = "";// error message for display

ManageMapState manageMapState;// marking the status while parsing
std::string lastDeclaredFunName;// marking the last declared function
std::string lastDeclaredVarName;// marking the last declared var

Scope globalScope;// storing the status


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮

    setFixedSize(this->width(),this->height());// enable size adjust

    // init state
    Flag_isOpen = 0; // mask: judge if open or create a file
    Flag_isNew = 1; // mask: 1 if create a new file, 0 other
    lexicalResult = "";

    this->sourceTextEdit = this->ui->sourceTextEdit;

    this->lexicalTextEdit = this->ui->lexicalTextEdit;
    this->lexicalTextEdit->setReadOnly(true);
    this->parserTextEdit = this->ui->parserTextEdit;
    this->parserTextEdit->setReadOnly(true);
    this->synerrorTextEdit = this->ui->synerrorTextEdit;
    this->synerrorTextEdit->setReadOnly(true);
    this->assemblyTextEdit = this->ui->assemblyTextEdit;
    this->assemblyTextEdit->setReadOnly(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * open a local file
 * load the content into sourceTextEdit
 * set Flag_isOpen value 1, Last_fileName
*/
void MainWindow::on_openFile_triggered()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("MiniC File (*.*)"));// set type of file
    if(fileName == "") //如果用户直接关闭了文件浏览对话框，那么文件名就为空值，直接返回
    {
        return;
    }
    else
    {
       QFile file(fileName);
       if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           QMessageBox::warning(this,tr("Error"),tr("Failed opening the file!"));
           return;
       }
       else
       {
           if(!file.isReadable())
           {
               QMessageBox::warning(this,tr("error"),tr("The file is unreable!"));
           }
           else
           {
               this->synerrorTextEdit->clear();
               this->lexicalTextEdit->clear();
               this->parserTextEdit->clear();

               lexicalResult = "";
               parserResult = "";

               QTextStream textStream(&file);       //读取文件，使用QTextStream
               while(!textStream.atEnd())
               {
                   this->sourceTextEdit->setPlainText(textStream.readAll());
               }
               this->sourceTextEdit->show();

               file.close();
               Flag_isOpen = 1;
               Flag_isNew = 0;
               Last_fileName = fileName;


           }
       }
    }
}

/*
 * save the content in the textEdit
 * save the exited file
 *   set Last_fileContent, Last_fileName, Flag_isNew=0
 * or create a new file
 *
*/
void MainWindow::on_saveFile_triggered()
{
    if(Flag_isNew){//toast fileDialog if 1 (is new file)
        if(this->sourceTextEdit->toPlainText()==""){
            QMessageBox::warning(this, tr("Warn"), tr("The content should not empty!"), QMessageBox::Ok);
        }else{
            QFileDialog fileDialog;
            QString str = fileDialog.getSaveFileName(this, tr("Open File"), "..", tr("MiniC File(*.*)"));//get new file name
            if(str == ""){
                return;
            }
            QFile fileName(str);
            if(!fileName.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this, tr("Error"), tr("Failed opening the file!"), QMessageBox::Ok);
                return;
            }else{
                QTextStream textStream(&fileName);
                QString str = this->sourceTextEdit->toPlainText();
                textStream<<str;
                Last_fileContent = str;
            }
            QMessageBox::information(this, "Save File", "Success saving the file!", QMessageBox::Ok);
            fileName.close();
            Flag_isNew = 0;
            Last_fileName = str;
        }
    }else{//is old file, save file
        if(Flag_isOpen){
            QFile file(Last_fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this, tr("Error"), tr("Failed opening the file!"), QMessageBox::Ok);
                return;
            }else{
                QTextStream textStream(&file);
                QString str = this->sourceTextEdit->toPlainText();
                textStream<<str;
                Last_fileContent = str;
                file.close();
            }
        }else{
            QMessageBox::warning(this, tr("Warn"), tr("Please create a new file or open a file!"));
        }
    }
}

/*
 *clear the textEdit
 * set Flag_isNew=1, Flag_isOpen=0
*/
void MainWindow::on_clearFile_triggered()
{
    if(this->sourceTextEdit->toPlainText()!=""){
        this->sourceTextEdit->clear();
    }
    if(this->lexicalTextEdit->toPlainText()!=""){
        this->lexicalTextEdit->clear();
    }
    if(this->parserTextEdit->toPlainText()!=""){
        this->parserTextEdit->clear();
    }
    if(this->synerrorTextEdit->toPlainText()!=""){
        this->synerrorTextEdit->clear();
    }
    Last_fileName = "";
    Last_fileContent = "";
    Flag_isNew = 1;
    Flag_isOpen = 0;
    lexicalResult = "";
    parserResult = "";
    lexicalMessage = "";
}

/*
 * lexical the content in the textEdit
*/
void MainWindow::on_lexicalFile_triggered()
{
    if(this->sourceTextEdit->toPlainText()==""){
        QMessageBox::warning(this, tr("Warn"), tr("The content should not empty!"), QMessageBox::Ok);
        return;
    }
    lexicalResult = "lexical the content of sourceTextEdit";
    //init Function map
    initMap();
    //调用生成语法树功能
    if(!source){
        QMessageBox::information(NULL, "Warning", "Can't open file",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }else{

         //生成语法树
         syntaxTree = parse();
         //若无语法错误，显示成功信息
         if(errorMessage == ""){
             errorMessage = Last_fileName.toStdString() + "  ==>  Success! 0 error(s)";
             this->lexicalTextEdit->setPlainText(QString::fromStdString(lexicalMessage));
             lexicalResult = QString::fromStdString(lexicalMessage);
         }else{
             syntaxTree = NULL;
             lexicalMessage = "";
         }
    }
    /*
     ** END
    */

    this->synerrorTextEdit->setPlainText(QString::fromStdString(errorMessage));


}

void * show(TreeNode * root, QTreeWidgetItem * twi){
    while (root!=NULL) {

    }
}

/*
 * parser the content in the textEdit
*/
void MainWindow::on_parseFile_triggered()
{
    if(lexicalMessage==""){
        this->on_lexicalFile_triggered();
    }
    if(lexicalMessage==""){
        return;
    }
    //打印语法树
    //QString result;//存储打印的语法树
    parserResult = printTree(syntaxTree);
    //显示输出语法树
    this->parserTextEdit->setPlainText(parserResult);

}

/*
 * create the assemble instruction of the source file
*/
void MainWindow::on_assemblyFile_triggered()
{
    if(this->sourceTextEdit->toPlainText()==""){
        QMessageBox::warning(this, tr("Warn"), tr("The content should not empty!"), QMessageBox::Ok);
        return;
    }
    if(parserResult==""){
        QMessageBox::warning(this, tr("Warn"), tr("The source should be parsed before output .CM file!"), QMessageBox::Ok);
        return;
    }

    string tempPath = (Last_fileName.split('.')[0]+".cm").toStdString();
    code = fopen(tempPath.c_str(), "w");

    if(code == NULL){
        QMessageBox::warning(this, tr("Error"), tr("Failed opening the file!"), QMessageBox::Ok);
        return;
    }else{
        //生成汇编文件.cm
        //初始化全局变量
        initMap();
        errorMessage = "";
        //生成语法树
        syntaxTree = parse();
        buildSymtab(syntaxTree);
        typeCheck(syntaxTree);
        //解析语法树成为.cm文件
        //代码产生并保存到名为“codefileName”的.cm文件中
        codeGen(syntaxTree, tempPath.c_str());
        this->synerrorTextEdit->setText(QString::fromStdString(errorMessage));

        //textStream<<str;
        //Last_fileContent = str;
        QMessageBox::information(this, "Save File", "Success saving the assemble instruction!", QMessageBox::Ok);
        fclose(code);
    }
    QFile file(tempPath.c_str());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(file.isReadable())
        {

            this->assemblyTextEdit->clear();

            QTextStream textStream(&file);       //读取文件，使用QTextStream
            while(!textStream.atEnd())
            {
                this->assemblyTextEdit->setPlainText(textStream.readAll());
            }
            this->assemblyTextEdit->show();

            file.close();
        }
    }
}

