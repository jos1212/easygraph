#include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent){

       changesSaved = false;
       instrWindow = new InstructionsWindow(0); //sets parent ot Mainwindow

        // creating a color picker for when the user wants a new theme
       theme_picker = new QColorDialog();
       theme_picker->setOption(QColorDialog::DontUseNativeDialog);
       theme_picker->setWindowFlags(Qt::Popup);
       theme_picker->setCurrentColor(QColor().fromHsv(175,175,175));


    // creating a new central widget for the whole window and rightside widget for the right column
    QWidget *central_widget = new QWidget();
    QWidget *rightside_widget = new QWidget();


    // declaring our table and chart
    window_table = new Table();
    window_chart = new Chart();

    // creating our layouts
    QHBoxLayout *window_layout = new QHBoxLayout(); // for the whole window
    rightside_layout = new QVBoxLayout(); // just for the right side column

    // declaring the generate push button
    generate_button = new QPushButton();

    generate_button->setStyleSheet(styleSheets);
    generate_button->setText("Generate Graph!");

    // The UI
    QHBoxLayout* UI_layout = new QHBoxLayout();
    QWidget* UI_widget = new QWidget();

    QVBoxLayout* UI_layout_right = new QVBoxLayout();
    QWidget* UI_widget_right = new QWidget();

    QVBoxLayout* UI_layout_left = new QVBoxLayout();
    QWidget* UI_widget_left = new QWidget();

    // Declaring and dressing up our combo box for x values
    dropdown_x = new QComboBox();
    //dropdown_x->setStyleSheet("QComboBox QAbstractItemView{background:QColor(36,36,36);}");//) "QComboBox { background-color: #242424; }");//

    dropdown_x->addItem("Default",-2);
    //dropdown_x->addItem("Select Data",-2); // select data is going to be for custom selections like click and dragigng on the table
    dropdown_x->setEditable(true);
    QLabel* x_values = new QLabel();
    x_values->setText(" x values");
    dropdown_x->setMinimumWidth(230); // keepin my boy big


    // Declaring and dressing up our combo box for chart types
    dropdown_chart_types = new QComboBox();
    dropdown_chart_types->setMinimumWidth(230); // keepin my boy big
    dropdown_chart_types->addItem("Linear"); // we add the choices for types of graph
    dropdown_chart_types->addItem("Scatter");
    dropdown_chart_types->addItem("Bar");
    dropdown_chart_types->addItem("Pie");
    dropdown_chart_types->setEditable(true);

    QLabel* chart_types = new QLabel();
    chart_types->setText(" Chart Type");


    UI_layout_left->addWidget(x_values);
    UI_layout_left->addWidget(dropdown_x);
    UI_layout_left->addStretch(1);

    UI_layout_left->addWidget(chart_types);
    UI_layout_left->addWidget(dropdown_chart_types);
    UI_layout_left->addStretch(1);

    UI_layout_left->addWidget(generate_button);
    UI_widget_left->setLayout(UI_layout_left);


    y_list = new QListWidget();
    QPalette pal =   y_list->palette();
    QColor bgColor = pal.color(QPalette::Window);
    QColor fgColor = pal.color(QPalette::Text);
            // Set the item selection color to be the background color of the list widget.
    y_list->setStyleSheet(QString("QListWidget:item:selected:active { background: %1;} ""QListWidget:item:selected:active {color: %2; }").arg(bgColor.name()).arg(fgColor.name()));

    QLabel* y_label = new QLabel("y values");

    UI_layout_right->addWidget(y_label);
    UI_layout_right->addWidget(y_list);
    UI_widget_right->setLayout(UI_layout_right);

    UI_layout->addWidget(UI_widget_left);
    UI_layout->addWidget(UI_widget_right);
    UI_layout->addStretch(1); // spacer
    UI_widget->setLayout(UI_layout);


    // this is the GOOD code, this two blocks make it so the table is 1/10th larger than the rightside
    QSizePolicy leftside_policy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    leftside_policy.setHorizontalStretch(10);
    window_table->setSizePolicy(leftside_policy);

    QSizePolicy rightside_policy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    rightside_policy.setHorizontalStretch(9);
    rightside_widget->setSizePolicy(rightside_policy);

    // here we are making a layout for the right side, of the graph and the UI below it
    // then we will add this to rightside wiget and that widget to the window layout

    rightside_layout->addWidget(window_chart);
    rightside_layout->addWidget(UI_widget);
    rightside_widget->setLayout(rightside_layout); //@test


    // we add our table and the right side widget to the layout and apply iy through the central widget
    window_layout->addWidget(window_table);
    window_layout->addWidget(rightside_widget);

    //now we simply set the central widget with the window layout to be the centralwidget of this window
    central_widget->setLayout(window_layout);
    this->setCentralWidget(central_widget);

    // asthetics
    this->resize(1300,800);
    this->setWindowTitle("Easy Graph");

    /*
    createActions();

    createMenus();
         //createContextMenu();
    createToolBars();
    createStatusBar();
         //readSettings();
    this->setWindowIcon(QIcon(":/Images/egicon_icon (1).png"));
    curFile = "";
*/

     createActions(); //initializes and connects all the actions to what they do
     createMenus(); //creates the dropdown menus at the top of the application and puts the actions in them
     createToolBars(); //creates the tool bar (planned on two at the start but just made it one for both table and chart)
     createStatusBar(); //status bar at the bottom provides info about each action when they are hovered or clicked

     this->setWindowIcon(QIcon(":/Images/egicon_icon (1).png")); //made a saucy icon
     curFile = ""; //default name of file is nothing

    // here, the user may choose a new theme color and this will be communicated to the table to begin choosing colors around this new center
    QObject::connect( this->theme_picker,SIGNAL( colorSelected(QColor) ), this->window_table, SLOT( set_color_theme(QColor) ) );


    // when a new header is defniend, this conncet letsrhe table add it to the drop down menu
    QObject::connect( this->window_table , SIGNAL( transfer_dropdown_x(QString,QColor,int) ), this, SLOT( update_dropdown_x(QString, QColor,int) ) );
    QObject::connect( this->window_table , SIGNAL( transfer_dropdown_y(QString,QColor,int) ), this, SLOT( update_list_y(QString,QColor,int) ) );

    // if a header is renamed, this connect removes the old one from the drop down menu
    QObject::connect( this->window_table , SIGNAL( dropdown_remove_signal(int) ), this, SLOT( dropdown_remove_x(int) ) );
    QObject::connect( this->window_table , SIGNAL( list_remove_signal(int) ), this, SLOT( list_remove_y(int) ) );

    // update chart funciton is cllaed on a generate button press, update chart is just below, its very important
    QObject::connect( this->generate_button , SIGNAL( clicked() ), this, SLOT( update_chart() ) );

    // this connect is jsut usefull for debugging, it print the values in the value vector to the console
    //QObject::connect( this->generate_button , SIGNAL( clicked() ), this->window_table, SLOT( print_values() ) );



}



