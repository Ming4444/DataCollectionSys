#ifndef FUNCTIONBUTTON_H
#define FUNCTIONBUTTON_H

#include "Headerfiles.h"

class passWord:public QDialog
{
    Q_OBJECT
public:
    passWord();

    QGridLayout *g_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *name_password_input = nullptr;
    QLabel *name_password_origin = nullptr;
    QLabel *name_password_new = nullptr;
    QLabel *name_password_confirm_again = nullptr;
    QLineEdit *input_password = nullptr;
    QLineEdit *change_password_origin = nullptr;
    QLineEdit *change_password_new = nullptr;
    QLineEdit *change_password_confirm_again = nullptr;
};

class addGoods:public QDialog
{
    Q_OBJECT
public:
    addGoods();

    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *goods_date = nullptr;
    QLabel *goods_name = nullptr;
    QLabel *goods_input_number = nullptr;
    QLabel *goods_input_unit_price = nullptr;
    QLabel *goods_input_price = nullptr;
    QLabel *goods_unit = nullptr;
    QLabel *goods_produce_date = nullptr;
    QLabel *goods_valid_date = nullptr;
    QLabel *goods_handler = nullptr;
    QLabel *goods_remark = nullptr;
    QSpinBox *date_year = nullptr;
    QSpinBox *date_month = nullptr;
    QSpinBox *date_day = nullptr;
    QLineEdit *name = nullptr;
    QDoubleSpinBox *input_number = nullptr;
    QDoubleSpinBox *input_unit_price = nullptr;
    QDoubleSpinBox *input_price = nullptr;
    QLineEdit *unit = nullptr;
    QComboBox *handler = nullptr;
    QLineEdit *produce_date = nullptr;
    QLineEdit *valid_date = nullptr;
    QLineEdit *remark = nullptr;
};

class fetchGoods:public QDialog
{
    Q_OBJECT
public:
    fetchGoods();

    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *goods_date = nullptr;
    QLabel *goods_name = nullptr;
    QLabel *goods_output_number = nullptr;
    QLabel *goods_output_uint_price = nullptr;
    QLabel *goods_output_price = nullptr;
    QLabel *goods_unit = nullptr;
    QLabel *goods_handler = nullptr;
    QLabel *goods_remark = nullptr;
    QSpinBox *date_year = nullptr;
    QSpinBox *date_month = nullptr;
    QSpinBox *date_day = nullptr;
    QLineEdit *name = nullptr;
    QDoubleSpinBox *output_number = nullptr;
    QDoubleSpinBox *output_uint_price = nullptr;
    QDoubleSpinBox *output_price = nullptr;
    QLineEdit *unit = nullptr;
    QComboBox *handler = nullptr;
    QLineEdit *remark = nullptr;
};

class modifyGoods:public QDialog
{
    Q_OBJECT
public:
    modifyGoods();

    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *goods_date = nullptr;
    QLabel *goods_name = nullptr;
    QLabel *goods_input_number = nullptr;
    QLabel *goods_input_unit_price = nullptr;
    QLabel *goods_input_price = nullptr;
    QLabel *goods_output_number = nullptr;
    QLabel *goods_output_unit_price = nullptr;
    QLabel *goods_output_price = nullptr;
    QLabel *goods_unit = nullptr;
    QLabel *goods_produce_date = nullptr;
    QLabel *goods_valid_date = nullptr;
    QLabel *goods_handler = nullptr;
    QLabel *goods_remark = nullptr;
    QSpinBox *date_year = nullptr;
    QSpinBox *date_month = nullptr;
    QSpinBox *date_day = nullptr;
    QLineEdit *name = nullptr;
    QDoubleSpinBox *input_number = nullptr;
    QDoubleSpinBox *input_unit_price = nullptr;
    QDoubleSpinBox *input_price = nullptr;
    QDoubleSpinBox *output_number = nullptr;
    QDoubleSpinBox *output_unit_price = nullptr;
    QDoubleSpinBox *output_price = nullptr;
    QLineEdit *unit = nullptr;
    QLineEdit *produce_date = nullptr;
    QLineEdit *valid_date = nullptr;
    QComboBox *handler = nullptr;
    QLineEdit *remark = nullptr;
};

