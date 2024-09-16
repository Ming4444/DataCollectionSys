#include "mainwindow.h"
#include "ui_mainwindow.h"

QString filepath = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    if(dataBaseInit())
    {
        ui->setupUi(this);
        setWindowTitle("悦来素生活小馆");
        setWindowIcon(QIcon(":/logo.png"));
        applicationInit();
        ui->pages->setCurrentIndex(0);//主页
        initFlag = true;
    }
}

MainWindow::~MainWindow()
{
    //释放动态分配内存
    delete store;
    delete money;
    delete ui;
    delete mysql;
    delete good_sqlModel;
    delete money_sqlModel;
    delete stock_sqlModel;
    delete addG;
    delete mdfyG;
    delete delG;
    delete fetG;
    delete addI;
    delete addE;
    delete mdfyM;
    delete good_thread;
    delete money_thread;
}

void MainWindow::closeEvent(QCloseEvent*)
{
    //关闭时备份数据库文件
    QDir backupDir;
    if(backupDir.mkpath("C:/SqliteDB"))
    {
        QFile dbFile("mysms.db");
        if(!dbFile.copy("C:/SqliteDB/mysmsBackup"+QString::number(QDateTime::currentDateTime().date().day()%5)+".db"))
        {
            QFile backupFile("C:/SqliteDB/mysmsBackup"+QString::number(QDateTime::currentDateTime().date().day()%5)+".db");
            if(backupFile.remove("C:/SqliteDB/mysmsBackup"+QString::number(QDateTime::currentDateTime().date().day()%5)+".db"))
            {
                if(!dbFile.copy("C:/SqliteDB/mysmsBackup"+QString::number(QDateTime::currentDateTime().date().day()%5)+".db"))
                {
                    QMessageBox msg(QMessageBox::Warning,"提示","数据库备份失败,请联系管理员");
                    msg.exec();
                }
            }
            else
            {
                QMessageBox msg(QMessageBox::Warning,"提示","数据库备份失败,请联系管理员");
                msg.exec();
            }
        }
        //备份说明文档
        QDir folder;
        QDir html_image_dir(QDir::currentPath() + "/img");
        QFile html_file(QDir::currentPath() + "/help.html");
        html_image_dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
        QStringList filelist = html_image_dir.entryList();
        if(folder.mkpath("C:/SqliteDB/img"))
        {
            html_file.copy("C:/SqliteDB/help.html");//获取备份html文件
            //获取html对应图片（存放再img文件夹）
            for(auto &filename : filelist)
            {
                QFile file(QDir::currentPath() + "/img/" + filename);
                file.copy("C:/SqliteDB/img/" + filename);
            }
        }
    }
    else
    {
        QMessageBox msg(QMessageBox::Warning,"提示","数据库备份失败,请联系管理员");
        msg.exec();
    }
}

bool MainWindow::dataBaseInit()
{
    //获取离今天最近的备份数据库
    QString filePath;
    QDir dir("C:/SqliteDB");
    QStringList filters("*.db");
    dir.setNameFilters(filters);
    QList<QFileInfo>fileInfo = (QList<QFileInfo>(dir.entryInfoList()));
    QList<QDateTime>dateTime;
    for(int i=0;i<fileInfo.size();i++)
    {
        dateTime<<fileInfo.at(i).lastModified();
    }
    std::sort(dateTime.begin(),dateTime.end());
    for(int i=0;i<fileInfo.size();i++)
    {
        if(fileInfo.at(i).lastModified() == dateTime[fileInfo.size()-1])
        {
            filePath = fileInfo.at(i).absoluteFilePath();
        }
    }
    mysql = new mySqlite;
    QFile backupFile(filePath); //判断备份数据库是否存在C盘SqliteDB文件夹
    QFile dbFile("mysms.db"); //判断当前路径是否有文件
    if(!dbFile.exists())
    {
        //备份文件存在则把备份文件复制出来, 否则新创建一个db文件
        if(backupFile.exists())
        {
            if(backupFile.copy("mysms.db"))
            {
                if(!mysql->connectDB("mysms.db"))
                {
                    QMessageBox msg(QMessageBox::Warning,"提示","数据库连接失败,请联系管理员");
                    msg.exec();
                    return false;
                }
                else return true;
            }
            else
            {
                QMessageBox msg(QMessageBox::Warning,"提示","备份数据库获取失败,请联系管理员");
                msg.exec();
                return false;
            }
        }
        else
        {
            QMessageBox msg(this);
            int ret = msg.warning(this,"警告","数据库配置失败,是否初始化",QMessageBox::Yes|QMessageBox::No);
            if(ret == QMessageBox::Yes)
            {
                if(!mysql->initSQL("mysms.db"))
                {
                    msg.warning(this,"警告","数据库初始化失败,请联系管理员");
                    return false;
                }
                else return true;
            }
            else
                return false;
        }
    }
    else
    {
        if(!mysql->connectDB("mysms.db"))
        {
            QMessageBox msg(QMessageBox::Warning,"提示","数据库连接失败,请联系管理员");
            msg.exec();
            return false;
        }
        else return true;
    }
}

void MainWindow::openHelpDocument()
{
    helpdoc = new help;
    helpdoc->show();
}

void MainWindow::copyBackUpHelpDocument()
{
    QDir folder;
    QDir html_image_dir("C:/SqliteDB/img");
    QFile html_file("C:/SqliteDB/help.html");
    html_image_dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    QStringList filelist = html_image_dir.entryList();
    if(folder.mkpath(QDir::currentPath() + "/img"))
    {
        html_file.copy("help.html");//获取备份html文件
        //获取html对应图片（存放再img文件夹）
        for(auto &filename : filelist)
        {
            QFile file("C:/SqliteDB/img/" + filename);
            file.copy(QDir::currentPath() + "/img/" + filename);
        }
    }
}

void MainWindow::applicationInit()
{
    //创建说明文档页面
    copyBackUpHelpDocument();
    helpBtn = new QAction("使用说明",this);
    helpBtn->setFont(QFont("Song",12));
    ui->menubar->addAction(helpBtn);
    //创建功能按钮弹出页面
    addG = new addGoods;
    mdfyG = new modifyGoods;
    fetG = new fetchGoods;
    delG = new deleteGoods;
    addI = new addIncomeMoney;
    addE = new addExpendMoney;
    mdfyM = new modifyMoney;
    createNavigationButton();
    createSqlView();
    //按钮链接
    connect(helpBtn,&QAction::triggered,this,&MainWindow::openHelpDocument);
    connect(goodsOfstock,&QAction::triggered,this,&MainWindow::switchPages);
    connect(store,&QAction::triggered,this,&MainWindow::switchPages);
    connect(money,&QAction::triggered,this,&MainWindow::switchPages);
    connect(profit,&QAction::triggered,this,&MainWindow::switchPages);
    connect(addG->confirm,&QPushButton::clicked,this,&MainWindow::insertAddGoodSQL);
    connect(mdfyG->confirm,&QPushButton::clicked,this,&MainWindow::modifyGoodSQL);
    connect(fetG->confirm,&QPushButton::clicked,this,&MainWindow::insertFetchGoodSQL);
    connect(delG->confirm,&QPushButton::clicked,this,&MainWindow::deleteGoodSQL);
    connect(addI->confirm,&QPushButton::clicked,this,&MainWindow::insertAddIncomeSQL);
    connect(addE->confirm,&QPushButton::clicked,this,&MainWindow::insertAddExpendSQL);
    connect(mdfyM->confirm,&QPushButton::clicked,this,&MainWindow::modifyMoneySQL);
    //创建多线程导出表格
    good_thread = new exportGoodExcel(good_sqlModel);
    money_thread = new exportMoneyExcel(money_sqlModel);
    profit_thread = new exportProfitExcel(profit_sqlModel);
    connect(good_thread,&QThread::finished,this,&MainWindow::exportGoodSuccess);
    connect(money_thread,&QThread::finished,this,&MainWindow::exportMoneySuccess);
    connect(profit_thread,&QThread::finished,this,&MainWindow::exportProfitSuccess);
}

void MainWindow::loginFunction()
{
    pwd = new passWord;
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn->text() == "登录")
    {
        login_type = 0;
        connect(pwd->confirm,&QPushButton::clicked,this,&MainWindow::passwordFunction);
        pwd->setWindowTitle("登录");
        pwd->input_password->setEnabled(true);
        pwd->change_password_new->setEnabled(false);
        pwd->change_password_origin->setEnabled(false);
        pwd->change_password_confirm_again->setEnabled(false);
        pwd->exec();
    }
    else if(btn->text() == "修改密码")
    {
        if(!loginFlag)
        {
            QMessageBox msg(QMessageBox::Information,"提示","请先登录！");
            msg.setFont(QFont("Song",12));
            msg.exec();
            return;
        }
        login_type = 1;
        pwd->input_password->setEnabled(false);
        connect(pwd->confirm,&QPushButton::clicked,this,&MainWindow::passwordFunction);
        pwd->setWindowTitle("修改密码");
        pwd->input_password->setEnabled(false);
        pwd->change_password_new->setEnabled(true);
        pwd->change_password_origin->setEnabled(true);
        pwd->change_password_confirm_again->setEnabled(true);
        pwd->exec();
        //修改密码
        QString newPassword = pwd->change_password_confirm_again->text();
    }
    delete pwd;
}

void MainWindow::passwordFunction()
{
    QSqlRecord record = password_sqlModel->record(0);
    if(login_type == 0)
    {
        if(record.value("password").toString() == pwd->input_password->text())
        {
            QMessageBox msg(QMessageBox::Information,"提示","登录成功！");
            pwd->close();
            msg.exec();
            ui->toolBar->setEnabled(true);//登录成功则允许使用
            loginFlag = true;
            ui->login->setEnabled(false);
        }
        else
        {
            QMessageBox msg(QMessageBox::Information,"提示","密码错误！");
            msg.exec();
            return;
        }
    }
    else if(login_type == 1)
    {
        QString origin_pwd = pwd->change_password_origin->text();
        QString new_pwd = pwd->change_password_new->text();
        QString confirm_pwd = pwd->change_password_confirm_again->text();
        if(origin_pwd=="" || new_pwd=="" || confirm_pwd=="")
        {
            QMessageBox msg(QMessageBox::Information,"提示","请输入内容");
            if(origin_pwd=="") msg.setText("请输入原密码");
            else if(origin_pwd=="") msg.setText("请输入新密码");
            else msg.setText("请输入再次输入新密码");
            msg.exec();
            return;
        }
        else if(origin_pwd != record.value("password").toString())
        {
            QMessageBox msg(QMessageBox::Information,"提示","原密码错误");
            msg.exec();
            return;
        }
        else if(origin_pwd == new_pwd)
        {
            QMessageBox msg(QMessageBox::Information,"提示","新密码不能与原密码相同");
            msg.exec();
            return;
        }
        else if(new_pwd != confirm_pwd)
        {
            QMessageBox msg(QMessageBox::Information,"提示","新密码两次输入不同");
            msg.exec();
            return;
        }
        else
        {
            record.setValue("password",confirm_pwd);
            password_sqlModel->setRecord(0,record);
            password_sqlModel->submitAll();
            QMessageBox msg(QMessageBox::Information,"提示","密码修改成功");
            pwd->close();
            msg.exec();
            ui->change_password->setEnabled(false);
        }
    }
}

