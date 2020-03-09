#include "table.h"
#include "ui_mainwindow.h"


// 33 by 10
Table::Table(QWidget *parent) : QMainWindow(parent), row(45), col(16), current_col(0), color_change(0), color_brightness(175), color_theme(175),changing_header(nullptr){ // intilize the rows and cols to known values
    //color themes 175 chameleon, 51 tropical brust , 295 cotten candy
    table = new MyQTable(row,col); // create a new table
    popup = new PopUpWindow();



    // this makes it blue when you click a cell, I like blue
    //table->setStyleSheet("QTableView {selection-background-color: pink;}");


    // this is the GOOD code. This sets the minimum size of a cell, and automatically resizes the cell size to the size of text
    // a minimum size must be set for if not,a cell wiht one number shrinks to one number wide - not ideal
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setMinimumSectionSize(60);


    //header = table->horizontalHeader();
    //header = new MyQHeader(table->horizontalHeader());
    header = table->horizontalHeader();

    this->setCentralWidget(table);

    // first we get the column you clicked
    QObject::connect( this->header , SIGNAL( sectionDoubleClicked(int) ), this, SLOT( set_current_col(int) ) );

    //then we go and check its previous color desinction
    QObject::connect( this->header , SIGNAL( sectionDoubleClicked(int) ), this, SLOT( find_color_state(int) ) );
    QObject::connect( this , SIGNAL( update_color_state(int) ), this->popup, SLOT( update_color_state(int) ) );
    // and here its previous axis distinction
    QObject::connect( this->header , SIGNAL( sectionDoubleClicked(int) ), this, SLOT( find_axis_state(int) ) );
    QObject::connect( this , SIGNAL( update_axis_state(int) ), this->popup, SLOT( update_axis_state(int) ) );

    // opening, activating, and putting focus on the text bar of the main window
    QObject::connect( this->header , SIGNAL( sectionDoubleClicked(int) ), this->popup, SLOT( open() ) );
    QObject::connect( this->header , SIGNAL( sectionDoubleClicked(int) ), this->popup->text_lineEdit, SLOT( setFocus() ) );

    //calling update header when the now 'apply' button is pressed
    QObject::connect( this->popup->setTextBut ,SIGNAL( clicked() ), this, SLOT( update_header() ) );

    // clears the text box in the popupwindow when it closes
    QObject::connect( this->popup ,SIGNAL( destroyed() ), this->popup->text_lineEdit, SLOT( clear() ) );

    // connects the revert button to the revert function
    QObject::connect( this->popup->revertBut ,SIGNAL( clicked() ), this, SLOT( revert_header() ) );

    // these two connects close the popup window if either of the buttons are pressed
    QObject::connect( this->popup->setTextBut ,SIGNAL( clicked() ), this->popup, SLOT( hide() ) );
    QObject::connect( this->popup->revertBut ,SIGNAL( clicked() ), this->popup, SLOT( hide() ) );
}