class deleteGoods:public QDialog
{
    Q_OBJECT
public:
    deleteGoods();
    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *goods_date_title = nullptr;
    QLabel *goods_name_title = nullptr;
    QLabel *goods_input_number_title = nullptr;
    QLabel *goods_input_unit_price_title = nullptr;
    QLabel *goods_input_price_title = nullptr;
    QLabel *goods_output_number_title = nullptr;
    QLabel *goods_output_unit_price_title = nullptr;
    QLabel *goods_output_price_title = nullptr;
    QLabel *goods_unit_title = nullptr;
    QLabel *goods_produce_date_title = nullptr;
    QLabel *goods_valid_date_title = nullptr;
    QLabel *goods_handler_title = nullptr;
    QLabel *goods_remark_title = nullptr;
    QLabel *goods_date = nullptr;
    QLabel *goods_name = nullptr;
    QLabel *goods_input_number = nullptr;
    QLabel *goods_input_unit_price = nullptr;
    QLabel *goods_input_price = nullptr;
    QLabel *goods_output_number = nullptr;
    QLabel *goods_output_unit_price = nullptr;
    QLabel *goods_output_price = nullptr;
    QLabel *goods_unit = nullptr;
    QLabel *goods_produce_date = nullptr;
    QLabel *goods_valid_date = nullptr;
    QLabel *goods_handler = nullptr;
    QLabel *goods_remark = nullptr;
};

class addIncomeMoney:public QDialog
{
    Q_OBJECT
public:
    addIncomeMoney();

    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *money_date = nullptr;
    QLabel *money_amount = nullptr;
    QLabel *money_pay_name = nullptr;
    QLabel *money_pay_money = nullptr;
    QLabel *money_income_name = nullptr;
    QLabel *money_income_unit_money = nullptr;
    QLabel *money_handler = nullptr;
    QLabel *money_remark = nullptr;
    QSpinBox *date_year = nullptr;
    QSpinBox *date_month = nullptr;
    QSpinBox *date_day = nullptr;
    QDoubleSpinBox *amount = nullptr;
    QLineEdit *pay_name = nullptr;
    QDoubleSpinBox *pay_money = nullptr;
    QLineEdit *income_name = nullptr;
    QDoubleSpinBox *income_unit_moeny = nullptr;
    QComboBox *handler = nullptr;
    QLineEdit *remark = nullptr;
};

class addExpendMoney:public QDialog
{
    Q_OBJECT
public:
    addExpendMoney();

    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *money_date = nullptr;
    QLabel *money_amount = nullptr;
    QLabel *money_pay_name = nullptr;
    QLabel *money_pay_money = nullptr;
    QLabel *money_income_name = nullptr;
    QLabel *money_income_unit_money = nullptr;
    QLabel *money_handler = nullptr;
    QLabel *money_remark = nullptr;
    QSpinBox *date_year = nullptr;
    QSpinBox *date_month = nullptr;
    QSpinBox *date_day = nullptr;
    QDoubleSpinBox *amount = nullptr;
    QLineEdit *pay_name = nullptr;
    QDoubleSpinBox *pay_money = nullptr;
    QLineEdit *income_name = nullptr;
    QDoubleSpinBox *income_unit_money = nullptr;
    QComboBox *handler = nullptr;
    QLineEdit *remark = nullptr;
};

class modifyMoney:public QDialog
{
    Q_OBJECT
public:
    modifyMoney();

    QGridLayout *g_layout = nullptr;
    QHBoxLayout *h_layout = nullptr;
    QPushButton *confirm = nullptr;
    QLabel *money_date = nullptr;
    QLabel *money_amount = nullptr;
    QLabel *money_pay_name = nullptr;
    QLabel *money_pay_money = nullptr;
    QLabel *money_income_name = nullptr;
    QLabel *money_income_unit_money = nullptr;
    QLabel *money_handler = nullptr;
    QLabel *money_remark = nullptr;
    QSpinBox *date_year = nullptr;
    QSpinBox *date_month = nullptr;
    QSpinBox *date_day = nullptr;
    QDoubleSpinBox *amount = nullptr;
    QLineEdit *pay_name = nullptr;
    QDoubleSpinBox *pay_money = nullptr;
    QLineEdit *income_name = nullptr;
    QDoubleSpinBox *income_unit_money = nullptr;
    QComboBox *handler = nullptr;
    QLineEdit *remark = nullptr;
};

#endif // FUNCTIONBUTTON_H