void MainWindow::createNavigationButton()
{
    //成功login前禁止使用导航栏
    ui->toolBar->setEnabled(false);
    //设置格式
    QFont word("Song",13);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar->setIconSize(QSize(40,40));
    ui->toolBar->layout()->setSpacing(50);
    //现货按钮
    goodsOfstock = new QAction("现货\n记录",this);
    goodsOfstock->setFont(word);
    goodsOfstock->setIcon(QIcon(":/icon/stock.png"));
    ui->toolBar->addAction(goodsOfstock);
    //仓库按钮
    store = new QAction("出入\n明细",this);
    store->setFont(word);
    store->setIcon(QIcon(":/icon/goods.png"));
    ui->toolBar->addAction(store);
    //收支按钮
    money = new QAction("收支\n明细",this);
    money->setFont(word);
    money->setIcon(QIcon(":/icon/income.png"));
    ui->toolBar->addAction(money);
    //盈利情况
    profit = new QAction("盈利\n报表",this);
    profit->setFont(word);
    profit->setIcon((QIcon(":/icon/profit.png")));
    ui->toolBar->addAction(profit);
    ui->year_comboBox->setMaxVisibleItems(10);
    ui->month_comboBox->setMaxVisibleItems(8);
    ui->year_comboBox->setEditable(true);
    ui->month_comboBox->setEditable(true);
    ui->year_comboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    ui->month_comboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    for (int i=2023; i<=2050; i++)
    {
        if(i==2023) ui->year_comboBox->addItem("");
        ui->year_comboBox->addItem(QString::number(i));
    }
    for (int i=1; i<=12; i++)
    {
        if(i==1) ui->month_comboBox->addItem("");
        ui->month_comboBox->addItem(QString("%1").arg(i,2,10,QLatin1Char('0')));
    }
    connect(ui->year_comboBox,&QComboBox::currentTextChanged,this,&MainWindow::profitShow);
    connect(ui->month_comboBox,&QComboBox::currentTextChanged,this,&MainWindow::profitShow);
}

void MainWindow::createSqlView()
{
    //SQL密码页设置
    password_sqlModel = new QSqlTableModel(this,mysql->myDataBase);
    password_sqlModel->setTable("password");
    password_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置保存策略为手动提交
    password_sqlModel->setHeaderData(0,Qt::Horizontal, "序号");
    password_sqlModel->setHeaderData(1,Qt::Horizontal, "密码");
    password_sqlModel->select();
    if(password_sqlModel->rowCount() == 0) //第一次创建数据库时初始化密码
    {
        QSqlRecord pwd_record = password_sqlModel->record();
        pwd_record.setValue("password",QString("123456"));
        password_sqlModel->insertRecord(0,pwd_record);
        password_sqlModel->submitAll();
    }
    //SQL库存页设置
    good_sqlModel = new QSqlTableModel(this,mysql->myDataBase);
    good_sqlModel->setTable("goods");//选择数据表
    good_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置保存策略为手动提交
    good_sqlModel->setHeaderData(0,Qt::Horizontal, "序号");
    good_sqlModel->setHeaderData(1,Qt::Horizontal, "出入库日期");
    good_sqlModel->setHeaderData(2,Qt::Horizontal, "名称");
    good_sqlModel->setHeaderData(3,Qt::Horizontal, "入库数量");
    good_sqlModel->setHeaderData(4,Qt::Horizontal, "入库单价");
    good_sqlModel->setHeaderData(5,Qt::Horizontal, "入库总价");
    good_sqlModel->setHeaderData(6,Qt::Horizontal, "出库数量");
    good_sqlModel->setHeaderData(7,Qt::Horizontal, "出库单价");
    good_sqlModel->setHeaderData(8,Qt::Horizontal, "出库总价");
    good_sqlModel->setHeaderData(9,Qt::Horizontal, "单位");
    good_sqlModel->setHeaderData(10,Qt::Horizontal, "生产日期");
    good_sqlModel->setHeaderData(11,Qt::Horizontal, "有效日期");
    good_sqlModel->setHeaderData(12,Qt::Horizontal, "经手人");
    good_sqlModel->setHeaderData(13,Qt::Horizontal, "备注");
    good_sqlModel->select(); //选取整个表的所有行
    ui->tableView_good->setFont(QFont("Song",12));
    ui->tableView_good->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView_good->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    ui->tableView_good->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_good->setModel(good_sqlModel);
    ui->tableView_good->horizontalHeader()->resizeSection(1,105);
    ui->tableView_good->horizontalHeader()->resizeSection(2,205);
    ui->tableView_good->horizontalHeader()->resizeSection(3,80);
    ui->tableView_good->horizontalHeader()->resizeSection(4,80);
    ui->tableView_good->horizontalHeader()->resizeSection(5,85);
    ui->tableView_good->horizontalHeader()->resizeSection(6,80);
    ui->tableView_good->horizontalHeader()->resizeSection(7,80);
    ui->tableView_good->horizontalHeader()->resizeSection(8,85);
    ui->tableView_good->horizontalHeader()->resizeSection(9,50);
    ui->tableView_good->horizontalHeader()->resizeSection(10,105);
    ui->tableView_good->horizontalHeader()->resizeSection(11,105);
    ui->tableView_good->horizontalHeader()->resizeSection(12,70);
    ui->tableView_good->horizontalHeader()->resizeSection(13,800);
    ui->tableView_good->hideColumn(0);
    ui->tableView_good->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableView_good->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&MainWindow::sortGoods);
    //SQL收支页设置
    money_sqlModel = new QSqlTableModel(this,mysql->myDataBase);
    money_sqlModel->setTable("money");
    money_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    money_sqlModel->setHeaderData(0,Qt::Horizontal, "序号");
    money_sqlModel->setHeaderData(1,Qt::Horizontal, "日期");
    money_sqlModel->setHeaderData(2,Qt::Horizontal, "支出项目");
    money_sqlModel->setHeaderData(3,Qt::Horizontal, "支出单额");
    money_sqlModel->setHeaderData(4,Qt::Horizontal, "支出总额");
    money_sqlModel->setHeaderData(5,Qt::Horizontal, "收入项目");
    money_sqlModel->setHeaderData(6,Qt::Horizontal, "收入单额");
    money_sqlModel->setHeaderData(7,Qt::Horizontal, "收入总额");
    money_sqlModel->setHeaderData(8,Qt::Horizontal, "经手人");
    money_sqlModel->setHeaderData(9,Qt::Horizontal, "备注");
    money_sqlModel->select();
    ui->tableView_money->setFont(QFont("Song",12));
    ui->tableView_money->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView_money->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    ui->tableView_money->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_money->setModel(money_sqlModel);
    ui->tableView_money->horizontalHeader()->resizeSection(1,105);
    ui->tableView_money->horizontalHeader()->resizeSection(2,270);
    ui->tableView_money->horizontalHeader()->resizeSection(3,85);
    ui->tableView_money->horizontalHeader()->resizeSection(4,85);
    ui->tableView_money->horizontalHeader()->resizeSection(5,270);
    ui->tableView_money->horizontalHeader()->resizeSection(6,85);
    ui->tableView_money->horizontalHeader()->resizeSection(7,85);
    ui->tableView_money->horizontalHeader()->resizeSection(8,70);
    ui->tableView_money->horizontalHeader()->resizeSection(9,800);
    ui->tableView_money->hideColumn(0);
    ui->tableView_money->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableView_money->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&MainWindow::sortMoney);
    //SQL Stock页设置
    stock_sqlModel = new QSqlTableModel(this,mysql->myDataBase);
    stock_sqlModel->setTable("stock");
    stock_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stock_sqlModel->setHeaderData(0,Qt::Horizontal,"序号");
    stock_sqlModel->setHeaderData(1,Qt::Horizontal,"现货名称");
    stock_sqlModel->setHeaderData(2,Qt::Horizontal,"现货数量");
    stock_sqlModel->setHeaderData(3,Qt::Horizontal,"单位");
    stock_sqlModel->select();
    ui->tableView_stock->setFont(QFont("Song",12));
    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    ui->tableView_stock->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_stock->setModel(stock_sqlModel);
    ui->tableView_stock->horizontalHeader()->resizeSection(1,300);
    ui->tableView_stock->horizontalHeader()->resizeSection(2,150);
    ui->tableView_stock->hideColumn(0);
    ui->tableView_stock->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //SQL盈利页设置
    profit_sqlModel = new QSqlTableModel(this,mysql->myDataBase);
    profit_sqlModel->setTable("profit");
    profit_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    profit_sqlModel->setHeaderData(0,Qt::Horizontal,"序号");
    profit_sqlModel->setHeaderData(1,Qt::Horizontal,"日期");
    profit_sqlModel->setHeaderData(2,Qt::Horizontal,"收入(元)");
    profit_sqlModel->setHeaderData(3,Qt::Horizontal,"支出(元)");
    profit_sqlModel->setHeaderData(4,Qt::Horizontal,"盈利(元)");
    profit_sqlModel->select();
    ui->tableView_profit->setFont(QFont("Song",12));
    ui->tableView_profit->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableView_profit->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_profit->setModel(profit_sqlModel);
    ui->tableView_profit->hideColumn(0);
    ui->tableView_profit->setEditTriggers(QAbstractItemView::NoEditTriggers);
    profitInit();
}

