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
    ui->listWidget->setAcceptDrops(false);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    ui->listWidget->setResizeMode(QListView::Adjust);

    ui->imageLabel->setAlignment(Qt::AlignCenter);
    //lehesen open foldert a menüben
    //context menüben tag-et vagy szöveget adni

    QString settings;
    QFile file{"data.json"};
    file.open(QIODevice::ReadOnly);
    settings = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(settings.toUtf8());
    QJsonObject json = sd.object();
    auto varmap = json.toVariantMap();
    if(!varmap.isEmpty())
    {
        for(QVariantMap::iterator iter = varmap.begin(); iter != varmap.end(); ++iter) {
          files[iter.key()] = iter.value().value<QString>();
        }
    }

    for(QMap<QString,QString>::iterator iter = files.begin();iter != files.end();++iter)
    {
        QListWidgetItem *it = new QListWidgetItem(iter.key());
        it->setIcon(QIcon(iter.value()));
        ui->listWidget->addItem(it);
    }
    {
        QFile file_tag("tag.txt");
        file_tag.open(QIODevice::ReadOnly);

        QTextStream in(&file_tag);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            tags.insert(fields[0],fields[1]);
        }
        file_tag.close();
    }
    {
        QFile file_tag("descreption.txt");
        file_tag.open(QIODevice::ReadOnly);

        QTextStream in(&file_tag);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            description.insert(fields[0],fields[1]);
        }
        file_tag.close();
        qDebug() << "sikeres leírás beolvasás\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    //https://stackoverflow.com/questions/27285889/how-do-you-serialize-a-qmap
    QVariantMap vmap;

    QMapIterator<QString, QString> i(files);
    while (i.hasNext()) {
        i.next();
        vmap.insert(i.key(), i.value());
    }

    QJsonDocument json = QJsonDocument::fromVariant(vmap);

       QByteArray bytes = json.toJson( QJsonDocument::Indented );
       QFile file{"data.json"};
       if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
       {
           QTextStream iStream( &file );
           iStream << bytes;
           file.close();
       }
       else
       {
           qDebug() << "file open failed: " << "data.json" << "\n";
       }

       // Create a new file
       //itt inkább txt-re váltottam mert a kiírása nem verbose
       {
       QFile tags_file("tag.txt");
       tags_file.open(QIODevice::WriteOnly | QIODevice::Text);
       QTextStream out(&tags_file);

       QMultiMap<QString,QString>::iterator item;
       for (item = tags.begin(); item != tags.end(); ++item)
       {
           out << item.key() + "," + item.value() + '\n';
       }
       qDebug() << "sikeres kiírás\n";
       tags_file.close();
        }

       {
       QFile descreption_file("descreption.txt");
       descreption_file.open(QIODevice::WriteOnly | QIODevice::Text);
       QTextStream out(&descreption_file);

       QMap<QString,QString>::iterator item;
       for (item = description.begin(); item != description.end(); ++item)
       {
           out << item.key() + "," + item.value() + '\n';
       }
       qDebug() << "sikeres kiírás a leírásnak\n";
       descreption_file.close();
       }
}


void MainWindow::on_add_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName();
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
    if(ui->listWidget->currentRow() == -1)
        return;

    QListWidgetItem *it = ui->listWidget->takeItem(ui->listWidget->currentRow());

    files.remove(it->text());

    delete it;

}

void MainWindow::on_tallozas_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page1);
}


void MainWindow::on_view_pushButton_clicked()
{
    auto *currentItem = ui->listWidget->currentItem();
    if(currentItem != NULL)
    {
        ui->stackedWidget->setCurrentWidget(ui->page2);
        QPixmap pic{files[currentItem->text()]};


        //ha van resize akkor ezis legyen
        ui->imageLabel->setPixmap(pic.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));


    }
    else
        qDebug() << "Select an item";
}


