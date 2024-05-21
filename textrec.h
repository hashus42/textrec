#ifndef TEXTREC_H
#define TEXTREC_H

#include <QWidget>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

#include <QScreen>
#include <QWindow>
#include <qlabel.h>
#include <QVBoxLayout>
#include <qsize.h>
#include <QApplication>
#include <QResizeEvent>
#include <QDebug>
#include <QtQuick/QQuickView>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>

QT_BEGIN_NAMESPACE
namespace Ui {
class textrec;
}
QT_END_NAMESPACE

class textrec : public QWidget
{
    Q_OBJECT

public:
    textrec(QWidget *parent = nullptr);
    ~textrec();

    int start_x;
    int start_y;
    int x_pos;
    int y_pos;
    int rect_width;
    int rect_height;
    bool drawing;

private:
    Ui::textrec *ui;

    QPixmap pixmap;
    QPixmap drawing_pixmap;
    QLabel *label = new QLabel();
    QVBoxLayout *layout = new QVBoxLayout();
    QSize screenSize = QApplication::primaryScreen()->size();
    QQuickView view;

    void updateLabel();
    void rec(const QPixmap &pixmap);
    void shootScreen();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
};
#endif // TEXTREC_H
