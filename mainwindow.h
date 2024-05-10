#include "sto.h"
#include<queue>
#include<QFileDialog>
#include<QMessageBox>
#include<QTimer>
#include<QString>
#include<QEventLoop>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Sto *storage;
    QEventLoop *eventloop;
    bool if_exentloop_in;
    bool goto_flag;
    node* goto_line;
    bool quit_signal;
    bool run_clear_flag;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void display(Sto *store);
    void display_code(Sto *store);
    void display_tree(Sto *store);
    void display_tree_help(node *tmp);
    void display_result(Sto *store);
    void display_result_help(Sta *s,int,Sto* sto);
    void direct_handle(Sta *sta);
    void load_handle();
    bool tree_error(node *tmp);
    void clear_handle();
    void clear_run_count(Sto*storage);
    void add_run_count(Sto* storage);
    void delete_run_count(Sto*storage);
    std::queue<exp_node*> tree_copy(std::queue<exp_node*> a);
signals:
    void input_end();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
