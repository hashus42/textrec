#include "textrec.h"
#include "ui_textrec.h"

textrec::textrec(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::textrec)
{
    ui->setupUi(this);

    // Screen shot
    shootScreen();

    // Fine tunings to adjust window in the best borderless full window
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->move(-11, -11);

    // label adjustments
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setAlignment(Qt::AlignCenter);

    // To give tesseract drawless pixmap give copied pixmap as input to it
    drawing_pixmap = pixmap.copy();

    // Set pixmap to label and label to layout
    label->setPixmap(drawing_pixmap);
    layout->addWidget(label);
    setLayout(layout);
}

// Get mouse starting point (-10 to reduce mouse size)
void textrec::mousePressEvent(QMouseEvent *event) {
    start_x = event->pos().x() - 10;
    start_y = event->pos().y() - 10;
}

// Get mouse current point (-10 to reduce mouse size)
void textrec::mouseMoveEvent(QMouseEvent *event) {
    x_pos = event->pos().x() - 10;
    y_pos = event->pos().y() - 10;
    drawRectangle();
}

void textrec::mouseReleaseEvent(QMouseEvent *event) {
    int rect_width = x_pos - start_x;
    int rect_height = y_pos - start_y;

    // Handles x and y positions to handle the situation that mouse is going to backward
    // in x axis or y axis otherwise the cropping process crops the pixmap in a wierd way
    if (rect_width < 0) {
        rect_width = start_x - x_pos;
        start_x = x_pos;
    }

    if (rect_height < 0) {
        rect_height = start_y - y_pos;
        start_y = y_pos;
    }

    // crops the pixmap
    QRect rect(start_x, start_y, rect_width, rect_height);
    QPixmap cropped = drawing_pixmap.copy(rect);

    // give cropped image as input to tesseract function
    rec(cropped);

    // Close the window after the drawing and recognition process finish
    if (event->button() == Qt::LeftButton) {
        this->close();
    }
}

// Starts to paint
void textrec::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawPixmap(0, 0, drawing_pixmap);
}

void textrec::drawRectangle() {
    // Create a temporary pixmap to draw the rectangle
    QPixmap temp_pixmap = drawing_pixmap;
    QPainter painter(&temp_pixmap);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::green);
    painter.setPen(pen);

    painter.drawRect(start_x, start_y, x_pos - start_x, y_pos - start_y);

    // Update the label with the temporary pixmap
    label->setPixmap(temp_pixmap);
    label->update(); // Ensure the label is updated
}

void textrec::shootScreen() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    pixmap = screen->grabWindow(0);
}

void textrec::rec(const QPixmap &pixmap) {
    QImage qImage = pixmap.toImage();

    // Convert QImage to cv::Mat
    cv::Mat mat(qImage.height(), qImage.width(), CV_8UC4, const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR); // Convert from BGRA to BGR

    // Initialize Tesseract OCR
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng"); // Specify language ("eng" for English)

    // Read the image using OpenCV
    // cv::Mat cvImage = cv::imread("/home/pardusumsu/code/textrec/test-imgs/yamuq.png", cv::IMREAD_COLOR);
    cv::Mat cvImage = mat;
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

    copyClipboard(outText);
    sendNotification(outText);

    // Clean up
    delete[] outText;
    pixDestroy(&image);
    tess.End();
}

// Copy the text to clipboard
void textrec::copyClipboard(char* outText) {
    const size_t len = strlen(outText);

    // Open a pipe to the xclip command
    FILE* pipe = popen("xclip -selection clipboard", "w");
    if (!pipe) {
        std::cerr << "Failed to open pipe to xclip" << std::endl;
    }

    // Write the output to the pipe
    fwrite(outText, sizeof(char), len, pipe);

    // Close the pipe
    pclose(pipe);
}

// Send an notification to user to inform him about recognized text
void textrec::sendNotification(char* outText) {
    std::string notify = "notify-send 'The recognized text copied to clipboard' '";
    std::string command = notify + outText + "' -t 5000";
    system(command.c_str());
}

textrec::~textrec()
{
    delete ui;
}