void MainWindow::profitInit()
{
    //每月创建一次这个月的利润表
    profit_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(QDate::currentDate().toString("yyyy")).arg(QDate::currentDate().toString("MM")));
    profit_sqlModel->select();
    if(profit_sqlModel->rowCount()==0)
    {
        int start = 0;
        QSqlRecord create_record = profit_sqlModel->record();
        QCalendar currentDate(QCalendar::System::Gregorian);//当月对应天数
        for(int i=1; i<=QDate::currentDate().daysInMonth(currentDate); i++)
        {
           QString date = QString("%1-%2-%3").arg(QDate::currentDate().toString("yyyy")).arg(QDate::currentDate().toString("MM")).arg(i,2,10,QLatin1Char('0'));
           create_record.setValue("date",date);
           create_record.setValue("income",0.1);
           create_record.setValue("expenditure",0.1);
           create_record.setValue("profit",0.1);
           profit_sqlModel->insertRecord(start,create_record);
           start++;
        }
        profit_sqlModel->submitAll();
    }
}

void MainWindow::profitWrite()
{
    QString currentMonth = QDate::currentDate().toString("yyyy-MM");
    money_sqlModel->setFilter(QString("date like '%%1%'").arg(currentMonth));
    money_sqlModel->select();
    QSqlRecord money_record;
    QSqlRecord profit_record;
    QCalendar currentDate(QCalendar::System::Gregorian);
    float d_profit = 0, d_income = 0, d_expend = 0;//d=day
    for(int i=1; i<=QDate::currentDate().daysInMonth(currentDate); i++)
    {
        d_profit = 0;
        d_income = 0;
        d_expend = 0;
        money_sqlModel->setFilter(QString("date = '%1-%2-%3'").arg(QDate::currentDate().toString("yyyy")).arg(QDate::currentDate().toString("MM")).arg(i,2,10,QLatin1Char('0')));
        money_sqlModel->select();
        profit_sqlModel->setFilter(QString("date = '%1-%2-%3'").arg(QDate::currentDate().toString("yyyy")).arg(QDate::currentDate().toString("MM")).arg(i,2,10,QLatin1Char('0')));
        profit_sqlModel->select();
        for(int i=0; i<money_sqlModel->rowCount(); i++)
        {
            money_record = money_sqlModel->record(i);
            d_income += money_record.value("incomemoney").toFloat();
            d_expend += money_record.value("paymoney").toFloat();
        }
        d_profit = d_income - d_expend;
        profit_record = profit_sqlModel->record(0);
        profit_record.setValue("profit",d_profit);
        profit_record.setValue("income",d_income);
        profit_record.setValue("expenditure",d_expend);
        profit_sqlModel->setRecord(0,profit_record);
        profit_sqlModel->submitAll();
        m_profit += d_profit;
        m_income += d_income;
        m_expend += d_expend;
    }
    //重置筛选结果
    money_sqlModel->setFilter("");
    money_sqlModel->select();
    QString year = ui->year_comboBox->currentText();
    QString month = ui->month_comboBox->currentText();
    profit_sqlModel->sort(1,Qt::AscendingOrder);
    profit_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(year).arg(month));
    profit_sqlModel->select();
}

void MainWindow::switchPages()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action->text()=="现货\n记录")
    {
        stock_sqlModel->setFilter("");
        stock_sqlModel->select();
        ui->pages->setCurrentIndex(1);
    }
    else if(action->text()=="出入\n明细")
    {
        ui->pages->setCurrentIndex(2);
    }
    else if(action->text()=="收支\n明细")
    {
        ui->pages->setCurrentIndex(3);
    }
    else if(action->text()=="盈利\n报表")
    {
        profitWrite();
        ui->pages->setCurrentIndex(4);
    }
}

void MainWindow::manageGoods()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if(btn->text()=="入库")
    {
        addG->setWindowTitle("入库");
        addG->exec();
    }
    else if(btn->text()=="出库")
    {
        fetG->output_uint_price->setEnabled(true);
        fetG->setWindowTitle("出库");
        fetG->exec();
    }
    else if(btn->text()=="损耗")
    {
        fetG->output_uint_price->setEnabled(false);
        fetG->setWindowTitle("损耗");
        fetG->exec();
    }
    else if(btn->text()=="修改")
    {
        //获取选取数据所在的行数
        int row = ui->tableView_good->currentIndex().row();
        //显示
        if(row==-1)
        {
            QMessageBox tips(QMessageBox::Information,"提示","请选择要修改的序号");
            tips.exec();
        }
        else
        {
            //获取第row行的数据填入modify窗口
            QSqlRecord good_record = good_sqlModel->record(row);
            QString date = good_record.value("date").toString();
            old_date = date;
            QString name = good_record.value("name").toString();
            old_name = name;
            float input_number = good_record.value("inputquantity").toFloat();
            old_input_number = input_number;
            float input_unit_price = good_record.value("inputunitprice").toFloat();
            float input_price = good_record.value("inputprice").toFloat();
            old_input_price = input_price;
            float output_number = good_record.value("outputquantity").toFloat();
            old_output_number = output_number;
            float output_unit_price = good_record.value("outputunitprice").toFloat();
            old_output_unit_price = output_unit_price;
            float output_price = good_record.value("outputprice").toFloat();
            QString unit = good_record.value("unit").toString();
            old_unit = unit;
            QString produceDate = good_record.value("produceDate").toString();
            QString validDate = good_record.value("validDate").toString();
            QString handler = good_record.value("handler").toString();
            old_handler = handler;
            QString remark = good_record.value("remark").toString();
            old_remark = remark;
            QStringList list = date.split("-");
            //判断是修改出库数据还是入库数据还是损耗数据
            QString ifInputGoods = good_record.value("inputquantity").toString();
            if(ifInputGoods != "/")//入库
            {
                mdfyG->date_year->setValue(list[0].toInt());
                mdfyG->date_month->setValue(list[1].toInt());
                mdfyG->date_day->setValue(list[2].toInt());
                mdfyG->name->setText(name);
                mdfyG->input_number->setEnabled(true);
                mdfyG->input_number->setValue(input_number);
                mdfyG->input_unit_price->setValue(input_unit_price);
                mdfyG->input_unit_price->setEnabled(false);
                mdfyG->input_price->setEnabled(true);
                mdfyG->input_price->setValue(input_price);
                mdfyG->output_number->setValue(output_number);
                mdfyG->output_number->setEnabled(false);
                mdfyG->output_unit_price->setValue(output_unit_price);
                mdfyG->output_unit_price->setEnabled(false);
                mdfyG->output_price->setValue(output_price);
                mdfyG->output_price->setEnabled(false);
                mdfyG->unit->setText(unit);
                mdfyG->produce_date->setEnabled(true);
                mdfyG->produce_date->setText(produceDate);
                mdfyG->valid_date->setEnabled(true);
                mdfyG->valid_date->setText(validDate);
                mdfyG->handler->setCurrentText(handler);
                mdfyG->remark->setText(remark);
            }
            else
            {
                if(good_record.value("outputprice").toString() != "/")//出库
                {
                    mdfyG->date_year->setValue(list[0].toInt());
                    mdfyG->date_month->setValue(list[1].toInt());
                    mdfyG->date_day->setValue(list[2].toInt());
                    mdfyG->name->setText(name);
                    mdfyG->input_number->setValue(input_number);
                    mdfyG->input_number->setEnabled(false);
                    mdfyG->input_unit_price->setValue(input_unit_price);
                    mdfyG->input_unit_price->setEnabled(false);
                    mdfyG->input_price->setValue(input_price);
                    mdfyG->input_price->setEnabled(false);
                    mdfyG->output_number->setEnabled(true);
                    mdfyG->output_number->setValue(output_number);
                    mdfyG->output_unit_price->setEnabled(true);
                    mdfyG->output_unit_price->setValue(output_unit_price);
                    mdfyG->output_price->setValue(output_price);
                    mdfyG->output_price->setEnabled(false);
                    mdfyG->unit->setText(unit);
                    mdfyG->produce_date->setText(produceDate);
                    mdfyG->produce_date->setEnabled(false);
                    mdfyG->valid_date->setText(validDate);
                    mdfyG->valid_date->setEnabled(false);
                    mdfyG->handler->setCurrentText(handler);
                    mdfyG->remark->setText(remark);
                }
                else//损耗
                {
                    mdfyG->date_year->setValue(list[0].toInt());
                    mdfyG->date_month->setValue(list[1].toInt());
                    mdfyG->date_day->setValue(list[2].toInt());
                    mdfyG->name->setText(name);
                    mdfyG->input_number->setValue(input_number);
                    mdfyG->input_number->setEnabled(false);
                    mdfyG->input_unit_price->setValue(input_unit_price);
                    mdfyG->input_unit_price->setEnabled(false);
                    mdfyG->input_price->setValue(input_price);
                    mdfyG->input_price->setEnabled(false);
                    mdfyG->output_number->setEnabled(true);
                    mdfyG->output_number->setValue(output_number);
                    mdfyG->output_unit_price->setValue(output_unit_price);
                    mdfyG->output_unit_price->setEnabled(false);
                    mdfyG->output_price->setValue(output_price);
                    mdfyG->output_price->setEnabled(false);
                    mdfyG->unit->setText(unit);
                    mdfyG->produce_date->setText(produceDate);
                    mdfyG->produce_date->setEnabled(false);
                    mdfyG->valid_date->setText(validDate);
                    mdfyG->valid_date->setEnabled(false);
                    mdfyG->handler->setCurrentText(handler);
                    mdfyG->remark->setText(remark);
                }
            }
            mdfyG->setWindowTitle("修改");
            mdfyG->exec();
            //重置money页的查询结果
            money_sqlModel->setFilter("");
            money_sqlModel->select();
        }
    }
    else if(btn->text()=="删除")
    {
        //获取需要删除数据所在的行数
        int row = ui->tableView_good->currentIndex().row();
        //显示
        if(row == -1)
        {
            QMessageBox tips(QMessageBox::Information,"提示","请选择要修改的序号");
            tips.exec();
        }
        else
        {
            //提示删除有风险
            QMessageBox msg(QMessageBox::Warning,"警告","注意！！删除的物品数据将无法恢复\n是否继续删除？");
            msg.addButton("是",QMessageBox::YesRole);
            msg.setDefaultButton(msg.addButton("否",QMessageBox::NoRole));
            if(msg.exec() == 1) return;
            QSqlRecord good_record = good_sqlModel->record(row);
            QString date = good_record.value("date").toString();
            QString name = good_record.value("name").toString();
            QString input_number;
            if(good_record.value("inputquantity").toString() != "/")
                input_number = QString::number(good_record.value("inputquantity").toFloat(),'f',1);
            else
                input_number = "/";
            QString input_unit_price;
            if(good_record.value("inputunitprice").toString() != "/")
                input_unit_price = QString::number(good_record.value("inputunitprice").toFloat(),'f',2);
            else
                input_unit_price = "/";
            QString input_price;
            if(good_record.value("inputprice").toString() != "/")
                input_price = QString::number(good_record.value("inputprice").toFloat(),'f',2);
            else
                input_price = "/";
            QString output_number;
            if(good_record.value("outputquantity").toString() != "/")
                output_number = QString::number(good_record.value("outputquantity").toFloat(),'f',1);
            else
                output_number = "/";
            QString output_unit_price;
            if(good_record.value("outputunitprice").toString() != "/")
                output_unit_price = QString::number(good_record.value("outputunitprice").toFloat(),'f',2);
            else
                output_unit_price = "/";
            QString output_price;
            if(good_record.value("outputprice").toString() != "/")
                output_price = QString::number(good_record.value("outputprice").toFloat(),'f',2);
            else
                output_price = "/";
            QString unit = good_record.value("unit").toString();
            QString produceDate = good_record.value("produceDate").toString();
            QString validDate = good_record.value("validDate").toString();
            QString handler = good_record.value("handler").toString();
            QString remark = good_record.value("remark").toString();
            delG->goods_date->setText(date);
            delG->goods_name->setText(name);
            delG->goods_input_number->setText(input_number);
            delG->goods_input_unit_price->setText(input_unit_price);
            delG->goods_input_price->setText(input_price);
            delG->goods_output_number->setText(output_number);
            delG->goods_output_unit_price->setText(output_unit_price);
            delG->goods_output_price->setText(output_price);
            delG->goods_unit->setText(unit);
            delG->goods_produce_date->setText(produceDate);
            delG->goods_valid_date->setText(validDate);
            delG->goods_handler->setText(handler);
            delG->goods_remark->setText(remark);
            delG->setWindowTitle("删除");
            delG->exec();
        }
    }
    else if(btn->text()=="导出库存明细表")
    {
        QString file = "";
        QString curPath = QCoreApplication::applicationDirPath();
        QString filter = "CSV文件(*.csv);;所有文件(*.*)";
        file = QFileDialog::getSaveFileName(this,"选择文件夹",curPath,filter);
        filepath = file;
        if(!file.isEmpty())
            good_thread->start();
    }
    else if(btn->text()=="查询")
    {
        searchGoods();
        if(good_sqlModel->rowCount()==0)
        {
            QMessageBox tips(QMessageBox::Warning,"提示","查询不到相关信息~");
            tips.exec();
            ui->nameEdit_good->clear();
            ui->dateEdit_good->clear();
            ui->priceEdit_good->clear();
            ui->handler_good->setCurrentIndex(0);
            searchGoods();
        }
    }
    else if(btn->text()=="重置查询结果")
    {
        ui->nameEdit_good->clear();
        ui->dateEdit_good->clear();
        ui->priceEdit_good->clear();
        ui->handler_good->setCurrentIndex(0);
        searchGoods();
        ui->tableView_good->sortByColumn(-1,Qt::SortOrder::AscendingOrder);//取消排序
    }
}

