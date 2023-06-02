#include "widget.h"
#include "ui_widget.h"

#include <random>
#include <algorithm>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene=new QGraphicsScene(this);
    ui->graphicsView_visualSorting->setScene(scene);
    ui->graphicsView_visualSorting->verticalScrollBar()->blockSignals(true);
    ui->graphicsView_visualSorting->horizontalScrollBar()->blockSignals(true);
    ui->spinBox_ArraySize->setValue(10);//initialize
    ui->label_AccessCount->setNum(0);
    ui->label_CpCount->setNum(0);qDebug()<<"instructing";

    appStates=0;//-1 App started

    sceneHeight=ui->graphicsView_visualSorting->size().height();qDebug()<<"sH:"<<sceneHeight;
    sceneWidth=ui->graphicsView_visualSorting->size().width();qDebug()<<"sW:"<<sceneWidth;
    comparisionCount=0;


    ui->pushButton_sort->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->spinBox_ArraySize->setEnabled(true);

    ArraySetup(1,100);

}

Widget::~Widget()
{
    stThread->terminate();
    delete ui;
}

void Widget::on_comparision(int i, int j)
{
    auto iRect = columns[i]->rect();
    auto jRect = columns[j]->rect();
    auto iColumnPos = iRect.left();
    auto jColumnPos = jRect.left();

    iRect.moveLeft(jColumnPos);
    jRect.moveLeft(iColumnPos);

    columns[i]->setRect(iRect);
    columns[j]->setRect(jRect);

    std::swap(columns[i], columns[j]);

    comparisionCount++;
    ui->label_CpCount->setNum(comparisionCount);//show its count
}

void Widget::sortButtonStates(int state)
{
    QString buttonText;
    QString style;

    switch(state)
    {
    case 0:
        buttonText = "sort";
        ui->spinBox_ArraySize->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->comboBox_sorting->setEnabled(true);
        style = "background-color: rgba(255, 0, 68, 255); color: #fff";
        break;

    case 1:
        buttonText = "cancel";
        ui->spinBox_ArraySize->setDisabled(true);
        ui->horizontalSlider->setDisabled(true);
        ui->comboBox_sorting->setDisabled(true);
        style = "background-color: #000; color: #fff";
        break;

    case 2:
        buttonText = "new sort";
        ui->pushButton_sort->setEnabled(true);
        style = "background-color: rgb(85, 0, 255); color: #fff";
        break;
    }

    appStates = state;
    ui->pushButton_sort->setText(buttonText);
    ui->pushButton_sort->setStyleSheet(style);
}

void Widget::sortDone(int i)
{
    columns[i]->setBrush(QBrush(QColor(0,220,0)));
}

void Widget::resetArray(int ms, int n)
{
    for(auto &p:columns)
        scene->removeItem(p);

    columnsHeight.clear();qDebug()<<"afterClear:"<<columnsHeight;

    ArraySetup(ms,n);
    threadUpdate(sortingDelay,algorithmKey);
}

void Widget::ArraySetup(int ms, int n)
{
    qDebug()<<"Arraysetup";
    ui->label_CpCount->setNum(0);
    ui->label_AccessCount->setNum(0);


    sortingDelay=ms;qDebug()<<"sortingDelay:"<<sortingDelay;
    ColumnsAmount=n;qDebug()<<"ColumnsAmount:"<<ColumnsAmount;
    columnsWidth=sceneWidth/ColumnsAmount;qDebug()<<"columnsWidth"<<columnsWidth;
    if(columnsWidth<=1)return;

    columns.resize(static_cast<unsigned>(ColumnsAmount));

    double incrementBy=sceneHeight/ColumnsAmount;
    qDebug()<<"sceneHeight:"<<sceneHeight;
    qDebug()<<"incrementBy:"<<incrementBy;

    for(auto i=incrementBy;i<=sceneHeight;i+=incrementBy)
    {
        columnsHeight.push_back(i);
        //qDebug()<<i;
    }
    qDebug()<<columnsHeight;

    static std::random_device m_random;
    static std::mt19937 e_random(m_random());qDebug()<<m_random();
    std::shuffle(columnsHeight.begin(),columnsHeight.end(),e_random);
    qDebug()<<"afterRandom:"<<columnsHeight;

    auto i=0;
    auto j=0.0;
    for(auto &p:columns)
    {
        p=new QGraphicsRectItem;
        p->setRect(j,(sceneHeight-columnsHeight[i]),columnsWidth,columnsHeight[i]);
        p->setBrush(QBrush(QColor(255,0,75,255)));

        if(ColumnsAmount<=200)
            p->setPen(QPen(Qt::black,2));
        else if(ColumnsAmount>200&&ColumnsAmount<=300)
            p->setPen(QPen(Qt::black,1));
        else
            p->setPen(Qt::NoPen);

        scene->addItem(p);

        i++;
        j+=columnsWidth;
    }
}

void Widget::threadUpdate(int ms, int key)//key-selected algorithm;ms-delay
{
    stThread=new sortingThread(ms,key,static_cast<int>(ColumnsAmount),columnsHeight,this);

    connect(stThread,SIGNAL(comparision(int,int)),this,SLOT(on_comparision(int,int)));
    connect(stThread,SIGNAL(sortDone(int)),this,SLOT(sortDone(int)));
    connect(stThread,SIGNAL(arrayAccess(int)),ui->label_AccessCount,SLOT(setNum(int)));
    connect(stThread,SIGNAL(buttonStates(int)),this,SLOT(sortButtonStates(int)));
}

void Widget::randomArray()
{
    std::random_device m_random;
    std::mt19937 e_random(m_random());
    std::shuffle(columnsHeight.begin(),columnsHeight.end(),e_random);
}


void Widget::on_pushButton_sort_clicked()
{
    qDebug()<<"sortButton clicked.";
    algorithmKey=ui->comboBox_sorting->currentIndex();qDebug()<<"algorithmkey:"<<algorithmKey;
    switch(appStates)
    {
        case 0:
            sortButtonStates(1);
            comparisionCount = 0;
            threadUpdate(sortingDelay, algorithmKey);
            stThread->start();
            ui->pushButton_ramdonArray->setDisabled(true);
            qDebug()<<appStates<<"(pushbutton)";
            break;

        case 1:
            stThread->terminate();
            sortButtonStates(2);
            qDebug()<<appStates<<"(pushbutton)";
            break;

        case 2:
            for(auto &p : columns)
                scene->removeItem(p);

            ui->pushButton_ramdonArray->setEnabled(true);
            columnsHeight.clear();
            ArraySetup(sortingDelay, static_cast<int>(ColumnsAmount));
            sortButtonStates(0);
            qDebug()<<appStates<<"(pushbutton)";
            break;

        default:
            qDebug()<<"defaultAppStates";
            break;
    }
}

void Widget::on_pushButton_ramdonArray_clicked()
{
    qDebug()<<"ramdomArrayStart================";
    qDebug()<<"appStates:"<<appStates;
    if(appStates==0)
    {
        resetArray(ui->horizontalSlider->value(),ui->spinBox_ArraySize->value()*10);

    }
    qDebug()<<"appStates::"<<appStates;
    qDebug()<<"randomArrayEnd==================";
}

void Widget::on_horizontalSlider_valueChanged(int ms)
{
    if(appStates==0)
    {
        resetArray(ms,static_cast<int>(ColumnsAmount));
    }
}

void Widget::on_spinBox_ArraySize_valueChanged(int n)
{
    if(appStates==0)
        resetArray(sortingDelay,n*10);
}