//update chart function is called when the gereate chart button is pressed
void MainWindow::update_chart() {

// this is the old color code, it is so powerful Im gonna keep it around just in case

//   window_table->set_values(this->dropdown_y->currentText(),
//                            this->dropdown_y->itemIcon(this->dropdown_y->currentIndex()).pixmap(16,16).toImage().pixelColor(QPoint(3,3)).hsvHue(), // see table.cpp
//                            this->dropdown_y->itemIcon(this->dropdown_y->currentIndex()).pixmap(16,16).toImage().pixelColor(QPoint(3,3)).hsvSaturation());

   // first we will make our vector of keys
   std::vector<int> keys;
   std::vector<QString> names; // names of each of the entires ot eb graphey
   std::vector<QColor> colors; // colors of each entr

   for( int i = 0; i < y_list->count(); i ++){ // going through the whole lsit
       if(y_list->item(i)->checkState() == Qt::Checked ){ // if we find an item that has been checked by the user
           keys.push_back(y_list->item(i)->data(Qt::WhatsThisRole).value<int>());
           names.push_back(y_list->item(i)->text());
           colors.push_back(y_list->item(i)->icon().pixmap(16,16).toImage().pixelColor(QPoint(3,3)));
       }
   }

   // getting the x values
   window_table->set_x_values(dropdown_x->currentData().value<int>());

if(keys.size() != 0){ // if y values arent empty we can make a new graph, if not nah

   window_table->set_values(keys);

   // this is a snippet of legacy code, ie im not using but it may be useful later, and yes I know thats not what legacy code is called buti think it sounds cool
   //Chart* new_chart = new Chart(window_table->values,this->dropdown_y->currentText(),this->dropdown_y->itemIcon(this->dropdown_y->currentIndex()).pixmap(16,16).toImage().pixelColor(QPoint(3,3)));

   Chart* new_chart = new Chart(); // our empty chart is the start chart which I want to be a scatter plot that says hello or somehting

   new_chart = new Chart(window_table->y_values,window_table->x_values ,names, colors,dropdown_x->currentText(),dropdown_chart_types->currentText());//,this->dropdown_y->currentText(),this->dropdown_y->itemIcon(this->dropdown_y->currentIndex()).pixmap(16,16).toImage().pixelColor(QPoint(3,3)));


   rightside_layout->replaceWidget(window_chart,new_chart);
   this->centralWidget()->repaint();
   delete window_chart;
   window_chart = new_chart;
}

}


