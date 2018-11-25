#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>
#include <QSqlRecord>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("school");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("9339");


    if(!db.open()){
        qDebug() << "Error: " << db.lastError().text();
    }
    
    else{
        qDebug() << "Success";
    }

    ui->classTable->hide();
    ui->backtoTables->hide();
    ui->groupBox->hide();

    showSchoolClasses();

    ui->markedit->setValidator(new QIntValidator(0, 100, this));
    ui->nameedit->setValidator(new QRegExpValidator(QRegExp("[А-Яа-я]{0,15}"), this ));
    ui->secondnameedit->setValidator(new QRegExpValidator(QRegExp("[А-Яа-я]{0,15}"), this ));

    ui->AddStudent->setEnabled(false);

    connect(ui->nameedit, SIGNAL(textChanged(const QString &) ), this, SLOT(enable_AddButton()));
    connect(ui->secondnameedit, SIGNAL(textChanged(const QString &) ), this, SLOT(enable_AddButton()));
    connect(ui->markedit, SIGNAL(textChanged(const QString &) ), this, SLOT(enable_AddButton()));
    connect(ui->backtoTables ,SIGNAL(clicked(bool)) ,this ,SLOT(on_backtoTables_clicked()));
}


void MainWindow::showSchoolClasses(){

    dataBaseTables = true;
    ui->AddButton->setText("Добавить класс");
    ui->classTable->hide();
    ui->databaseTables->show();
    int rowcount = 0;
    QStringList table_list =  db.tables();
    tables = new QStandardItemModel(table_list.size(), 1, this);
    for(int i = 0; i < table_list.size(); i++){
        tableName = new QStandardItem(table_list[i]);
        tables->setItem(rowcount, tableName);
        rowcount++;
    }

    ui->databaseTables->setModel(tables);
    connect(ui->databaseTables, SIGNAL(pressed(const QModelIndex &)), this, SLOT(onItemPressed(const QModelIndex &)));
}


void MainWindow::on_AddButton_clicked(){

}


void MainWindow::on_backtoTables_clicked(){
    ui->backtoTables->hide();
    showSchoolClasses();
}


void MainWindow::on_Submit_clicked(){
    model->submitAll();
}



void MainWindow::on_AddStudent_clicked(){
    QSqlQuery query;
    query.prepare("INSERT INTO "+classTable+" (Name,Secondname, Midmark) "
                  "VALUES (:Name, :Secondname, :Midmark)");


    query.addBindValue(ui->nameedit->text() );
    query.addBindValue(ui->secondnameedit->text());
    query.addBindValue(ui->markedit->text());
    query.exec();

    ui->nameedit->clear();
    ui->secondnameedit->clear();
    ui->markedit->clear();
    ui->classTable->setModel(model);
    ((QSqlTableModel*)ui->classTable->model())->select();
}


void MainWindow::enable_AddButton(){
    if(ui->nameedit->text().isEmpty() ||
       ui->secondnameedit->text().isEmpty() ||
       ui->markedit->text().isEmpty()){

        ui->AddStudent->setEnabled(false);
    }

    else{
        ui->AddStudent->setEnabled(true);
    }
}


void MainWindow::on_Delete_clicked(){
    if(dataBaseTables == false){
        model->removeRow(ui->classTable->currentIndex().row());
        model->submitAll();
    }
}


void MainWindow::onItemPressed(const QModelIndex &index){
    dataBaseTables = false;
    ui->groupBox->show();
    ui->databaseTables->hide();
    ui->AddButton->hide();
    int row = index.row();
    classTable = index.sibling(row, 0).data().toString();
    model = new QSqlTableModel(this, db);
    ui->classTable->setModel(model);
    model->setTable(classTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->classTable->show();
    ui->backtoTables->show();
    connect(ui->AddStudent, SIGNAL(clicked(bool)), this, SLOT(on_AddStudent_clicked()));
}


void MainWindow::on_Exit_clicked(){
    this->close();
}


MainWindow::~MainWindow(){
    delete ui;
}

