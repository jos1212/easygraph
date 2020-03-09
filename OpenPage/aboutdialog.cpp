
#include "aboutdialog.h"

aboutDialog::aboutDialog()
{
    aboutLabel = new QLabel();
    aboutLabel->setText(" \n \n \n \n \n \n \n EasyGraph Inc. is a software company that caters to users needs. We believe that data should be simple and easy to see visually. \n We have implemented a simple and intuitive way for users to record their small datasets and display them with simple graphical representations. \n "
                        "EasyGraph is designed for home, business, and scholarly use. ");
    aboutLabel->setStyleSheet("color: black");
    backButton = new QPushButton();
    backButton->setText("Back");

    layout = new QGridLayout();
    layout->addWidget(aboutLabel, 0, 0, 1, 3);
    layout->addWidget(backButton, 3, 3, 1, 1);

    setLayout(layout);

}
