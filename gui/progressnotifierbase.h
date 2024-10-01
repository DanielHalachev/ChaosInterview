#ifndef PROGRESSNOTIFIERBASE_H
#define PROGRESSNOTIFIERBASE_H

#include "lighting/ProgressNotifier.h"
#include "QProgressBar"

class ProgressNotifierBase : public ProgressNotifier{
private:
    QProgressBar* progressBar;
public:
    ProgressNotifierBase(QProgressBar* progressBar){
        this->progressBar = progressBar;
    }
    virtual void reportProgress(unsigned short done, unsigned short total) final {
        // this causes a crash
        // this->progressBar->setValue(std::clamp(static_cast<int>(100.0 * (static_cast<double>(done)/static_cast<double>(total))), 0, 100));
        QMetaObject::invokeMethod(progressBar, "setValue", Q_ARG(int, std::clamp(static_cast<int>(100.0 * (static_cast<double>(done) / static_cast<double>(total))), 0, 100)));
    }
};

#endif // PROGRESSNOTIFIERBASE_H
