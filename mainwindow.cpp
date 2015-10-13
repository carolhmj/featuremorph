#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "morph.h"
#include <QImage>
#include <QPixmap>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    anim::Morph m = anim::Morph("../img/caraline.jpg", "../img/bradline.jpg", 570, 720, 0.0005, 0.6, 0.5);
    //ui->origLabel->setPixmap(QPixmap::fromImage(m.img1));
    //ui->destLabel->setPixmap(QPixmap::fromImage(m.img2));
    //Terminar de adicionar features!
    m.addToFeatureList(QVector2D(189,720-530),QVector2D(162,720-455),QVector2D(122,720-546),QVector2D(96,720-438));
    m.addToFeatureList(QVector2D(434,720-534),QVector2D(468,720-401),QVector2D(455,720-549),QVector2D(477,720-419));
    m.addToFeatureList(QVector2D(258,720-479),QVector2D(389,720-498),QVector2D(213,720-518),QVector2D(369,720-509));
    m.addToFeatureList(QVector2D(234,720-353),QVector2D(299,720-356),QVector2D(149,720-312),QVector2D(239,720-315));
    m.addToFeatureList(QVector2D(371,720-365),QVector2D(437,720-372),QVector2D(342,720-306),QVector2D(419,720-305));
    m.addToFeatureList(QVector2D(258,720-579),QVector2D(348,720-588),QVector2D(221,720-648),QVector2D(342,720-651));
    float morphInd = 0.0;
    for (int i = 0; i < 120; i++){
        QImage morphed = m.morphStep(morphInd);
        morphInd += 1.0/120.0;
        cout << "morphInd > " << morphInd << "\n";
        flush(cout);
        QString tmp = QString("image");
        QString tmp2;
        tmp2.setNum(i);
        if (i < 10)
            tmp.append("00").append(tmp2);
        else if (i < 100)
            tmp.append("0").append(tmp2);
        else
            tmp.append(tmp2);
        tmp.append(".png");
        morphed.save(tmp);
    }

    //ui->imgLabel->setPixmap(QPixmap::fromImage(morphed));

}

MainWindow::~MainWindow()
{
    delete ui;
}
