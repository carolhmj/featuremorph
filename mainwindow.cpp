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
    QPainter testPainter(&tmp);
    testPainter.setPen(QColor(255,0,0));
    testPainter.drawLine(QPointF(20,30),QPointF(30,40));
    ui->origLabel->setPixmap(tmp);
    ui->destLabel->setPixmap(tmp);
    ui->imgLabel->setPixmap(tmp);


    connect(ui->openSource,SIGNAL(clicked(bool)),this,SLOT(openSourceImg()));
    connect(ui->openDest,SIGNAL(clicked(bool)),this,SLOT(openDestImg()));
    connect(ui->addFeature,SIGNAL(clicked(bool)),this,SLOT(addNewFeature()));
    connect(ui->morphSingle,SIGNAL(clicked(bool)),this,SLOT(morphSingle()));
    connect(ui->morphAnimation,SIGNAL(clicked(bool)),this,SLOT(morphAnimation()));
    //anim::Morph m = anim::Morph("../img/caraline.jpg", "../img/bradline.jpg", 570, 720, 0.0005, 0.6, 0.5);
    //ui->origLabel->setPixmap(QPixmap::fromImage(m.img1));
    //ui->destLabel->setPixmap(QPixmap::fromImage(m.img2));
    morph.setOriginImg("../img/cara.jpg");
    morph.setDestImg("../img/brad.jpg");
    morph.addToFeatureList(QVector2D(189,530),QVector2D(162,455),QVector2D(122,546),QVector2D(96,438));
    morph.addToFeatureList(QVector2D(434,534),QVector2D(468,401),QVector2D(455,549),QVector2D(477,419));
    morph.addToFeatureList(QVector2D(258,479),QVector2D(389,498),QVector2D(213,518),QVector2D(369,509));
    morph.addToFeatureList(QVector2D(234,353),QVector2D(299,356),QVector2D(149,312),QVector2D(239,315));
    morph.addToFeatureList(QVector2D(371,365),QVector2D(437,372),QVector2D(342,306),QVector2D(419,305));
    morph.addToFeatureList(QVector2D(258,579),QVector2D(348,588),QVector2D(221,648),QVector2D(342,651));
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
    drawFeatures();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int xPos = event->pos().x();
    int yPos = event->pos().y();
    int xPosWidg, yPosWidg;
    switch(this->currentStatus){
    case ADD_P1:
        if (ui->origLabel->underMouse()) {
            xPosWidg = xPos - ui->origLabel->x();
            yPosWidg = yPos - ui->origLabel->y();
            qDebug() << "[" << xPosWidg << " " << yPosWidg << "]\n";
            points[0] = QVector2D(xPosWidg,yPosWidg);
            this->currentStatus = ADD_P2;
        }
        break;
    case ADD_P2:
        if (ui->origLabel->underMouse()){
            xPosWidg = xPos - ui->origLabel->x();
            yPosWidg = yPos - ui->origLabel->y();
            qDebug() << "[" << xPosWidg << " " << yPosWidg << "]\n";
            points[1] = QVector2D(xPosWidg,yPosWidg);
            this->currentStatus = ADD_Q1;
        }
        break;
    case ADD_Q1:
        if (ui->destLabel->underMouse()){
            xPosWidg = xPos - ui->destLabel->x();
            yPosWidg = yPos - ui->destLabel->y();
            qDebug() << "[" << xPosWidg << " " << yPosWidg << "]\n";
            points[2] = QVector2D(xPosWidg,yPosWidg);
            this->currentStatus = ADD_Q2;
        }
        break;
    case ADD_Q2:
        if (ui->destLabel->underMouse()){
            xPosWidg = xPos - ui->destLabel->x();
            yPosWidg = yPos - ui->destLabel->y();
            qDebug() << "[" << xPosWidg << " " << yPosWidg << "]\n";
            points[3] = QVector2D(xPosWidg,yPosWidg);
            this->currentStatus = ADD_ALL;
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

//void MainWindow::resizeEvent(QResizeEvent *)
//{
//    this->imgWidth = ui->origLabel->width();
//    this->imgHeight = ui->origLabel->height();

//    morph.changeImgSize(this->imgWidth,this->imgHeight);
//}

void MainWindow::openSourceImg()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open Source Image"), "$$PWD/../img", tr("Image files (*.png *.jpg *.bmp)"));
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

}

void MainWindow::drawFeatures()
{
    QPixmap sourcePix = QPixmap::fromImage(morph.img1);
    QPixmap destPix = QPixmap::fromImage(morph.img2);
    QPainter sourcePaint(&sourcePix);
    QPainter destPaint(&destPix);
    for (auto f : morph.featureList){
        //qDebug() << "p1: [" << f.p1.x() << " | " << f.p1.y() << "] p2: [" << f.p2.x() << " | " << f.p2.y() << "] q1: [" << f.q1.x() << " | " << f.q1.y() << "] q2: [" << f.q2.x() << " | " << f.q2.y() << "]\n";
        sourcePaint.drawLine(f.p1.toPointF(),f.p2.toPointF());
        destPaint.drawLine(f.q1.toPointF(),f.q2.toPointF());
    }
    //sourcePaint.drawLine(QPointF(0,0),QPointF(135,240));
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
    progress.close();
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
    qDebug() << "video size" << video.size() << "\n";
    for (auto i : video){
        ui->imgLabel->setPixmap(QPixmap::fromImage(i));
        delay(1000/FRAMERATE);
    }
}
