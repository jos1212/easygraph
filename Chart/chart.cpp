#include "chart.h"

Chart::Chart(QWidget *parent) : QMainWindow(parent)
{

    chart = new QtCharts::QChart(); // create a new chart

    int max = 20;

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,max);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->axes(Qt::Vertical).first()->hide();

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0,max);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->axes(Qt::Horizontal).first()->hide();


    for(int i = 0; i < max - 11; i+=2){
    QtCharts::QLineSeries* series = new QtCharts::QLineSeries();

        series->append(max-i,i);
        series->append(max-i,max-i);
        series->append(i,max-i);
        series->append(i,i);
        series->append(max-i,i);


       chart->addSeries(series); // we add these to the chart
       series->attachAxis(axisX);
       series->attachAxis(axisY);
}

    chart->legend()->hide();


    chart_view = new QtCharts::QChartView(chart); // we add the chart to the chart view

    this->setCentralWidget(chart_view);

    //QSplitter *splitter = new QSplitter();

    chart_title = new  ChartTextBox(); // creation
    chart_title->setText("Welcome!");

        // formating  // 243, 143
    chart_title->setFont(QFont("Arial", 15, QFont::Bold)); // for font formating
    chart_title->setStyleSheet("QLineEdit {background-color : white; color : black; }"); // use this for color formating
    chart_title->setFrame(QFrame::NoFrame); // we can use frame to add borders very easily
    chart_title->setGeometry(this->width()/2 - 80,143,80,30);

    connect(chart_title, SIGNAL(textChanged(QString)), chart_title, SLOT(resize()));

    //chart_title->setParent(this); // adding it in

    //chart_title->setGeometry(parentWidget()->width()/2,parentWidget()->height()/2,80,30); // size properties

}

Chart::Chart(std::vector<std::vector<QString>> data,std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors,QString x_label, QString type) {

    chart_title = nullptr;
    y_axis_title = nullptr;

    if(type == "Linear"){
    linear(data, x_values, names, colors, x_label); // need point values to show up on linear
    }
    if(type == "Scatter"){
    scatter(data, x_values,names, colors,x_label);
    }
    if(type == "Bar"){
    bar(data, x_values,names, colors,x_label);
    }
    if(type == "Pie"){
    pie(data, x_values,names, colors,x_label);
    }
}
void Chart::fill_vectors(std::vector<QString> &values,std::vector<double> &position,std::vector<int> &pop){

    // given a vector of Qstrings we will get the numbers we need form it and the places to hop over a value
     bool conversion;

    for(size_t i =0; i < values.size(); i++){ // for all the values

    double num = (values[i].toDouble(&conversion));
    if(conversion){ // if converstion is succesful
    position.push_back(num);
    }
                // the case of a blank or text typed inbetween data values
    else { //so we are not at the end yet //  if(i < values.size()) this was an if else but it wokrs fine with out the if
        pop.push_back(i);
        position.push_back(0);
        }
    }

}

double Chart::std_deviation(const std::vector<double> &position, const std::vector<int> &pop){

    double sum = 0;
    double mean;
    double variance = 0;

    int value = 0;

       for(size_t i = 0; i < position.size(); ++i){ // for all the position elements
           if(pop.size()!= 0){
               if(i != pop[value]){ // if this isnt a string
                  value++;
               }
               else{
                  sum += position[i];
               }
           }
           else{
           sum += position[i];
           }

       }
       mean = sum / position.size();
       value = 0;
      for(size_t i = 0; i < position.size(); ++i){ // for all the position values
          if(pop.size()!= 0){ // if there are holes in the data
              if(i == pop[value]){ // if there is a hole right here
                 value++;
              }
              else{
                  variance += pow(position[i] - mean, 2);
              }
          }
          else{
          variance += pow(position[i] - mean, 2);
          }
       }
         variance = variance/position.size();
         return sqrt(variance);
}

double Chart::minimum(const std::vector<double> &position, const std::vector<int> &pop){ // to return minimum in a vector
    double min = position[0];
     int value = 0;
     for(size_t i = 0; i < position.size(); ++i){
         if(position[i] < min){
             if(pop.size()!= 0){
                 if(i == pop[value]){
                    value++;
                 }
                 else{
                    min = position[i];
                 }
             }
             else{
             min = position[i];
             }
         }
     }
     return min;
}

