#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsBlurEffect>
#include <QGraphicsColorizeEffect>
#include <QPushButton>

class ImageProcessor : public QWidget {
    Q_OBJECT

public:
    ImageProcessor(QWidget *parent = nullptr) : QWidget(parent) {
        setupUi();
        connectSlots();
    }

private slots:
    void loadImage() {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

        if (!filePath.isEmpty()) {
            originalPixmap.load(filePath);
            displayImage(originalPixmap);
        }
    }

    void applyGrayscale() {
        QImage grayscaleImage = originalPixmap.toImage().convertToFormat(QImage::Format_Grayscale8);
        displayImage(QPixmap::fromImage(grayscaleImage));
    }

    void applyBlur() {
        QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurRadius(5);
        applyEffect(blurEffect);
    }

    void applySharpen() {
        QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect(this);
        colorizeEffect->setColor(Qt::white);
        applyEffect(colorizeEffect);
    }

    void applyEffect(QGraphicsEffect *effect) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        QGraphicsPixmapItem *item = scene->addPixmap(originalPixmap);
        item->setGraphicsEffect(effect);
        displayScene(scene);
    }

    void displayScene(QGraphicsScene *scene) {
        imageView->setScene(scene);
        imageView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }

    void displayImage(const QPixmap &pixmap) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(pixmap);
        displayScene(scene);
    }

private:
    void setupUi() {
        QVBoxLayout *layout = new QVBoxLayout(this);

        imageView = new QGraphicsView(this);
        layout->addWidget(imageView);

        QPushButton *loadImageButton = new QPushButton(tr("Load Image"), this);
        layout->addWidget(loadImageButton);

        QPushButton *grayscaleButton = new QPushButton(tr("Grayscale"), this);
        layout->addWidget(grayscaleButton);

        QPushButton *blurButton = new QPushButton(tr("Blur"), this);
        layout->addWidget(blurButton);

        QPushButton *sharpenButton = new QPushButton(tr("Sharpen"), this);
        layout->addWidget(sharpenButton);

        setLayout(layout);
    }

    void connectSlots() {
        connect(loadImageButton, &QPushButton::clicked, this, &ImageProcessor::loadImage);
        connect(grayscaleButton, &QPushButton::clicked, this, &ImageProcessor::applyGrayscale);
        connect(blurButton, &QPushButton::clicked, this, &ImageProcessor::applyBlur);
        connect(sharpenButton, &QPushButton::clicked, this, &ImageProcessor::applySharpen);
    }

    QPixmap originalPixmap;
    QGraphicsView *imageView;
    QPushButton *loadImageButton;
    QPushButton *grayscaleButton;
    QPushButton *blurButton;
    QPushButton *sharpenButton;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ImageProcessor imageProcessor;
    imageProcessor.show();

    return app.exec();
}

#include "main.moc"

