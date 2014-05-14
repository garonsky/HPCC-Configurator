/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionRecent;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionClose;
    QAction *actionAbout;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionStart_Macro_Capture;
    QAction *actionStop_Macro_Capture;
    QAction *actionSave_Macro;
    QAction *actionAdd_Roxie;
    QAction *actionAdd_Thor_s;
    QWidget *centralWidget;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAdd_Component;
    QMenu *menuAdd_Service;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuWizards;
    QMenu *menuMacros;
    QMenu *menuSettings;
    QMenu *menuValidation;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 663);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionRecent = new QAction(MainWindow);
        actionRecent->setObjectName(QStringLiteral("actionRecent"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionStart_Macro_Capture = new QAction(MainWindow);
        actionStart_Macro_Capture->setObjectName(QStringLiteral("actionStart_Macro_Capture"));
        actionStop_Macro_Capture = new QAction(MainWindow);
        actionStop_Macro_Capture->setObjectName(QStringLiteral("actionStop_Macro_Capture"));
        actionSave_Macro = new QAction(MainWindow);
        actionSave_Macro->setObjectName(QStringLiteral("actionSave_Macro"));
        actionAdd_Roxie = new QAction(MainWindow);
        actionAdd_Roxie->setObjectName(QStringLiteral("actionAdd_Roxie"));
        actionAdd_Thor_s = new QAction(MainWindow);
        actionAdd_Thor_s->setObjectName(QStringLiteral("actionAdd_Thor_s"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea_2 = new QScrollArea(centralWidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(300, 20, 981, 931));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 979, 929));
        verticalLayoutWidget = new QWidget(scrollAreaWidgetContents_2);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 981, 941));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(0, 20, 291, 571));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAdd_Component = new QMenu(menuBar);
        menuAdd_Component->setObjectName(QStringLiteral("menuAdd_Component"));
        menuAdd_Service = new QMenu(menuBar);
        menuAdd_Service->setObjectName(QStringLiteral("menuAdd_Service"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuWizards = new QMenu(menuBar);
        menuWizards->setObjectName(QStringLiteral("menuWizards"));
        menuMacros = new QMenu(menuWizards);
        menuMacros->setObjectName(QStringLiteral("menuMacros"));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuValidation = new QMenu(menuBar);
        menuValidation->setObjectName(QStringLiteral("menuValidation"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuAdd_Component->menuAction());
        menuBar->addAction(menuAdd_Service->menuAction());
        menuBar->addAction(menuWizards->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuBar->addAction(menuValidation->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionRecent);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionClose);
        menuHelp->addAction(actionAbout);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuWizards->addSeparator();
        menuWizards->addAction(actionAdd_Roxie);
        menuWizards->addAction(actionAdd_Thor_s);
        menuWizards->addSeparator();
        menuWizards->addAction(menuMacros->menuAction());
        menuMacros->addAction(actionStart_Macro_Capture);
        menuMacros->addAction(actionStop_Macro_Capture);
        menuMacros->addAction(actionSave_Macro);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Configurator Central", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionRecent->setText(QApplication::translate("MainWindow", "Recent", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", 0));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0));
        actionStart_Macro_Capture->setText(QApplication::translate("MainWindow", "Start Macro Capture", 0));
        actionStop_Macro_Capture->setText(QApplication::translate("MainWindow", "Stop Macro Capture", 0));
        actionSave_Macro->setText(QApplication::translate("MainWindow", "Save Macro", 0));
        actionAdd_Roxie->setText(QApplication::translate("MainWindow", "Add New Hardware and Configure", 0));
        actionAdd_Thor_s->setText(QApplication::translate("MainWindow", "Reconfigure Existing Hardware", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuAdd_Component->setTitle(QApplication::translate("MainWindow", "Add Component", 0));
        menuAdd_Service->setTitle(QApplication::translate("MainWindow", "Add Service", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuWizards->setTitle(QApplication::translate("MainWindow", "Wizards", 0));
        menuMacros->setTitle(QApplication::translate("MainWindow", "Macros", 0));
        menuSettings->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        menuValidation->setTitle(QApplication::translate("MainWindow", "Validation", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