double Chart::maximum(const std::vector<double> &position, const std::vector<int> &pop){ // to return minimum in a vector
    double max = position[0];
    int value = 0;
     for(size_t i = 0; i < position.size(); ++i){

         if(position[i] > max){
             if(pop.size()!= 0){
                 if(i == pop[value]){
                    value++;
                 }
                 else{
                    max = position[i];
                 }
             }
             else{
             max = position[i];
             }
         }

     }
     return max;
}

void Chart::set_x_axis(QValueAxis* &x_axis,const std::vector<double> &x_position,const std::vector<int> &x_pop,int length){

    if(x_position.size() != 0){
        double bottom;
        double top;
        double x_maximum = maximum(x_position,x_pop);
        double x_minimum = minimum(x_position,x_pop);
        double x_std = std_deviation(x_position,x_pop);
     if(x_minimum != x_maximum){

      if( minimum(x_position,x_pop) >= 0 )     {   //finding the bottom
         bottom = 0;
       }
       else{
        bottom = floor(x_minimum  - x_std);
       }

        top = ceil(x_maximum + x_std);

        if( abs(x_maximum - x_minimum) < 1){
            bottom = x_minimum - x_std;
            top = x_maximum + x_std;
        }
      x_axis->setRange(bottom, top);
     }
     else{ // this is the case of a dot or just one point which requires special treatment
        if(x_position[0] != 0){
         if( x_minimum >= 0 )     {   //finding the bottom
            bottom = 0;
          }
         else{
             bottom = 2*x_position[0];
         }
           top = x_position[0] + abs(x_position[0]);
           x_axis->setRange(bottom, top);
        }
        else{
            x_axis->setRange(-1,1);
        }
     }
    }
    else{

     // now we have the case of the default x axis, we know its going to start at zero but.. what will it end at
     // we have to find the longest y data set and use that, this is the simplest case

    std::vector<double> default_x;
    for(int i =0; i < length; i ++){
        default_x.push_back(i);
    }

    if(default_x.size() == 0){
        x_axis->setRange(0,2);
    }
    else{
         x_axis->setRange(0, ceil(length + 2 + std_deviation(default_x)));
    }
 }


}

void Chart::set_y_axis(QValueAxis* &y_axis,const std::vector<std::vector<double>> &y_position,const std::vector<std::vector<int>> &y_pop){


    // now for the y axis.. which is trickier since we can have many of them so we will have to find the ideal canidates
    // we will use the minimum of the minimmums, max of max, and highest standard deviation

    std::vector<double> minimums;
    std::vector<double> maximums;
    std::vector<double> deviations;

    for(size_t k = 0; k < y_position.size(); k++){ // now thats what im talking about! getting all that info so simply
     if(y_position[k].size() != 0){
        minimums.push_back(minimum(y_position[k],y_pop[k])); // getting the mins from ALL of our y value sin one cal!
        maximums.push_back(maximum(y_position[k],y_pop[k]));
        deviations.push_back(std_deviation(y_position[k],y_pop[k]));
     }
    }

  if(minimums.size() != 0){
      double y_std = maximum(deviations);
      double y_min = minimum(minimums);
      double y_max = maximum(maximums);
      double y_bottom;
      double y_top;
   if(y_std != 0){

    if( y_min >= 0 )     {   //finding the bottom
      y_bottom = 0;
     }
    else{
      y_bottom = floor(y_min - y_std);
     }

     y_top = ceil(y_max +  y_std);


    if( abs(y_max - y_min) < 1){
        y_bottom = y_min - y_std;
        y_top = y_max + y_std;
    }

     y_axis->setRange(y_bottom, y_top);
      }
   else{ // this is the case of a dot or just one point which requires special treatment
      if(y_min != 0){ // very special case of a dot with y = 0 needs its one treatment
       if( y_min > 0 )     {   //finding the bottom, note: in this case y_bottom = y_top and stand dev is useless
         y_bottom = 0;
        }
       else{
            y_bottom = 2*y_min;
        }
         y_top = y_max + abs(y_max);
         y_axis->setRange(y_bottom, y_top);
      }
      else{
      y_axis->setRange(-1,1);
      }
    }
 }



}

