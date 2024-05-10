#include "mainwindow.h"
#include "ui_mainwindow.h"

extern std::vector<QString> VAR_STORE;
extern std::vector<int> VAL_STORE;
extern std::vector<int> VAR_USE_STORE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1300,1300);
    this->setWindowTitle("GuiBasic");
    storage = new Sto();
    eventloop = new QEventLoop(this);
    goto_flag = false;
    run_clear_flag = false;
    if_exentloop_in = false;
    connect(ui->clear,&QPushButton::clicked,[=](){
        if(eventloop->isRunning()){run_clear_flag = true;}
        qDebug()<<run_clear_flag;
        clear_handle();
        if(eventloop->isRunning()){
            eventloop->exit();
            if_exentloop_in = false;
            ui->tree_display->clear();
//            delete eventloop;
//            eventloop = new QEventLoop(this);
        }

    });
    connect(storage,&Sto::direct_signal,this,&MainWindow::direct_handle);
    connect(storage,&Sto::run_signal,[=](){
       clear_run_count(storage);
       if(eventloop->isRunning()){
           eventloop->exit();
           if_exentloop_in = false;
           ui->tree_display->clear();
//           delete eventloop;
//           eventloop = new QEventLoop(this);
       }
      display(storage);
      VAR_STORE.clear();
      VAL_STORE.clear();
//      if(if_exentloop_in){eventloop->exit();if_exentloop_in = false;}
    });
    connect(storage,&Sto::clear_signal,[=](){
        clear_handle();
    });
    connect(storage,&Sto::load_signal,[=](){
        load_handle();
    });
    connect(storage,&Sto::quit_signal,[=](){
        this->close();
    });
    connect(storage,&Sto::help_signal,[=](){
        QFile file("C:/Users/86191/Desktop/QBasic/HELP.txt");
        if(!file.open(QIODevice::ReadOnly | QFile::Text)){
            QMessageBox::warning(this,"Warning","Failed to open file:" + file.errorString());
            return ;
        }
        QTextStream stream(&file);
        while(!stream.atEnd()){
            QString text = stream.readLine();
            ui->result_display->append(text);
        }
    });
    connect(ui->run,&QPushButton::clicked,[=](){
        clear_run_count(storage);
        if(eventloop->isRunning()){
            eventloop->exit();
            if_exentloop_in = false;
            ui->tree_display->clear();
//            delete eventloop;
//            eventloop = new QEventLoop(this);
        }
        display(storage);
        VAR_STORE.clear();
        VAL_STORE.clear();
        VAR_USE_STORE.clear();
//        if(if_exentloop_in){eventloop->exit();if_exentloop_in = false;}
//        clear_run_count(storage);
    });
    connect(ui->load,&QPushButton::clicked,[=](){
        load_handle();
    });
    connect(ui->input,&QLineEdit::returnPressed,[=](){
            QString change = ui->input->text();
            ui->input->clear();
            if(change.mid(0,3) == " ? "){
                int input = (change.mid(3)).toInt();
                Sta*tmp = storage->head->statement;
                tmp->val_change(VAR_STORE.back(),input);
                emit input_end();
            }
            else{
                try{
                    storage->codeChange(change);
                    display_code(storage);
                }
                catch(Custom_exception e){
                    if(e.what() == "line_invalid"){ ui->result_display->append( "Error:The input line is not valid!");}
                    else if(e.what() == "syntax_error"){ ui->result_display->append( "Error:There is a syntax error in the statement ");}
                    else if(e.what() == "var_invalid" ){ ui->result_display->append( "Error:use of undeclared variable");}
                }
            }

    });
    connect(this,&MainWindow::input_end,[=](){
        if(eventloop->isRunning()){
            eventloop->exit();
            if_exentloop_in = false;
            ui->tree_display->clear();
//            delete eventloop;
//            eventloop = new QEventLoop(this);
        }
    });

}
void MainWindow::clear_handle(){
    delete storage;
    storage = new Sto();
    quit_signal = true;
    ui->code_display->clear();
    ui->result_display->clear();
    ui->tree_display->clear();
    if(ui->input->text().length() >= 2 && ui->input->text()[1] == '?'){ui->input->clear();}

    connect(storage,&Sto::direct_signal,this,&MainWindow::direct_handle);
    connect(storage,&Sto::run_signal,[=](){
       if(eventloop->isRunning()){
           eventloop->exit();
           if_exentloop_in = false;
           ui->tree_display->clear();
//           delete eventloop;
//           eventloop = new QEventLoop(this);
       }
      display(storage);
      VAR_STORE.clear();
      VAL_STORE.clear();
      VAR_USE_STORE.clear();
//      if(if_exentloop_in){eventloop->quit();if_exentloop_in = false;}
//      clear_run_count(storage);
    });
    connect(storage,&Sto::clear_signal,[=](){
        clear_handle();
    });
    connect(storage,&Sto::load_signal,[=](){
        load_handle();
    });
    connect(storage,&Sto::quit_signal,[=](){
        this->close();
    });
    connect(storage,&Sto::help_signal,[=](){
        QFile file("C:/Users/86191/Desktop/QBasic/HELP.txt");
        if(!file.open(QIODevice::ReadOnly | QFile::Text)){
            QMessageBox::warning(this,"Warning","Failed to open file:" + file.errorString());
            return ;
        }
        QTextStream stream(&file);
        while(!stream.atEnd()){
            QString text = stream.readLine();
            ui->result_display->append(text);
        }
    });
    goto_flag = false;
    std::vector<QString>().swap(VAR_STORE);
    std::vector<int>().swap(VAL_STORE);
    std::vector<int>().swap(VAR_USE_STORE);
}
void MainWindow::direct_handle(Sta *sta){
    display_result_help(sta,-1,this->storage);
}
void MainWindow::display(Sto *store){
    display_code(store);
    display_tree(store);
    ui->tree_display->clear();
    display_result(store);
    add_run_count(store);
    display_tree(store);
    delete_run_count(store);
    if(run_clear_flag){ui->tree_display->clear();run_clear_flag = false;}
}
void MainWindow::delete_run_count(Sto *storage){
    node*tmp = storage->head->next;
    while(tmp != nullptr){
        if(tmp->statement->type == REM){}
        else if(tmp->statement->type != IF){
            tmp->statement->element = tmp->statement->element.trimmed();
            int lastspace = tmp->statement->element.lastIndexOf(' ');
            tmp->statement->element = tmp->statement->element.left(lastspace);
        }
        else {
            tmp->statement->element = tmp->statement->element.trimmed();
            int lastspace = tmp->statement->element.lastIndexOf(' ');
            tmp->statement->element = tmp->statement->element.left(lastspace);
            int last_secondspace = tmp->statement->element.lastIndexOf(' ');
            tmp->statement->element = tmp->statement->element.left(last_secondspace);
        }
        tmp = tmp->next;
    }
}
void MainWindow::add_run_count(Sto *storage){
    node*tmp = storage->head->next;
    while(tmp != nullptr){
        if(tmp->statement->type == REM){}
        else if(tmp->statement->type != IF){tmp->statement->element += " " + QString::number(tmp->statement->run_count);}
        else {
            tmp->statement->element += " " + QString::number(tmp->statement->sat_count) + " " + QString::number(tmp->statement->unsat_count);
        }
        tmp = tmp->next;
    }
}
void MainWindow::clear_run_count(Sto *storage){
    node*tmp = storage->head->next;
    while(tmp != nullptr){
        tmp->statement->run_count = 0;
        tmp->statement->sat_count = 0;
        tmp->statement->unsat_count = 0;
        tmp = tmp->next;
    }
}
void MainWindow::load_handle(){
    QString filename = QFileDialog::getOpenFileName(this,"open");
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Failed to open file:" + file.errorString());
        return ;
    }
    QTextStream stream(&file);
    try{
    while(!stream.atEnd()){
        QString text = stream.readLine();
        storage->codeChange(text);
    }
    }
    catch(Custom_exception e){
        if(e.what() == "line_invalid"){ ui->result_display->append( "Error:The input line is not valid!");}
        else if(e.what() == "syntax_error"){ ui->result_display->append( "Error:There is a syntax error in the statement ");}
        else if(e.what() == "var_invalid" ){ ui->result_display->append( "Error:use of undeclared variable");}
    }
    display_code(storage);
}

