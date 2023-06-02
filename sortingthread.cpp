#include "sortingthread.h"

sortingThread::sortingThread(int ms,int algorithm,int n,std::vector<double>columns,QObject *parent=nullptr):QThread(parent)
{
    sortingAlgorithm=algorithm;
    amount=n;
    columnsHeight=columns;
    arrayAccessVariable=0;

    sortDoneDelay=(amount>=300?2:5);
    sortingDelay=ms;
}

void sortingThread::run()
{
    switch(sortingAlgorithm)
    {
    case 0:bubbleSort();break;
    case 1:bubbleSort_Recursive(amount);break;
    case 2:cocktailSort();break;
    case 3:gnomeSort();break;
    case 4:selectionSort();break;
    case 5:insertionSort();break;
    case 6:quickSort(0,amount-1);break;
    case 7:quickSort2ways(0,amount-1);break;
    case 8:quickSort3ways(0,amount-1);break;
    }
    Sorted();
}

void sortingThread::swap(int i, int j)
{
    std::swap(columnsHeight[i],columnsHeight[j]);
    emit comparision(i,j);
}

void sortingThread::isAccessToArray()
{
    arrayAccessVariable++;
    emit arrayAccess(arrayAccessVariable);
}

void sortingThread::Sorted()
{
    for(auto i=0;i<amount;i++)
    {
        emit sortDone(i);
        msleep(sortDoneDelay);
    }
    emit buttonStates(2);//2-finished sorting
}
//----------------------------------------------------
void sortingThread::bubbleSort()
{
    for(auto i=0;i<amount;++i)
    {
        for(auto j=0;j<amount-1;++j)
        {
            if(columnsHeight[j]>columnsHeight[j+1])
                swap(j,j+1);

            isAccessToArray();
            msleep(sortingDelay);
        }
    }
}

void sortingThread::bubbleSort_Recursive(int n)
{
    if (n == 1)
        return;

    for (int i=0; i<n-1; i++)
    {
        if (columnsHeight[i] > columnsHeight[i+1])
            swap(i, i+1);

        isAccessToArray();
        msleep(sortingDelay);
    }
    bubbleSort_Recursive(n-1);
}

void sortingThread::cocktailSort()
{
    bool swapped = true;
    auto start = 0;
    auto end = amount-1;

    while(swapped)
    {
        swapped = false;
        for (auto i = start; i < end; i++)
        {
            if(columnsHeight[i] > columnsHeight[i + 1])
            {
                swap(i, i+1);
                swapped = true;
            }
            isAccessToArray();
            msleep(sortingDelay);
        }

        if (!swapped)
            break;

        swapped = false;
        end--;

        for (auto i = end-1; i >= start; i--)
        {
            if (columnsHeight[i] > columnsHeight[i + 1])
            {
                swap(i, i+1);
                swapped = true;
            }
            isAccessToArray();
            msleep(sortingDelay);
        }
        start++;
    }
}

void sortingThread::gnomeSort()
{
    int index = 0;

    while (index < amount)
    {
        if (index == 0)
            index++;
        if (columnsHeight[index] >= columnsHeight[index-1])
            index++;
        else
        {
            swap(index, index-1);
            index--;
        }
        isAccessToArray();
        msleep(sortingDelay);
    }
}

void sortingThread::selectionSort()
{
    for(auto i=0;i<amount;++i)
    {
        int min_index=i;
        for(auto j=i+1;j<amount;++j)
        {
            if(columnsHeight[j]<columnsHeight[min_index])
                min_index=j;

            isAccessToArray();
            msleep(sortingDelay);
        }
        swap(i,min_index);

        isAccessToArray();
        msleep(sortingDelay);
    }
}

void sortingThread::insertionSort()
{
    for(auto i=0;i<amount;++i)
    {
        for(auto j=i;j>0&&columnsHeight[j]<columnsHeight[j-1];--j)
        {
            swap(j,j-1);

            isAccessToArray();
            msleep(sortingDelay);
        }
    }
}

int sortingThread::quickSort_partition(int indexBegin, int indexEnd)
{
    auto pivot=columnsHeight[indexEnd];
    auto i=(indexBegin-1);

    for(auto j=indexBegin;j<=indexEnd-1;j++)
    {
        if(columnsHeight[j]<=pivot)
        {
            i++;
            swap(i,j);
        }

        isAccessToArray();
        msleep(sortingDelay);
    }

    swap(i+1,indexEnd);

    msleep(sortingDelay);
    return (i+1);
}

void sortingThread::quickSort(int indexBegin, int indexEnd)
{
    if(indexBegin<indexEnd)
    {
        auto pi=quickSort_partition(indexBegin,indexEnd);

        quickSort(indexBegin,pi-1);
        quickSort(pi+1,indexEnd);
    }
}

int sortingThread::quickSort_partition_2ways(int indexBegin, int indexEnd)
{
    auto pivot = columnsHeight[indexBegin];
    auto i=indexBegin+1;
    auto j=indexEnd;
    while (true)
    {
        while (i <= indexEnd && columnsHeight[i] <= pivot)
        {
            i++;
            isAccessToArray();
            msleep(sortingDelay);
        }
        while (j >= indexBegin && columnsHeight[j] > pivot)
        {
            j--;
            isAccessToArray();
            msleep(sortingDelay);
        }

        if (i > j)
        {
            isAccessToArray();
            msleep(sortingDelay);
            break;
        }

        swap(i++, j--);
    }
    swap(indexBegin, j);

    return j;
}

void sortingThread::quickSort2ways(int indexBegin,int indexEnd)
{
    if(indexBegin<indexEnd)
    {
        auto piv=quickSort_partition_2ways(indexBegin,indexEnd);
        quickSort2ways(indexBegin,piv-1);
        quickSort2ways(piv+1,indexEnd);
    }
}

void sortingThread::quickSort3ways(int indexBegin, int indexEnd)
{
    if (indexBegin >= indexEnd)
        return;

    int pivot = columnsHeight[indexBegin];
    // columnsHeight[l ... lt-1] < pivot
    // columnsHeight[lt ... gt-1] == pivot
    // columnsHeight[gt ... r] > pivot
    int lt = indexBegin;
    int i = indexBegin + 1;
    int gt = indexEnd + 1;
    while (i < gt)
    {
        if (columnsHeight[i] < pivot)
        {swap(i++, ++lt);
            isAccessToArray();
            msleep(sortingDelay);}


        else if (columnsHeight[i] > pivot)
        {
            swap(i, --gt);
            isAccessToArray();
            msleep(sortingDelay);
        }

        else
        {
            i++;
            isAccessToArray();
            msleep(sortingDelay);
        }
    }

    swap(indexBegin, lt);

    quickSort3ways( indexBegin, lt - 1);
    quickSort3ways( gt, indexEnd);
}



