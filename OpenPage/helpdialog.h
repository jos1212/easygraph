#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

class helpDialog : public QWidget
{
public:
    helpDialog(QWidget* parent);
    QLabel* helpDescription;
    QPushButton* backButton;
    QGridLayout* layout;
};

#endif // HELPDIALOG_H
