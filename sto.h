#include"sta.h"
#include<QObject>
#include<QString>
#include<QDebug>
#include<QMainWindow>
#include "ui_mainwindow.h"
#ifndef STO_H
#define STO_H


struct node: public QObject{
    Q_OBJECT
public:
    node(QString in, int line, node *n = nullptr):input(in), next(n),linenum(line){

        error_flag = false;
        type = in.section(' ',1,1);
        QString l = in.section(' ',0,0);
        int firstSpace = in.indexOf(' ');
        QString s = in.mid(firstSpace + 1);
        if(type == "REM"){statement = new Rem();}
        else if(type == "LET"){statement = new Let();}
        else if(type == "PRINT"){statement = new Print();}
        else if(type == "INPUT"){statement = new Input();}
        else if(type == "GOTO"){statement = new Goto();}
        else if(type == "IF"){statement = new If();}
        else if(type == "END"){statement = new End();}
        connect(statement,&Sta::error_signal,[=](){
            error_flag = true;
        });
        statement->gen_tree(s);


    };
    node(){};
    node *next;
    int linenum;
    Sta *statement;
    QString input;
    QString type;
    bool error_flag;
signals:

};
class Sto:public QObject
{
     Q_OBJECT
public:

    node *head ;
    void codeChange(QString change);
    void insert(int line ,QString i);
    void remove(int line);
    bool search(int line);
    void exitChange(int line ,QString i,QString s);
    void display_code(){};
    void commandRun(QString com_type,QString command);
    Sto(){
        head = new node();
        head->next = nullptr;
    };

signals:
    void direct_signal(Sta* sta);
    void run_signal();
    void help_signal();
    void quit_signal();
    void load_signal();
    void clear_signal();
};





#endif // STO_H
