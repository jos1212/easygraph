#include "startmenu.h"
#include <QPixmap>

startMenu::startMenu(QWidget *parent) :QMainWindow(parent) {

    this->resize(800, 600);
    this->setFixedSize(800,600);

    QPixmap bkgnd(":/Images/EGHomeScreen.PNG"); //Does this have memory leaks?
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    setFont(QFont("Times New Roman"));
   // setStyleSheet({"background-image: /Images/EasyGraph_StartMenu_Background.jpg"});
/*    QFont defaultFont("Times New Roman", 20, QFont::Normal, false);
    setFont(defaultFont);
    sizePolicy = QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred); //set size of window and set size of font and label
    sizePolicy.setVerticalStretch(2);*/

    setWindowTitle("EasyGraph");

    central = new firstPage();
    helpMenu = new helpDialog(nullptr);
    aboutMenu = new aboutDialog();

//Add Widgets to stackedWidget
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(central);
    stackedWidget->addWidget(helpMenu);
    stackedWidget->addWidget(aboutMenu);

    stackedWidget->setCurrentIndex(0);

    setCentralWidget(stackedWidget);

    aboutButton = new QPushButton("About");
    helpButton = new QPushButton("Help");

    statusBar()->addWidget(helpButton);
    statusBar()->addWidget(aboutButton);

    setStyleSheet(styleSheets);

    this->setWindowIcon(QIcon(":/Images/egicon_icon (1).png"));


    connect(helpButton, SIGNAL(clicked()), this, SLOT(changeToHelpMenu()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(changeToAboutMenu()));
    connect(helpMenu->backButton, SIGNAL(clicked()), this, SLOT(goHome()));
    connect(aboutMenu->backButton, SIGNAL(clicked()), this, SLOT(goHome()));
}

startMenu::~startMenu()
{
}

void startMenu::changeToHelpMenu()
{
   stackedWidget->setCurrentIndex(1);
}

void startMenu::changeToAboutMenu()
{
    stackedWidget->setCurrentIndex(2);
}

void startMenu::goHome()
{
    stackedWidget->setCurrentIndex(0);
}
