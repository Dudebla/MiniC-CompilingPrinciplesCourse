#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include<QTextCodec>

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

const int STYLE_TREE_LIST = 1;
const int STYLE_TREE_GRAPH = 0;

TypeToken token;
std::map<string, VarStruct> VarStructMap;
std::map<string, FunStruct> FunStructMap;

int Flag_isOpen; // mask: judge if open or create a file
int Flag_isNew; // mask: create a new file if 1
int Flag_parserResultStyle; //mask: parse result style is tree list if 1, tree graph other
QString Last_fileName; // name of file have saved last time
QString Last_fileContent; // content of file have saved last time
QString lexicalResult;//
//保存生成的语法树
TreeNode *syntaxTree;
//保存打开的源文件
FILE * source;
int lineno = 0;
FILE * listing;
FILE * code;
int EOF_flag = false;

string lexicalMessage = "词法分析输出，格式：行号. 识别的ID";
string errorMessage = "";

ManageMapState manageMapState;
std::string lastDeclaredFunName;

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
    Flag_parserResultStyle = STYLE_TREE_LIST; //mask: default 1, parse result show as list
    lexicalResult = "";

    this->sourceTextEdit = this->ui->sourceTextEdit;

    this->lexicalTextEdit = this->ui->lexicalTextEdit;
    this->lexicalTextEdit->setReadOnly(true);
    this->parserTextEdit = this->ui->parserTextEdit;
    this->parserTextEdit->setReadOnly(true);
    this->synerrorTextEdit = this->ui->synerrorTextEdit;
    this->synerrorTextEdit->setReadOnly(true);

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
    Last_fileName = "";
    Last_fileContent = "";
    Flag_isNew = 1;
    Flag_isOpen = 0;
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
         }
    }
    /*
     ** END
    */
    this->lexicalTextEdit->setPlainText(QString::fromStdString(lexicalMessage));
    this->synerrorTextEdit->setPlainText(QString::fromStdString(errorMessage));
    lexicalResult = QString::fromStdString(lexicalMessage);

}

/*
 * parser the content in the textEdit
*/
void MainWindow::on_parseFile_triggered()
{
    if(lexicalMessage==""){
        this->on_lexicalFile_triggered();
    }
    //打印语法树
    QString result;//存储打印的语法树
    result = printTree(syntaxTree);
    //显示输出语法树
    this->parserTextEdit->setPlainText(result);
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

    bool sourceIsCompile = true;//======sourceIsCompile sate=====
    //==================set the sourceIsCompile sate here========
    // edit code here
    //


    if(!sourceIsCompile){
        QMessageBox::warning(this, tr("Error"), tr("Failed creating assemble instruction!"), QMessageBox::Ok);
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
            QString str = "=======assemble instruction here=============";
            //================ set the assemble instruction here=========
            // edit code here
            //
            //生成汇编文件.cm
            //初始化全局变量
            initMap();
            //生成语法树
            syntaxTree = parse();
            //解析语法树成为.cm文件
            //代码产生并保存到名为“codefileName”的.cm文件中
//            codeGen(syntaxTree, codefileName);

            textStream<<str;
            Last_fileContent = str;
        }
        QMessageBox::information(this, "Save File", "Success saving the assemble instruction!", QMessageBox::Ok);
        fileName.close();
    }
}

/*
 * change the style showing in parse widget
*/
void MainWindow::on_changeStyleButton_clicked()
{
    if(Flag_parserResultStyle==STYLE_TREE_LIST){
        Flag_parserResultStyle = STYLE_TREE_GRAPH;
    }else{
        Flag_parserResultStyle = STYLE_TREE_LIST;
    }
    if(this->ui->tabWidget->currentIndex()==1){ // reload

    }
}

