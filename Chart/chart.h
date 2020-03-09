#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QChartGlobal>


#include <QtCharts>



class Chart : public QMainWindow{

     Q_OBJECT
     class ChartTextBox; // nested class im defining outside the body

      //friend class ChartTextBox;// the text box needs access to the windows dimensions so it can do border checks


public:
    explicit Chart(QWidget *parent = 0);

    Chart(std::vector<std::vector<QString>> values, std::vector<QString> x_values,std::vector<QString> names,std::vector<QColor> colors,QString x_label, QString type);

    void linear(std::vector<std::vector<QString>> y_values, std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors, QString x_label);
    void scatter(std::vector<std::vector<QString>> y_values, std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors, QString x_label);
    void bar(std::vector<std::vector<QString>> y_values, std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors, QString x_label);
    void pie(std::vector<std::vector<QString>> y_values, std::vector<QString> x_values, std::vector<QString> names, std::vector<QColor> colors, QString x_label);

    ~Chart();

private:
    QtCharts::QChart *chart;
    QtCharts::QChartView *chart_view; // to display a chart we are gonna need a chart view
    ChartTextBox* chart_title;
    ChartTextBox* y_axis_title;

    void fill_vectors(std::vector<QString> &values,std::vector<double> &position,std::vector<int> &pop);
    double std_deviation(const std::vector<double> &position, const std::vector<int> &pop = std::vector<int>());
    double minimum(const std::vector<double> &position, const std::vector<int> &pop = std::vector<int>()); // default arguments woohoo!!
    double maximum(const std::vector<double> &position, const std::vector<int> &pop = std::vector<int>()); // we are using default arguments for the vecotr passed for the y values will be curated
    void set_x_axis(QValueAxis* &x_axis,const std::vector<double> &x_position,const std::vector<int> &x_pop, int length);
    void set_y_axis(QValueAxis* &y_axis,const std::vector<std::vector<double>> &y_position,const std::vector<std::vector<int>> &y_pop);
};


class Chart::ChartTextBox: public QLineEdit // nested class for text boxes
{
    Q_OBJECT

public:
    explicit ChartTextBox(QWidget *parent=0);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void resize();

private:
    QPoint position;
    bool flag;
};





#endif // CHART_H
