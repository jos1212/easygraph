#include "instructionswindow.h"

/*
 * Instructions window constructor
 * Its just a pixmap
 */
InstructionsWindow::InstructionsWindow(QWidget *parent) : QWidget(parent)
{

    backgroundPicture = new QPixmap(":/Images/InstuctionsImage.PNG");
    backgroundPicture->scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, *backgroundPicture);
    this->setPalette(palette);

    setFixedSize(900, 680);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setWindowTitle("Instructions");
    setWindowIcon(QIcon(":/Images/egicon_icon (1).png"));
}
