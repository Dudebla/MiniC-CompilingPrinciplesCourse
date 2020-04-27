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
std::map<char*, VarStruct> VarStructMap;
std::map<char*, FunStruct> FunStructMap;


//保存生成的语法树
TreeNode *syntaxTree;
//保存打开的源文件
FILE * source;
int lineno = 0;
FILE * listing;
FILE * code;

string lexicalMessage;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init state
    this->Flag_isOpen = 0; // mask: judge if open or create a file
    this->Flag_isNew = 1; // mask: 1 if create a new file, 0 other
    this->Flag_parserResultStyle = STYLE_TREE_LIST; //mask: default 1, parse result show as list
    this->lexicalResult = "";

    this->sourceTextEdit = this->ui->sourceTextEdit;

    this->lexicalTextEdit = this->ui->lexicalTextEdit;
//    this->lexicalWidget = this->ui->tabWidget->widget(0);
//    this->parseWidget = this->ui->tabWidget->widget(1);

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
               QTextStream textStream(&file);       //读取文件，使用QTextStream
               while(!textStream.atEnd())
               {
                   this->sourceTextEdit->setPlainText(textStream.readAll());
               }
               this->sourceTextEdit->show();

               /*
                ** C语言打开文件并存为FILE类型，保存到source变量中
               */
               //可打开含中文路径文件
               QTextCodec *code = QTextCodec::codecForName("GB2312");
               std::string name = code->fromUnicode(fileName).data();
               source=fopen(name.c_str(),"rb");

               //调用生成语法树功能
               if(!source){
                   QMessageBox::information(NULL, "Warning", "Can't open file",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                   return;
               }else{
                    //init Function map
                    initMap();
                    //生成语法树
                    syntaxTree = parse();
                    //打印语法树
//                    QString result;//存储打印的语法树
//                    result = printTree(syntaxTree);

                    //显示输出语法树
//                    this->lexicalTextEdit->setPlainText(result);

               }
               /*
                ** END
               */


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
    if(this->Flag_isNew){//toast fileDialog if 1 (is new file)
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
                this->Last_fileContent = str;
            }
            QMessageBox::information(this, "Save File", "Success saving the file!", QMessageBox::Ok);
            fileName.close();
            this->Flag_isNew = 0;
            this->Last_fileName = str;
        }
    }else{//is old file, save file
        if(this->Flag_isOpen){
            QFile file(this->Last_fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this, tr("Error"), tr("Failed opening the file!"), QMessageBox::Ok);
                return;
            }else{
                QTextStream textStream(&file);
                QString str = this->sourceTextEdit->toPlainText();
                textStream<<str;
                this->Last_fileContent = str;
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
    this->lexicalResult = "lexical the content of sourceTextEdit";
    //==========edit the lexical result=========================
    // edit code here
    //

    if(this->lexicalResult!=""){
        this->lexicalTextEdit->clear();
        this->lexicalTextEdit->setText(this->lexicalResult);
    }
}

/*
 * parser the content in the textEdit
*/
void MainWindow::on_parseFile_triggered()
{

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

            textStream<<str;
            this->Last_fileContent = str;
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
    if(this->Flag_parserResultStyle==STYLE_TREE_LIST){
        this->Flag_parserResultStyle = STYLE_TREE_GRAPH;
    }else{
        this->Flag_parserResultStyle = STYLE_TREE_LIST;
    }
    if(this->ui->tabWidget->currentIndex()==1){ // reload

    }
}

