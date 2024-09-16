#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Headerfiles.h"
#include "MySqlite.h"
#include "FunctionButton.h"
#include "ExportExcel.h"
#include "HelpDocument.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool loginFlag = false;
    bool initFlag = false;
    int login_type = 0;//0-登录 1-修改密码
    float m_profit = 0;
    float m_income = 0;
    float m_expend = 0;

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    void createNavigationButton();
    bool dataBaseInit();
    void openHelpDocument();
    void copyBackUpHelpDocument();
    void applicationInit();
    void createSqlView();
    void stockCheck();//检查物品实际库存与出入库显示库存是否对应
    void profitInit();
    void profitWrite();

    passWord *pwd = nullptr;
    help *helpdoc = nullptr;
    QAction *helpBtn = nullptr;
    QAction *goodsOfstock = nullptr;
    QAction *store = nullptr;
    QAction *money = nullptr;
    QAction *profit = nullptr;
    addGoods *addG = nullptr;
    fetchGoods *fetG = nullptr;
    modifyGoods *mdfyG = nullptr;
    deleteGoods *delG = nullptr;
    addIncomeMoney *addI = nullptr;
    addExpendMoney *addE = nullptr;
    modifyMoney *mdfyM = nullptr;
    mySqlite *mysql = nullptr;
    QSqlTableModel *password_sqlModel = nullptr;
    QSqlTableModel *good_sqlModel = nullptr;
    QSqlTableModel *money_sqlModel = nullptr;
    QSqlTableModel *stock_sqlModel = nullptr;
    QSqlTableModel *profit_sqlModel = nullptr;
    exportGoodExcel *good_thread = nullptr;
    exportMoneyExcel *money_thread = nullptr;
    exportProfitExcel *profit_thread = nullptr;
    float temp_stock_number = 0; //用来存放stock页中的stock变量以供修改使用
    //QString temp_stock_name;//用来存放stock页中的name变量以供修改使用
    QString old_date;
    QString old_name;
    float old_input_number;
    float old_input_price;
    float old_output_number;
    float old_output_unit_price;
    QString old_unit;
    QString old_handler;
    QString old_remark;

private slots:
    void loginFunction();
    void passwordFunction();
    void switchPages();
    void manageGoods();
    void insertAddGoodSQL();
    void insertFetchGoodSQL();
    void modifyGoodSQL();
    void deleteGoodSQL();
    void searchGoods();
    void sortGoods(int,Qt::SortOrder);
    void exportGoodSuccess();
    void manageMoney();
    void insertAddIncomeSQL();
    void insertAddExpendSQL();
    void modifyMoneySQL();
    void searchMoney();
    void sortMoney(int,Qt::SortOrder);
    void exportMoneySuccess();
    void manageStock();
    void searchStock();
    void stockRecordDelete();
    void profitExcel();
    void createProfit();
    void profitShow(QString);
    void computeProfit();
    void exportProfitSuccess();

signals:
    void loginSuccess();
};

#endif // MAINWINDOW_H
