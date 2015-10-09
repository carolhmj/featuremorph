#include "feature.h"

anim::Feature::Feature()
{

}

anim::Feature::Feature(QVector2D p1, QVector2D p2, QVector2D q1, QVector2D q2)
{
    this->p1 = p1;
    this->p2 = p2;
    this->q1 = q1;
    this->q2 = q2;
}

anim::Feature::~Feature()
{

}
