#ifndef HELPDOCUMENT_H
#define HELPDOCUMENT_H

#include "Headerfiles.h"

class help:public QMainWindow
{
    Q_OBJECT
public:
    help(QWidget *parent = nullptr);
    ~help();
    void closeEvent(QCloseEvent *);

private:
    QString file_link;
    QAxWidget *webwidget;
    QGridLayout *glayout;
};

#endif // HELPDOCUMENT_H