void MainWindow::update_dropdown_x(QString new_choice, QColor new_color, int new_key){

    for( int i = 0; i < dropdown_x->count(); i ++){ // going though the entire x drop down
        if( new_key == dropdown_x->itemData(i).value<int>()){ // if we find an element with the key we want
            dropdown_x->removeItem(i); // we remove it
            break;
        }
    }


    QIcon icon;
    //QColor background_color = QColor(36,36,36);; // this will be the background color of the icon
    QColor background_color = Qt::black; // this wil

    if(new_color.alpha() != 0){ // if we are given a real color
        QImage img = QImage(14,14,QImage::Format_RGB32); // here we make a a QImage
        QPainter p(&img);
        p.fillRect(img.rect(), background_color); // background color to match the drop downs
        QRect rect = img.rect().adjusted(1,1,-1,-1);
        p.fillRect(rect, new_color);            // give it the color it came over with
        icon.addPixmap(QPixmap::fromImage(img));
    }
    else{ // else we will use an empty color
    QImage img = QImage(14,14,QImage::Format_ARGB32);
    img.fill(qRgba(0, 0, 0, 0));
    icon.addPixmap(QPixmap::fromImage(img));
    }

    dropdown_x->addItem(icon,new_choice,new_key); // and we add the item with that color and the key

    dropdown_x->model()->sort(0); // makes everything aphabetical

   dropdown_x->removeItem(dropdown_x->findText("Default"));
   dropdown_x->insertItem(dropdown_x->count() - 1,"Default",-2);
}



void MainWindow::dropdown_remove_x(int key){
    for( int i = 0; i < dropdown_x->count(); i ++){ // going thorugh the entire x drop down
        if( key == dropdown_x->itemData(i).value<int>()){ // if we find the header we want
            dropdown_x->removeItem(i); // remove it
            break;
        }
    }
}

void MainWindow::update_list_y(QString new_choice,QColor new_color,int new_key){

    QListWidgetItem* item = new  QListWidgetItem(new_choice);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);

        // first we have the case of the user repalcing an already existing data field
        // we need to do this to remove old ones, and to keep consistent check state
    for(int i = 0; i < y_list->count(); i++){ // for the whole list
        if(new_key == y_list->item(i)->data(Qt::WhatsThisRole).value<int>()){ // if we find an element with the matching key
           item->setCheckState(y_list->item(i)->checkState()); // assinging the old check state to the new
           delete y_list->takeItem(i); // deleteing the old one
           break; // we can leave once we find it
        }
    }



    QIcon icon;
    //QColor background_color = QColor(36,36,36);; // this will be the background color of the icon
    QColor background_color = Qt::black; // this wil

    if(new_color.alpha() != 0){ // if we are given a real color
        QImage img = QImage(14,14,QImage::Format_RGB32); // here we make a a QImage
        QPainter p(&img);
        p.fillRect(img.rect(), background_color); // background color to match the drop downs
        QRect rect = img.rect().adjusted(1,1,-1,-1);
        p.fillRect(rect, new_color);            // give it the color it came over with
        icon.addPixmap(QPixmap::fromImage(img));
    }
    else{
    QImage img = QImage(14,14,QImage::Format_ARGB32);
    img.fill(qRgba(0, 0, 0, 0));
    icon.addPixmap(QPixmap::fromImage(img));
    }

    item->setIcon(icon);
    item->setData(Qt::WhatsThisRole,new_key);

    y_list->addItem(item);
}


void MainWindow::list_remove_y(int key){

    for( int i = 0; i < y_list->count(); i ++){ // going through the whole list
        if( key == y_list->item(i)->data(Qt::WhatsThisRole).value<int>()){ // if we find the right key
            delete y_list->takeItem(i);
            break;
        }
    }
}


MainWindow::~MainWindow(){

}


/*
 * CreateActions creates all the QActions necessary to have a functioning program
 * Creates opening, saving, copy, paste etc. Defines these function but does not provide implementation
 * @params: none  @returns: nothing
 */
