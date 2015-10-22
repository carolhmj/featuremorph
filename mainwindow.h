#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "morph.h"
enum featureLineAddStatus {NOT_ADD, ADD_P1, ADD_P2, ADD_Q1, ADD_Q2};

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
    featureLineAddStatus currentStatus;
    QVector2D points[4];
protected:
    //void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void openSourceImg();
    void openDestImg();
    void addNewFeature();
};

#endif // MAINWINDOW_H
