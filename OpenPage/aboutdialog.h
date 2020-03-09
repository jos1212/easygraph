#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

class aboutDialog : public QWidget
{
public:
    aboutDialog();
    QLabel* aboutLabel;
    QPushButton* backButton;
    QGridLayout* layout;
};

#endif // ABOUTDIALOG_H