void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{

        if(ui->listWidget->currentItem() != NULL)
        {
            auto currentItemText = ui->listWidget->currentItem()->text();
            QMenu *menu=new QMenu(this);
            auto newTagAction = new QAction("New description", this);
            menu->addAction(newTagAction);

            connect(newTagAction,&QAction::triggered ,[this](){
                QWidget *wdg = new QWidget;
                auto *layout = new QVBoxLayout();
                auto *lineEditContext = new QLineEdit();
                auto *button = new QPushButton("Save");
                QString currentItemText("");
                if(ui->listWidget->currentItem() != NULL)
                    currentItemText = ui->listWidget->currentItem()->text();

                layout->addWidget(lineEditContext);
                layout->addWidget(button);


                connect(button,&QPushButton::clicked,[this,currentItemText,lineEditContext]{
                    if(currentItemText != "")
                        description.insert(currentItemText, lineEditContext->text());
                    lineEditContext->clear();
                });

                layout->setAlignment(Qt::AlignCenter);
                wdg->setLayout(layout);
                wdg->show();
            });


            auto action2 = new QAction("Add/remove tags", this);
            menu->addAction(action2);
            connect(action2,&QAction::triggered ,[&,currentItemText](){
                QWidget *wdg = new QWidget;
                auto *layout = new QVBoxLayout();
                auto *listWidgetContext = new QListWidget;
                auto *buttonLayout = new QHBoxLayout;
                auto *addButton = new QPushButton("+");
                auto *removeButton = new QPushButton("-");
                auto *lineEdit = new QLineEdit;

                buttonLayout->addWidget(lineEdit);
                buttonLayout->addWidget(addButton);
                buttonLayout->addWidget(removeButton);
                layout->addLayout(buttonLayout);

                for(auto item:tags.values(currentItemText))
                {
                    listWidgetContext->addItem(item);
                }

//                QMultiMap<QString,QString>::iterator item;
//                for (item = tags.begin(); item != tags.end(); ++item)
//                {
//                    auto *listItem = new QListWidgetItem;
//                    listItem->setText(item.value());
//                    listWidgetContext->addItem(listItem);
//                }
                qDebug() << tags.size();

                connect(removeButton,&QPushButton::clicked,[&,listWidgetContext,currentItemText](){
                    if(listWidgetContext->currentItem() != NULL)
                    {
                        auto selectedText = listWidgetContext->currentItem()->text();
                        delete listWidgetContext->takeItem(listWidgetContext->currentRow());
                        tags.remove(currentItemText,selectedText);
                    }

                });

                connect(addButton,&QPushButton::clicked,[&,lineEdit,listWidgetContext,currentItemText](){


                    if(lineEdit->text() != "" && !tags.contains(currentItemText,lineEdit->text()))
                    {
                        auto *item = new QListWidgetItem;
                        item->setText(lineEdit->text());
                        lineEdit->clear();
                        listWidgetContext->addItem(item);
                        tags.insert(currentItemText,item->text());
                    }

                });

                listWidgetContext->setFrameShape(QFrame::NoFrame);

                layout->addWidget(listWidgetContext);
                wdg->setLayout(layout);
                wdg->show();
            });


            menu->popup(ui->listWidget->viewport()->mapToGlobal(pos));
        }

}

void MainWindow::on_search_pushButton_5_clicked()
{
    auto lineEditText = ui->lineEdit->text().simplified().replace(" ","");

    if(lineEditText != "" && ui->checkBox->isChecked())
        {
        ui->listWidget->clear();
        auto uniqueKeys = tags.uniqueKeys();

        auto tagsInput =  lineEditText.split(",");
        for(QString &uniqueKey:uniqueKeys)
        {
            for(QString& tag:tagsInput)
            {
                if(!tags.contains(uniqueKey,tag))
                {
                    uniqueKey.remove(uniqueKey);
                }
            }
        }


        QStringList::iterator item;
        for (item = uniqueKeys.begin(); item != uniqueKeys.end(); ++item)
       {
            if(*item != "")
            {
               auto listItem = new QListWidgetItem;
               listItem->setIcon(QIcon(files.value(*item)));
               listItem->setText(*item);
               ui->listWidget->addItem(listItem);
           }
        }
     }

    else if(lineEditText != "" && ui->descriptioncheckBox->isChecked())
    {

        ui->listWidget->clear();
        QMap<QString,QString>::iterator item;
        for (item = description.begin(); item != description.end(); ++item)
        {
            if(item.value().contains(lineEditText))
            {
                auto listItem = new QListWidgetItem;
                listItem->setIcon(QIcon(files.value(item.key())));
                listItem->setText(item.key());
                ui->listWidget->addItem(listItem);
            }
        }
    }

    else if(lineEditText != "")
    {
        ui->listWidget->clear();
        QMap<QString,QString>::iterator item;
        for (item = files.begin(); item != files.end(); ++item)
        {
            if(item.key().contains(lineEditText))
            {
                auto listItem = new QListWidgetItem;
                listItem->setIcon(QIcon(files.value(item.key())));
                listItem->setText(item.key());
                ui->listWidget->addItem(listItem);
            }
        }
    }
}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    if (arg1 == "")
    {
        ui->listWidget->clear();
        QMap<QString, QString>::iterator item;
        for (item = files.begin(); item != files.end(); ++item)
       {
           auto listItem = new QListWidgetItem;
            listItem->setIcon(QIcon(item.value()));
            listItem->setText(item.key());
            ui->listWidget->addItem(listItem);
        }
    }
}


void MainWindow::on_descriptioncheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->checkBox->setCheckable(false);
    }
    else if(arg1 == Qt::Unchecked)
    {
        ui->checkBox->setCheckable(true);
    }
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->descriptioncheckBox->setCheckable(false);
    }
    else if(arg1 == Qt::Unchecked)
    {
        ui->descriptioncheckBox->setCheckable(true);
    }
}

