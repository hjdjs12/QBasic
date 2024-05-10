#include "sta.h"

extern std::vector<QString> VAR_STORE;
extern std::vector<int> VAL_STORE;
extern std::vector<int> VAR_USE_STORE;
Sta::Sta(){
    run_count = sat_count = unsat_count = 0;
    error_flag = false;
}
void Rem::gen_tree(QString s) {
            element = "REM";
            int firstSpace = s.indexOf(' ');
            QString comments = s.mid(firstSpace + 1);
            child.push(Rem_gen_exp(comments));//单独定义，不是expressionA};
}
exp_node* Rem::Rem_gen_exp(QString comments){
    exp_node *a = new Identifier;
    a->val = comments;
    return a;
}

void Let::gen_tree(QString s)  {
    //设置element的值，将产生的expression树压入child
            element = "LET =";
            int firstSpace = s.indexOf(' ');
            s = s.mid(firstSpace + 1);
            int firstsign = s.indexOf('=');
            QString variable = s.left(firstsign);
            QString expression = s.right(s.length() - firstsign - 1);
            if(firstsign == -1 || variable.trimmed().isEmpty() || expression.trimmed().isEmpty() || a.gen_exp(variable) == nullptr || b.gen_exp(expression) == nullptr){
                emit error_signal();
                error_flag = true;
                return;
            }
            child.push(a.gen_exp(variable));
            child .push(b.gen_exp(expression));

}

void Print::gen_tree(QString s){
    element = "PRINT";
    int firstspace = s.indexOf(' ');
    QString expression = s.mid(firstspace + 1);
    if(expression.trimmed().isEmpty() || firstspace == -1 || b.gen_exp(expression) == nullptr){emit error_signal();error_flag = true;return;}
    child.push(b.gen_exp(expression));
};

void Input::gen_tree(QString s){
    element = "INPUT";
    int firstSpace = s.indexOf(' ');
    QString variable = s.mid(firstSpace + 1);
    if(firstSpace == -1 || variable.trimmed().isEmpty() || a.gen_exp(variable) == nullptr){emit error_signal();error_flag = true;return;}
    child.push(a.gen_exp(variable));
};
void Goto::gen_tree(QString s){
    element = "GOTO";
    int firstSpace = s.indexOf(' ');
    QString con = s.mid(firstSpace + 1);
    if(firstSpace == -1 || con.trimmed().isEmpty() || c.gen_exp(con) == nullptr){emit error_signal();error_flag = true;return;}
    child.push(c.gen_exp(con));
};
void If::gen_tree(QString s){
    element = "IF THEN";
    int firstSpace = s.indexOf(' ');
    s = s.mid(firstSpace + 1);
    int firstop = ((s.indexOf('=') + 1) | (s.indexOf('>') + 1) | (s.indexOf('<') + 1)) - 1;
    int then_lpos = s.indexOf("THEN");
    int then_rpos = then_lpos + 3;
    QString then_left = s.left(then_lpos);
    QString then_right = s.right(s.length() - then_rpos -1);
    if(then_left.trimmed().isEmpty()
            || then_right.trimmed().isEmpty()
            || b.gen_exp(then_left.left(firstop)) == nullptr
            || a.gen_exp(s.at(firstop)) == nullptr
            || b.gen_exp(then_left.right(then_left.length()-firstop -1)) == nullptr
            || c.gen_exp(then_right) == nullptr
            || firstSpace == -1
            || then_lpos == -1){emit error_signal();error_flag = true;return;}
    child.push(b.gen_exp(then_left.left(firstop)));
    child.push(a.gen_exp(s.at(firstop)));
    child.push(b.gen_exp(then_left.right(then_left.length()-firstop -1)));
    child.push(c.gen_exp(then_right));
};
void End::gen_tree(QString s){
    element = "END";
}
//变量定义和值的改变
void Sta::val_change(QString val, int result){
    std::vector<QString>::iterator it = VAR_STORE.begin();
    std::vector<int>::iterator at = VAL_STORE.begin();
    std:: vector<int>::iterator ct = VAR_USE_STORE.begin();
    int find_flag = false;
    for(;it != VAR_STORE.end();it++,at++){
        if(*it == val){*at = result;(*ct)++;find_flag = true;}
    }
    if(!find_flag){
        VAR_STORE.push_back(val);
        VAL_STORE.push_back(result);
        VAR_USE_STORE.push_back(1);
    }
}
bool Sta::if_judge_condition(exp_node* left_exp, QString op, exp_node* right_exp){
    int left_val = left_exp->exp_result();
    int right_val = right_exp->exp_result();
    if(op == ">"){return left_val > right_val;}
    else if(op == "<"){return left_val < right_val;}
    else if(op == "="){return left_val == right_val;}
}
