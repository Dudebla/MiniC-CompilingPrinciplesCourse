/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *lexicalFile;
    QAction *openFile;
    QAction *saveFile;
    QAction *parseFile;
    QAction *assemblyFile;
    QAction *clearFile;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QTextEdit *sourceTextEdit;
    QPushButton *changeStyleButton;
    QTabWidget *tabWidget;
    QWidget *lexical;
    QTextEdit *lexicalTextEdit;
    QWidget *parser;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(831, 589);
        lexicalFile = new QAction(MainWindow);
        lexicalFile->setObjectName(QStringLiteral("lexicalFile"));
        lexicalFile->setCheckable(true);
        lexicalFile->setChecked(false);
        openFile = new QAction(MainWindow);
        openFile->setObjectName(QStringLiteral("openFile"));
        saveFile = new QAction(MainWindow);
        saveFile->setObjectName(QStringLiteral("saveFile"));
        parseFile = new QAction(MainWindow);
        parseFile->setObjectName(QStringLiteral("parseFile"));
        assemblyFile = new QAction(MainWindow);
        assemblyFile->setObjectName(QStringLiteral("assemblyFile"));
        clearFile = new QAction(MainWindow);
        clearFile->setObjectName(QStringLiteral("clearFile"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        sourceTextEdit = new QTextEdit(centralWidget);
        sourceTextEdit->setObjectName(QStringLiteral("sourceTextEdit"));
        sourceTextEdit->setAutoFillBackground(true);

        gridLayout_2->addWidget(sourceTextEdit, 1, 0, 1, 1);

        changeStyleButton = new QPushButton(centralWidget);
        changeStyleButton->setObjectName(QStringLiteral("changeStyleButton"));

        gridLayout_2->addWidget(changeStyleButton, 2, 1, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        lexical = new QWidget();
        lexical->setObjectName(QStringLiteral("lexical"));
        lexicalTextEdit = new QTextEdit(lexical);
        lexicalTextEdit->setObjectName(QStringLiteral("lexicalTextEdit"));
        lexicalTextEdit->setGeometry(QRect(-10, 0, 411, 461));
        tabWidget->addTab(lexical, QString());
        parser = new QWidget();
        parser->setObjectName(QStringLiteral("parser"));
        tabWidget->addTab(parser, QString());

        gridLayout_2->addWidget(tabWidget, 0, 1, 2, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        mainToolBar->addAction(openFile);
        mainToolBar->addAction(saveFile);
        mainToolBar->addAction(lexicalFile);
        mainToolBar->addAction(parseFile);
        mainToolBar->addAction(assemblyFile);
        mainToolBar->addAction(clearFile);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        lexicalFile->setText(QApplication::translate("MainWindow", "lexical", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        lexicalFile->setToolTip(QApplication::translate("MainWindow", "lexical the code in textEdit", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        openFile->setText(QApplication::translate("MainWindow", "open", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        openFile->setToolTip(QApplication::translate("MainWindow", "open a exited file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        saveFile->setText(QApplication::translate("MainWindow", "save", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        saveFile->setToolTip(QApplication::translate("MainWindow", "save a file to local", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        parseFile->setText(QApplication::translate("MainWindow", "parse", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        parseFile->setToolTip(QApplication::translate("MainWindow", "parse the code in textEdit", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        assemblyFile->setText(QApplication::translate("MainWindow", "assembly", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        assemblyFile->setToolTip(QApplication::translate("MainWindow", "create the assemble instruction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        clearFile->setText(QApplication::translate("MainWindow", "clear", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        clearFile->setToolTip(QApplication::translate("MainWindow", "clear the textEdit", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainWindow", "MiniC Source Code", Q_NULLPTR));
        changeStyleButton->setText(QApplication::translate("MainWindow", "Change Style", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(lexical), QApplication::translate("MainWindow", "lexical", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(parser), QApplication::translate("MainWindow", "parser", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
