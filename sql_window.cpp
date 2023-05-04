#include "sql_window.h"
#include "ui_sql_window.h"
#include "parser.h"

sql_window::sql_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sql_window)
{
    ui->setupUi(this);
}

sql_window::~sql_window()
{
    delete ui;
}

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(10);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void sql_window::on_run_button_clicked()
{
    auto query = ui->sql_query->toPlainText();
    QStringList queries = query.split("\n");
    SQLParser<string> parser;
    for (int i=0;i<queries.count();i++){
        vector<string> result = parser.parse(queries[i].toStdString());
        for (const auto& res: result){
            QLabel *label = new QLabel(this);
            label->setText(QString::fromStdString(res));
            ui->results->addWidget(label);
        }
        QLabel *label = new QLabel(this);
        label->setText("Ending this query...");
        ui->results->addWidget(label);
        delay();
    }
}

