// This project is by Bryan Josloff and Charlie Stoksik     Winter 2019

#include "MainWindow/mainwindow.h"
#include "OpenPage/startmenu.h"
#include <QApplication>
#include <QSplashScreen>
#include <QFont>
#include <QObject>


/// SAVING to save the table data we can go model then databse and then clone the data base, to learn more search QSqlTableModel


///  GENERAL MACHINARY (rundown of the 'brain' of the code, AKA all the connects that drive everything)
// when the user double clicks a header, a signal calls a popupwindow to appear -> in table.cpp
// when the user enters a value in the popupwindow, a signal calls update_header, a slot in table.cpp -> in table.cpp
// update_header changes the text of the selected header, and emits the signal update_dropdown -> in table.cpp
// update_dropdown adds this new header to the dropdown menu -> in mainwindow.cpp
// when the user clicks generate chart, the signal update chart is called -> in mainwidnow.cpp
// updatechart call setvalues on the table, set values in a table.cpp function and fills the vector values wiht the appropaite data



int main(int argc, char *argv[]) // current : update chart fucntion in mainwindow.cpp
{

    QApplication a(argc, argv);
    //QApplication::setStyle(QStyleFactory::create("Fusion")); // this supposedly adds color support to windows on qt 5.12

    // creatinf a default font
    QFont default_font("Helvetica", 11, QFont::Normal, false);
    a.setFont(default_font);

    // creating our splash screen
    QSplashScreen* splash  = new QSplashScreen();
    splash->setPixmap(QPixmap(":/Images/egicon_icon (1).png"));
    splash->show();

    // creating the start menu
    startMenu w;
    splash->finish(&w);
    delete splash;
    w.show();

    // creating the mainwindow
    MainWindow m;


    // connecting the start menu to the main window and allowing the new file and open buttons to function
    QObject::connect( w.central->newFile, SIGNAL( clicked() ), &m, SLOT( show() ) );
    QObject::connect( w.central->newFile, SIGNAL( clicked() ), &w, SLOT( hide() ) );
    QObject::connect(w.central->openFile, SIGNAL( clicked()), &m, SLOT(open()));



    return a.exec();
}
