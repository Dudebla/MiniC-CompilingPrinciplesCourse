/**
  * mainwindow.h
  *
  * @brief The program's UI control file. Handle user event.
  * @version 1.0.0
  * @authors PW. & Dudebla
  * @date 2020/4/10
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTextEdit * sourceTextEdit;
    QTextEdit * lexicalTextEdit;
    QTextEdit * parserTextEdit;
    QTextEdit * synerrorTextEdit;
    QTextEdit * assemblyTextEdit;

private slots:
    // file operation
    void on_openFile_triggered();
    void on_changeStyleButton_clicked();
    void on_saveFile_triggered();
    void on_lexicalFile_triggered();
    void on_parseFile_triggered();
    void on_assemblyFile_triggered();
    // show style operation
    void on_clearFile_triggered();
};

#endif // MAINWINDOW_H