void MainWindow::insertAddGoodSQL()
{
    if(addG->name->text().isEmpty())
    {
        QMessageBox msg(QMessageBox::Information,"提示","请填写商品名称");
        msg.exec();
        return;
    }
    QString date = QString("%1-%2-%3").arg(addG->date_year->text()).arg(addG->date_month->value(),2,10,QLatin1Char('0')).arg(addG->date_day->value(),2,10,QLatin1Char('0'));
    QString name = addG->name->text().trimmed();
    float input_price = addG->input_price->value();
    float input_number = addG->input_number->value();
    QString unit = addG->unit->text().trimmed();
    QString handler = addG->handler->currentText().trimmed();
    QString produceDate = addG->produce_date->text().trimmed();
    QString validDate = addG->valid_date->text().trimmed();
    QString remark = addG->remark->text().trimmed();
    float input_unit_price;
    if(input_number == 0) input_unit_price = 0;
    else input_unit_price = input_price/input_number*1.0;
    //查找数据库stock页的匹配数据来设置remain_number的值
    float remain_number = 0;
    stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
    stock_sqlModel->select();
    if(stock_sqlModel->rowCount() == 0)
    {
        remain_number = input_number;
        QSqlRecord stockRecord = stock_sqlModel->record();
        stockRecord.setValue("name",name);
        stockRecord.setValue("stock",remain_number);
        stockRecord.setValue("unit",unit);
        stock_sqlModel->insertRecord(0,stockRecord);
    }
    else
    {
        remain_number = stock_sqlModel->record(0).value("stock").toFloat();
        //计算库存,并存回数据库
        remain_number = remain_number + input_number;
        QSqlRecord stockRecord = stock_sqlModel->record(0);
        stockRecord.setValue("stock",remain_number);
        stock_sqlModel->setRecord(0,stockRecord);
    }
    stock_sqlModel->submitAll();
    //添加数据
    QSqlRecord record = good_sqlModel->record();
    record.setValue("date",date);
    record.setValue("name",name);
    record.setValue("inputquantity",QString::number(input_number,'f',1));
    record.setValue("inputunitprice",QString::number(input_unit_price,'f',2));
    record.setValue("inputprice",QString::number(input_price,'f',2));
    record.setValue("outputquantity","/");
    record.setValue("outputunitprice","/");
    record.setValue("outputprice","/");
    record.setValue("unit",unit);
    record.setValue("produceDate",produceDate);
    record.setValue("validDate",validDate);
    record.setValue("handler",handler);
    record.setValue("remark",remark);
    good_sqlModel->insertRecord(good_sqlModel->rowCount(), record);
    good_sqlModel->submitAll();
    //自动将入库数据添加到收支页
    money_sqlModel->setFilter("");
    money_sqlModel->select();
    QSqlRecord money_record = money_sqlModel->record();
    money_record.setValue("date",date);
    money_record.setValue("payname",name+QString::number(input_number,'f',1)+unit);
    money_record.setValue("payunitmoney",QString::number(input_unit_price,'f',2));
    money_record.setValue("paymoney",QString::number(input_price,'f',2));
    money_record.setValue("incomename","/");
    money_record.setValue("incomeunitmoney","/");
    money_record.setValue("incomemoney","/");
    money_record.setValue("handler",handler);
    money_record.setValue("remark",remark);
    money_sqlModel->insertRecord(money_sqlModel->rowCount(),money_record);
    money_sqlModel->submitAll();
    //成功提示
    QMessageBox msg(QMessageBox::Information,"提示","入库成功！");
    msg.exec();
}

void MainWindow::insertFetchGoodSQL()
{
    //获取名称对应的库存数量
    QString date = QString("%1-%2-%3").arg(fetG->date_year->text()).arg(fetG->date_month->value(),2,10,QLatin1Char('0')).arg(fetG->date_day->value(),2,10,QLatin1Char('0'));
    QString name = fetG->name->text().trimmed();
    float output_number = fetG->output_number->value();
    float output_unit_price = fetG->output_uint_price->value();
    float output_price = output_unit_price*output_number*1.0;
    float remain_number;
    QString unit = fetG->unit->text().trimmed();
    QString handler = fetG->handler->currentText().trimmed();
    QString remark = fetG->remark->text().trimmed();
    //选出与name和unit对应的行,并取出
    stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
    stock_sqlModel->select();
    remain_number = stock_sqlModel->record(0).value("stock").toFloat();
    //计算库存,并存回数据库
    remain_number = remain_number - output_number;
    if(remain_number < 0)
    {
        QMessageBox msg(QMessageBox::Warning,"提醒","出库失败!请检查\n物品剩余库存是否充足？\n物品名称和单位是否与库存对应？");
        msg.exec();
        return;
    }
    QSqlRecord stockColum = stock_sqlModel->record(0);
    stockColum.setValue("stock",remain_number);
    stock_sqlModel->setRecord(0,stockColum);
    stock_sqlModel->submitAll();
    //添加数据
    QSqlRecord record = good_sqlModel->record();
    if(fetG->output_uint_price->isEnabled()) //出库
    {
        record.setValue("date",date);
        record.setValue("name",name);
        record.setValue("inputquantity","/");
        record.setValue("inputunitprice","/");
        record.setValue("inputprice","/");
        record.setValue("outputquantity",QString::number(output_number,'f',1));
        record.setValue("outputunitprice",QString::number(output_unit_price,'f',2));
        record.setValue("outputprice",QString::number(output_price,'f',2));
        record.setValue("unit",unit);
        record.setValue("handler",handler);
        record.setValue("produceDate","/");
        record.setValue("validDate","/");
        record.setValue("remark",remark);
        good_sqlModel->insertRecord(good_sqlModel->rowCount(), record);
        good_sqlModel->submitAll();
        //自动添加出库数据到收支页
        money_sqlModel->setFilter("");
        money_sqlModel->select();
        QSqlRecord money_record = money_sqlModel->record();
        money_record.setValue("date",date);
        money_record.setValue("payname","/");
        money_record.setValue("payunitmoney","/");
        money_record.setValue("paymoney","/");
        money_record.setValue("incomename",name+QString::number(output_number,'f',1)+unit);
        money_record.setValue("incomeunitmoney",QString::number(output_unit_price,'f',2));
        money_record.setValue("incomemoney",QString::number(output_price,'f',2));
        money_record.setValue("handler",handler);
        money_record.setValue("remark",remark);
        money_sqlModel->insertRecord(money_sqlModel->rowCount(),money_record);
        money_sqlModel->submitAll();
    }
    else //损耗
    {
        record.setValue("date",date);
        record.setValue("name",name);
        record.setValue("inputquantity","/");
        record.setValue("inputunitprice","/");
        record.setValue("inputprice","/");
        record.setValue("outputquantity",QString::number(output_number,'f',1));
        record.setValue("outputunitprice","/");
        record.setValue("outputprice","/");
        record.setValue("unit",unit);
        record.setValue("handler",handler);
        record.setValue("produceDate","/");
        record.setValue("validDate","/");
        record.setValue("remark",remark);
        good_sqlModel->insertRecord(good_sqlModel->rowCount(), record);
        good_sqlModel->submitAll();
    }
    //成功提示
    QMessageBox msg(QMessageBox::Information,"提示","出库成功！");
    msg.exec();
}

