#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    try{
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
//    }
//    catch(Custom_exception e){
//        if(e.what() == "line_invalid"){ qDebug()<< "Error:The input line is not valid!";}
//        else if(e.what() == "syntax_error"){ qDebug()<<"Error:There is a syntax error in the statement ";}
//        else if(e.what() == "var_invalid" ){qDebug()<<"Error:use of undeclared variable";}
//    }
}