// called for a linear graph
void Chart::linear(std::vector<std::vector<QString>> y_values,std::vector<QString> x_values ,std::vector<QString> names, std::vector<QColor> colors, QString x_label){

    chart = new QtCharts::QChart(); // create a new chart

    std::vector<double> x_position;
    std::vector<int> x_pop;
    fill_vectors(x_values,x_position,x_pop); // here we fill x_pop with the elments of which x needs to skip, x_position with its useable values

    std::vector< std::vector<double>> y_position;
        y_position.resize(y_values.size());
    std::vector<std::vector<int>> y_pop;
        y_pop.resize(y_values.size());

    for(size_t i = 0; i< y_values.size(); i++){
            fill_vectors(y_values[i],y_position[i],y_pop[i]); // now we will y_ositon with all the values for each y term, and y pop with all the y values for the skip parts
     }

     int length = 0;
        for(size_t k = 0; k < y_position.size(); k ++){ // now thats what im talking about! getting all that info so simply
            if( (y_position[k].size() - y_pop.size()) > length ){
                length = (y_position[k].size() - y_pop.size());
            }
        }

     QValueAxis *axisY = new QValueAxis();
     set_y_axis(axisY,y_position,y_pop);
     chart->addAxis(axisY, Qt::AlignLeft);

     QValueAxis *axisX = new QValueAxis();
     set_x_axis(axisX,x_position,x_pop,length);
     chart->addAxis(axisX, Qt::AlignBottom);

    bool found = false;

   for(size_t k = 0; k < y_values.size(); k ++){

    int y_value = 0;
    int x_value = 0; // these are used to keep track of the ndicies of the pop vectors

    QtCharts::QLineSeries* series = new QtCharts::QLineSeries();
    series->setName(names[k]);

    QPen my_pen = QPen(colors[k]);
    my_pen.setWidth(2);
    series->setPen(my_pen);

    series->setPointsVisible(true);

    size_t limit = y_position[k].size();
    if(x_position.size() < limit && x_position.size() != 0){ // here we pick the limit, if there are less x values then y values we have to stop early
        limit = x_position.size();
    }

    for(size_t i = 0; i < limit; i ++){
    found = false;
        if(x_position.size() == 0){ // in the case of x being empty

            if(y_pop[k].size() != 0){ // pop is sued to find holes in the data and jump over them
                if(i ==  y_pop[k][y_value] ){
                 y_value++;

                 }
                 else{
                 series->append(i,y_position[k][i]);
                 }
            }
            else{
            series->append(i+1,y_position[k][i]);
            }
          }
        else{
                if(x_pop.size() != 0 || y_pop.size() != 0 ){ // only checking as long as one of the pops has a value
                    if(x_pop.size() != 0){ // if has values
                        if(i == x_pop[x_value]){ // if x wants us to stop
                         x_value++;
                         found = true;
                         }
                     }
                     if(y_pop[k].size() != 0){ // if has values
                         if(i == y_pop[k][y_value]){ // if x wants us to stop
                          found = true;
                          y_value++;
                          }
                      }
                     if(!found){
                        series->append(x_position[i],y_position[k][i]);
                      }
                  }
                  else{
                      series->append(x_position[i],y_position[k][i]);
                   }
            }
        }
    chart->addSeries(series); // we add these to the chart
    series->attachAxis(axisY);
    series->attachAxis(axisX);
    }

   // this section is to find appropiate names for the axis and chart title
  if( x_label == "Default" ||x_label == "Set Values" ){       // if the user doesnt specify we will jsut name it time
  x_label = "Time";
  }

  chart->axes(Qt::Horizontal).first()->setTitleText(x_label);
      if(names.size() == 1){
       chart->axes(Qt::Vertical).first()->setTitleText(names[0]);  // of we have a good y value to use we will make thosoe too
       chart->setTitle(names[0] + " by " + x_label);
      }

    chart->legend()->setAlignment(Qt::AlignRight);

    chart->setAnimationOptions(QChart::AllAnimations);

    chart_view = new QtCharts::QChartView(chart); // we add the chart to the chart view
    chart_view->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(chart_view);
}

