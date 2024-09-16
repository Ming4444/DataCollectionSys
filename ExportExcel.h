#ifndef EXPORTEXCEL_H
#define EXPORTEXCEL_H

#include "Headerfiles.h"
#include "MySqlite.h"
#include <QThread>

class exportGoodExcel:public QThread
{
public:
    exportGoodExcel(QSqlTableModel *model);
    ~exportGoodExcel();
    bool isFail = false;

protected:
    void run() override;

private:
    QSqlTableModel *myModel = nullptr;
};

class exportMoneyExcel:public QThread
{
public:
    exportMoneyExcel(QSqlTableModel *model);
    ~exportMoneyExcel();
    bool isFail = false;

protected:
    void run() override;

private:
    QSqlTableModel *myModel = nullptr;
};

class exportProfitExcel:public QThread
{
public:
    exportProfitExcel(QSqlTableModel *model);
    ~exportProfitExcel();
    bool isFail = false;

protected:
    void run() override;

private:
    QSqlTableModel *myModel = nullptr;
};

#endif // EXPORTEXCEL_H
