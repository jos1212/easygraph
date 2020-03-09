#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QStyle>
#include <QCheckBox>
#include <QDebug>
#include <QColorDialog>
#include <QMouseEvent>



class PopUpWindow;


class PopUpWindow : public QDialog
{
    Q_OBJECT

public:
     class MyQCheckBox;
     class MyQColorCheckBox;
     void hideEvent(QHideEvent *); // this is an overwritten function that it inherits from qwidet
public slots:
    void activate();
    void only_both();
    void only_x();
    void only_y();
    void only_custom();
    void only_theme();
    void only_no();

signals:
    void window_hidden();

public:
    explicit PopUpWindow(QWidget *parent = nullptr);
    ~PopUpWindow();
    QLineEdit* text_lineEdit;
    QLabel* text_label;
    QPushButton* setTextBut;
    QPushButton* revertBut;
    QVBoxLayout* popup_layout;
    QCheckBox* x_checkbox;
    QCheckBox* y_checkbox;
    QCheckBox* both_checkbox;
    MyQCheckBox* no_checkbox;
    MyQColorCheckBox* custom_checkbox;
    MyQCheckBox* theme_checkbox;
    QColorDialog* color_picker;

    QColor get_choice_color() const;

public slots:
    void open();
    void update_color_state(int new_state);
    void update_axis_state(int new_state);
    void update_choice_color(QColor new_choice);


private:
    int color_state; // 1 for no, 2 for custom, 3 for theme, -1  at decleration
    int axis_state; // 1 for x, 2 for y, 3 for both, -1  at decleration
    QColor choice;
};

class PopUpWindow::MyQCheckBox: public QCheckBox{

public:

    MyQCheckBox(QString new_text);

    virtual void mousePressEvent(QMouseEvent *event) {
        if( Qt::Unchecked == this->checkState()){
            emit released();
            this->setCheckState(Qt::Checked);
        }
    }

};

class PopUpWindow::MyQColorCheckBox: public QCheckBox{

public:

    MyQColorCheckBox(QString new_text);

    virtual void mousePressEvent(QMouseEvent *event) {
        if( Qt::Unchecked == this->checkState()){
            this->setCheckState(Qt::Checked);
        }
          emit released();
    }

};

#endif // POPUPWINDOW_H
