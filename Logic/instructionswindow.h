#ifndef INSTRUCTIONSWINDOW_H
#define INSTRUCTIONSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QString>
#include <QPixmap>
#include <QPalette>

class InstructionsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InstructionsWindow(QWidget *parent = nullptr);

signals:

public slots:

private:
    QPixmap* backgroundPicture;

};

#endif // INSTRUCTIONSWINDOW_H