// called for a scatter plot
void Chart::scatter(std::vector<std::vector<QString>> y_values,std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors,QString x_label){

    chart = new QtCharts::QChart(); // create a new chart

    std::vector<double> x_position;
    std::vector<int> x_pop;
    fill_vectors(x_values,x_position,x_pop); // here we fill x_pop with the elments of which x needs to skip, x_position with its useable values

    std::vector< std::vector<double>> y_position;
        y_position.resize(y_values.size());
    std::vector<std::vector<int>> y_pop;
        y_pop.resize(y_values.size());
    for(size_t i = 0; i< y_values.size(); i++){
        fill_vectors(y_values[i],y_position[i],y_pop[i]); // now we will y_ositon with all the values for each y term, and y pop with all the y values for the skip parts
    }

    int length = 0;
       for(size_t k = 0; k < y_position.size(); k ++){ // now thats what im talking about! getting all that info so simply
           if( (y_position[k].size() - y_pop.size()) > length ){
               length = (y_position[k].size() - y_pop.size());
           }
       }

    QValueAxis *axisY = new QValueAxis();
    set_y_axis(axisY,y_position,y_pop);
    chart->addAxis(axisY, Qt::AlignLeft);

    QValueAxis *axisX = new QValueAxis();
    set_x_axis(axisX,x_position,x_pop,length);
    chart->addAxis(axisX, Qt::AlignBottom);

    bool found = false;

   for(size_t k = 0; k < y_values.size(); k ++){

    int y_value = 0;
    int x_value = 0; // these are used to keep track of the ndicies of the pop vectors

    QtCharts::QScatterSeries* series = new QtCharts::QScatterSeries();
    series->setName(names[k]);

    QPen my_pen = QPen(colors[k]);
    my_pen.setWidth(2);
    series->setPen(my_pen);
    series->setColor(colors[k]); // if this isnt called but only pen is, then the color is just a neat out line pf the shape, comment out this command and check it out

    //chart->setTheme(QChart::ChartThemeLight);

    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setMarkerSize(10.0);

    size_t limit = y_position[k].size();
    if(x_position.size() < limit && x_position.size() != 0){ // here we pick the limit, if there are less x values then y values we have to stop early
        limit = x_position.size();
    }



   for(size_t i = 0; i < limit; i ++){
    found = false;
    if(x_position.size() == 0){ // in the case of x being empty

        if(y_pop[k].size() != 0){ // pop is sued to find holes in the data and jump over them
            if(i == y_pop[k][y_value]){
             y_value++;

             }
             else{
             series->append(i,y_position[k][i]);
             }
        }
        else{
        series->append(i+1,y_position[k][i]);
        }
      }
        else{
                if(x_pop.size() != 0 || y_pop.size() != 0 ){ // only checking as long as one of the pops has a value
                    if(x_pop.size() != 0){ // if has values
                        if(i == x_pop[x_value]){ // if x wants us to stop
                         x_value++;
                         found = true;
                         }
                     }
                     if(y_pop[k].size() != 0){ // if has values
                         if(i == y_pop[k][y_value]){ // if x wants us to stop
                          found = true;
                          y_value++;
                          }
                      }
                     if(!found){
                        series->append(x_position[i],y_position[k][i]);
                      }
                  }
                  else{
                      series->append(x_position[i],y_position[k][i]);
                   }
            }
        }
     chart->addSeries(series); // we add these to the chart
     series->attachAxis(axisX);
     series->attachAxis(axisY);
    }


     // this section is to find appropiate names for the axis and chart title
    if( x_label == "Default" ||x_label == "Set Values" ){       // if the user doesnt specify we will jsut name it time
    x_label = "Time";
    }

    chart->axes(Qt::Horizontal).first()->setTitleText(x_label);
        if(names.size() == 1){
         chart->axes(Qt::Vertical).first()->setTitleText(names[0]);  // of we have a good y value to use we will make thosoe too
         chart->setTitle(names[0] + " by " + x_label);
        }


    chart->legend()->setAlignment(Qt::AlignRight);

    chart->setAnimationOptions(QChart::AllAnimations);

    chart_view = new QtCharts::QChartView(chart); // we add the chart to the chart view
    chart_view->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(chart_view);
}

