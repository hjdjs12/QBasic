#include<queue>
#include<QDebug>
#include"exp.h"
#ifndef STA_H
#define STA_H

enum state_t{REM,LET,PRINT,INPUT,GOTO,IF,END};

class Sta:public QObject
{
    Q_OBJECT
public:

    Sta();
    //通过这三个变量调用不同派生类的gen_tree函数
    Identifier a;//变量名和比较符号
    Compound b;//复合表达式
    Constant c;//常数
    state_t type;
    int run_count;
    int sat_count;
    int unsat_count;
    std::queue<exp_node*>child;
    QString element;
    bool error_flag;
    virtual void gen_tree(QString s) = 0;//设置根节点element的值，并且将exp树的节点压入栈，child中每个节点都是一个exp表达式
    void val_change(QString val,int result);
    bool if_judge_condition(exp_node* left_exp ,QString op, exp_node* right_exp);
signals:
    void error_signal();
};
class Rem:public Sta{
public:
    Rem(){type = REM;};
    void gen_tree(QString s);
    exp_node* Rem_gen_exp(QString comments);
};

class Let:public Sta{
public:
    Let(){type = LET;};
    void gen_tree(QString s) ;
};
class Print:public Sta{
public:
    Print(){type = PRINT;};
    void gen_tree(QString s);
};
class Input:public Sta{
public:
    Input(){type = INPUT;};
    void gen_tree(QString s);
};
class Goto:public Sta{
public:
    Goto(){type = GOTO;};
    void gen_tree(QString s);
};
class If:public Sta{
public:
    If(){type = IF;}
    void gen_tree(QString s);
};

class End:public Sta{
public:
    End(){ type = END;};
    void gen_tree(QString s);
};
#endif // STA_H
