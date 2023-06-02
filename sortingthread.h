#ifndef SORTINGTHREAD_H
#define SORTINGTHREAD_H

#include <QThread>
#include <QtWidgets>

class sortingThread:public QThread
{
    Q_OBJECT
public:
    sortingThread(int,int,int,std::vector<double>,QObject*);

signals:
    void comparision(int,int);
    void sortDone(int);
    void arrayAccess(int);
    void buttonStates(int);//button state changes

protected:
    void run()override;

private:
    std::vector<double> columnsHeight;
    int sortDoneDelay;
    int sortingDelay;
    int amount;
    int sortingAlgorithm;
    int arrayAccessVariable;

    void swap(int,int);
    void isAccessToArray();
    void Sorted();//When sorted, green

    void bubbleSort();
    void bubbleSort_Recursive(int);//
    void cocktailSort();
    void gnomeSort();
    void selectionSort();
    void insertionSort();

    int quickSort_partition(int,int);
    void quickSort(int,int);//two array indexs

    int quickSort_partition_2ways(int,int);
    void quickSort2ways(int,int);

    //int quickSort_partition_3ways(int,int);
    void quickSort3ways(int,int);




};

#endif // SORTINGTHREAD_H
