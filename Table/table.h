#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QMainWindow>

#include <QStringList> // to set the name of the table
#include <QList> // for the header names of the table columns
#include <QHeaderView> // this is for size and allignment stuff
#include <QDebug>
#include <QColor>
#include <vector>
#include <QClipboard>

#include "Logic/popupwindow.h"



class Table : public QMainWindow{

    friend class col_functor;
    public:  class key_functor;
     Q_OBJECT
    public: class QTableHeaderWidget;
    class MyQTable; //  custom table for charlies function stuff if he wants it
    class MyQHeader; // right click for header elements 3rd times the charm



            class col_functor {   // this is a functor used for picking color for a given theme

            public:
                col_functor();
                void operator()(Table &T);
                void reset();

            private:
                int flag;
                int inc;
                int mult;
                int quick_counter;
                bool first_run;
                int var; // this is to add some extra variation to the further iterations of colors
            };

            class key_functor {   // this is a function used to assing unique interger key values to each header // very simple

            public:
                key_functor();
                void operator()(QTableHeaderWidget &header);
                int get_value() { return value;}
                void set_value(int new_value) { value = new_value;}

            private:
                int value;
            };




public:
    explicit Table(QWidget *parent = 0);
    ~Table();
        // these vectors are select data made public so the chart can acess them
        // there isnt risk here though for the vector of pointers is const, the data is set -> cant be changed

            // use pairs to keep track if its an int or double

    std::vector<const QTableHeaderWidget*> new_headers; // list of headers
    std::vector<std::vector<QString>> y_values; // this is ALL of the y values
    std::vector<QString> x_values; // list for x values

    MyQTable* getTable() const;
    QTableHeaderWidget* createNewHeaderWidget(int ccolumn, int ccolor, int caxis, QString cname);
    //may cause memory leaks, ask Bryan/Michael
    //Charlie function for saving
    int getRows() const;
    int getCols() const;
    void addYVals (int row, QString name);

    void reinstall_headers();
     key_functor key_changer;

public slots:
    void update_header();
    void set_values(std::vector<int> keys);
    void set_x_values(int key);

    void print_values(); // just used for debugging
    void set_color_theme(QColor new_color);

    void cut();
    void paste();
    void copy();
    void delData();

private slots:
     void set_current_col(int new_col); // sets the current column the user is accessing
     void revert_header(); // called when the revert button is clicked
     void find_color_state(int color_state);
     void find_axis_state(int axis_state);



signals:
    void transfer_dropdown_y(QString,QColor,int);
    void transfer_dropdown_x(QString,QColor,int);
    void dropdown_remove_signal(int key);
    void list_remove_signal(int key);
    void update_color_state(int new_color_state);
    void update_axis_state(int new_axis_state);

private:
    int row;    // number of rows
    int col; // number of columns
    int current_col; // this value is the current column the user is accessing
    int color_change;
    int color_brightness;
    int color_theme;

    MyQTable *table;
    QHeaderView * header;
    QTableHeaderWidget *changing_header;
    PopUpWindow *popup;
    col_functor color_changer;


};


//nested class, I need this becuase header items dont store column values in QT, so i have to just add that data field
class Table::MyQTable:public QTableWidget{ // Qt doesnt have it so Ill make it myself
    Q_OBJECT

public:
    MyQTable(int row, int col);
//explicit MyQTable(QTableWidget*parent = 0);
   ~MyQTable();
    QString get_text() const; //Charlie addition
    QTableWidgetItem* createNewItem(int row, int column, const QString& itemData);
    QTableWidgetItem* getItem(int row, int column);



};

//nested class, I need this becuase header items dont store column values in QT, so i have to just add that data field
class Table::QTableHeaderWidget:public QObject, public QTableWidgetItem{ // Qt doesnt have it so Ill make it myself
    Q_OBJECT
friend class key_functor;


public:
explicit QTableHeaderWidget(QTableWidgetItem *parent = 0);
QTableHeaderWidget(QString new_text, int new_col, int new_color_state, int new_axis_state);
~QTableHeaderWidget();
int column() const;
void set_key(int new_key);
int get_key() const;
int get_color_state() const;
int get_axis_state() const;

void set_color(QColor new_color);
QColor get_color() const;



private:
int col; // this is why Im adding this, I need a data for the column
int key; // this will be a unique trait I can use to detect a header from another! who needs color!
int color_state; // keeps track of the checkbox associated with color
int axis_state;
QColor my_color;
};


/*
class Table::MyQHeader:public QHeaderView{ // Qt doesnt have it so Ill make it myself
    Q_OBJECT

public:


   explicit MyQHeader(QHeaderView *parent = 0);

     MyQHeader(QHeaderView &copy);

virtual void mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::RightButton){
           qDebug() << "right"; //emit rightClicked();
        }
        else if (event->button() == Qt::LeftButton) {
             qDebug() << "left"; //emit clicked();
        }
}

};
 */

#endif // TABLE_H
