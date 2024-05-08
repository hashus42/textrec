#include "textrec.h"
#include "ui_textrec.h"

textrec::textrec(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::textrec)
{
    ui->setupUi(this);

    shootScreen();
    // pixmap = pixmap.scaled(pixmap.width() * 0.99, pixmap.height() * 0.98, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // label->setAlignment(Qt::AlignCenter);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->move(-11, -11);

    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setAlignment(Qt::AlignCenter);

    // Adjsut the label according to screen size
    // const QRect screenGeometry = screen()->geometry();
    // label->setMinimumSize(screenGeometry.width(), screenGeometry.height() * 0.953);

    label->setPixmap(pixmap);
    layout->addWidget(label);
    setLayout(layout);
}



void textrec::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        this->close();
    }
}

void textrec::updateLabel() {
    label->setPixmap(pixmap.scaled(label->size(),
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation));
}

void textrec::resizeEvent(QResizeEvent *event) {
    QSize scaledSize = pixmap.size();
    scaledSize.scale(label->size(), Qt::KeepAspectRatio);
    if (scaledSize != label->pixmap().size())
        updateLabel();
}

void textrec::shootScreen() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    pixmap = screen->grabWindow(0);
}

void textrec::rec() {
    // Initialize Tesseract OCR
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng"); // Specify language ("eng" for English)

    // Read the image using OpenCV
    cv::Mat cvImage = cv::imread("/home/pardusumsu/code/textrec/test-imgs/yamuq.png", cv::IMREAD_COLOR);
    if (cvImage.empty()) {
        std::cerr << "Error: Unable to open image file!" << std::endl;
    }

    // Convert OpenCV image to Leptonica's Pix format
    Pix *image = pixCreate(cvImage.size().width, cvImage.size().height, 8);
    for (int y = 0; y < cvImage.rows; ++y) {
        for (int x = 0; x < cvImage.cols; ++x) {
            cv::Vec3b color = cvImage.at<cv::Vec3b>(cv::Point(x, y));
            int val = (color[0] + color[1] + color[2]) / 3; // Convert to grayscale
            pixSetPixel(image, x, y, val);
        }
    }

    // Set the image to Tesseract
    tess.SetImage(image);

    // Perform OCR
    char* outText = tess.GetUTF8Text();
    std::cout << "Recognized Text:" << std::endl << outText << std::endl;

    // Clean up
    delete[] outText;
    pixDestroy(&image);
    tess.End();
}

textrec::~textrec()
{
    delete ui;
}
