#include "popupwindow.h"
#include <QDialog>

PopUpWindow::PopUpWindow(QWidget *parent) :
    QDialog(parent), color_state(-1), axis_state(-1), choice(QColor())
{

    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::Popup);

    this->resize(300, 250); // width by height
    this->setFixedSize(300,250);

    popup_layout = new QVBoxLayout;

        // this block is for the text portion
    text_lineEdit = new QLineEdit("");
    text_label = new QLabel("New Name: ");
    QHBoxLayout* text_layout = new QHBoxLayout();
    text_layout->addWidget(text_label);
    text_layout->addWidget(text_lineEdit);
    QWidget *text_widget = new QWidget();
    text_widget->setLayout(text_layout);

        // this block is for the buttons
    setTextBut = new QPushButton("Apply");
    setTextBut->setMinimumWidth(150); // to make it a little bigger
    revertBut = new QPushButton("Revert");
    QHBoxLayout* button_layout = new QHBoxLayout();
    button_layout->addWidget(revertBut);
    button_layout->addWidget(setTextBut);
    QWidget *button_widget = new QWidget();
    button_widget->setLayout(button_layout);

        // this block is delcaring and creating the layout for the check box values
    x_checkbox = new QCheckBox("x value");
    y_checkbox = new QCheckBox("y value");
    both_checkbox = new QCheckBox("both");
    QHBoxLayout* checkbox_values_layout = new QHBoxLayout();
    checkbox_values_layout->addStretch(1); // spacer
    checkbox_values_layout->addWidget(x_checkbox);
    checkbox_values_layout->addStretch(1); // spacer
    checkbox_values_layout->addWidget(y_checkbox);
    checkbox_values_layout->addStretch(3); // spacer
    checkbox_values_layout->addWidget(both_checkbox);
    checkbox_values_layout->addStretch(1); // spacer
    QWidget *checkbox_values_widget = new QWidget();
    checkbox_values_widget->setLayout(checkbox_values_layout);

    no_checkbox = new MyQCheckBox("no color");
    custom_checkbox = new MyQColorCheckBox("custom color");
    theme_checkbox = new MyQCheckBox("theme color");
    QHBoxLayout* checkbox_color_layout = new QHBoxLayout();

    checkbox_color_layout->addWidget(no_checkbox);
    checkbox_color_layout->addWidget(custom_checkbox);
    checkbox_color_layout->addStretch(3); // spacer
    checkbox_color_layout->addWidget(theme_checkbox);

    QWidget *checkbox_color_widget = new QWidget();
    checkbox_color_widget->setLayout(checkbox_color_layout);


    color_picker = new QColorDialog();
    color_picker->setOption(QColorDialog::DontUseNativeDialog);
    color_picker->setWindowFlags(Qt::Popup);
    color_picker->setCurrentColor(QColor().fromHsv(175,175,175));



    popup_layout->addWidget(text_widget);
    popup_layout->addWidget(checkbox_color_widget);
    popup_layout->addWidget(checkbox_values_widget);
    popup_layout->addWidget(button_widget);



    this->setLayout(popup_layout);

    setStyleSheet( "QPushButton:hover:!pressed {"
                   "background-color: #4682b4;"
                   "}"
                   "QPushButton {"
                   "background-color: #000080;"
                   "color: white;"
                   "border-style: outset;"
                   "border-width: 2px;"
                   "border-radius: 10px;"
                   "padding: 6px; "
                   "}"
                   "PopUpWindow {"
                   "font-size: 36}"
                   );

    // this connect takes the signal custom window_hidden and calls clear on the line edit
    // essentiall when the popupwindow is hidden the text in the line edit is cleared
    QObject::connect( this,SIGNAL( window_hidden() ), this->text_lineEdit, SLOT( clear() ) );

    QObject::connect( this->text_lineEdit,SIGNAL( returnPressed() ), this->setTextBut, SLOT( click() ) );

    // this block makes it so when one box is checked the others are unchecked
    QObject::connect( this->both_checkbox, SIGNAL( released() ), this, SLOT( only_both() ) );
    QObject::connect( this->x_checkbox,SIGNAL( released() ), this, SLOT( only_x() ) );
    QObject::connect( this->y_checkbox,SIGNAL( released() ), this, SLOT( only_y() ) );

    QObject::connect( this->theme_checkbox, SIGNAL( released() ), this, SLOT( only_theme() ) );
    QObject::connect( this->no_checkbox,SIGNAL( released() ), this, SLOT( only_no() ) );
    QObject::connect( this->custom_checkbox,SIGNAL( released() ), this, SLOT( only_custom() ) );

    QObject::connect( this->custom_checkbox,SIGNAL( released() ), this->color_picker, SLOT( show() ) );
    QObject::connect( this->color_picker,SIGNAL( colorSelected(QColor) ), this, SLOT( update_choice_color(QColor) ) );

}

void::PopUpWindow::open(){

    this->show();
}

void PopUpWindow::only_both(){
    x_checkbox->setCheckState(Qt::Unchecked);
    y_checkbox->setCheckState(Qt::Unchecked);
}
void PopUpWindow::only_y(){
    x_checkbox->setCheckState(Qt::Unchecked);
    both_checkbox->setCheckState(Qt::Unchecked);
}
void PopUpWindow::only_x(){
    both_checkbox->setCheckState(Qt::Unchecked);
    y_checkbox->setCheckState(Qt::Unchecked);
}

void PopUpWindow::only_custom(){
    theme_checkbox->setCheckState(Qt::Unchecked);
    no_checkbox->setCheckState(Qt::Unchecked);
}
void PopUpWindow::only_no(){
    theme_checkbox->setCheckState(Qt::Unchecked);
    custom_checkbox->setCheckState(Qt::Unchecked);
}
void PopUpWindow::only_theme(){
    no_checkbox->setCheckState(Qt::Unchecked);
    custom_checkbox->setCheckState(Qt::Unchecked);
}

void PopUpWindow::update_color_state(int new_state){
    color_state = new_state;

    if(color_state == 1){
        no_checkbox->setCheckState(Qt::Checked);
        this->only_no();
    }
    if(color_state == 2){
        custom_checkbox->setCheckState(Qt::Checked);
        this->only_custom();
    }
    if(color_state == 3){
        theme_checkbox->setCheckState(Qt::Checked);
        this->only_theme();
    }

}

void PopUpWindow::update_axis_state(int new_axis){
    axis_state = new_axis;

    if(axis_state == 1){
        x_checkbox->setCheckState(Qt::Checked);
        this->only_x();
    }
    if(axis_state == 2){
        y_checkbox->setCheckState(Qt::Checked);
        this->only_y();
    }
    if(axis_state == 3){
        both_checkbox->setCheckState(Qt::Checked);
        this->only_both();
    }

}


// this allows the user to click between header elements seemlesly
void PopUpWindow::activate(){
    this->activateWindow(); // activate window just makes the text bar active
}

void PopUpWindow::update_choice_color(QColor new_choice){
    choice = new_choice;
}

QColor PopUpWindow::get_choice_color() const{
    return choice;
}

// this emits the dummy signal window_hidden when hide even is set
void PopUpWindow::hideEvent(QHideEvent *){
    emit window_hidden();
}


PopUpWindow::~PopUpWindow(){
}

PopUpWindow::MyQCheckBox::MyQCheckBox(QString new_text): QCheckBox(new_text) {}

PopUpWindow::MyQColorCheckBox::MyQColorCheckBox(QString new_text): QCheckBox(new_text) {}
