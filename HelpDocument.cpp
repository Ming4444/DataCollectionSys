#include "HelpDocument.h"

help::help(QWidget *parent): QMainWindow(parent)
{
    setWindowTitle("使用说明");
    file_link = QDir::currentPath() + "/help.html";
    webwidget = new QAxWidget(this);
    webwidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    webwidget->setProperty("DisplayAlerts",true); //不显示任何警告信息。
    webwidget->setProperty("DisplayScrollBars",true); // 显示滚动条
    webwidget->dynamicCall("Navigate(const QString&)",file_link);
    webwidget->setMinimumSize(735,800);
    setCentralWidget(webwidget);
    setMinimumSize(webwidget->size());
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

help::~help()
{
    delete webwidget;
}

void help::closeEvent(QCloseEvent *event)
{
    event->accept();
    delete this;
}