void MainWindow::modifyGoodSQL()
{
    QString date = QString("%1-%2-%3").arg(mdfyG->date_year->text()).arg(mdfyG->date_month->value(),2,10,QLatin1Char('0')).arg(mdfyG->date_day->value(),2,10,QLatin1Char('0'));
    QString name = mdfyG->name->text().trimmed();
    float input_number = mdfyG->input_number->value();
    float input_price = mdfyG->input_price->value();
    float input_unit_price;
    if(input_number == 0) input_unit_price = 0;
    else input_unit_price = input_price/input_number*1.0;
    float output_number = mdfyG->output_number->value();
    float output_unit_price = mdfyG->output_unit_price->value();
    float output_price = output_unit_price*output_number*1.0;
    QString unit = mdfyG->unit->text().trimmed();
    QString produceDate = mdfyG->produce_date->text().trimmed();
    QString validDate = mdfyG->valid_date->text().trimmed();
    QString handler = mdfyG->handler->currentText().trimmed();
    QString remark = mdfyG->remark->text().trimmed();

    stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
    stock_sqlModel->select();
    //未查找到相应库存，则进行提示
    if(stock_sqlModel->rowCount() == 0)
    {
        QMessageBox msg(QMessageBox::Warning,"警告","修改失败！没有相应的库存数据\n请检查修改的物品名称与单位是否正确");
        msg.exec();
        stock_sqlModel->setFilter("");
        stock_sqlModel->select();
        good_sqlModel->revertAll();
        money_sqlModel->revertAll();
        stock_sqlModel->revertAll();
        return;
    }
    else//查找到相应库存，则直接修改
    {
        int row = ui->tableView_good->currentIndex().row();
        QSqlRecord good_record = good_sqlModel->record(row);
        QSqlRecord money_record;
        QSqlRecord stock_record;
        if(mdfyG->input_number->isEnabled())//入库数据修改
        {
            //先减去旧的入库数量再加上新修改的数量
            //如果同时有单位a和单位b存在,且把a修改成b,则a减数量,b加数量
            if(unit==old_unit && input_number!=old_input_number)//单位不修改
            {
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                temp_stock_number = temp_stock_number - old_input_number + input_number;
                if(temp_stock_number < 0)
                {
                    QMessageBox msg(QMessageBox::Warning,"警告","修改后将导致库存数量为负\n请检查此物品的出入库数量关系\n是否继续修改？");
                    msg.addButton("是",QMessageBox::YesRole);
                    msg.setDefaultButton(msg.addButton("否",QMessageBox::NoRole));
                    if(msg.exec() == 1)
                    {
                        mdfyG->close();
                        stock_sqlModel->setFilter("");
                        stock_sqlModel->select();
                        good_sqlModel->revertAll();
                        money_sqlModel->revertAll();
                        stock_sqlModel->revertAll();
                        return;
                    }
                }
                stock_record.setValue("stock",temp_stock_number);
                stock_sqlModel->setRecord(0,stock_record);
            }
            else if(input_number != old_input_number)//单位修改,且修改的单位存在
            {
                stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(old_unit));
                stock_sqlModel->select();
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                if(temp_stock_number-old_input_number < 0)
                {
                    QMessageBox msg(QMessageBox::Warning,"警告","修改后将导致库存数量为负\n请检查此物品的出入库数量关系\n是否继续修改？");
                    msg.addButton("是",QMessageBox::YesRole);
                    msg.setDefaultButton(msg.addButton("否",QMessageBox::NoRole));
                    if(msg.exec() == 1)
                    {
                        mdfyG->close();
                        stock_sqlModel->setFilter("");
                        stock_sqlModel->select();
                        good_sqlModel->revertAll();
                        money_sqlModel->revertAll();
                        stock_sqlModel->revertAll();
                        return;
                    }
                }
                stock_record.setValue("stock",temp_stock_number - old_input_number);
                stock_sqlModel->setRecord(0,stock_record);
                stock_sqlModel->submitAll();
                stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
                stock_sqlModel->select();
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                stock_record.setValue("stock",temp_stock_number + input_number);
                stock_sqlModel->setRecord(0,stock_record);
            }
            //获取money页中同样存在的入库数据以供修改
            QString moneySqlFiler = QString("date = '%1' and payname like '%%2%3%' and paymoney = '%4'").arg(old_date).arg(old_name).arg(QString::number(old_input_number,'f',1)).arg(QString::number(old_input_price,'f',2));
            money_sqlModel->setFilter(moneySqlFiler);
            money_sqlModel->select();
            money_record = money_sqlModel->record(0);
            //修改good和money页数据
            if(date != old_date)
            {
                good_record.setValue("date",date);
                money_record.setValue("date",date);
            }
            if(input_number!=old_input_number || unit!=old_unit)
            {
                good_record.setValue("inputquantity",QString::number(input_number,'f',1));
                good_record.setValue("unit",unit);
                money_record.setValue("payname",name+QString::number(input_number,'f',1)+unit);
                input_unit_price = input_price/input_number;
                good_record.setValue("inputunitprice",QString::number(input_unit_price,'f',2));
                good_record.setValue("inputprice",QString::number(input_price,'f',2));
                money_record.setValue("payunitmoney",QString::number(input_unit_price,'f',2));
                money_record.setValue("paymoney",QString::number(input_price,'f',2));
            }
            if(input_price != old_input_price)
            {
                input_unit_price = input_price/input_number;
                good_record.setValue("inputunitprice",QString::number(input_unit_price,'f',2));
                good_record.setValue("inputprice",QString::number(input_price,'f',2));
                money_record.setValue("payunitmoney",QString::number(input_unit_price,'f',2));
                money_record.setValue("paymoney",QString::number(input_price,'f',2));
            }
            good_record.setValue("produceDate",produceDate);
            good_record.setValue("validDate",validDate);
            if(handler != old_handler)
            {
                good_record.setValue("handler",handler);
                money_record.setValue("handler",handler);
            }
            if(remark != old_remark)
            {
                good_record.setValue("remark",remark);
                money_record.setValue("remark",remark);
            }
            QMessageBox msg(QMessageBox::Information,"提示","修改成功");
            if(good_sqlModel->setRecord(row,good_record) && money_sqlModel->setRecord(0,money_record))
            {
                good_sqlModel->submitAll();
                money_sqlModel->submitAll();
                stock_sqlModel->submitAll();
                mdfyG->close();
                msg.exec();
            }
            else
            {
                good_sqlModel->revertAll();
                money_sqlModel->revertAll();
                stock_sqlModel->revertAll();
                msg.setText("修改失败！\n请检查收支明细是否有对应数据");
                msg.exec();
            }
        }
        else if(mdfyG->output_number->isEnabled())//出库数据修改
        {
            //先加上旧的出库数量再减去新修改的出库数量
            //如果同时有单位a和单位b存在,且把a修改成b,则a加数量,b减数量
            if(unit==old_unit && output_number!=old_output_number)
            {
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                temp_stock_number = temp_stock_number + old_output_number - output_number;
                if(temp_stock_number < 0)
                {
                    QMessageBox msg(QMessageBox::Warning,"警告","错误！剩余库存小于要修改的出库数量！");
                    msg.exec();
                    stock_sqlModel->setFilter("");
                    stock_sqlModel->select();
                    good_sqlModel->revertAll();
                    money_sqlModel->revertAll();
                    stock_sqlModel->revertAll();
                    return;
                }
                stock_record.setValue("stock",temp_stock_number);
                stock_sqlModel->setRecord(0,stock_record);
            }
            else if(output_number != old_output_number)
            {
                stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(old_unit));
                stock_sqlModel->select();
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                stock_record.setValue("stock",temp_stock_number + old_input_number);
                stock_sqlModel->setRecord(0,stock_record);
                stock_sqlModel->submitAll();
                stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
                stock_sqlModel->select();
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                if(output_number > temp_stock_number)
                {
                    QMessageBox msg(QMessageBox::Warning,"警告","错误！剩余库存小于要修改的出库数量！");
                    msg.exec();
                    stock_sqlModel->setFilter("");
                    stock_sqlModel->select();
                    good_sqlModel->revertAll();
                    money_sqlModel->revertAll();
                    stock_sqlModel->revertAll();
                    return;
                }
                stock_record.setValue("stock",temp_stock_number - output_number);
                stock_sqlModel->setRecord(0,stock_record);
            }
            //同步获取money页中同样存在的出库数据以供修改
            QString moneySqlFiler = QString("date = '%1' and incomename like '%%2%3%' and incomeunitmoney = '%4'").arg(old_date).arg(old_name).arg(QString::number(old_output_number,'f',1)).arg(QString::number(old_output_unit_price,'f',2));
            money_sqlModel->setFilter(moneySqlFiler);
            money_sqlModel->select();
            money_record = money_sqlModel->record(0);
            //修改good和money页数据
            if(date != old_date)
            {
                good_record.setValue("date",date);
                money_record.setValue("date",date);
            }
            if(output_number!=old_output_number || unit!=old_unit)
            {
                good_record.setValue("outputquantity",QString::number(output_number,'f',1));
                good_record.setValue("unit",unit);
                money_record.setValue("incomename",name+QString::number(output_number,'f',1)+unit);
                output_price = output_unit_price*output_number;
                good_record.setValue("outputunitprice",QString::number(output_unit_price,'f',2));
                good_record.setValue("outputprice",QString::number(output_price,'f',2));
                money_record.setValue("incomeunitmoney",QString::number(output_unit_price,'f',2));
                money_record.setValue("incomemoney",QString::number(output_price,'f',2));
            }
            if(output_unit_price != old_output_unit_price)
            {
                output_price = output_unit_price*output_number;
                good_record.setValue("outputunitprice",QString::number(output_unit_price,'f',2));
                good_record.setValue("outputprice",QString::number(output_price,'f',2));
                money_record.setValue("incomeunitmoney",QString::number(output_unit_price,'f',2));
                money_record.setValue("incomemoney",QString::number(output_price,'f',2));
            }
            if(handler != old_handler)
            {
                good_record.setValue("handler",handler);
                money_record.setValue("handler",handler);
            }
            if(remark != old_remark)
            {
                good_record.setValue("remark",remark);
                money_record.setValue("remark",remark);
            }
            QMessageBox msg(QMessageBox::Information,"提示","修改成功");
            if(good_sqlModel->setRecord(row,good_record) && money_sqlModel->setRecord(0,money_record))
            {
                good_sqlModel->submitAll();
                money_sqlModel->submitAll();
                stock_sqlModel->submitAll();
                mdfyG->close();
                msg.exec();
            }
            else
            {
                good_sqlModel->revertAll();
                money_sqlModel->revertAll();
                stock_sqlModel->revertAll();
                msg.setText("修改失败！\n请检查收支明细是否有对应数据");
                msg.exec();
            }
        }
        else//损耗数据修改
        {
            //先加上旧的出库数量再减去新修改的出库数量
            //如果同时有单位a和单位b存在,且把a修改成b,则a加数量,b减数量
            if(unit == old_unit)
            {
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                if(output_number > temp_stock_number)
                {
                    QMessageBox msg(QMessageBox::Warning,"警告","错误！剩余库存小于要修改的出库数量！");
                    msg.exec();
                    stock_sqlModel->setFilter("");
                    stock_sqlModel->select();
                    good_sqlModel->revertAll();
                    money_sqlModel->revertAll();
                    stock_sqlModel->revertAll();
                    return;
                }
                temp_stock_number = temp_stock_number + old_output_number - output_number;
                stock_record.setValue("stock",temp_stock_number);
                stock_sqlModel->setRecord(0,stock_record);
            }
            else
            {
                stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(old_unit));
                stock_sqlModel->select();
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                stock_record.setValue("stock",temp_stock_number + old_input_number);
                stock_sqlModel->setRecord(0,stock_record);
                stock_sqlModel->submitAll();
                stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
                stock_sqlModel->select();
                stock_record = stock_sqlModel->record(0);
                temp_stock_number = stock_record.value("stock").toFloat();
                if(output_number > temp_stock_number)
                {
                    QMessageBox msg(QMessageBox::Warning,"警告","错误！剩余库存小于要修改的出库数量！");
                    msg.exec();
                    stock_sqlModel->setFilter("");
                    stock_sqlModel->select();
                    good_sqlModel->revertAll();
                    money_sqlModel->revertAll();
                    stock_sqlModel->revertAll();
                    return;
                }
                stock_record.setValue("stock",temp_stock_number - output_number);
                stock_sqlModel->setRecord(0,stock_record);
            }
            if(output_number!=old_output_number || unit!=old_unit) good_record.setValue("outputquantity",QString::number(output_number,'f',1));
            if(unit != old_unit) good_record.setValue("unit",unit);
            if(handler != old_handler) good_record.setValue("handler",handler);
            if(remark != old_remark) good_record.setValue("remark",remark);
            QMessageBox msg(QMessageBox::Information,"提示","修改成功");
            if(good_sqlModel->setRecord(row,good_record))
            {
                good_sqlModel->submitAll();
                stock_sqlModel->submitAll();
                msg.exec();
                mdfyG->close();
            }
            else
            {
                good_sqlModel->revertAll();
                stock_sqlModel->revertAll();
                msg.setText("修改失败！请重试");
                msg.exec();
            }
        }
    }
    //重置收支页面的筛选结果
    money_sqlModel->setFilter("");
    money_sqlModel->select();
    //重置现货页面的筛选结果
    stock_sqlModel->setFilter("");
    stock_sqlModel->select();
}

