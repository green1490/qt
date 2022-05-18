#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlistwidget.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include <qmap.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qaction.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <QMultiMap>
#include <iostream>

//TO DO
//menüsor
//beállítások
//kétnyelvű
//hordozható változat
//context menű a megjegyzésre
//megjegyzés tőrlése
//leírás mentése
//a túl mérezett kép label widget nem lehet vissza méretezni

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

    void on_tallozas_pushButton_clicked();

    void on_view_pushButton_clicked();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_search_pushButton_5_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_descriptioncheckBox_stateChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QMap<QString,QString> files;
    QMap<QString,QString> description;
    QMultiMap<QString,QString> tags;
};
#endif // MAINWINDOW_H