// called for a bar graph
void Chart::bar(std::vector<std::vector<QString>> y_values,std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors, QString x_label){

    chart = new QtCharts::QChart(); // create a new chart

    // for bar charts text is expected for the x values whihc makes my job a lot easier in the reagard I dont need to make a pop vector

    std::vector< std::vector<double>> y_position;
        y_position.resize(y_values.size());
    std::vector<std::vector<int>> y_pop;
        y_pop.resize(y_values.size());
    for(size_t i = 0; i< y_values.size(); i++){
        fill_vectors(y_values[i],y_position[i],y_pop[i]); // now we will y_ositon with all the values for each y term, and y pop with all the y values for the skip parts
    }

    QBarSeries *series = new QBarSeries();

    QValueAxis *axisY = new QValueAxis();
    set_y_axis(axisY,y_position,y_pop);
    chart->addAxis(axisY, Qt::AlignLeft);

    QBarCategoryAxis *axisX = new QBarCategoryAxis(); // the x axis doesnt take into account the selection of default
    QStringList categories;                         // this is because when default is selected, the empty axis we create will autoamtically be fileld with the same values we would but in any way ie 1,2,3,4.
    for(size_t i = 0; i < x_values.size(); i ++){
        categories.push_back(x_values[i]);
    }
    axisX->setCategories(categories);
    chart->addAxis(axisX, Qt::AlignBottom);

   bool found = false;

   for(size_t k = 0; k < y_values.size(); k ++){

   int y_value = 0;

    //chart->setTheme(QChart::ChartThemeLight);

    size_t limit = y_position[k].size();
    if(x_values.size() < limit && x_values.size() != 0){ // here we pick the limit, if there are less x values then y values we have to stop early
        limit = x_values.size();
    }


    QBarSet *set = new QBarSet(names[k]);
    set->setColor(colors[k]);

    for(size_t i = 0; i < limit; i ++){

    found = false;

        if(x_values.size() == 0){ // in the case of x being empty

            if(y_pop[k].size() != 0){ // pop is sued to find holes in the data and jump over them
                if(i == static_cast<size_t>(y_pop[k][y_value])){
                 y_value++;

                 }
                 else{
                    categories << QString::number(i);
                    set->append(y_position[k][i]);
                 }
            }
            else{
                categories << QString::number(i+1);
                set->append(y_position[k][i]);
            }
          }
         else{
              if(y_pop[k].size() != 0){ // if has values
                 if(i == y_pop[k][y_value]){ // if x wants us to stop
                 found = true;
                 y_value++;
                 }
                 else{
                   categories << x_values[i];
                   set->append(y_position[k][i]);
                 }
              }
              else{
                categories << x_values[i];
                set->append(y_position[k][i]);
              }
           }
       }
      series->append(set);

    }
chart->addSeries(series); // we add these to the char
series->attachAxis(axisX);
series->attachAxis(axisY);


    QtCharts::QLineSeries* origin = new QtCharts::QLineSeries();
    QPen my_pen = QPen(QColor(214,214,214));
    my_pen.setWidth(1);
    my_pen.setStyle(Qt::DashLine);
    origin->setPen(my_pen);
    origin->append(-10,0); // 0,0 is intuitive but the bar graph is shifted away from pure zero for asthetics, -10 solves this problem as an x value
    origin->append(categories.size(),0);
    chart->addSeries(origin);
    origin->attachAxis(axisX);
    origin->attachAxis(axisY);
    chart->legend()->markers(origin)[0]->setVisible(false);

        // this section is to find appropiate names for the axis and chart title
    if( x_label == "Default" ||x_label == "Set Values" ){       // if the user doesnt specify we will jsut name it time
    x_label = "Time";
    }

    chart->axes(Qt::Horizontal).first()->setTitleText(x_label);
      if(names.size() == 1){
         chart->axes(Qt::Vertical).first()->setTitleText(names[0]);  // of we have a good y value to use we will make thosoe too
         chart->setTitle(names[0] + " by " + x_label);
      }


    chart->legend()->setAlignment(Qt::AlignRight);

    chart->setAnimationOptions(QChart::AllAnimations);

    chart_view = new QtCharts::QChartView(chart); // we add the chart to the chart view
    chart_view->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(chart_view);

}


