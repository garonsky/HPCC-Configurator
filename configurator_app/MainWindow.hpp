#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QThread>
#include <QString>
#include "Worker.hpp"
#include "AppData.hpp"

class QQuickView;

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

//    QThread *m_pThread;
//    CWorker *m_pWorker;

private slots:
//    void on_componentListUpdated();
    void on_actionOpen_triggered();
    void on_treeView_clicked(const QModelIndex &index);
    void on_actionReload_triggered();
};

#endif // MAINWINDOW_HPP
