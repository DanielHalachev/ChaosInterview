#include <QObject>
#include <QThread>
#include <vector>
#include <chrono>
#include "progressnotifierbase.h"
#include "lighting/LightingParser.h"

class LightParserWorker : public QObject {
    Q_OBJECT

public:
    LightParserWorker(const std::string &filePath, QProgressBar *progressBar)
        : stopFlag(false), selectedFilePath(filePath), progressBar(progressBar) {}

    void stop() { stopFlag = true; }

public slots:
    void doWork() {
        auto startTime = std::chrono::high_resolution_clock::now();
        ProgressNotifierBase notifier(progressBar);
        LightingParser<double> parser;

        std::future<std::vector<std::vector<double>>> futureResult = std::async(std::launch::async, [&]() {
            return parser.parse(selectedFilePath, &notifier);
        });

        while (futureResult.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            if (stopFlag) {
                emit finished(QPixmap(), 0.0);
                return;
            }
        }

        if (stopFlag) {
            emit finished(QPixmap(), 0.0);
            return;
        }

        std::vector<std::vector<double>> array = futureResult.get();
        QImage image(array[0].size(), array.size(), QImage::Format_Grayscale8);

        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                if (stopFlag) {
                    emit finished(QPixmap(), 0.0);  // Exit early if stop flag is set
                    return;
                }
                float value = array[y][x] / 10.0;
                uint8_t grayscale = static_cast<uint8_t>(std::clamp(value * 255.0f, 0.0f, 255.0f));
                image.setPixel(x, y, qRgb(grayscale, grayscale, grayscale));
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;

        if (!stopFlag) {
            QPixmap pixmap = QPixmap::fromImage(image);
            emit finished(pixmap, duration.count());
        } else {
            emit finished(QPixmap(), 0.0);
        }
    }

signals:
    void finished(QPixmap pixmap, double duration);

private:
    bool stopFlag;
    std::string selectedFilePath;
    QProgressBar* progressBar;
};