// called for a pie graph
void Chart::pie(std::vector<std::vector<QString>> y_values,std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors, QString x_label){

    chart = new QtCharts::QChart(); // create a new chart


    std::vector< std::vector<double>> y_position;
        y_position.resize(y_values.size());
    std::vector<std::vector<int>> y_pop;
        y_pop.resize(y_values.size());
    for(size_t i = 0; i< y_values.size(); i++){
        fill_vectors(y_values[i],y_position[i],y_pop[i]); // now we will y_ositon with all the values for each y term, and y pop with all the y values for the skip parts
    }

   QPieSeries *series = new QPieSeries();

   bool found = false;

   for(size_t k = 0; k < y_values.size(); k ++){

   int y_value = 0;

   size_t limit = y_position[k].size();
   if(x_values.size() < limit && x_values.size() != 0){ // here we pick the limit, if there are less x values then y values we have to stop early
       limit = x_values.size();
   }



    for(size_t i = 0; i < limit; i ++){
    found = false;

        if(x_values.size() == 0){ // in the case of x being empty

            if(y_pop[k].size() != 0){ // pop is sued to find holes in the data and jump over them
                if(i == static_cast<size_t>(y_pop[k][y_value])){
                 y_value++;

                 }
                 else{
                    series->append(QString::number(i),y_position[k][i]);
                 }
            }
            else{
                  series->append(QString::number(i),y_position[k][i]);
            }
          }
         else{
              if(y_pop[k].size() != 0){ // if has values
                 if(i == y_pop[k][y_value]){ // if x wants us to stop
                 found = true;
                 y_value++;
                 }
                 else{
                     series->append( x_values[i],y_position[k][i]);
                 }
              }
              else{
                   series->append( x_values[i],y_position[k][i]);
              }
           }
       }
    }

chart->addSeries(series); // we add these to the char

    chart->setTheme(QChart::ChartThemeLight); // need to use themes for pie chart since we cant speicfy colors from the user

    chart->legend()->setAlignment(Qt::AlignRight);

    chart->setAnimationOptions(QChart::AllAnimations);

    chart_view = new QtCharts::QChartView(chart); // we add the chart to the chart view
    chart_view->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(chart_view);

}

Chart::~Chart(){

}

Chart::ChartTextBox::ChartTextBox(QWidget *parent):  QLineEdit(parent), flag(false) {}

void Chart::ChartTextBox::mouseDoubleClickEvent(QMouseEvent *event)
{
    flag = true;
    position = event->globalPos();

}

void Chart::ChartTextBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(flag){ // if you double clicked

    const QPoint delta = event->globalPos() - position;
    position = event->globalPos();

    int x_pos = x()+delta.x(); // determining the x position with collisions for the size of the widget
    if(x_pos < 0){
        x_pos = 0;
    }
    if(x_pos > parentWidget()->width() - this->width()){
        x_pos = parentWidget()->width() - this->width();
    }

    int y_pos = y()+delta.y();  // determining the y position with collisions for the size of the widget
    if(y_pos < 0){
        y_pos = 0;
    }
    if(y_pos > parentWidget()->height() - this->height()){
        y_pos = parentWidget()->height() - this->height();

    }

    this->setGeometry(x_pos,y_pos,100,30); // size propertie
    }

    flag = false; // resetting the flag, flag is used to only execute the move if double clicked happened
}


void Chart::ChartTextBox::mouseMoveEvent(QMouseEvent *event)
{
    if(flag){
        const QPoint delta = event->globalPos() - position;
        position = event->globalPos();

        int x_pos = x()+delta.x(); // determining the x position with collisions for the size of the widget
        if(x_pos < 0){
            x_pos = 0;
        }
        if(x_pos > parentWidget()->width() - this->width()){
            x_pos = parentWidget()->width() - this->width();
        }

        int y_pos = y()+delta.y();  // determining the y position with collisions for the size of the widget
        if(y_pos < 0){
            y_pos = 0;
        }
        if(y_pos > parentWidget()->height() - this->height()){
            y_pos = parentWidget()->height() - this->height();

        }

        this->setGeometry(x_pos,y_pos,100,30); // size propertie
        }

    }

void Chart::ChartTextBox::resize() {

    QString text = this->text();
    QFontMetrics fm = this->fontMetrics();
    int width = fm.boundingRect(text).width();
    if(width < 100){
        width = 100;
    }
    this->setFixedSize(width + 4, this->height());
    }
