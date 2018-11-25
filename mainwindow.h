#pragma once

#include <QString>
#include <QWidget>
#include <qDebug>
#include <QSql>
#include <QString>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QStandardItemModel>
//#include <QStandardItem>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel* model;
    QStandardItemModel* tables;
    QStandardItem* tableName;
    QList<QString*> name;
    QString classTable;
    bool dataBaseTables;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void showSchoolClasses();

private slots:
    void on_Submit_clicked();
    void on_AddStudent_clicked();
    void on_Delete_clicked();
    void on_Exit_clicked();
    void enable_AddButton();
    void onItemPressed(const QModelIndex &index);
    void on_backtoTables_clicked();
    void on_AddButton_clicked();
};