void Table::update_header(){ // changes the text and adds the new

    bool found = false; // im gonna use this to track if the user is renamning a already changed header
    QColor old_color; // this is goign to store the old color of a redifned header
    QString new_text; // this is going to be the text we use
    int new_color_state = 3;// 3 is the 'default' for these values, corresponds to theme color -> also the defualt even if everything is unchecked
    int old_color_state = 3;
    int old_axis_state = 3;
    int new_axis_state = -1;  // by defualt, none of the boxes are checked
    int old_key = -1;

     // have to deal with the case of renaming something first
     for(size_t i = 0; i < new_headers.size(); i ++){ // goinig through the new_headers vector

         if(current_col == new_headers[i]->column() ){ // if another header of the same column already exits

             old_color = new_headers[i]->backgroundColor(); // we grab the color of the header befor we get rid of it
             old_color_state = new_headers[i]->get_color_state();
             old_axis_state = new_headers[i]->get_axis_state();
             new_text = new_headers[i]->text(); // set new_text to old text
             old_key = new_headers[i]->get_key();
             new_headers.erase(new_headers.begin() + i); // we remove the old ele,ennt from our vector
             found = true; // this vlaue was already found so we set found to true;
             break;

         }
     }


     //determing the correct text to use
     if(popup->text_lineEdit->text() == ""){ // if the text is left blank
         if(!found){ // if this is a new header, we will just name it the number of its column
         new_text = QString::number(current_col + 1);
         }
     }

     else{  // if the user entered new text, hit em with it
     new_text = popup->text_lineEdit->text();
     }

     if(this->popup->custom_checkbox->isChecked()){ // if this isnt a renamed one, by defualt its no color, so all we have to check is if its custom
         new_color_state = 2;
     }
     if(this->popup->no_checkbox->isChecked()){ // if this isnt a renamed one, by defualt its no color, so all we have to check is if its custom
         new_color_state = 1;
     }

     if(this->popup->x_checkbox->isChecked()){ // if this isnt a renamed one, by defualt its no color, so all we have to check is if its custom
         new_axis_state = 1;
     }
     if(this->popup->y_checkbox->isChecked()){ // if this isnt a renamed one, by defualt its no color, so all we have to check is if its custom
         new_axis_state = 2;
     }
     if(this->popup->both_checkbox->isChecked()){ // if this isnt a renamed one, by defualt its no color, so all we have to check is if its custom
         new_axis_state = 3;
     }

     // here we can hanlde the case of a different axis state
     if(new_axis_state != 3){ // in this case, the header is not supposed to be in both of the dropdowns
         if(new_axis_state != old_axis_state){ // here we check if the user changed thier preferences
             if(new_axis_state == 1){ // if x value
                  emit list_remove_signal(old_key);
             }
             if(new_axis_state == 2){ // if y value
                  emit dropdown_remove_signal(old_key);
             }
             if(new_axis_state == -1){ // in this case, the user wants it not shown on either dropdown
                 if(old_axis_state != 3){
                     if(old_axis_state == 1){ // if it was a x value
                          emit dropdown_remove_signal(old_key);
                     }
                     if(old_axis_state == 2){ // if it was a y value
                          emit list_remove_signal(old_key);
                     }
                 }
                 else{ // the case of going from neither to both
                     emit list_remove_signal(old_key);
                     emit dropdown_remove_signal(old_key);
                 }

             }
         }
     }


     changing_header = new QTableHeaderWidget( new_text, current_col, new_color_state, new_axis_state );
    if(found){ // if its an old header
        changing_header->set_key(old_key);
     }
    else{ // if its new
     key_changer(*changing_header); // asssignign a new unique key
    }

     //new_headers.push_back(changing_header); // now we add the new header to our vector of headers
     table->setHorizontalHeaderItem(current_col,changing_header); // and we change the text

        // this block make sure the color values are approiate
        // note if all boxes are unchecked the defulat (theme) will be used
    if( !(this->popup->no_checkbox->isChecked()) || !(this->popup->custom_checkbox->isChecked()) ){ // if we want a theme color, we will grab one before declaring a color
        color_changer(*this); // setTextColor
    }

    QColor new_color = QColor().fromHsv(color_theme + color_change,color_brightness, 175); // declaring the new color

     if(this->popup->no_checkbox->isChecked()){ // if we want no color
         new_color = QColor(0,0,0,0);  // both colors are set to nuthing
         old_color = QColor(0,0,0,0);
     }
     if(this->popup->custom_checkbox->isChecked()){ // if we want custom color
        new_color = this->popup->get_choice_color();
        found = false; // this is an easy way to take care of the case, where the custom color is simply changed from one to another
     }

        // if the header is renamed, we do our changes here

     if(found && (old_color_state == new_color_state) ){ // if the color state hasnt changed we will use the old color
         table->horizontalHeaderItem(current_col)->setBackgroundColor(old_color);
         changing_header->set_color(old_color);


         // this block is just telling the dropdowns the final result and to use the old color
         if(this->popup->both_checkbox->isChecked() || this->popup->x_checkbox->isChecked()){ // if both or x
             emit transfer_dropdown_x(new_text, old_color,changing_header->get_key() ); // and we add this element with this color to the dropdown
         }
         if( this->popup->both_checkbox->isChecked() || this->popup->y_checkbox->isChecked()){ // if both or y
             emit transfer_dropdown_y(new_text, old_color, changing_header->get_key()); // and we add this element with this color to the dropdown
         }


         }
     else{              // in all other cases we assign a new color
         table->horizontalHeaderItem(current_col)->setBackgroundColor(new_color);

         changing_header->set_color(new_color);

         // this block is just telling the dropdowns the final result and to use the old color
         if(this->popup->both_checkbox->isChecked() || this->popup->x_checkbox->isChecked()){ // if both or x
             emit transfer_dropdown_x(new_text, new_color,changing_header->get_key() ); // and we add this element with this color to the dropdown
         }
         if( this->popup->both_checkbox->isChecked() || this->popup->y_checkbox->isChecked()){ // if both or y
             emit transfer_dropdown_y(new_text, new_color, changing_header->get_key()); // and we add this element with this color to the dropdown
         }

     }

     new_headers.push_back(changing_header); // now we add the new header to our vector of headers


}

