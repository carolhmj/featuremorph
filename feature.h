#ifndef FEATURE_H
#define FEATURE_H

#include <QVector2D>
#include <string>
namespace anim{
    class Feature {
    public:
        QVector2D p1, p2, q1, q2;
        float plen, qlen;
        Feature();
        Feature(QVector2D p1, QVector2D p2, QVector2D q1, QVector2D q2);
        ~Feature();
    };
}
#endif // FEATURE_H
