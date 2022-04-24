#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlistwidget.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include <qmap.h>
#include <qfile.h>
#include <qdatastream.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_pushButton_clicked();

    void on_remove_pushButton_2_clicked();

    void on_MainWindow_destroyed();

private:
    Ui::MainWindow *ui;
    QMap<QString,QString> files;
};
#endif // MAINWINDOW_H
