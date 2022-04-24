#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->add_pushButton->setIcon(QIcon(":/icons/plus.ico"));
    ui->remove_pushButton_2->setIcon(QIcon(":/icons/minus.png"));
    ui->search_pushButton_5->setIcon(QIcon(":/icons/search.png"));

    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setIconSize(QSize(100,100));

    //megnyitáskor tölste be az adatokat
    //lehesen open foldert a menüben
    //context menüben tag-et vagy szöveget adni

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_pushButton_clicked()
{
    //icon méret változtatás
    QString path = QFileDialog::getOpenFileName();
    //valszeg windows-on nem mükszik
    QStringList list = path.split('/');
    QString file = list.last();

    if(files.contains(file))
        return;

    if(path != "")
    {
        QListWidgetItem *it = new QListWidgetItem(file);
        it->setIcon(QIcon(path));
        ui->listWidget->addItem(it);
        files[file] = path;
    }

}

void MainWindow::on_remove_pushButton_2_clicked()
{
    QListWidgetItem *it = ui->listWidget->takeItem(ui->listWidget->currentRow());

    files.remove(it->text());

    delete it;

}


void MainWindow::on_MainWindow_destroyed()
{
    //https://stackoverflow.com/questions/27285889/how-do-you-serialize-a-qmap
}