void MainWindow::display_code(Sto *store){
    node * tmp = store->head->next;
    ui->code_display->clear();
    while(tmp != nullptr){
        ui->code_display->append(tmp->input);
        tmp = tmp->next;
    }
}

void MainWindow::display_tree(Sto *store){
    node * tmp = store->head->next;
    ui->tree_display->clear();
    while(tmp != nullptr){
        if(tree_error(tmp)){tmp->error_flag = true;}
        display_tree_help(tmp);
        tmp = tmp->next;
    }
}

bool MainWindow::tree_error(node *tmp){
    std::queue<exp_node*> copy = tree_copy(tmp->statement->child);
    while(!copy.empty()){
        exp_node* test = copy.front();
        if(((test->val).trimmed()).isEmpty()){return true;}
        if(test->child[0] != nullptr){copy.push(test->child[0]);}
        if(test->child[1] != nullptr){copy.push(test->child[1]);}
        copy.pop();
    }
    return false;
}
void MainWindow::display_tree_help(node*tmp){
    QString first_line = (QString::number(tmp->linenum)) + " " + tmp->statement->element;
    if(tmp->error_flag ){
        first_line = QString::number(tmp->linenum) + " Error"  ;
        ui->tree_display->append(first_line);
        return ;
    }

    ui->tree_display->append(first_line);
    std::queue<exp_node*> copy = tree_copy(tmp->statement->child);
    QString space = "    ";
    if(tmp->statement->type == LET){
        int num = 0;
        std::vector<QString>::iterator it = VAR_STORE.begin();
        std::vector<int>::iterator at = VAL_STORE.begin();
        std:: vector<int>::iterator ct = VAR_USE_STORE.begin();
        for(;it != VAR_STORE.end();it++,at++,ct++){
            if(*it == copy.front()->val){num = *ct;}
        }
        QString use_num = space + copy.front()->val + " " + QString::number(num);
        ui->tree_display->append(use_num);
        copy.pop();
    }
    while(!copy.empty()){
        int num = copy.size();
        int count = 0;
        while(count < num){
            exp_node *aa = copy.front();
            ui->tree_display->append(space + aa->val);
            if(aa->child[0] != nullptr){copy.push(aa->child[0]);}
            if(aa->child[1] != nullptr){copy.push(aa->child[1]);}
            copy.pop();
            count++;
        }
        space += "    ";
    }
}
std::queue<exp_node*> MainWindow::tree_copy(std::queue<exp_node*> a){
    std::queue<exp_node*> copy;
    while(!a.empty()){
        copy.push(a.front());
        a.pop();
    }
    return copy;
}

