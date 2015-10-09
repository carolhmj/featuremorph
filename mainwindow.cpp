#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "morph.h"
#include <QImage>
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    anim::Morph m = anim::Morph("../img/cara.jpg", "../img/brad.jpg", 570, 720, 0.0005, 0.6, 0.5);
    ui->origLabel->setPixmap(QPixmap::fromImage(m.img1));
    ui->destLabel->setPixmap(QPixmap::fromImage(m.img2));
    //Terminar de adicionar features!
    m.addToFeatureList(QVector2D(0,0),QVector2D(265,360),QVector2D(0,0),QVector2D(0,360));
    m.addToFeatureList(QVector2D(0,720),QVector2D(285,180),QVector2D(0,720),QVector2D(570,720));
    //m.addToFeatureList(QVector2D(285,0),QVector2D(285,720),QVector2D(285,0),QVector2D(285,720));
    QImage morphed = m.morphStep(0.5);
    ui->imgLabel->setPixmap(QPixmap::fromImage(morphed));
}

MainWindow::~MainWindow()
{
    delete ui;
}