// simply sets the current_col parameter, this is used to keep track fo what column the user last clicked
void Table::set_current_col(int new_col){ // when the user double clicks a header, this sets col = to that
    current_col = new_col;
}

void Table::set_x_values(int key){

    x_values.clear(); // getting rid of whatever we had

     int row = 0;
     for(size_t i = 0; i < new_headers.size(); i ++){ // going through all the heaers

         if(key == new_headers[i]->get_key()){ // if we find one that matches

             while(true){ // now we go grab the data values from the correct column

                 QTableWidgetItem* item = table->item(row,new_headers[i]->column()); // we check if its empty
                 if(!item){ // this is to make sure we get a vlid item
                      break;
                 }
                 if(item->text() == ""){ // double check
                     break; // i dont trust the earlier break to catch somethigns so this is a bcakup
                 }

             x_values.push_back(item->text()); // we are getting all the text in the column
             row++;

         }
     }
}

}

void Table::set_values(std::vector<int> keys){



    for(size_t i = 0; i < y_values.size(); i ++){ // for each vector in the y_values vecotors
         y_values[i].clear(); //we clead
    }
    y_values.resize(0); // then we can resize it to zero

    for(size_t k = 0; k < keys.size(); k ++){ // for each key given

        for(size_t i = 0; i < new_headers.size(); i ++){ // we go through the headers

            int row = 0;

            if(keys[k] == new_headers[i]->get_key()){ // and if we find a match

                 std::vector<QString> data; // we go ahaed and creat a vector to hold the data

                     while(true){ // we go throught the entire column to get its values

                    QTableWidgetItem* item = table->item(row,new_headers[i]->column()); // we check if its empty
                    if(!item){
                         break;
                    }
                    if(item->text() == ""){
                        break; // i dont trust the earlier break to catch somethigns so this is a bcakup
                    }

                    data.push_back(item->text());
                    row++;
                    //delete item;
                     }
                  y_values.push_back(data); // we are getting all the text in the column
                  break;
                 }
            }

        }

}

void Table::revert_header(){

    // of this header was already intilized (the most likely case) we have to find it and do clean up

    for(size_t i = 0; i < new_headers.size(); i ++){ // going thtough all the headers
        if(current_col == new_headers[i]->column() ){ // if another header of the same column already exits
            emit dropdown_remove_signal(new_headers[i]->get_key()); // get rid of the old one from the drop down
            emit list_remove_signal(new_headers[i]->get_key()); // get rid of the old one from the drop down
            new_headers.erase(new_headers.begin() + i); // we remove the old ele,ennt from our vector
            break;
        }
    }

        // here we simply  make a new header that mimics the orignal and set it
     changing_header = new QTableHeaderWidget( QString::number(current_col + 1), current_col, 1,3 );
     table->setHorizontalHeaderItem(current_col,changing_header);
}

void Table::print_values(){     // this function is just used for debugging and os conencted to the generate button in mainwindow.cpp


    for( size_t i = 0; i < y_values.size(); i ++){// for each vector
        qDebug() << y_values[i]; // print out the value
    }

 qDebug() << "print values called";

}

void::Table::find_color_state(int color_state){ // when we double click we get its color state, theme as default (3)

color_state = 3; // setting to theme as default

for(size_t i = 0; i < new_headers.size(); i ++){ // for  each header
    if(current_col == new_headers[i]->column() ){ // if another header of the same column already exits
        color_state = new_headers[i]->get_color_state();
        break;
    }
}

emit update_color_state(color_state); // then we send it to the popup window

}


void Table::set_color_theme(QColor new_color){
    color_theme = new_color.hue();
    color_brightness = new_color.hsvSaturation();
    if(color_brightness < 75){ // if the brightness gets too low
        color_brightness = 75;
    }
    color_changer.reset();
}

void::Table::find_axis_state(int axis_state){ // when we double click we get its color state, theme as default (3)

axis_state = 3; // setting to theme as default

for(size_t i = 0; i < new_headers.size(); i ++){ // for each header
    if(current_col == new_headers[i]->column() ){ // if another header of the same column already exits
        axis_state = new_headers[i]->get_axis_state();
        break;
    }
}




emit update_axis_state(axis_state); // then we send it to the popup window

}

