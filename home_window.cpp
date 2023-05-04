#include "home_window.h"
#include "sql_window.h"
#include "ui_home_window.h"

home_window::home_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::home_window)
{
    ui->setupUi(this);
}

home_window::~home_window()
{
    delete ui;
}


void home_window::on_sql_button_clicked()
{
    sql_window sqlwindow;
    sqlwindow.setModal(true);
    sqlwindow.exec();
}

