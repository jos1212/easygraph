#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QObject>
#include <QComboBox>
#include <QLabel>
#include <QPalette>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyleFactory>
#include <QtGui>
#include <QAction>
#include <QDataStream>


#include "Table/table.h"
#include "Chart/chart.h"
#include "Logic/stylesheets.h"
#include "Logic/instructionswindow.h"

    // this class holds the table and chart and manages communication and organization of the two

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

     bool changesSaved; // used to make sure the user saves thier data

private slots:
     void update_chart(); // this slot updates the chart and places it in the layout, called by generate_button

     void update_dropdown_x(QString new_choice, QColor new_color, int new_key);  // slot to be called when the user adds a new custom header
     void update_list_y(QString new_choice,QColor new_color,int new_key); // updates the list for y when a user makes a new header

     void dropdown_remove_x(int key); // removes a header from the dropdown
     void list_remove_y(int key); // removes a header from the list
     Table* get_table(); // is used to get access to the table for saving and loading

private:
    Table* window_table;
    Chart* window_chart;
    QColorDialog* theme_picker; // used to let the user pick a new theme color, its public so the action get connect to its show command
    QVBoxLayout* rightside_layout;
    QPushButton* generate_button;
    QComboBox*  dropdown_x;
    QComboBox*  dropdown_chart_types;
    QListWidget* y_list;
    InstructionsWindow* instrWindow;



    // everything below is the mainwindow actions ie, save, load, copy, paste

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void changesSavedChange();

private:

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    bool okToContinue();
    bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);

    QLabel* locationLabel;
    QLabel* formulaLabel;
    QStringList recentFiles;
    QString curFile;
        //Set max number of recent files to be 5

    QAction* separatorAction;

    QMenu *fileMenu; //file dropdown menu
    QMenu* editMenu; //edit dropdown menu
    QMenu* viewMenu;
    QMenu* colorMenu;
    QMenu* chartMenu;
    QMenu* aboutMenu;

    QToolBar* toolBar;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;

    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* delAction;

    QAction* colorAction;
    QAction* instructionsAction;

    /*
private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void openRecentFile();
    void updateStatusBar();
    void spreadsheetModified();
    void changeTheme();

private:

    void createActions();
    void createMenus();
   // void createContextMenu(); //this is for edit
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);

    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    QLabel* locationLabel;
    QLabel* formulaLabel;
    QStringList recentFiles;
    QString curFile;
        //Set max number of recent files to be 5
    enum { MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction* separatorAction;

    QMenu *fileMenu; //file dropdown menu
    QMenu* editMenu; //edit dropdown menu
    QMenu* viewMenu;
    QMenu* colorMenu;
    QMenu* chartMenu;
    QMenu* aboutMenu;

    QToolBar* fileToolBar;
    QAction* newAction;
    QAction* openAction;
    QAction* openRecentAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;
    QAction* showGridAction;
    QAction* aboutQtAction;

    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;

    QAction* themeAction;
    QAction* colorAction;

    QAction* calcAction;
    QAction* timerAction;

*/
};




#endif // MAINWINDOW_H
