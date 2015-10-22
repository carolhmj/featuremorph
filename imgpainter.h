#ifndef IMGPAINTER_H
#define IMGPAINTER_H

#include <QWidget>
#include <QImage>
#include <vector>

/*
 * Classe responsável por desenhar a imagem, os pontos e as feature lines
 */
namespace anim {
    class ImgPainter : public QWidget
    {
    public:
        QImage img;
        int xSize, ySize;



        ImgPainter(QWidget *parent = 0);
        void setImage(QImage img);

    };
}
#endif // IMGPAINTER_H
