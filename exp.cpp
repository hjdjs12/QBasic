#include "exp.h"
//找寻变量对应的值
std::vector<QString> VAR_STORE;
std::vector<int> VAL_STORE;
std::vector<int> VAR_USE_STORE;
bool exp_node::search_var(QString var, int &a){
    var = var.trimmed();
    std::vector<QString>::iterator it = VAR_STORE.begin();
    std::vector<int>::iterator at = VAL_STORE.begin();
    std:: vector<int>::iterator ct = VAR_USE_STORE.begin();
    for(;it != VAR_STORE.end();it++,at++,ct++){
        if(*it == var){a = *at;(*ct)++;return true;}
    }
    return false;
}
int exp_node::exp_result(){
    if(type == CONSTANT){return (val.toInt());}
    else if(type == COMPOUND){
        if(val == "+"){
            return child[0]->exp_result() + child[1]->exp_result();
        }
        else if(val == "-"){return child[0]->exp_result() - child[1]->exp_result();}
        else if(val == "*"){return child[0]->exp_result() * child[1]->exp_result();}
        else if(val == "/"){return child[0]->exp_result() / child[1]->exp_result();}
        else if(val == "MOD"){return child[0]->exp_result() % child[1]->exp_result();}
        else if(val == "**"){return qPow(child[0]->exp_result(),child[1]->exp_result());}
    }
    else if(type == IDENTIFIER){
        int a = 0;
        qDebug() << a;
        if(search_var(val,a)){
            return a;
        }
        else{
            throw Custom_exception("var_invalid");
        }

    }
    else{

    }
}
exp_node* Constant::gen_exp(QString in){
    in = in.trimmed();
    exp_node *node = new Constant;
    if(is_total_negtive(in)){
        node->val = "-" + (node->gen_exp(in.mid(in.indexOf('-') + 1 )))->val;
        node->type = CONSTANT;
    }
    else{
        int i = 0;
        for(; i < in.length(); i++){
            if(in.at(i) != ' '){break;}
        }
        QString num_beg = in.mid(i);
        int num_count = num_beg.indexOf(' ');
        QString num = num_beg.mid(0,num_count);
        node->val = num;
        node->type = CONSTANT;
    }
    return node;
}
exp_node* Identifier::gen_exp(QString in) {
    in = in.trimmed();
    exp_node *node = new Identifier();
    int i = 0;
    for(; i < in.length(); i++){
        if(in.at(i) != ' '){break;}
    }
    QString var_beg = in.mid(i);
    int var_count = var_beg.indexOf(' ');
    QString var = var_beg.mid(0,var_count);
    node->val = var;
    node->type = IDENTIFIER;
    return node;
}

bool exp_node::isnumber(QString in){
    if(in[0] == '-'){
        int i = 0;
        do{
            i++;
            if(in[i].isDigit()){return true;}
        }while(in[i] == " ");
    }
    return false;
}
int exp_node::lastnum(QString in){
    int i = 0;
    do{
        i++;
        if(in[i].isDigit()){break;}
    }while(in[i] == " ");
    do{
        i++;
        if(i == in.length()){break;}
    }
    while(in[i].isDigit());
    return i;
}

bool exp_node::is_total_negtive(QString in){
    return isnumber(in) &&lastnum(in) == in.length();
}
exp_node* Compound::gen_exp(QString in) {
   in = in.trimmed();
   if(is_total_negtive(in)){
       exp_node *a = new Constant;
       return a->gen_exp(in);
   }
   if(isnumber(in)){
       in.insert(lastnum(in),')');
       in.insert(0,'(');
   }
   bool is_not_exp1 = (in.indexOf('+') == -1)
           && (in.indexOf('-') == -1)
           && (in.indexOf('*') == -1)
           && (in.indexOf("**") == -1)
           && (in.indexOf('/') == -1)
           && (in.indexOf("MOD") == -1);
   bool is_int;
   if(is_not_exp1){
       in.toInt(&is_int);
       if(is_int){
           exp_node *a = new Constant;
           return a->gen_exp(in);
       }
       else{
           exp_node *a = new Identifier;
           return a->gen_exp(in);
       }
   }
   else{
       std::vector<QString> op;
       std::vector<int> op_pos;
       for(int i = 0; i < in.length(); i++){
           QString single_sign = in.at(i);
           QString second_sign = " ";
           QString third_sign = " ";
           if(i < in.length() - 1){second_sign = in.at(i+1);}
           if(i < in.length() - 2){third_sign = in.at(i+2);}
           if((single_sign == '+') | (single_sign == '-') | (single_sign == '/') | (single_sign == '(') | (single_sign == ')')){op.push_back(single_sign);op_pos.push_back(i);}
           else if(single_sign == '*' && second_sign == '*'){op.push_back("**");op_pos.push_back(i);i++;}
           else if(single_sign == '*' ){op.push_back("*");op_pos.push_back(i);}
           else if(single_sign == 'M' && second_sign == 'O' && third_sign == 'D'){op.push_back("MOD");op_pos.push_back(i);}
       }
       return com_handle(op,op_pos,in);
   }
}



exp_node* Compound::com_handle(std::vector<QString> op, std::vector<int> op_pos,QString in){
    exp_node *a = new Compound();
    a->type = COMPOUND;
    std::vector<int>pre_pos;
    std::vector<int>post_pos;//在op容器中的位置序号
    std::vector<QString>::iterator it = op.begin();
    std::vector<int>::iterator at = op_pos.begin();
//    std::find(op.begin(),op.end(),"(");
    int count = 0;
    for(; it != op.end(); it++, at++,count++){
        if(*it == "("){pre_pos.push_back(count);}
        if(*it == ")"){post_pos.push_back(count);}
    }
    if(pre_pos.size() != post_pos.size()){return nullptr;}
    std::reverse(post_pos.begin(),post_pos.end());
    for(at = op_pos.begin(),it = op.begin();at < op_pos.end();at++,it++){
        if(*it == "("){it += (*(post_pos.begin()) - *(pre_pos.begin()));at += (*(post_pos.begin()) - *(pre_pos.begin()));continue;}
        if(*it == "+" | *it == "-" ){
            a->val = *it;
            a->child[0] = a->gen_exp(in.left(*at));
            a->child[1] = a->gen_exp(in.right(in.length() - *at - 1));
            return a;
        }
    }
    for(at = op_pos.begin(),it = op.begin();at != op_pos.end();at++,it++){
        if(*it == "("){it += (*(post_pos.begin()) - *(pre_pos.begin()));at += (*(post_pos.begin()) - *(pre_pos.begin()));continue;}
        if(*it == "*" | *it == "/" ){
            a->val = *it;
            a->child[0] = a->gen_exp(in.left(*at));
            a->child[1] = a->gen_exp(in.right(in.length() - *at - 1));
            return a;
        }
    }
    for(at = op_pos.begin(),it = op.begin();at != op_pos.end();at++,it++){
        if(*it == "("){it += (*(post_pos.begin()) - *(pre_pos.begin()));at += (*(post_pos.begin()) - *(pre_pos.begin()));continue;}
        if(*it == "**" ){
            a->val = *it;
            a->child[0] = a->gen_exp(in.left(*at));
            a->child[1] = a->gen_exp(in.right(in.length() - *at - 2));
            return a;
        }
    }
    if((std::find(op.begin(),op.end(),"(")) == op.end()){return nullptr;}
    QString get_rid_of_bra = in.mid(op_pos[pre_pos[0]] + 1,op_pos[post_pos[0]]-op_pos[pre_pos[0]]-1);
    return a->gen_exp(get_rid_of_bra);

}
