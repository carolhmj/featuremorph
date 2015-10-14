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
    m.addToFeatureList(QVector2D(189,530),QVector2D(162,455),QVector2D(122,546),QVector2D(96,438));
    m.addToFeatureList(QVector2D(434,534),QVector2D(468,401),QVector2D(455,549),QVector2D(477,419));
    m.addToFeatureList(QVector2D(258,479),QVector2D(389,498),QVector2D(213,518),QVector2D(369,509));
    m.addToFeatureList(QVector2D(234,353),QVector2D(299,356),QVector2D(149,312),QVector2D(239,315));
    m.addToFeatureList(QVector2D(371,365),QVector2D(437,372),QVector2D(342,306),QVector2D(419,305));
    m.addToFeatureList(QVector2D(258,579),QVector2D(348,588),QVector2D(221,648),QVector2D(342,651));
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
