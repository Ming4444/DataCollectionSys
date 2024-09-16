#ifndef MYSQLITE_H
#define MYSQLITE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

class mySqlite
{
public:
    mySqlite();
    ~mySqlite();
    bool initSQL(const char *);
    bool connectDB(const char *);
    bool createTable();

    QSqlDatabase myDataBase;

private:
    QSqlQuery *query = nullptr;
};

#endif // MYSQLITE_H
