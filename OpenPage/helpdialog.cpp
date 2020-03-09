#include "helpdialog.h"

helpDialog::helpDialog(QWidget* parent) : QWidget(parent)
{
    helpDescription = new QLabel;
    helpDescription->setText(" \n \n \n \n \n \n \n For help, open a new project and under the about tab at the top click instructions "
                             "\n "
                             "Or for further instruction visit www.google.com/search");
    helpDescription->setStyleSheet("color: black");
    backButton = new QPushButton("Back");

    layout = new QGridLayout;
    layout->addWidget(helpDescription, 0, 0, 1, 3);
    layout->addWidget(backButton, 3, 3, 1, 1);

    setLayout(layout);
}