void MainWindow::display_result(Sto *store){
    node*tmp = store->head->next;
    ui->result_display->clear();
    quit_signal = false;
    try{
    while(tmp != nullptr){
        if(tmp->error_flag == true){throw Custom_exception("syntax_error");}
        if(quit_signal){return;}
        display_result_help(tmp->statement,tmp->linenum,store);
        if(goto_flag){tmp = goto_line;goto_flag = false;}
        else{tmp = tmp->next;}
    }
    }
    catch(Custom_exception e){
        if(e.what() == "line_invalid"){ ui->result_display->append( "Error:The input line is not valid!");}
        else if(e.what() == "syntax_error"){ ui->result_display->append( "Error:There is a syntax error in the statement ");}
        else if(e.what() == "var_invalid" ){ ui->result_display->append( "Error:use of undeclared variable");}
    }
}
void MainWindow::display_result_help(Sta *tmp,int linenum,Sto*sto){
    if(tmp->error_flag == true){throw Custom_exception("syntax_error");}
    tmp->run_count++;
    state_t  cur_type = tmp->type;
    if(cur_type == REM ){
        return;
    }
    else if(cur_type == LET){
        std::queue<exp_node*> copy = tree_copy(tmp->child);
        exp_node*var = copy.front();
        copy.pop();
        exp_node*result = copy.front();
        tmp->val_change(var->val,result->exp_result());
    }
    else if(cur_type == PRINT){
        std::queue<exp_node*> copy = tree_copy(tmp->child);
        exp_node *result = copy.front();
        ui->result_display->append(QString::number(result->exp_result()));
        qDebug() << "aaa";
    }
    else if(cur_type == INPUT){
        ui->input->setText(" ? ");
        ui->input->setCursorPosition(3);
        std::queue<exp_node*> copy = tree_copy(tmp->child);
        exp_node*var=copy.front();
        tmp->val_change(var->val,0);
        if(!eventloop->isRunning()){
            if_exentloop_in = true;
            eventloop->exec();
        }
//可以在上面returnpressed处实现;
    }
    else if(cur_type == GOTO){
        std::queue<exp_node*> copy = tree_copy(tmp->child);
        int line = ((copy.front())->val).toInt();
        if(sto->search(line)){
            node *abc = sto->head->next;
            while(abc!=nullptr){
                if(abc->linenum == line){goto_line = abc;goto_flag = true;;break;}
                abc = abc->next;
            }
        }
        else{
            QString format = "Error:line %1.The line number behind 'GOTO' does not exist!";
            QString message = format.arg(QString::number(linenum));
            ui->result_display->append(message);
        }//还要报个错误
    }
    else if(cur_type == IF){
        std::queue<exp_node*> copy = tree_copy(tmp->child);
        exp_node* left_exp = copy.front();
        copy.pop();
        QString op = copy.front()->val;
        copy.pop();
        exp_node* right_exp = copy.front();
        copy.pop();
        if(tmp->if_judge_condition(left_exp,op,right_exp)){
            tmp->sat_count++;
            node *abc = sto->head->next;
            while(abc!=nullptr){
                if(abc->linenum == (copy.front()->val).toInt()){goto_line = abc;goto_flag = true;return;}
                abc = abc->next;
            }
            QString format = "Error:line %1.The line number behind 'THEN' does not exist!";
            QString message = format.arg(copy.front()->val.toInt());
            ui->result_display->append(message);
        }
        else{
            tmp->unsat_count++;
            return;
        }
    }
    else if(cur_type == END){
        quit_signal = true;
        return ;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
