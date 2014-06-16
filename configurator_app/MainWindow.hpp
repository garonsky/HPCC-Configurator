#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QThread>
#include <QString>
#include "Worker.hpp"
#include "AppData.hpp"

class QQuickView;
class ApplicationData;
class TableDataModel;
class ComponentDataModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void addComponentToList(char *pComponent);
    void addServiceToList(char *pService);

private:
    Ui::MainWindow *ui;
    QQuickView *m_pView;
    QString m_envFile;
    ApplicationData *m_pAppData;
    TableDataModel *m_pTableDataModel;
    ComponentDataModel *m_pComponentDataModel;

//    QThread *m_pThread;
//    CWorker *m_pWorker;

private slots:
//    void on_componentListUpdated();
    void on_actionOpen_triggered();
    void on_treeView_clicked(const QModelIndex &index);
    void on_actionReload_triggered();
    void on_actionGenerate_QML_triggered();
    void on_actionGenerate_DockBook_triggered();
    void on_actionGenerate_Internal_Data_Structure_Debug_triggered();
    void on_actionGenerate_Dojo_triggered();
};

#endif // MAINWINDOW_HPP