Table::col_functor::col_functor(): flag(1), inc(25) , mult(1), quick_counter(0), first_run(true), var(0) {

}

void Table::col_functor::reset(){
    // this function is called everytime a new theme color is choosen
    // the color fucntor must be set back to a new center and here we do that
    // if this fucntion wasnt done, then the col_functor would continue on its previous cycle, we want it to start fresh on a new center
    flag = 1;
    mult = 1;
    quick_counter = 0;
    first_run = true;
}

void Table::col_functor::operator()(Table &T){ // resposible for updateing the color automaitcally

    if( T.color_brightness >= 25){ // this is a catch all, if we use 15 colors, we stop giving more, cause we ran out
                                    // we start at 175, we - 50 each  5 colors, we stop at 75,thats 15 colors we get

    // crikey! so this is how this code works. we start with a color_theme which is intilized in the table default constructor.
    // from there we want incremnts of 25 around it. for example if the theme is 175, we want the colors 175, 200, 150, 225, 125 in that order
    // all this code is get thoose values



    if(quick_counter == 2){ // ever time we get two values we have to up the multiplier to get further away from the starter color
        mult++;
        quick_counter = 0;
    }

    if(mult == 3){ // after 5 colors we start again at center but..
        mult = 1;
        first_run  = true; // starting at the center color again
        var += 10; // straying off the center a little for more variation
        T.color_brightness -= 2*inc; // this time we make it dimmer
    }


    if(first_run){  // on the first run of a new five we dont want to maniputlate anything
         T.color_change = var; // setting at the center, for later iteratiosn the cneter drifts
        first_run = false;
    }
    else{

    T.color_change = flag*mult*inc + var; // flag is for the direction from center and mult is the distance, inc is predetmerined

    // now we have to adress the possibility of going out of spectrum. the limit is 360, so i want to let it wrap abck to zero

    // theme is 300, change is 70

  if( (T.color_theme + T.color_change) > 360){ // if the color is out of spectrum
       T.color_change  = -T.color_theme +  ( (T.color_theme + T.color_change) - 360 );
   }

  if( (T.color_theme + T.color_change) < 0){ // if below spectrum
       T.color_change  = 360 + T.color_theme + T.color_change;
   }

    quick_counter++;
    flag = -flag; // changing direction

    }
    }

}

Table::key_functor::key_functor(): value(0){
}

void Table::key_functor::operator()(QTableHeaderWidget &header){
    header.set_key(value);
    value++;
}


Table::~Table(){
delete changing_header;
delete popup;
}

Table::QTableHeaderWidget::QTableHeaderWidget(QString new_text, int new_col, int new_color_state, int new_axis_state): QTableWidgetItem(new_text), col(new_col), key(-1), color_state(new_color_state), axis_state(new_axis_state) {
// note by default, all headers have a key of negative 1, all keys assigned are 0 or positive
}

int Table::QTableHeaderWidget::column() const{
    return col;
}

void Table::QTableHeaderWidget::set_key(int new_key) {
    key = new_key;
}

int Table::QTableHeaderWidget::get_key() const{
    return key;
}


int Table::QTableHeaderWidget::get_color_state() const{
    return color_state;
}

int Table::QTableHeaderWidget::get_axis_state() const{
    return axis_state;
}

Table::QTableHeaderWidget::~QTableHeaderWidget(){

}

Table::MyQTable::MyQTable(int row, int col): QTableWidget (row,col){}

Table::MyQTable::~MyQTable(){

}


//Table::MyQHeader::MyQHeader(QHeaderView *parent): QHeaderView(Qt::Horizontal,parent){}

//Table::MyQHeader::MyQHeader(QHeaderView &copy): QHeaderView(copy.orientation(), copy.parentWidget()) {}


void Table::copy()
{


    QList<QTableWidgetSelectionRange> totalRange = table->selectedRanges(); //Grabs it all
    QTableWidgetSelectionRange range = totalRange.first(); //Returns a pointer to the first dude
    QString toBeCopied; //String that will be copied

    for (int i = 0; i < range.rowCount(); ++i) { //While we iterate through the range's rows we add a newline if its not the first row
        if (i > 0)
            toBeCopied += "\n";
        for (int j = 0; j < range.columnCount(); ++j) { //While we iterate through ranges column count we add a tab char
            if ( j > 0)
                toBeCopied += "\t";
            if (table->getItem(range.topRow() + i, range.leftColumn() + j) != nullptr){
            toBeCopied += table->getItem(range.topRow() + i, range.leftColumn() + j)->text();
            }//add the items text to the string
        }
    }
    QApplication::clipboard()->setText(toBeCopied); //put text on clipboard

    QString copied = QApplication::clipboard()->text();
    qDebug() << copied.length(); //This was for debugging purposes
}

