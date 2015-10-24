#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "morph.h"

#define FRAMERATE 24

enum featureLineAddStatus {ADD_P1, ADD_P2, ADD_Q1, ADD_Q2, ADD_ALL};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    anim::Morph morph;
    int imgWidth;
    int imgHeight;
    int selectedFeatureIndex = -1;
    featureLineAddStatus currentStatus = ADD_P1;
    QVector2D points[4];
protected:
    //void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    //void paintEvent(QPaintEvent *event);
    void delay(int millisecondsToWait);
    int rand(int low, int high);
public slots:
    void openSourceImg();
    void openDestImg();
    void addNewFeature();
    void drawFeatures();
    void morphSingle();
    void morphAnimation();
    void setSelectedFeature(int index);
    void deleteSelectedFeature();
};

#endif // MAINWINDOW_H