void MainWindow::deleteGoodSQL()
{
    QString date = delG->goods_date->text();
    QString name = delG->goods_name->text();
    QString input_number = delG->goods_input_number->text();
    QString input_price = delG->goods_input_price->text();
    QString output_number = delG->goods_output_number->text();
    QString output_price = delG->goods_output_price->text();
    QString unit = delG->goods_unit->text();
    int row = ui->tableView_good->currentIndex().row();
    QMessageBox msg(QMessageBox::Warning,"警告","确定要删除该数据？");
    msg.addButton("是",QMessageBox::YesRole);
    msg.setDefaultButton(msg.addButton("否",QMessageBox::NoRole));
    if(msg.exec() == 0)
    {
        QString moneySqlFilter;
        if(output_price != "/")
           moneySqlFilter = QString("date = '%1' and incomename like '%%2%3%' and incomemoney = '%4'").arg(date).arg(name).arg(output_number).arg(output_price);
        else
           moneySqlFilter = QString("date = '%1' and payname like '%%2%3%' and paymoney = '%4'").arg(date).arg(name).arg(input_number).arg(input_price);
        money_sqlModel->setFilter(moneySqlFilter);
        money_sqlModel->select();
        if(input_price=="/" && output_price=="/")
        {
            good_sqlModel->removeRow(row);
            good_sqlModel->submitAll();
            //修改对应数据库存数量
            stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
            stock_sqlModel->select();
            QSqlRecord stock_record = stock_sqlModel->record(0);
            if(stock_sqlModel->rowCount() == 0)
            {
                QMessageBox msg(QMessageBox::Information,"提示","删除失败，无对应库存数据");
                msg.exec();
                stock_sqlModel->setFilter("");
                stock_sqlModel->select();
                good_sqlModel->revertAll();
                money_sqlModel->revertAll();
                stock_sqlModel->revertAll();
                return;
            }
            else
            {
                temp_stock_number = stock_record.value("stock").toFloat();
                temp_stock_number += output_number.toFloat();
                stock_record.setValue("stock",temp_stock_number);
                stock_sqlModel->setRecord(0,stock_record);
                stock_sqlModel->submitAll();
            }
            stock_sqlModel->setFilter("");
            stock_sqlModel->select();
            QMessageBox tips(QMessageBox::Information,"提示","删除成功");
            delG->close();
            tips.exec();
        }
        else if(money_sqlModel->removeRow(0))
        {
            //删除出入库与收支对应数据
            good_sqlModel->removeRow(row);
            //删除库存对应数据
            stock_sqlModel->setFilter(QString("name = '%1' and unit = '%2'").arg(name).arg(unit));
            stock_sqlModel->select();
            QSqlRecord stock_record = stock_sqlModel->record(0);
            if(stock_sqlModel->rowCount() == 0)
            {
                QMessageBox msg(QMessageBox::Information,"提示","删除失败，无对应库存数据");
                msg.exec();
                stock_sqlModel->setFilter("");
                stock_sqlModel->select();
                good_sqlModel->revertAll();
                money_sqlModel->revertAll();
                stock_sqlModel->revertAll();
                return;
            }
            else
            {
                if(input_price != "/")
                {
                    temp_stock_number = stock_record.value("stock").toFloat();
                    temp_stock_number -= input_number.toFloat();
                    if(temp_stock_number < 0)
                    {
                        QMessageBox msg(QMessageBox::Warning,"警告","注意！！由于此物品已有出库数据\n删除入库数据后将导致库存数量为负！\n是否继续删除？");
                        msg.addButton("是",QMessageBox::YesRole);
                        msg.setDefaultButton(msg.addButton("否",QMessageBox::NoRole));
                        if(msg.exec() == 1)
                        {
                            delG->close();
                            stock_sqlModel->setFilter("");
                            stock_sqlModel->select();
                            good_sqlModel->revertAll();
                            money_sqlModel->revertAll();
                            stock_sqlModel->revertAll();
                            return;
                        }
                    }
                    stock_record.setValue("stock",temp_stock_number);
                    stock_sqlModel->setRecord(0,stock_record);
                }
                else
                {
                    temp_stock_number = stock_record.value("stock").toFloat();
                    temp_stock_number += output_number.toFloat();
                    stock_record.setValue("stock",temp_stock_number);
                    stock_sqlModel->setRecord(0,stock_record);
                }
                stock_sqlModel->submitAll();
                stock_sqlModel->setFilter("");
                stock_sqlModel->select();
            }
            QMessageBox tips(QMessageBox::Information,"提示","删除成功");
            delG->close();
            tips.exec();
            good_sqlModel->submitAll();
            money_sqlModel->submitAll();
        }
        else
        {
            good_sqlModel->revertAll();
            money_sqlModel->revertAll();
            stock_sqlModel->revertAll();
            QMessageBox tips(QMessageBox::Information,"提示","收支明细无对应项，删除失败");
            delG->close();
            tips.exec();
        }
        //重置收支页面的筛选结果
        money_sqlModel->setFilter("");
        money_sqlModel->select();
    }
    else return;
}

void MainWindow::searchGoods()
{
    QString name =  QString("name like '%%1%'").arg(ui->nameEdit_good->text().trimmed());
    QString handler = QString("handler like '%%1%'").arg(ui->handler_good->currentText().trimmed());
    QString price = QString("price like '%%1%'").arg(ui->priceEdit_good->text().trimmed());
    QString date = QString("date like '%%1%'").arg(ui->dateEdit_good->text().trimmed());
    QString filterStr = "";
    if(!ui->nameEdit_good->text().isEmpty())
        filterStr.append(name);
    if(!filterStr.isEmpty() && (ui->handler_good->currentText()!=""))
    {
        filterStr.append(" and ");
        filterStr.append(handler);
    }
    else if((ui->handler_good->currentText()!=""))
        filterStr.append(handler);
    if(!filterStr.isEmpty() && (!ui->priceEdit_good->text().isEmpty()))
    {
        filterStr.append(" and ");
        filterStr.append(price);
    }
    else if(!ui->priceEdit_good->text().isEmpty())
        filterStr.append(price);
    if(!filterStr.isEmpty() && (!ui->dateEdit_good->text().isEmpty()))
    {
        filterStr.append(" and ");
        filterStr.append(date);
    }
    else if(!ui->dateEdit_good->text().isEmpty())
        filterStr.append(date);
    good_sqlModel->setFilter(filterStr);
    good_sqlModel->select();
}

void MainWindow::sortGoods(int col, Qt::SortOrder order)
{
    good_sqlModel->setSort(col,order);
    good_sqlModel->select();
}

void MainWindow::exportGoodSuccess()
{
    QMessageBox tipSuccess(QMessageBox::Information,"提示","Excel表格导出成功!");
    QMessageBox tipFail(QMessageBox::Information,"提示","Excel表格导出失败!");
    good_thread->wait();
    if(good_thread->isFail)
        tipFail.exec();
    else
        tipSuccess.exec();
}