void Table::paste()
{
    QList<QTableWidgetSelectionRange> totalRange = table->selectedRanges();
    QTableWidgetSelectionRange range = totalRange.first();

    QString toBePasted = QApplication::clipboard()->text();
    QStringList rowTexts = toBePasted.split('\n'); // splits it into rows in a string list

    int rows = rowTexts.count();
    int cols = rowTexts.first().count('\t') +1;

    if (range.rowCount() * range.columnCount() != 1 && (range.rowCount() != rows || range.columnCount() != cols))
    {
        return;
    }

    for (int i = 0; i < rows; ++i)
    {
        QStringList columnTexts = rowTexts[i].split('\t'); //split the row texts into column texts for pasting
        for (int j = 0; j < cols; ++j)
        {
            int rowCount = range.topRow() + i;
            int colCount = range.leftColumn() + j;
            if (rowCount < row && colCount < col)
                table->createNewItem(rowCount, colCount, columnTexts[j]);
        }
    }
}

QTableWidgetItem* Table::MyQTable::getItem(int row, int column)
{
    return item(row, column);
}

QTableWidgetItem* Table::MyQTable::createNewItem(int row, int column, const QString& itemData)
{
    QTableWidgetItem* newItem = getItem(row, column);
    if (newItem == nullptr)
    {
        newItem = new QTableWidgetItem();
        setItem(row, column, newItem);
    }
    newItem->setText(itemData);
    //return newItem;
}

Table::MyQTable* Table::getTable() const {
    return table;
}

Table::QTableHeaderWidget* Table::createNewHeaderWidget(int ccolumn, int ccolor, int caxis, QString cname)
{
    Table::QTableHeaderWidget* newHeader = new Table::QTableHeaderWidget(cname, ccolumn, ccolor, caxis);
    return newHeader;
}

int Table::getRows() const
{
    return this->row;
}
int Table::getCols() const
{
    return this->col;
}

void Table::addYVals (int row, QString name)
{
    if (row >= y_values.size()) {
        std::vector<QString> newVec;
        newVec.push_back(name);
        y_values.push_back(newVec);
    }
    else {
        y_values[row].push_back(name);
    }
}

void Table::reinstall_headers(){

      for(size_t i = 0; i < new_headers.size(); i ++){

          if(new_headers[i]->get_axis_state() == 1  ||  new_headers[i]->get_axis_state() == 3){
           emit transfer_dropdown_x(new_headers[i]->text(),  new_headers[i]->get_color(),new_headers[i]->get_key()); // and we add this element with this color to the dropdown
          }

          if(new_headers[i]->get_axis_state() == 2  ||  new_headers[i]->get_axis_state() == 3){
           emit transfer_dropdown_y(new_headers[i]->text(), new_headers[i]->get_color(), new_headers[i]->get_key()); // and we add this element with this color to the dropdown
          }

      }
}

void Table::QTableHeaderWidget::set_color(QColor new_color){
    my_color = new_color;

}
QColor Table::QTableHeaderWidget::get_color() const{
    return  my_color;
}

/*
 * deletes the data from a selected range
 * @params none
 * @return none
 */
void Table::delData()
{
    QList<QTableWidgetSelectionRange> totalRange = getTable()->selectedRanges(); //Grabs it all
    QTableWidgetSelectionRange range = totalRange.first(); //Returns a pointer to the first dude

    for (int i = 0; i < range.rowCount(); ++i) { //While we iterate through the range's rows we add a newline if its not the first row;
        for (int j = 0; j < range.columnCount(); ++j) { //While we iterate through ranges column count we add a tab char
            if (getTable()->getItem(range.topRow() + i, range.leftColumn() + j) != nullptr){
                getTable()->getItem(range.topRow() + i, range.leftColumn() + j)->setText("");
            }
        }
    }
}

/*
 * copies then deletes the data
 * @params none
 * @return none
 */
void Table::cut()
{
    copy(); //simple
    delData();
}


