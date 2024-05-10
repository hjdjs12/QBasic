#include "sto.h"
#include "ui_mainwindow.h"

void Sto::insert(int l, QString i){
    if(head->next == nullptr){
        head->next = new node(i,l,nullptr);
        return;
    }
    node *infront = head;
    node *tmp = head->next;
    while(tmp != nullptr && tmp->linenum < l){tmp = tmp->next;infront = infront->next;}
    infront->next = new node(i,l,tmp);
}
void Sto::codeChange(QString i){
    i = i.trimmed();
    int j = 0;
    while( i[j] == ' '){
        j++;
    }
    i = i.mid(j);
    QString line = i.section(' ',0,0);
    int firstSpace = i.indexOf(' ');
    QString statement = i.mid(firstSpace + 1);
    int ll = statement.indexOf(' ');
//    if(ll == -1){return;}
    QString ss =  statement.section(' ',0,0);

    bool successflag = false;
    int l = line.toInt(&successflag);
    if(successflag){
        if(l > 1000000 || l <=0 ){ throw Custom_exception("line_invalid");}
        if(search(l)){
            if(firstSpace == -1){remove(l);return;}
            if(ss != "LET"
                    && ss != "PRINT"
                    && ss != "GOTO"
                    && ss != "REM"
                    && ss != "INPUT"
                    && ss != "IF"
                    && ss != "END"){return;}
            exitChange(l,i,statement);
        }
        else{
            if(firstSpace==-1){return;}
            else{insert(l,i);}
        }
    }
    else if(line == "PRINT" | line == "LET" | line == "INPUT"){
        Sta * a ;
        if(line == "PRINT"){a = new Print();a->type = PRINT;}
        else if(line == "LET"){a = new Let();a->type = LET;}
        else if(line == "INPUT"){a = new Input();a->type = INPUT;}
        a->element = i;
        a->gen_tree(i);
        emit direct_signal(a);
    }
    else if(line == "RUN" | line == "CLEAR" | line == "HELP" | line == "QUIT" | line == "LOAD"){
        commandRun(line,i);
    }
}

void Sto::exitChange(int line, QString i, QString s){
        node *infront = head;
        node *tmp = head->next;
        while(tmp != nullptr){
            if(tmp->linenum == line){break;}
            tmp = tmp->next;
            infront = infront->next;
        }
       node *newnode = new node(i, line,tmp->next);
       infront->next = newnode;
       delete tmp;

}
void Sto::remove(int line){
    node *infront = head;
    node *tmp = head->next;
    while(tmp != nullptr){
        if(tmp->linenum == line){break;}
        tmp = tmp->next;
        infront = infront->next;
    }
    infront->next = tmp->next;
    tmp->next = nullptr;
    delete tmp;
}
bool Sto::search(int line){
    if(head->next == nullptr){return false;}
    node *tmp = head->next;
    while(tmp != nullptr){
        if(tmp->linenum == line){return true;}
        if(tmp->linenum > line){break;}
        tmp = tmp->next;
    }
    return false;
}
void Sto::commandRun(QString com_type, QString command){
    if(com_type == "RUN"){
        emit run_signal();
    }
    else if (com_type == "CLEAR"){
        emit clear_signal();
    }
    else if(com_type == "HELP"){
        emit help_signal();
    }
    else if(com_type == "LOAD"){
        emit load_signal();
    }
    else if(com_type == "QUIT"){
        emit quit_signal();
    }
}
