#include<QObject>
#include<QString>
#include<vector>
#include<QtMath>
#include<QDebug>
#include<stdexcept>
#include<custom_exception.h>

#ifndef EXP_H
#define EXP_H

enum op_t{SUM,SUB,MUL,DIV,MOD,INDEX,PRE_BRA,POST_BRA};
enum exp_t{CONSTANT,IDENTIFIER,COMPOUND};
class Identifier;
class Compound;
class Constant;
class exp_node: public QObject{
    Q_OBJECT
public:
    //通过这三个变量调用不同派生类的gen_tree函数
    exp_node *child[2];
    exp_t type;
    QString val;
    exp_node(){
        child[0] = child[1] = nullptr;
    }
    virtual exp_node* gen_exp(QString in) = 0;
    int exp_result();
    bool search_var(QString var,int &a);
    bool isnumber(QString in);//判断是否第一个数为负数
    bool is_total_negtive(QString in);
    int lastnum(QString in);//找到负数的最后一个数字的后一位
signals:
    void error_signal();
};

class Constant:public exp_node
{
public:
    Constant():exp_node(){type = CONSTANT;}
    exp_node *gen_exp(QString in);
};

class Identifier:public exp_node
{
public:
    Identifier():exp_node(){type = IDENTIFIER;}
    exp_node *gen_exp(QString in);
};

class Compound:public exp_node
{
public:
    Compound():exp_node(){type = COMPOUND;}
    exp_node *gen_exp(QString in);
    exp_node* com_handle(std::vector<QString> a,std::vector<int> op_pos,QString in);
};


#endif // EXP_H