void MainWindow::createActions()
{
    //Create the actions for each QAction
    newAction = new QAction(tr("New"), this); //tr stands for translation
    newAction->setIcon(QIcon(":/Images/newicon_icon.png")); //set icon of new in the tool bar
    newAction->setShortcut(QKeySequence::New); //keeps the command n or control n cross-platform
    newAction->setStatusTip(tr("Create a new EasyGraph file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("Open"), this);
    openAction->setIcon(QIcon(":/Images/openIcon.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("Save"), this);
    saveAction->setIcon(QIcon(":/Images/saveIcon.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save file"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("SaveAs"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save file as..."));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the Application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(tr("Cut"));
    cutAction->setIcon(QIcon(":/Images/cutIcon.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip("Cut the Data");
    connect(cutAction, SIGNAL(triggered()), this->window_table, SLOT(cut()));

    copyAction = new QAction(tr("Copy"));
    copyAction->setIcon(QIcon(":/Images/copyIcon.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip("Copy the Data");
    QObject::connect(copyAction, SIGNAL(triggered()), this->window_table, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"));
    pasteAction->setIcon(QIcon(":/Images/pasteIcon.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip("Paste the Data");
    QObject::connect(pasteAction, SIGNAL(triggered()), this->window_table, SLOT(paste()));

    delAction = new QAction(tr("Delete"));
    delAction->setIcon(QIcon(":/Images/delicon_icon (1).png"));
    delAction->setShortcut(QKeySequence::Delete);
    delAction->setStatusTip("Delete the Data");
    QObject::connect(delAction, SIGNAL(triggered()), this->window_table, SLOT(delData()));

    colorAction = new QAction(tr("Change Theme Color"), this);
    colorAction->setIcon(QIcon(":/Images/colorIcon"));
    colorAction->setStatusTip("Change the Base Theme Color");
    QObject::connect(colorAction, SIGNAL(triggered()), this->theme_picker, SLOT(show()));

    instructionsAction = new QAction(tr("Instructions"));
    instructionsAction->setStatusTip("Show Instructions Page");
    QObject::connect(instructionsAction, SIGNAL(triggered()), this->instrWindow, SLOT(show()));

//    calcAction = new QAction(tr("Calculator"), this);
//    calcAction->setIcon(QIcon(":/Images/calcIcon"));
//    timerAction = new QAction(tr("Timer"), this);
//    timerAction->setIcon(QIcon(":/Images/timerIcon"));
}

/*
 * CreateMenus creates all the QMenus to put QActions in them
 * Creates the dropdown menus
 * @params: none  @returns: nothing
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(delAction);

    viewMenu = menuBar()->addMenu(tr("View"));

    colorMenu = menuBar()->addMenu(tr("Color"));
    colorMenu->addAction(colorAction);

    chartMenu = menuBar()->addMenu(tr("Chart"));

    aboutMenu = menuBar()->addMenu(tr("About"));
    aboutMenu->addAction(instructionsAction);
}

/*
 * CreateToolBars creates the single tool bar in our program
 * Toolbar holds all the qActions as icons
 * @params: none  @returns: nothing
 */
void MainWindow::createToolBars()
{
    toolBar = addToolBar(tr("&File")); //I dont understand the &
    toolBar->addAction(newAction); //add the actions to the tool bar
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction); //Tool bar is just icons, these guys need icons

    toolBar->setMovable(false);
    toolBar->setOrientation(Qt::Horizontal);

    toolBar->addSeparator(); //separator to distiguish categories for the user
    toolBar->addAction(cutAction);
    toolBar->addAction(copyAction);
    toolBar->addAction(pasteAction);
    toolBar->addAction(delAction);

    toolBar->addSeparator();
    toolBar->addAction(colorAction);

    toolBar->addSeparator();

    toolBar->setStyleSheet("QToolButton:hover:!pressed{background: #87ceeb;"
                               "border: 2px;"
                               "padding: 2px}"
                               "QToolBar {border: none;"
                               "background: #1c1c46;}");

}

/*
 * CreateStatusBar creates the status bar at the bottom of the window
 * Tells you what each QAction does when you hover over it
 * @params: none  @returns: nothing
 */
void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" EasyGraph "); //Oh yeah
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint()); // Set it st it fits the EasyGraph text

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

}

/*
 * NewFile creates a new MainWindow
 * @params none @returns none
 */
void MainWindow::newFile() //If I create a new file it seems it deletes the entire window that exists
{
    MainWindow* newWin = new MainWindow(); //Do I need to delete him at the end?
    newWin->showMaximized();
    //newWin->setProperty(Qt::WA_DeleteOnClose);
}

/*
 * okToContinue checks if the file has been saved (added it because I could and I thought the message box would be cool)
 * @params none @returns none
 */
bool MainWindow::okToContinue() //Got this function idea from a book
{
    if (!changesSaved) //creates a warning box if the window is modified before you try to exit
    {
        setStyleSheet(messageBoxStyle);
        int choice = QMessageBox::warning(this, tr("EasyGraph"), tr("The document not been saved.\n"
                                                                 "Do you want to save changes?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel //Wish the QMessageBox would not fit to the words
                                     );

        if (choice == QMessageBox::Yes) {
            return save();
        } else if (choice == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

/*
 * open opens a new file from the computer
 * our file type is .ezg so cool
 * @params none, @returns none
 */
void MainWindow::open()
{
    if (!okToContinue()) //check if they want to save the data
        return; //exit the function

     QString fileName = QFileDialog::getOpenFileName(this, tr("Open EasyGraph"), ".", tr("EasyGraph files (*.ezg)")); //Retrieves the file name we want to open, makes sure its .ezg
     if(!fileName.isEmpty())
         loadFile(fileName); //go to the big boy

}

/*
 * save saves a file if it already has a name and returns saveAs if it has no name
 * @params none @returns bool, returned a bool bc I saw that it was recommended in a book
 */
bool MainWindow::save()
{
    if (curFile.isEmpty()) //if there is nothing in the string
        return saveAs();
    else
        return saveFile(curFile); //the big boy

}

/*
 * SaveFile is a monster, shout out to Michael for all the help!
 * SaveFile uses QDataStream to export all the relevant data from the EasyGraph window to a .ezg file
 * @parmas QString filename - the name of the file to be saved to
 * @return bool, true if save occurs, false if it does not
 */
bool MainWindow::saveFile(const QString& fileName) //REview
{
    curFile = fileName;

    QFile file(fileName); //Open a Qfile related to the file we are writing too

    if(!file.open(QIODevice::WriteOnly)) //make it so we can only write to this file
    {
        qDebug() << "Could not save your file, unlucky bro...";
        return false;
    } //if he doesnt open we return false

    QDataStream out(&file); //outwards data stream to get all the data out
    out.setVersion(QDataStream::Qt_5_9); //set the version of the data stream to the latest boy

    int identifier = 0; //will identify which object to create while loading
    int nullNumber = 48093; //Will be outputed when there is no data to output
    QColor nullColor = QColor();
    //Saving table data!
    for (int i = 0; i < window_table->getTable()->rowCount(); ++i) {
         for (int j = 0; j < window_table->getTable()->columnCount(); ++j) {

            if (window_table->getTable()->getItem(i, j) != nullptr){
                QString itemData = window_table->getTable()->getItem(i, j)->text();
                if (!itemData.isEmpty()) {
                    out << quint16(identifier) << quint16(i) << quint16(j) << quint16(nullNumber) << quint16(nullNumber) << nullColor << itemData;
                    qDebug() << itemData;
                }
            }
         }

     }

    //save x values
    identifier = 2;
    for (size_t j = 0; j < get_table()->x_values.size(); ++j) //gotta love the logic here
    {
        QString output = get_table()->x_values[j];
        out << quint16(identifier) << quint16(j) << quint16(nullNumber) << quint16(nullNumber) << quint16(nullNumber) << nullColor << output;
        qDebug() << output << "saved";
    }


    //save y values
    identifier = 3;

    for (size_t i = 0; i < get_table()->y_values.size(); ++i) {
        for (size_t j = 0; j < get_table()->y_values[i].size(); ++j) //how will I know what goes where????
        {
            QString output = get_table()->y_values[i][j];
            out <<quint16(identifier) << quint16(i) << quint16(j) << quint16(get_table()->y_values.size()) << quint16(nullNumber) << nullColor << output; // Will output these boys so I know where to put each boy in the vector
        }
    }

    //save headers
    identifier = 1;

    for (size_t i = 0; i < window_table->new_headers.size(); ++i)
    {
        int col = window_table->new_headers[i]->column();
        int key = window_table->new_headers[i]->get_key();
        int colorState = window_table->new_headers[i]->get_color_state();
        int axisState = window_table->new_headers[i]->get_axis_state();
        QColor color = window_table->new_headers[i]->get_color();
        QString words = window_table->new_headers[i]->text();
        qDebug() << window_table->new_headers[i]->text() << window_table->new_headers[i]->column();
        out << quint16(identifier) << quint16(col) << quint16(key) << quint16(colorState) << quint16(axisState) << color << words;
    }

    identifier = 4;
    QString blank = "blank";
    out << quint16(identifier) << quint16(get_table()->key_changer.get_value()) << quint16(nullNumber) << quint16(nullNumber) << quint16(nullNumber) << nullColor << blank;

    changesSaved = true; //nice almost global variable :)
    return true;

}

/*
 * Loadfile is similar to savefile, as it takes in the data from the file name and reconstructs the EasyGraph page
 * LoadFile will create a new mainWindow when loading this file
 * @params Qstring file name
 * @returns bool true if successful, false if it couldnt load the file
 */
bool MainWindow::loadFile(const QString& fileName)
{

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't load file, unlucky bro...";
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_9);

    quint16 identifier;
    quint16 row;
    quint16 column;
    quint16 colorState;
    quint16 axisState;
    QColor color;
    QString itemData = "";

    qDebug() << "Working";
    do { //need to change so we can read rest of the file, could separate with a fat number, !in.atEnd()
        qDebug() << "Reading in file";

        in >> identifier >> row >> column >> colorState >> axisState >> color >> itemData;
        qDebug() << identifier << "Set data" << row << ", " << column << ", " << itemData;
        if (identifier == 0) {
            get_table()->getTable()->createNewItem(row, column, itemData);
        }
        else if (identifier == 1) {
            Table::QTableHeaderWidget *changing_header =  get_table()->createNewHeaderWidget(row, colorState, axisState, itemData);
            changing_header->set_key(column);
            changing_header->set_color(color);
            get_table()->new_headers.push_back(changing_header);
            get_table()->getTable()->setHorizontalHeaderItem(row,changing_header);
             get_table()->getTable()->horizontalHeaderItem(row)->setBackgroundColor(color); //need to ask Bryan if he thinks we should just override instead of loading up a new boy
        } //seems like the potential for memory leaks
        else if (identifier == 2) {
            get_table()->x_values.push_back(itemData);
        }
        else if (identifier == 3) {
            get_table()->addYVals(row, itemData);
        }
        else if (identifier == 4){
            get_table()->key_changer.set_value(row);
        }

    } while (!in.atEnd());//row != w->get_table()->getRows() - 1 && column != w->get_table()->getCols() - 1);


    window_table->reinstall_headers();

    curFile = fileName;
    showMaximized();
    changesSaved = true;
    return true;
}

/*
 * SaveAs allows us to create a new file to save the file to. QFileDialog does all the heavy lifting here
 * @params none, @return bool, false if it fails
 */
bool MainWindow::saveAs() //REview
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save EasyGraph"), ".",
                                                    tr("EasyGraph files (*.ezg)")); //Heavy lifter, similar to open but for saving
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

/*
 * closeEvent is a part of Qt, if the function okToContinue is true then we can close the window, if not we return back to the window
 * @params - QCloseEvent
 * @return - none
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        event->accept(); //if we passed the test close the window
    } else {
        event->ignore(); //else we go back
    }
}

/*
 * get_table() is a getter function for window_table
 * Do not confuse with getTable() -> this one is for the tablewidget within window_table
 * @params none
 * @returns - a pointer to a Table object, or in this case window_table from MainWindow
 */
Table* MainWindow::get_table()
{
    return window_table;
}

void MainWindow::changesSavedChange()
{
    if (changesSaved == false)
        changesSaved = true;
    else
        changesSaved = false;
}


/*

void MainWindow::createActions()
{
    //New Action code
    newAction = new QAction(tr("&New"), this); //tr stands for translation
    newAction->setIcon(QIcon(":/Images/newicon_icon.png")); //set icon of new in the tool bar
    newAction->setShortcut(QKeySequence::New); //keeps the command n or control n cross-platform
    newAction->setStatusTip(tr("Create a new EasyGraph file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":/Images/openIcon.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/Images/saveIcon.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save file"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("&SaveAs"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save file as..."));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    //Open Recent Files code
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the Application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

//    showGridAction = new QAction(tr("&Show Grid"), this);
//    showGridAction->setCheckable(true);
//    showGridAction->setChecked(window_table->table->showGrid());
//    showGridAction->setStatusTip(tr("Show or hide the window_table->table's grid"));

    cutAction = new QAction(tr("Cut"));
    cutAction->setIcon(QIcon(":/Images/cutIcon.png"));
    cutAction->setShortcut(QKeySequence::Cut);

    copyAction = new QAction(tr("Copy"));
    copyAction->setIcon(QIcon(":/Images/copyIcon.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    QObject::connect(copyAction, SIGNAL(triggered()), this->window_table, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"));
    pasteAction->setIcon(QIcon(":/Images/pasteIcon.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    QObject::connect(pasteAction, SIGNAL(triggered()), this->window_table, SLOT(paste()));

    themeAction = new QAction(tr("Dark Theme"), this);
//    themeAction->setCheckable(true);
//    themeAction->setChecked(false);
    themeAction->setStatusTip(tr("Change to dark theme"));
    QObject::connect(themeAction, SIGNAL(triggered()), this, SLOT(changeTheme()));

    colorAction = new QAction(tr("Change Theme Color"), this);
    colorAction->setIcon(QIcon(":/Images/colorIcon"));
    QObject::connect(colorAction, SIGNAL(triggered()), this->theme_picker, SLOT(show()));

    calcAction = new QAction(tr("Calculator"), this);
    calcAction->setIcon(QIcon(":/Images/calcIcon"));
    timerAction = new QAction(tr("Timer"), this);
    timerAction->setIcon(QIcon(":/Images/timerIcon"));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(themeAction);

    colorMenu = menuBar()->addMenu(tr("Color"));
    colorMenu->addAction(colorAction);

    chartMenu = menuBar()->addMenu(tr("Chart"));
    aboutMenu = menuBar()->addMenu(tr("About"));
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File")); //I dont understand the &
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction); //Tool bar is just icons, these guys need icons

    fileToolBar->setMovable(false);
    fileToolBar->setOrientation(Qt::Horizontal);

    fileToolBar->addSeparator();
    fileToolBar->addAction(cutAction);
    fileToolBar->addAction(copyAction);
    fileToolBar->addAction(pasteAction);

    fileToolBar->addSeparator();
    fileToolBar->addAction(colorAction);

    fileToolBar->addSeparator();

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    fileToolBar->addWidget(spacer);

    fileToolBar->addSeparator();
    fileToolBar->addAction(calcAction);
    fileToolBar->addAction(timerAction);

    fileToolBar->setStyleSheet(
                               "QToolButton:hover:!pressed{background: #87ceeb;"
                               "border: 2px;}"
                               "QToolBar {border: none;"
                               "background: #1c1c46;}");

}

//Setting up the status bar
void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint()); //I dont understand this line

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

   //connect(window_table->table, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
    //connect(window_table->table, SIGNAL(modified()), this, SLOT(spreasheetModified()));

    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
   // locationLabel->setText(window_table->table->currentLocation());
    //formulaLabel->setText(window_table->table->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
   // updateStatusBar();
}

void MainWindow::newFile() //If I create a new file it seems it deletes the entire window that exists
{
    MainWindow* newWin = new MainWindow();
    newWin->show();
}

bool MainWindow::okToContinue()
{
    if (!changesSaved) //creates a warning box if the window is modified before you try to exit
    {
        setStyleSheet(messageBoxStyle);
        int choice = QMessageBox::warning(this, tr("EasyGraph"), tr("The document not been saved.\n"
                                                                 "Do you want to save changes?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                                     );

        if (choice == QMessageBox::Yes) {
            return save();
        } else if (choice == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::open()
{
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open EasyGraph"), ".", tr("EasyGraph files (*.ezg)"));
     if(!fileName.isEmpty())
         loadFile(fileName);

}

bool MainWindow::save()
{
    if (curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);

}

bool MainWindow::saveFile(const QString& fileName) //REview
{
    curFile = fileName;

    QFile file(fileName); //Open a Qfile related to the file we are writing too

    if(!file.open(QIODevice::WriteOnly)) //make it so we can only write to this file
    {
        qDebug() << "Could not save your file, unlucky bro...";
        return false;
    } //if he doesnt open we return false

    QDataStream out(&file); //outwards data stream to get all the data out
    out.setVersion(QDataStream::Qt_5_9); //set the version of the data stream to the latest boy

    int identifier = 0;
    int nullNumber = 48093;
    QColor nullColor = QColor();
    //Saving table data!
    for (int i = 0; i < window_table->getTable()->rowCount(); ++i) {
         for (int j = 0; j < window_table->getTable()->columnCount(); ++j) {

            if (window_table->getTable()->getItem(i, j) != nullptr){
                QString itemData = window_table->getTable()->getItem(i, j)->text();
                if (!itemData.isEmpty()) {
                    out << quint16(identifier) << quint16(i) << quint16(j) << quint16(nullNumber) << quint16(nullNumber) << nullColor << itemData;
                    qDebug() << itemData;
                }
            }
         }

     }

    //save x values
    identifier = 2;
    for (size_t j = 0; j < get_table()->x_values.size(); ++j)
    {
        QString output = get_table()->x_values[j];
        out << quint16(identifier) << quint16(j) << quint16(nullNumber) << quint16(nullNumber) << quint16(nullNumber) << nullColor << output;
        qDebug() << output << "saved";
    }


    //save y values
    identifier = 3;

    for (size_t i = 0; i < get_table()->y_values.size(); ++i) {
        for (size_t j = 0; j < get_table()->y_values[i].size(); ++j) //how will I know what goes where????
        {
            QString output = get_table()->y_values[i][j];
            out <<quint16(identifier) << quint16(i) << quint16(j) << quint16(get_table()->y_values.size()) << quint16(nullNumber) << nullColor << output; // Will output these boys so I know where to put each boy in the vector
        }
    }

    //save headers
    identifier = 1;

    for (size_t i = 0; i < window_table->new_headers.size(); ++i)
    {
        int col = window_table->new_headers[i]->column();
        int key = window_table->new_headers[i]->get_key();
        int colorState = window_table->new_headers[i]->get_color_state();
        int axisState = window_table->new_headers[i]->get_axis_state();
        QColor color = window_table->new_headers[i]->get_color();
        QString words = window_table->new_headers[i]->text();
        qDebug() << window_table->new_headers[i]->text() << window_table->new_headers[i]->column();
        out << quint16(identifier) << quint16(col) << quint16(key) << quint16(colorState) << quint16(axisState) << color << words;
    }

    identifier = 4;
    QString blank = "blank";
    out << quint16(identifier) << quint16(window_table->key_changer.get_value()) << quint16(nullNumber) << quint16(nullNumber) << quint16(nullNumber) << nullColor << blank;

    changesSaved = true;
    return true;

}

bool MainWindow::loadFile(const QString& fileName)
{

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't load file, unlucky bro...";
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_9);

    MainWindow* w = new MainWindow();

    quint16 identifier;
    quint16 row;
    quint16 column;
    quint16 colorState;
    quint16 axisState;
    QColor color;
    QString itemData = "";

    qDebug() << "Working";
    do { //need to change so we can read rest of the file, could separate with a fat number, !in.atEnd()
        qDebug() << "Reading in file";

        in >> identifier >> row >> column >> colorState >> axisState >> color >> itemData;
        qDebug() << identifier << "Set data" << row << ", " << column << ", " << itemData;
        if (identifier == 0) {
            w->get_table()->getTable()->createNewItem(row, column, itemData);
        }
        else if (identifier == 1) {
            Table::QTableHeaderWidget *changing_header =  w->get_table()->createNewHeaderWidget(row, colorState, axisState, itemData);
            changing_header->set_key(column);
            changing_header->set_color(color);
            w->get_table()->new_headers.push_back(changing_header);
            w->get_table()->getTable()->setHorizontalHeaderItem(row,changing_header);
             w->get_table()->getTable()->horizontalHeaderItem(row)->setBackgroundColor(color);
        }
        else if (identifier == 2) {
            w->get_table()->x_values.push_back(itemData);
        }
        else if (identifier == 3) {
            w->get_table()->addYVals(row, itemData);
        }
        else if (identifier == 4){
            w->get_table()->key_changer.set_value(row);
        }
    } while (!in.atEnd());//row != w->get_table()->getRows() - 1 && column != w->get_table()->getCols() - 1);


    w->window_table->reinstall_headers();

    curFile = fileName;
    w->show();
    w->changesSaved = true;
    return true;
}

bool MainWindow::saveAs() //REview
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save EasyGraph"), ".",
                                                    tr("EasyGraph files (*.ezg)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        //writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setCurrentFile(const QString &fileName) //REview
{
    curFile = fileName;
    setWindowModified(false);
    QString shownName = tr("Untitled");
    if(!curFile.isEmpty())
    {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
    }
    setWindowTitle(shownName + "[*] - EasyGraph");
}

QString MainWindow::strippedName(const QString &fullFileName) //REview
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions() //Review
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
            i.remove();
    }
    for (int j = 0; j < MaxRecentFiles; ++j)
    {
        if (j < recentFiles.count()){
            QString title = QString(j+1) + strippedName(recentFiles[j]); //Does this work?
            recentFileActions[j]->setText(title);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
            }
        else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::openRecentFile() //Review this, understand it
{
    if (okToContinue())
    {
        QAction* action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

//  By default QSettings stores the applications data in platfomr specific locations
//  It doesnt store current files data but the applications data itself
//  This includes the recent files, current geometry (idk), may want colors/theme stored

void MainWindow::writeSettings(){
    QSettings settings("EasyGraph Inc.", "window_table->table Edition");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
   // settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

void MainWindow::readSettings()
{
    QSettings settings("EasyGraph Inc.", "window_table->table Edition");

    restoreGeometry(settings.value("geometry").toByteArray());
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);
}

void MainWindow::changeTheme()
{
//    if(!darkThemeOn){
//        setStyleSheet(mainWindowStyleSheets);
//    }
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
     qApp->setPalette(darkPalette);
     qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    fileToolBar->setStyleSheet("border: none;"
                               "background: gray;"
                               );
}

Table* MainWindow::get_table()
{
    return window_table;
}
*/
