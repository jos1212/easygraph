#include "firstpage.h"

firstPage::firstPage()
{
    setStyleSheet(ComboStyle);

//    sizePolicy.setVerticalStretch(2);
//    setSizePolicy(sizePolicy);

    newFile = new QPushButton("New Project");
//    newFile->setSizePolicy(sizePolicy);
    openFile = new QPushButton("Open Existing Project");
//    openFile->setSizePolicy(sizePolicy);

    description = new QLabel;
    description->setText("");
    description->setAlignment(Qt::AlignCenter);

    GridLayout = new QGridLayout;
    GridLayout->addWidget(description, 0, 0, 1, 2);
    GridLayout->addWidget(newFile, 1, 0, 1, 1);

    GridLayout->addWidget(openFile, 1, 1, 1, 1);

    setLayout(GridLayout);

   /* setStyleSheet("QWidget { background-color: #E0E0FA}"
                    "QPushButton { background-color: #E0FFFF; color: #808080;}"
                               "QComboBox { background-color: #E0FFFF; color: #808080}"
                  "QLabel {color: #808080}");
*/
}