void MainWindow::manageMoney()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if(btn->text()=="收入")
    {
        addI->setWindowTitle("收入");
        addI->exec();
    }
    else if(btn->text()=="支出")
    {
        addE->setWindowTitle("支出");
        addE->exec();
    }
    else if(btn->text()=="修改")
    {
        int row = ui->tableView_money->currentIndex().row();
        //显示
        if(row==-1)
        {
            QMessageBox tips(QMessageBox::Information,"提示","请选择要修改的序号");
            tips.exec();
        }
        else
        {
            //提示，修改与出入库对应的收支数据有风险
            QMessageBox msg(QMessageBox::Warning,"警告","注意！！请勿修改与出入库对应的收支数据\n否则将引发严重数据错误");
            msg.addButton("继续修改",QMessageBox::YesRole);
            msg.setDefaultButton(msg.addButton("取消",QMessageBox::NoRole));
            if(msg.exec() == 1) return;
            //获取第row行的数据填入modify窗口
            QSqlRecord record = money_sqlModel->record(row);
            QString date = record.value("date").toString();
            QString pay_name = record.value("payname").toString();
            float pay_money = record.value("paymoney").toFloat();
            QString income_name = record.value("incomename").toString();
            float income_unit_money = record.value("incomeunitmoney").toFloat();
            QString handler = record.value("handler").toString();
            QString remark = record.value("remark").toString();
            QStringList list = date.split("-");
            //根据收支类型修改数据
            QString isIncome = record.value("incomemoney").toString();
            if(isIncome != "/")
            {
                mdfyM->date_year->setValue(list[0].toInt());
                mdfyM->date_month->setValue(list[1].toInt());
                mdfyM->date_day->setValue(list[2].toInt());
                mdfyM->income_name->setEnabled(true);
                mdfyM->income_name->setText(income_name);
                mdfyM->income_unit_money->setEnabled(true);
                mdfyM->income_unit_money->setValue(income_unit_money);
                mdfyM->pay_name->setText(pay_name);
                mdfyM->pay_name->setEnabled(false);
                mdfyM->pay_money->setValue(pay_money);
                mdfyM->pay_money->setEnabled(false);
                mdfyM->handler->setCurrentText(handler);
                mdfyM->remark->setText(remark);
            }
            else
            {
                mdfyM->date_year->setValue(list[0].toInt());
                mdfyM->date_month->setValue(list[1].toInt());
                mdfyM->date_day->setValue(list[2].toInt());
                mdfyM->income_name->setText(income_name);
                mdfyM->income_name->setEnabled(false);
                mdfyM->income_unit_money->setValue(income_unit_money);
                mdfyM->income_unit_money->setEnabled(false);
                mdfyM->pay_name->setEnabled(true);
                mdfyM->pay_name->setText(pay_name);
                mdfyM->pay_money->setEnabled(true);
                mdfyM->pay_money->setValue(pay_money);
                mdfyM->handler->setCurrentText(handler);
                mdfyM->remark->setText(remark);
            }
            mdfyM->setWindowTitle("修改");
            mdfyM->exec();
        }
    }
    else if(btn->text()=="导出收支明细表")
    {
        QString file = "";
        QString curPath = QCoreApplication::applicationDirPath();
        QString filter = "CSV文件(*.csv);;所有文件(*.*)";
        file = QFileDialog::getSaveFileName(this,"选择文件夹",curPath,filter);
        filepath = file;
        if(!file.isEmpty())
            money_thread->start();
    }
    else if(btn->text()=="查询")
    {
        searchMoney();
        if(money_sqlModel->rowCount()==0)
        {
            QMessageBox tips(QMessageBox::Warning,"提示","查询不到相关信息~");
            tips.exec();
            ui->nameinput_money->clear();
            ui->nameoutput_money->clear();
            ui->dateEdit_money->clear();
            ui->handler_money->setCurrentIndex(0);
            searchMoney();
        }
    }
    else if(btn->text()=="重置查询结果")
    {
        ui->nameinput_money->clear();
        ui->nameoutput_money->clear();
        ui->dateEdit_money->clear();
        ui->handler_money->setCurrentIndex(0);
        searchMoney();
        ui->tableView_money->sortByColumn(-1,Qt::SortOrder::AscendingOrder);//取消排序
    }
}

void MainWindow::insertAddIncomeSQL()
{
    if(addI->income_name->text().isEmpty())
    {
        QMessageBox msg(QMessageBox::Information,"提示","请填写收入项目");
        msg.exec();
        return;
    }
    QString date = QString("%1-%2-%3").arg(addI->date_year->text().trimmed()).arg(addI->date_month->value(),2,10,QLatin1Char('0')).arg(addI->date_day->value(),2,10,QLatin1Char('0'));
    QString income_name = addI->income_name->text().trimmed();
    float income_unit_money = addI->income_unit_moeny->value();
    float income_money = income_unit_money*addI->amount->value();
    QString handler = addI->handler->currentText().trimmed();
    QString remark = addI->remark->text().trimmed();

    QSqlRecord record = money_sqlModel->record();
    record.setValue("date",date);
    record.setValue("payname","/");
    record.setValue("payunitmoney","/");
    record.setValue("paymoney","/");
    record.setValue("incomename",income_name);
    record.setValue("incomeunitmoney",QString::number(income_unit_money,'f',2));
    record.setValue("incomemoney",QString::number(income_money,'f',2));
    record.setValue("handler",handler);
    record.setValue("remark",remark);
    money_sqlModel->insertRecord(money_sqlModel->rowCount(), record);
    money_sqlModel->submitAll();
    //成功提示
    QMessageBox msg(QMessageBox::Information,"提示","添加成功！");
    msg.exec();
}

void MainWindow::insertAddExpendSQL()
{
    if(addE->pay_name->text().isEmpty())
    {
        QMessageBox msg(QMessageBox::Information,"提示","请填写支出项目");
        msg.exec();
        return;
    }
    QString date = QString("%1-%2-%3").arg(addE->date_year->text().trimmed()).arg(addE->date_month->value(),2,10,QLatin1Char('0')).arg(addE->date_day->value(),2,10,QLatin1Char('0'));
    QString pay_name = addE->pay_name->text().trimmed();
    float pay_money = addE->pay_money->value();
    float pay_unit_money = pay_money/addE->amount->value()*1.0;
    QString handler = addE->handler->currentText().trimmed();
    QString remark = addE->remark->text().trimmed();

    QSqlRecord record = money_sqlModel->record();
    record.setValue("date",date);
    record.setValue("payname",pay_name);
    record.setValue("payunitmoney",QString::number(pay_unit_money,'f',2));
    record.setValue("paymoney",QString::number(pay_money,'f',2));
    record.setValue("incomename","/");
    record.setValue("incomeunitmoney","/");
    record.setValue("incomemoney","/");
    record.setValue("handler",handler);
    record.setValue("remark",remark);
    money_sqlModel->insertRecord(money_sqlModel->rowCount(), record);
    money_sqlModel->submitAll();
    //成功提示
    QMessageBox msg(QMessageBox::Information,"提示","添加成功！");
    msg.exec();
}

void MainWindow::modifyMoneySQL()
{
    QString date = QString("%1-%2-%3").arg(mdfyM->date_year->text().trimmed()).arg(mdfyM->date_month->value(),2,10,QLatin1Char('0')).arg(mdfyM->date_day->value(),2,10,QLatin1Char('0'));
    QString pay_name = mdfyM->pay_name->text().trimmed();
    float pay_money = mdfyM->pay_money->value();
    float pay_unit_money;
    QString income_name = mdfyM->income_name->text().trimmed();
    float income_unit_money = mdfyM->income_unit_money->value();
    float income_money;
    QString handler = mdfyM->handler->currentText().trimmed();
    QString remark = mdfyM->remark->text().trimmed();
    //修改数据
    int row = ui->tableView_money->currentIndex().row();
    QSqlRecord record = money_sqlModel->record(row);
    if(mdfyM->income_unit_money->isEnabled())
    {
        record.setValue("date",date);
        record.setValue("payname","/");
        record.setValue("payunitmoney","/");
        record.setValue("paymoney","/");
        record.setValue("incomename",income_name);
        record.setValue("incomeunitmoney",QString::number(income_unit_money,'f',2));
        income_money = income_unit_money*mdfyM->amount->value();
        record.setValue("incomemoney",QString::number(income_money,'f',2));
        record.setValue("handler",handler);
        record.setValue("remark",remark);
    }
    else if(mdfyM->pay_money->isEnabled())
    {
        record.setValue("date",date);
        record.setValue("payname",pay_name);
        pay_unit_money = pay_money/mdfyM->amount->value();
        record.setValue("payunitmoney",QString::number(pay_unit_money,'f',2));
        record.setValue("paymoney",QString::number(pay_money,'f',2));
        record.setValue("incomename","/");
        record.setValue("incomeunitmoney","/");
        record.setValue("incomemoney","/");
        record.setValue("handler",handler);
        record.setValue("remark",remark);
    }
    if(money_sqlModel->setRecord(row,record))
    {
        QMessageBox tips(QMessageBox::Information,"提示","修改成功");
        money_sqlModel->submitAll();
        tips.exec();
        mdfyM->close();
    }
    else
    {
        money_sqlModel->revertAll();
        QMessageBox tips(QMessageBox::Information,"提示","修改失败！请重试");
        tips.exec();
    }
}

void MainWindow::searchMoney()
{
    QString income_name = QString("incomename like '%%1%'").arg(ui->nameinput_money->text().trimmed());
    QString pay_name =  QString("payname like '%%1%'").arg(ui->nameoutput_money->text().trimmed());
    QString handler =  QString("handler like '%%1%'").arg(ui->handler_money->currentText().trimmed());
    QString date = QString("date like '%%1%'").arg(ui->dateEdit_money->text().trimmed());
    QString filterStr = "";
    if(!ui->nameinput_money->text().isEmpty())
        filterStr.append(income_name);
    if(!filterStr.isEmpty() && (ui->handler_money->currentText()!=""))
    {
        filterStr.append(" and ");
        filterStr.append(handler);
    }
    else if((ui->handler_money->currentText()!=""))
        filterStr.append(handler);
    if(!filterStr.isEmpty() && (!ui->nameoutput_money->text().isEmpty()))
    {
        filterStr.append(" and ");
        filterStr.append(pay_name);
    }
    else if(!ui->nameoutput_money->text().isEmpty())
        filterStr.append(pay_name);
    if(!filterStr.isEmpty() && (!ui->dateEdit_money->text().isEmpty()))
    {
        filterStr.append(" and ");
        filterStr.append(date);
    }
    else if(!ui->dateEdit_money->text().isEmpty())
        filterStr.append(date);
    money_sqlModel->setFilter(filterStr);
    money_sqlModel->select();
}

void MainWindow::sortMoney(int col, Qt::SortOrder order)
{
    money_sqlModel->setSort(col,order);
    money_sqlModel->select();
}

void MainWindow::exportMoneySuccess()
{
    QMessageBox tipSuccess(QMessageBox::Information,"提示","Excel表格导出成功!");
    QMessageBox tipFail(QMessageBox::Information,"提示","Excel表格导出失败!");
    money_thread->wait();
    if(money_thread->isFail)
        tipFail.exec();
    else
        tipSuccess.exec();
}

void MainWindow::manageStock()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn->text() == "重置查询结果")
    {
        ui->nameEdit_stock->clear();
        searchStock();
    }
    else if(btn->text() == "查询")
    {
        searchStock();
        if(stock_sqlModel->rowCount() == 0)
        {
            QMessageBox tips(QMessageBox::Warning,"提示","查询不到相关信息~");
            tips.exec();
            ui->nameEdit_stock->clear();
            searchStock();
        }
    }
    else if(btn->text() == "删除")
    {
        stockRecordDelete();
    }
}

