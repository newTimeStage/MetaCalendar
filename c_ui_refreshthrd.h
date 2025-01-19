#ifndef C_UI_REFRESHTHRD_H
#define C_UI_REFRESHTHRD_H

#include <QThread>


class C_UI_RefreshThrd : public QThread
{
    Q_OBJECT
    void run() override;
public:
    explicit C_UI_RefreshThrd(const qint64& iInterval);
    ~C_UI_RefreshThrd(void);
signals:
    void resultReady(const QString &s);
private:
    qint64 Inval = 0L;
};

#endif // C_UI_REFRESHTHRD_H
