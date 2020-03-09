#ifndef FIRSTPAGE_H
#define FIRSTPAGE_H
#include <QPushButton>
#include <QComboBox>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QString>
#include "Logic/stylesheets.h"

class firstPage : public QWidget
{
public:
    firstPage();
    QPushButton* newFile;
    QPushButton* openFile;

    QGridLayout* GridLayout;

    QLabel* description;
    QSizePolicy sizePolicy;

    QPixmap* backgroundImage;
};

#endif // FIRSTPAGE_H
