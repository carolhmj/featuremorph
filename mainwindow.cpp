#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "morph.h"
#include <QImage>
#include <QPixmap>
#include <iostream>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QProgressDialog>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QtGlobal>
#include <vector>
#include <cstdlib>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->imgWidth = 270;
    this->imgHeight = 480;

    //Cria o morph usando o tamanho do widget de visualização
    this->morph = anim::Morph(imgWidth, imgHeight, 0.0005, 0.6, 0.5);
    QPixmap tmp = QPixmap(imgWidth, imgHeight);
    tmp.fill(QColor(0,0,0));
    ui->origLabel->setPixmap(tmp);
    ui->destLabel->setPixmap(tmp);
    ui->imgLabel->setPixmap(tmp);


    connect(ui->openSource,SIGNAL(clicked(bool)),this,SLOT(openSourceImg()));
    connect(ui->openDest,SIGNAL(clicked(bool)),this,SLOT(openDestImg()));
    connect(ui->addFeature,SIGNAL(clicked(bool)),this,SLOT(addNewFeature()));
    connect(ui->morphSingle,SIGNAL(clicked(bool)),this,SLOT(morphSingle()));
    connect(ui->morphAnimation,SIGNAL(clicked(bool)),this,SLOT(morphAnimation()));
    connect(ui->addFeature,SIGNAL(clicked(bool)),this,SLOT(addNewFeature()));
    connect(ui->featureSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(setSelectedFeature(int)));
    //anim::Morph m = anim::Morph("../img/caraline.jpg", "../img/bradline.jpg", 570, 720, 0.0005, 0.6, 0.5);
    //ui->origLabel->setPixmap(QPixmap::fromImage(m.img1));
    //ui->destLabel->setPixmap(QPixmap::fromImage(m.img2));
    morph.setOriginImg("../img/cara.jpg");
    morph.setDestImg("../img/brad.jpg");
//    morph.addToFeatureList(QVector2D(189,530),QVector2D(162,455),QVector2D(122,546),QVector2D(96,438));
//    morph.addToFeatureList(QVector2D(434,534),QVector2D(468,401),QVector2D(455,549),QVector2D(477,419));
//    morph.addToFeatureList(QVector2D(258,479),QVector2D(389,498),QVector2D(213,518),QVector2D(369,509));
//    morph.addToFeatureList(QVector2D(234,353),QVector2D(299,356),QVector2D(149,312),QVector2D(239,315));
//    morph.addToFeatureList(QVector2D(371,365),QVector2D(437,372),QVector2D(342,306),QVector2D(419,305));
//    morph.addToFeatureList(QVector2D(258,579),QVector2D(348,588),QVector2D(221,648),QVector2D(342,651));
//    float morphInd = 0.0;
//    for (int i = 0; i < 120; i++){
//        QImage morphed = m.morphStep(morphInd);
//        morphInd += 1.0/120.0;
//        cout << "morphInd > " << morphInd << "\n";
//        flush(cout);
//        QString tmp = QString("image");
//        QString tmp2;
//        tmp2.setNum(i);
//        if (i < 10)
//            tmp.append("00").append(tmp2);
//        else if (i < 100)
//            tmp.append("0").append(tmp2);
//        else
//            tmp.append(tmp2);
//        tmp.append(".png");
//        morphed.save(tmp);
//    }

    //ui->imgLabel->setPixmap(QPixmap::fromImage(morphed));
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    drawFeatures();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    switch(this->currentStatus){
    case ADD_P1:
        if (ui->origLabel->underMouse()) {
            points[0] = QVector2D( ui->origLabel->mapFrom(this,pos) );
            this->currentStatus = ADD_P2;
            drawFeatures();
        }
        break;
    case ADD_P2:
        if (ui->origLabel->underMouse()){
            points[1] = QVector2D( ui->origLabel->mapFrom(this,pos) );
            this->currentStatus = ADD_Q1;
            drawFeatures();
        }
        break;
    case ADD_Q1:
        if (ui->destLabel->underMouse()){
            points[2] = QVector2D( ui->destLabel->mapFrom(this,pos) );
            this->currentStatus = ADD_Q2;
            drawFeatures();
        }
        break;
    case ADD_Q2:
        if (ui->destLabel->underMouse()){
            points[3] = QVector2D( ui->destLabel->mapFrom(this,pos) );
            this->currentStatus = ADD_ALL;
            drawFeatures();
        }
        break;
    case ADD_ALL:
        break;
    }
}

void MainWindow::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

int MainWindow::rand(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

//void MainWindow::resizeEvent(QResizeEvent *)
//{
//    this->imgWidth = ui->origLabel->width();
//    this->imgHeight = ui->origLabel->height();

//    morph.changeImgSize(this->imgWidth,this->imgHeight);
//}

void MainWindow::openSourceImg()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open Source Image"), "../img", tr("Image files (*.png *.jpg *.bmp)"));
    qDebug() << name << "\n";
    if (!name.isEmpty()) {
        morph.setOriginImg(name);
    }
    ui->origLabel->setPixmap(QPixmap::fromImage(morph.img1));
}

void MainWindow::openDestImg()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open Source Image"), "$$PWD/img", tr("Image files (*.png *.jpg *.bmp)"));
    if (!name.isEmpty()) {
        morph.setDestImg(name);
    }
    ui->destLabel->setPixmap(QPixmap::fromImage(morph.img2));
}

