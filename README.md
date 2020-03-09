# easygraph
Qt application for easy and intuitive graph creation from spreadsheet

Application was created by Bryan Josloff and Charles Stoksik

*The file usage.pdf explains how the application looks and is behaves towards the user.*

*The purpose of this readme is to explain a little about the architecture used and some interesting topics or chooses made in the development process.*

The main design principle of the application was object oriented and compartmentalized. Each class that is only used by another class is typically nested within it. All data fields that can be kept private are. The three main components of the application are the MainWindow, the Table and Chart.


The MainWindow's role is simply to ensure the Table and Chart are displayed in correct proportion and to mediate the exchange of information between the two. 


The Table class
--
The Table class contains the spreadsheet and the logic that allows the headers of the columns to be altered and customized. The Table class features three nested classes, one that inherits from Qt’s class for table headers and two that are functors that allow customization of the header cells. 

There is a color functor which will automatically generate a color theme from a given color and since we wanted to allow the user to both name and match the color of two different header cells, a key functor is used to ensure unique keys are given to each header cell to differentiate them.

The only logical challenge in the Table class can be found in the update header function. Users are allowed to make changes to header cells they have created. This is becomes complicated since every header cell has default elements, and previous elements that must be copied, and the new information the user would like to change. The solution to this process is to keep track of both the previous and new state of the check box and menu items throughout the assignment process. The result is a lot of variable management that can be seen by looking at the update_header() function in the file table.cpp.

All the data values are kept as private data and are never directly shared or accessed by any other class. 


The Chart Class
--
The Chart class is responsible for generating the appropriate graph from the selected data. The largest hurdle here is having the chart class graph incomplete/nonuniform data sets appropriately. 

To gather the data, the Chart class creates a vector that the Table class than fills with the appropriate data and transfers back. This extra step allows the data values to be kept as private member fields.

The graph is generated completely autonomously, so the axis size and spacing must be calculated for the user. The standard deviation, max, and min, are all taken into account to map the data in a pleasant way. The exact formula was found through trial and error with data sets with minuscule data values and some with extreme outliers.

The most interesting challenge was dealing with incomplete data sets. This is defined as a column with an empty cell in the middle or a data set that is shorter than its companion it is mapped against. It was decided that there was no way to deal with these cases on the fly so the size and ‘holes’ in a data set are found before graphing takes place. The ‘holes’ are identified by created a vector to store the index values of each empty cell and these indices are then ignored. The challenge occurs when the user is using 5 different data sets all with several holes and different lengths. The result is a for loop with many if statements checking along the way that only valid data points are being graphed. This is implemented in chart.cpp for each graph type supported.

