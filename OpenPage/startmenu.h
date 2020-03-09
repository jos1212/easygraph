#ifndef STARTMENU_H
#define STARTMENU_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QStatusBar>
#include <QSizePolicy>
#include <QDockWidget>
#include <QPalette>
#include "helpdialog.h"
#include "aboutdialog.h"
#include "firstpage.h"
#include <QStackedLayout>
#include <QStackedWidget>
#include <QStyle>
#include <QComboBox>
#include <QPixmap>


class startMenu;


class startMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit startMenu(QWidget *parent = nullptr);
    ~startMenu();

private:

public:
//    QPushButton* newFile;
//    QPushButton* openFile;
//    QComboBox* recentFile;
    QPushButton* helpButton;
    QPushButton* aboutButton;

//    QGridLayout* GridLayout;
//    //QHBoxLayout* HelpAboutLay;

//    QLabel* description;
//    QSizePolicy sizePolicy;
public:
    firstPage* central;
    QStackedWidget* stackedWidget;
    helpDialog* helpMenu;
    aboutDialog* aboutMenu;

public slots:
    void changeToHelpMenu();
    void changeToAboutMenu();
    void goHome();
};

#endif //STARTMENU_H