void MainWindow::addNewFeature()
{
    if (this->currentStatus == ADD_ALL){
        QString defaultName;
        defaultName.append("Feature ");
        defaultName.append(QString().setNum(this->morph.featureList.size()+1));

        bool ok;
        QString name = QInputDialog::getText(this,tr("Feature"),tr("Input the Feature Name:"),QLineEdit::Normal,defaultName, &ok);
        if (ok) {
            if (name.isEmpty()) {
                name = defaultName;
            }
            QColor color(rand(0,255),rand(0,255),rand(0,255));
            morph.addToFeatureList(points[0],points[1],points[2],points[3],name,color);
            this->currentStatus = ADD_P1;
            ui->featureSelect->addItem(name);
        }
    }
    drawFeatures();
}

void MainWindow::drawFeatures()
{
    //Setup dos elementos de desenho
    QPixmap sourcePix = QPixmap::fromImage(morph.img1);
    QPixmap destPix = QPixmap::fromImage(morph.img2);
    QPainter sourcePaint(&sourcePix);
    QPainter destPaint(&destPix);   
    QPen sourcePen;
    QPen destPen;
    sourcePen.setWidth(3);
    destPen.setWidth(3);
    sourcePaint.setPen(sourcePen);
    sourcePaint.setRenderHint(QPainter::Antialiasing);

    destPaint.setPen(destPen);
    destPaint.setRenderHint(QPainter::Antialiasing);

    //Loop de desenho dos feature prontos
    int i = 0;
    for (auto f : morph.featureList){
        //qDebug() << "p1: [" << f.p1.x() << " | " << f.p1.y() << "] p2: [" << f.p2.x() << " | " << f.p2.y() << "] q1: [" << f.q1.x() << " | " << f.q1.y() << "] q2: [" << f.q2.x() << " | " << f.q2.y() << "]\n";
        sourcePen.setColor(morph.featureColors.at(i));
        destPen.setColor(morph.featureColors.at(i));
        if (i == this->selectedFeatureIndex) {
            sourcePen.setStyle(Qt::DotLine);
            destPen.setStyle(Qt::DotLine);
        } else {
            sourcePen.setStyle(Qt::SolidLine);
            destPen.setStyle(Qt::SolidLine);
        }
        sourcePaint.setPen(sourcePen);
        destPaint.setPen(destPen);
        sourcePaint.drawLine(f.p1.toPointF(),f.p2.toPointF());
        destPaint.drawLine(f.q1.toPointF(),f.q2.toPointF());
        i++;
    }

    //Loop de desenho dos features em construção
    sourcePen.setColor(Qt::black);
    sourcePen.setStyle(Qt::SolidLine);
    destPen.setColor(Qt::black);
    destPen.setStyle(Qt::SolidLine);
    sourcePaint.setPen(sourcePen);
    destPaint.setPen(destPen);
    switch(this->currentStatus){
    case ADD_P1:
        break;
    case ADD_P2:
        sourcePaint.drawPoint(points[0].toPointF());
        break;
    case ADD_Q1:
        sourcePaint.drawLine(points[0].toPointF(),points[1].toPointF());
        break;
    case ADD_Q2:
        sourcePaint.drawLine(points[0].toPointF(),points[1].toPointF());
        destPaint.drawPoint(points[2].toPointF());
        break;
    case ADD_ALL:
        sourcePaint.drawLine(points[0].toPointF(),points[1].toPointF());
        destPaint.drawLine(points[2].toPointF(),points[3].toPointF());
        break;
    }

    ui->origLabel->setPixmap(sourcePix);
    ui->destLabel->setPixmap(destPix);
}

void MainWindow::morphSingle()
{
    float morphAmount = ui->morphSingleAmount->value();
    QMessageBox progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setText("Generating image...");
    progress.exec();
    QImage morphedImage = morph.morphStep(morphAmount);
    ui->imgLabel->setPixmap(QPixmap::fromImage(morphedImage));
}

void MainWindow::morphAnimation()
{
    float animSeconds = ui->morphAnimationSec->value();
    std::vector<QImage> video;
    float morphInd = 0.0;
    int numFrames = animSeconds * FRAMERATE;
    QProgressDialog progress("Generating image frames...", "Cancel", 1, numFrames, this);
    progress.setWindowModality(Qt::WindowModal);
    for (int i = 0; i < numFrames; i++){
        QImage morphed = morph.morphStep(morphInd);
        progress.setValue(i+1);
        morphInd += 1.0/numFrames;
        video.push_back(morphed);
        if (progress.wasCanceled()) {
            break;
        }
    }
    progress.setValue(numFrames);
    QMessageBox completionDialog;
    completionDialog.setWindowModality(Qt::WindowModal);
    completionDialog.setText("Generating done!");
    completionDialog.exec();
   //qDebug() << "video size" << video.size() << "\n";
    for (auto i : video){
        ui->imgLabel->setPixmap(QPixmap::fromImage(i));
        delay(1000/FRAMERATE);
    }
}

void MainWindow::setSelectedFeature(int index)
{
    qDebug() << "selected feature " << index << "\n";
    this->selectedFeatureIndex = index;
    drawFeatures();
}

void MainWindow::deleteSelectedFeature()
{
    if (this->selectedFeatureIndex > 0){
        morph.featureList.erase(morph.featureList.begin()+selectedFeatureIndex);
        morph.featureNames.erase(morph.featureNames.begin()+selectedFeatureIndex);
        morph.featureColors.erase(morph.featureColors.begin()+selectedFeatureIndex);
    }
    this->selectedFeatureIndex = -1;
}
