#ifndef MORPH_H
#define MORPH_H

#include <QImage>
#include <QVector2D>
#include <QColor>
#include <QString>
#include <vector>
#include "feature.h"

namespace anim {

    class Morph
    {
    private:
    public:
        std::vector<Feature> featureList;
        QImage img1, img2;
        float xImgSize, yImgSize;
        float a,b,p;

        Morph();
        Morph(const char* imgName1, const char* imgName2, float xImgSize, float yImgSize, float a, float b, float p);
        Morph(QString imgName1, QString imgName2, float xImgSize, float yImgSize, float a, float b, float p);
        ~Morph();
        void addToFeatureList(QVector2D p1, QVector2D p2, QVector2D q1, QVector2D q2);
        QVector2D perpendicularHelper(const QVector2D &v);
        Feature interpolateOrigin(Feature f, float i);
        Feature interpolateDest(Feature f, float i);
        void morph(QImage &origin, QImage &destination, std::vector<Feature> &featureList);
        QImage morphStep(float interpConstant);
    };
}

#endif // MORPH_H