void MainWindow::stockRecordDelete()
{
    int row = ui->tableView_stock->currentIndex().row();
    if(row == -1)
    {
        QMessageBox msg(QMessageBox::Information,"提示","请选择要删除的记录");
        msg.exec();
    }
    else
    {
        QMessageBox msg(QMessageBox::Warning,"警告","删除的库存数据将无法恢复\n是否继续删除该数据？");
        msg.addButton("是",QMessageBox::YesRole);
        msg.setDefaultButton(msg.addButton("否",QMessageBox::NoRole));
        if(msg.exec() == 0)
        {
            stock_sqlModel->removeRow(row);
            stock_sqlModel->submitAll();
        }
        else return;
    }
}

void MainWindow::searchStock()
{
    QString name =  QString("name like '%%1%'").arg(ui->nameEdit_stock->text().trimmed());
    QString filterStr = "";
    if(!ui->nameEdit_stock->text().isEmpty())
        filterStr.append(name);
    stock_sqlModel->setFilter(filterStr);
    stock_sqlModel->select();
}

void MainWindow::profitExcel()
{
    if(profit_sqlModel->rowCount() == 0)
    {
        QMessageBox msg(QMessageBox::Information,"提示","表格为空，无法导出");
        msg.exec();
        return;
    }
    QString file = "";
    QString curPath = QCoreApplication::applicationDirPath();
    QString filter = "CSV文件(*.csv);;所有文件(*.*)";
    file = QFileDialog::getSaveFileName(this,"选择文件夹",curPath,filter);
    filepath = file;
    if(!file.isEmpty())
        profit_thread->start();
}

void MainWindow::createProfit()
{
    QString year = ui->year_comboBox->currentText();
    QString month = ui->month_comboBox->currentText();
    QCalendar currentDate;
    if( (year == "") || (month == "") )
    {
        QMessageBox msg(QMessageBox::Warning,"提示","请选择年份和月份");
        msg.exec();
        return;
    }
    profit_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(year).arg(month));
    profit_sqlModel->select();
    if(profit_sqlModel->rowCount()==0)
    {
        int start = 0;
        QSqlRecord create_record = profit_sqlModel->record();
        for(int i=1; i<=currentDate.daysInMonth(month.toInt(),year.toInt()); i++)
        {
           QString date = QString("%1-%2-%3").arg(year).arg(month).arg(i,2,10,QLatin1Char('0'));
           create_record.setValue("date",date);
           create_record.setValue("income",0.1);
           create_record.setValue("expenditure",0.1);
           create_record.setValue("profit",0.1);
           profit_sqlModel->insertRecord(start,create_record);
           start++;
        }
        profit_sqlModel->submitAll();
        //写入对应日期数据
        money_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(year).arg(month));
        money_sqlModel->select();
        QSqlRecord money_record;
        QSqlRecord profit_record;
        float d_profit = 0, d_income = 0, d_expend = 0;//d=day
        for(int i=1; i<=currentDate.daysInMonth(month.toInt(),year.toInt()); i++)
        {
            d_profit = 0;
            d_income = 0;
            d_expend = 0;
            money_sqlModel->setFilter(QString("date = '%1-%2-%3'").arg(year).arg(month).arg(i,2,10,QLatin1Char('0')));
            money_sqlModel->select();
            profit_sqlModel->setFilter(QString("date = '%1-%2-%3'").arg(year).arg(month).arg(i,2,10,QLatin1Char('0')));
            profit_sqlModel->select();
            for(int i=0; i<money_sqlModel->rowCount(); i++)
            {
                money_record = money_sqlModel->record(i);
                d_income += money_record.value("incomemoney").toFloat();
                d_expend += money_record.value("paymoney").toFloat();
            }
            d_profit = d_income - d_expend;
            profit_record = profit_sqlModel->record(0);
            profit_record.setValue("profit",d_profit);
            profit_record.setValue("income",d_income);
            profit_record.setValue("expenditure",d_expend);
            profit_sqlModel->setRecord(0,profit_record);
            profit_sqlModel->submitAll();
            m_profit += d_profit;
            m_income += d_income;
            m_expend += d_expend;
        }
        //重置筛选结果
        money_sqlModel->setFilter("");
        money_sqlModel->select();
        profit_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(year).arg(month));
        profit_sqlModel->select();
        QMessageBox tips(QMessageBox::Information,"提示","创建报表成功");
        tips.exec();
    }
    else//直接更新数据
    {
        //写入对应日期数据
        money_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(year).arg(month));
        money_sqlModel->select();
        QSqlRecord money_record;
        QSqlRecord profit_record;
        float d_profit = 0, d_income = 0, d_expend = 0;//d=day
        for(int i=1; i<=currentDate.daysInMonth(month.toInt(),year.toInt()); i++)
        {
            d_profit = 0;
            d_income = 0;
            d_expend = 0;
            money_sqlModel->setFilter(QString("date = '%1-%2-%3'").arg(year).arg(month).arg(i,2,10,QLatin1Char('0')));
            money_sqlModel->select();
            profit_sqlModel->setFilter(QString("date = '%1-%2-%3'").arg(year).arg(month).arg(i,2,10,QLatin1Char('0')));
            profit_sqlModel->select();
            for(int i=0; i<money_sqlModel->rowCount(); i++)
            {
                money_record = money_sqlModel->record(i);
                d_income += money_record.value("incomemoney").toFloat();
                d_expend += money_record.value("paymoney").toFloat();
            }
            d_profit = d_income - d_expend;
            profit_record = profit_sqlModel->record(0);
            profit_record.setValue("profit",d_profit);
            profit_record.setValue("income",d_income);
            profit_record.setValue("expenditure",d_expend);
            profit_sqlModel->setRecord(0,profit_record);
            profit_sqlModel->submitAll();
            m_profit += d_profit;
            m_income += d_income;
            m_expend += d_expend;
        }
        //重置筛选结果
        money_sqlModel->setFilter("");
        money_sqlModel->select();
        profit_sqlModel->setFilter(QString("date like '%%1-%2%'").arg(year).arg(month));
        profit_sqlModel->select();
        QMessageBox tips(QMessageBox::Information,"提示","数据更新成功");
        tips.exec();
    }
}

void MainWindow::profitShow(QString)
{
    profit_sqlModel->sort(1,Qt::AscendingOrder);
    QComboBox *combobox = qobject_cast<QComboBox*>(sender());
    if(combobox->whatsThis() == "年份")
    {
        QString filter = QString("date like '%%1-%2%'").arg(ui->year_comboBox->currentText()).arg(ui->month_comboBox->currentText());
        profit_sqlModel->setFilter(filter);
        profit_sqlModel->select();
        ui->lcdNumber_profit->display(0);
        ui->lcdNumber_income->display(0);
        ui->lcdNumber_expend->display(0);
    }
    else if(combobox->whatsThis()=="月份" && ui->month_comboBox->currentText()!="")
    {
        QString filter = QString("date like '%%1-%2-%'").arg(ui->year_comboBox->currentText()).arg(ui->month_comboBox->currentText());
        profit_sqlModel->setFilter(filter);
        profit_sqlModel->select();
        ui->lcdNumber_profit->display(0);
        ui->lcdNumber_income->display(0);
        ui->lcdNumber_expend->display(0);
    }
    else if(ui->year_comboBox->currentText()=="" && ui->month_comboBox->currentText()=="")
    {
        profit_sqlModel->setFilter("");
        profit_sqlModel->select();
    }
    else
    {
        QString filter = QString("date like '%%1%'").arg(ui->year_comboBox->currentText());
        profit_sqlModel->setFilter(filter);
        profit_sqlModel->select();
        ui->lcdNumber_profit->display(0);
        ui->lcdNumber_income->display(0);
        ui->lcdNumber_expend->display(0);
    }
}

void MainWindow::computeProfit()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString year = ui->year_comboBox->currentText();
    QString month = ui->month_comboBox->currentText();

    if(year == "")
    {
        QMessageBox msg(QMessageBox::Information,"提示","请选择年份！");
        msg.exec();
        return;
    }
    //计算年数据
    QSqlRecord record;
    if(month == "")
    {
        if(btn->text() == "计算总盈利")
        {
            float year_profit = 0;
            for(int i=0; i<profit_sqlModel->rowCount(); i++)
            {
                record = profit_sqlModel->record(i);
                year_profit += record.value("profit").toFloat();
            }
            ui->lcdNumber_profit->display(QString::number(year_profit,'f',2));
        }
        else if(btn->text() == "计算总收入")
        {
            float year_income = 0;
            for(int i=0; i<profit_sqlModel->rowCount(); i++)
            {
                record = profit_sqlModel->record(i);
                year_income += record.value("income").toFloat();
            }
            ui->lcdNumber_income->display(QString::number(year_income,'f',2));
        }
        else if(btn->text() == "计算总支出")
        {
            float year_expend = 0;
            for(int i=0; i<profit_sqlModel->rowCount(); i++)
            {
                record = profit_sqlModel->record(i);
                year_expend += record.value("expenditure").toFloat();
            }
            ui->lcdNumber_expend->display(QString::number(year_expend,'f',2));
        }
    }
    else //计算月数据
    {
        if(btn->text() == "计算总盈利")
        {
            float year_profit = 0;
            for(int i=0; i<profit_sqlModel->rowCount(); i++)
            {
                record = profit_sqlModel->record(i);
                year_profit += record.value("profit").toFloat();
            }
            ui->lcdNumber_profit->display(QString::number(year_profit,'f',2));
        }
        else if(btn->text() == "计算总收入")
        {
            float year_income = 0;
            for(int i=0; i<profit_sqlModel->rowCount(); i++)
            {
                record = profit_sqlModel->record(i);
                year_income += record.value("income").toFloat();
            }
            ui->lcdNumber_income->display(QString::number(year_income,'f',2));
        }
        else if(btn->text() == "计算总支出")
        {
            float year_expend = 0;
            for(int i=0; i<profit_sqlModel->rowCount(); i++)
            {
                record = profit_sqlModel->record(i);
                year_expend += record.value("expenditure").toFloat();
            }
            ui->lcdNumber_expend->display(QString::number(year_expend,'f',2));
        }
    }
}

void MainWindow::exportProfitSuccess()
{
    QMessageBox tipSuccess(QMessageBox::Information,"提示","Excel表格导出成功!");
    QMessageBox tipFail(QMessageBox::Information,"提示","Excel表格导出失败!");
    profit_thread->wait();
    if(profit_thread->isFail)
        tipFail.exec();
    else
        tipSuccess.exec();
}
