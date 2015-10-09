#include "morph.h"
#include <cmath>
#include <QVector3D>
#include <iostream>

using namespace std;

anim::Morph::Morph()
{

}

anim::Morph::Morph(const char *imgName1, const char *imgName2, float xImgSize, float yImgSize, float a, float b, float p)
{
    this->img1 = QImage(imgName1).scaled(xImgSize,yImgSize);
    this->img2 = QImage(imgName2).scaled(xImgSize,yImgSize);
    this->xImgSize = xImgSize;
    this->yImgSize = yImgSize;
    this->a = a;
    this->b = b;
    this->p = p;

}

anim::Morph::Morph(QString imgName1, QString imgName2, float xImgSize, float yImgSize, float a, float b, float p)
{
    this->img1 = QImage(imgName1).scaled(xImgSize,yImgSize);
    this->img2 = QImage(imgName2).scaled(xImgSize,yImgSize);
    this->xImgSize = xImgSize;
    this->yImgSize = yImgSize;
    this->a = a;
    this->b = b;
    this->p = p;
}

anim::Morph::~Morph()
{

}

void anim::Morph::addToFeatureList(QVector2D p1, QVector2D p2, QVector2D q1, QVector2D q2)
{
    this->featureList.push_back(Feature(p1,p2,q1,q2));
}

//Retorna o vetor obtido pela rotação de v em -90 graus
QVector2D anim::Morph::perpendicularHelper(const QVector2D &v)
{
    return QVector2D(v.y(), -v.x());
}

/*Interpola origem e destino e retorna como destino, e coloca
  origem como origem
*/
anim::Feature anim::Morph::interpolateOrigin(anim::Feature f, float i)
{
    QVector2D nq1 = (1-i)*f.p1 + i*f.q1;
    QVector2D nq2 = (1-i)*f.p2 + i*f.q2;

    return Feature(f.p1,f.p2,nq1,nq2);
}
/*Interpola origem e destino e retorna como destino, e coloca
  destino como origem
*/
anim::Feature anim::Morph::interpolateDest(anim::Feature f, float i)
{
    QVector2D nq1 = (1-i)*f.p1 + i*f.q1;
    QVector2D nq2 = (1-i)*f.p2 + i*f.q2;

    return Feature(f.q1,f.q2,nq1,nq2);
}

void anim::Morph::morph(QImage &origin, QImage &destination, std::vector<Feature> &featureList)
{
    QVector2D p, pdest, pfinaldest, p1, p2, q1, q2, displacement, displacementSum;
    float u, v, distance, weight, weightSum;
    QVector2D S,T,Q;


    for (int i=0; i < this->xImgSize; i++){
        for (int j=0; j < this->yImgSize; j++){
            weightSum = 0;
            displacementSum.setX(0); displacementSum.setY(0);
            p = QVector2D(i,j);

            for (unsigned int k=0; k < featureList.size(); k++){
                p1 = featureList[k].p1;
                p2 = featureList[k].p2;
                q1 = featureList[k].q1;
                q2 = featureList[k].q2;


                /*Dado o pixel (i,j), encontrar as coordenadas (u,v) dele
                 *Mapeamento para u,v é do site http://www.hammerhead.com/thad/morph.html
                 */
//                QVector2D pq1 = p - q1;
//                QVector2D p2p1 = p2 - p1;
//                cout << "pq1: " << pq1.x() << "|" << pq1.y() << " p2p1: " << p2p1.x() << "|" << p2p1.y() << "\n";
//                flush(cout);

                u = QVector2D::dotProduct(p - q1, q2 - q1) / (q2 - q1).lengthSquared();
                //cout << "len q2 - q1: " << (q2-q1).length() << " len perpHelp q2 - q1: " << perpendicularHelper(q2 -q1).length() << "\n";
                flush(cout);
                v = QVector2D::dotProduct(p - q1, perpendicularHelper(q2 - q1)) / (q2 - q1).length();
                pdest = p1 + u*(p2 - p1) + (v*perpendicularHelper(p2 - p1)) / (p2 - p1).length();

                displacement = pdest - p;

                distance = p.distanceToLine(q1, q2 - q1);

                weight = std::pow(((q2 - q1).length() / (this->a + distance)), this->b);

                displacementSum += weight * displacement;
                weightSum += weight;
            }

            pfinaldest = p + displacementSum / weightSum;
            destination.setPixel(p.x(),p.y(),origin.pixel(int(pfinaldest.x()),int(pfinaldest.y())));
        }
    }

}

QImage anim::Morph::morphStep(float interpConstant)
{
    //Primeiro gera a imagem a partir da origem
    QImage intermediateImageFromOrigin = QImage(this->xImgSize,this->yImgSize,this->img1.format());
    intermediateImageFromOrigin.fill(QColor(255,255,255));
    std::vector<Feature> interOriginList;
    for (unsigned int k = 0; k < this->featureList.size(); k++){
        interOriginList.push_back(interpolateOrigin(this->featureList[k], interpConstant));
    }
    //std::vector<Feature> interOriginList = interpolateOrigin(this->featureList, interpConstant);
    morph(this->img1, intermediateImageFromOrigin, interOriginList);

    //Depois gera a imagem a partir do destino
    QImage intermediateImageFromDest = QImage(this->xImgSize,this->yImgSize,this->img2.format());
    intermediateImageFromDest.fill(QColor(255,255,255));
     std::vector<Feature> interDestList;
    for (unsigned int k = 0; k < this->featureList.size(); k++){
        interDestList.push_back(interpolateDest(this->featureList[k], interpConstant));
    }
    //std::vector<Feature> interDestList = interpolateDest(this->featureList, interpConstant);
    morph(this->img2, intermediateImageFromDest, interDestList);

    //Cross-dissolve nas duas imagens
    QImage crossDissolvedImage = QImage(this->xImgSize,this->yImgSize,this->img1.format());
    crossDissolvedImage.fill(QColor(255,255,255));
    for (int i = 0; i < this->xImgSize; i++){
        for (int j = 0; j < this->yImgSize; j++){
            /*Mapeamento das cores em um e em outro (Não podemos multiplicar o QRgb direto, porque ele é)
             *um unsigned int com os valores direto!
             */
            QColor originColor = QColor(intermediateImageFromOrigin.pixel(i,j));
            QColor destColor = QColor(intermediateImageFromDest.pixel(i,j));
            QVector3D originColorV = QVector3D(originColor.red(), originColor.green(), originColor.blue());
            QVector3D destColorV = QVector3D(destColor.red(), destColor.green(), destColor.blue());
            QVector3D pixelColorV = (1-interpConstant)*originColorV + interpConstant*destColorV;
            QColor pixelColor = QColor(int(pixelColorV.x()),int(pixelColorV.y()),int(pixelColorV.z()));

            crossDissolvedImage.setPixel(i,j,pixelColor.rgb());
        }
    }

    return crossDissolvedImage;
}
