#ifndef SQL_WINDOW_H
#define SQL_WINDOW_H

#include <QDialog>
#include <QTime>

namespace Ui {
class sql_window;
}

class sql_window : public QDialog
{
    Q_OBJECT

public:
    explicit sql_window(QWidget *parent = nullptr);
    ~sql_window();

private slots:
    void on_run_button_clicked();

    void on_sql_query_textChanged();

private:
    Ui::sql_window *ui;
};

#endif // SQL_WINDOW_H
