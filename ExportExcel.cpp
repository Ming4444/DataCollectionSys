#include "ExportExcel.h"

exportGoodExcel::exportGoodExcel(QSqlTableModel *model)
{
    myModel = model;
}

exportGoodExcel::~exportGoodExcel()
{

}

void exportGoodExcel::run()
{
    QFile excel(filepath);
    if(!excel.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        isFail = true;
        exit();
    }
    QTextStream output(&excel);
    output.setAutoDetectUnicode(true);//显示汉字
    int rowCount = myModel->rowCount();
    int colCount = myModel->columnCount();
    QSqlRecord record;
    QString header = "", data = "", temp = "";
    //获取表头
    for(int i = 0; i< colCount; i++)
    {
        temp = myModel->headerData(i,Qt::Orientation::Horizontal).toString();
        if(i == colCount-1)
            header = header + temp + '\n';
        else
            header = header + temp + ',';
    }
    output<<header;
    //获取数据
    for(int i = 0; i < rowCount; i++)
    {
        record = myModel->record(i);
        for(int j = 0; j< colCount; j++)
        {
            temp = record.value(j).toString();
            if(j == colCount-1)
                data = data + temp + '\n';
            else
                data = data + temp + ',';
        }
    }
    output<<data;
    excel.close();
    isFail = false;
    exit();
}

exportMoneyExcel::exportMoneyExcel(QSqlTableModel *model)
{
    myModel = model;
}

exportMoneyExcel::~exportMoneyExcel()
{

}

void exportMoneyExcel::run()
{
    QFile excel(filepath);
    if(!excel.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        isFail = true;
        exit();
    }
    QTextStream output(&excel);
    output.setAutoDetectUnicode(true);//显示汉字
    int rowCount = myModel->rowCount();
    int colCount = myModel->columnCount();
    QSqlRecord record;
    QString header = "", data = "", temp = "";
    //获取表头
    for(int i = 0; i< colCount; i++)
    {
        temp = myModel->headerData(i,Qt::Orientation::Horizontal).toString();
        if(i == colCount-1)
            header = header + temp + '\n';
        else
            header = header + temp + ',';
    }
    output<<header;
    //获取数据
    for(int i = 0; i < rowCount; i++)
    {
        record = myModel->record(i);
        for(int j = 0; j< colCount; j++)
        {
            temp = record.value(j).toString();
            if(j == colCount-1)
                data = data + temp + '\n';
            else
                data = data + temp + ',';
        }
    }
    output<<data;
    excel.close();
    isFail = false;
    exit();
}

exportProfitExcel::exportProfitExcel(QSqlTableModel *model)
{
    myModel = model;
}

exportProfitExcel::~exportProfitExcel()
{

}

void exportProfitExcel::run()
{
    QFile excel(filepath);
    if(!excel.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        isFail = true;
        exit();
    }
    QTextStream output(&excel);
    output.setAutoDetectUnicode(true);//显示汉字
    int rowCount = myModel->rowCount();
    int colCount = myModel->columnCount();
    QSqlRecord record;
    QString header = "", data = "", temp = "";
    //获取表头
    for(int i = 0; i< colCount; i++)
    {
        temp = myModel->headerData(i,Qt::Orientation::Horizontal).toString();
        if(i == colCount-1)
            header = header + temp + '\n';
        else
            header = header + temp + ',';
    }
    output<<header;
    //获取数据
    for(int i = 0; i < rowCount; i++)
    {
        record = myModel->record(i);
        for(int j = 0; j< colCount; j++)
        {
            temp = record.value(j).toString();
            if(j == colCount-1)
                data = data + temp + '\n';
            else
                data = data + temp + ',';
        }
    }
    output<<data;
    excel.close();
    isFail = false;
    exit();
}

