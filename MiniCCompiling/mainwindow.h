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

    int Flag_isOpen; // mask: judge if open or create a file
    int Flag_isNew; // mask: create a new file if 1
    int Flag_parserResultStyle; //mask: parse result style is tree list if 1, tree graph other
    QString Last_fileName; // name of file have saved last time
    QString Last_fileContent; // content of file have saved last time
    QString lexicalResult;//

    QTextEdit * sourceTextEdit;
    QTextEdit * lexicalTextEdit;

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
