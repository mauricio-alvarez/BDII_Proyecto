#ifndef HOME_WINDOW_H
#define HOME_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class home_window; }
QT_END_NAMESPACE

class home_window : public QMainWindow
{
    Q_OBJECT

public:
    home_window(QWidget *parent = nullptr);
    ~home_window();

private slots:
    void on_sql_button_clicked();

private:
    Ui::home_window *ui;
};
#endif // HOME_WINDOW_H
