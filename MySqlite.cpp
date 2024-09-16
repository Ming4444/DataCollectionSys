#include "Headerfiles.h"
#include "MySqlite.h"

mySqlite::mySqlite()
{

}

mySqlite::~mySqlite()
{
    delete query;
}

bool mySqlite::connectDB(const char *DBname)
{
    myDataBase = QSqlDatabase::addDatabase("QSQLITE");
    myDataBase.setDatabaseName(DBname);
    if(!myDataBase.open())
        return false;
    return true;
}

bool mySqlite::initSQL(const char *DBname)
{
    if(!connectDB(DBname))
        return false;
    else if (!createTable())
        return false;
    return true;
}

bool mySqlite::createTable()
{
    query = new QSqlQuery;
    QString passwordStr = "create table if not exists password("
                          "[id] integer primary key,"
                          "[password] varchar(30)"
                          ")";
    QString goodsStr = "create table if not exists goods("
                      "[id] integer primary key,"
                      "[date] varchar(30),"
                      "[name] varchar(30),"
                      "[inputquantity] varchar(30),"
                      "[inputunitprice] varchar(30),"
                      "[inputprice] varchar(30),"
                      "[outputquantity] varchar(30),"
                      "[outputunitprice] varchar(30),"
                      "[outputprice] varchar(30),"
                      "[unit] varchar(30),"
                      "[produceDate] varchar(30),"
                      "[validDate] varchar(30),"
                      "[handler] varchar(30),"
                      "[remark] varchar(30)"
                      ")";
    QString moneyStr = "create table if not exists money("
                          "[id] integer primary key,"
                          "[date] varchar(30),"
                          "[payname] varchar(30),"
                          "[payunitmoney] varchar(30),"
                          "[paymoney] varchar(30),"
                          "[incomename] varchar(30),"
                          "[incomeunitmoney] varchar(30),"
                          "[incomemoney] varchar(30),"
                          "[handler] varchar(30),"
                          "[remark] varchar(30)"
                          ")";
    QString stockStr = "create table if not exists stock("
                          "[id] integer primary key,"
                          "[name] varchar(30),"
                          "[stock] float,"
                          "[unit] varchar(30)"
                          ")";
    QString profitStr = "create table if not exists profit("
                          "[id] integer primary key,"
                          "[date] varchar(30),"
                          "[income] float,"
                          "[expenditure] float,"
                          "[profit] float"
                          ")";
    if(!(query->exec(passwordStr) && query->exec(goodsStr) && query->exec(moneyStr) && query->exec(stockStr) && query->exec(profitStr)))
        return false;
    return true;
}
