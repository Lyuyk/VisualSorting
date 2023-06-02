#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "sortingthread.h"

//QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
//QT_END_NAMESPACE

class sortingThread;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

private slots:
    void on_pushButton_sort_clicked();
    //void on_algorithmSelection_itemClicked(QListWidgetItem*);
    //void on_countChanger_valueChanged(int);
    //void on_delayChanger_valueChanged(int);

    void on_comparision(int,int);
    void sortButtonStates(int);
    void sortDone(int);
    void resetArray(int,int);

    void on_pushButton_ramdonArray_clicked();

    void on_horizontalSlider_valueChanged(int ms);

    void on_spinBox_ArraySize_valueChanged(int n);

private:

    sortingThread *stThread;
    QGraphicsScene *scene;

    void ArraySetup(int,int);
    void threadUpdate(int,int);

    void randomArray();

    std::vector<QGraphicsRectItem*> columns;
    std::vector<double> columnsHeight;

    //QMap<int,QString> AlgorithmList;

    int sortingDelay;

    double columnsWidth;
    double sceneHeight;
    double sceneWidth;

    int ColumnsAmount;
    int comparisionCount;

    int algorithmKey;

    int appStates;
};
#endif // WIDGET_H
