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

    void rec();
    void shootScreen();
    void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::textrec *ui;

    QPixmap pixmap;
    QLabel *label = new QLabel();
    QVBoxLayout *layout = new QVBoxLayout();
    QSize screenSize = QApplication::primaryScreen()->size();
    QQuickView view;

    void updateLabel();

};
#endif // TEXTREC_H
