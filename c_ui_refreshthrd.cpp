#include "c_ui_refreshthrd.h"
#include <QElapsedTimer>


C_UI_RefreshThrd::C_UI_RefreshThrd(const qint64& iInterval) {
    Inval = iInterval;
}

void C_UI_RefreshThrd::run(void) {
    QString result;
    QElapsedTimer Timer;
    Timer.start();
    while(!isInterruptionRequested()) {
        const qint64 Elapsed = Timer.elapsed();
        if(Elapsed >= Inval) {
            emit resultReady(result);
            Timer.restart();
        }
        // QThread::usleep(1);
    }
    
    return;
}


C_UI_RefreshThrd::~C_UI_RefreshThrd(void) {}
