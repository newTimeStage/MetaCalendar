#include "c_form_clndr.h"
#include "ui_c_form_clndr.h"
#include <c_core_metaclndr.h>
#include <c_ui_refreshthrd.h>
#include <QTimer>
#include <QString>
#include <QDateTime>

#define dStrFormNameClndr    "元始历"
#define dMetaClndrStart      QDate(-59752L, 6L, 4L), QTime(0L, 0L, 0L)

class C_FormClndr : public QObject
{
public:
    C_FormClndr(Ui::C_Form_Clndr *iFrm);
    void Refrash(void);
    QString TransFromGeli(const QDateTime& iGeli);
    ~C_FormClndr(void);
private slots:
    void on_pushButtonTrans_clicked();

private:
    QDateTime        *clsSttDateTime = nullptr;
    QDateTime        *clsRefDateTime = nullptr;
    QTextEdit        *clsTxtEdtMetaClndr = nullptr;
    C_Core_MetaClndr *clsMetaClndr = nullptr;
    static uint64_t fDateTimeDiffMil(const QDateTime& iDateTimeA,
                                     const QDateTime& iDateTimeB);
};

static C_FormClndr * clsFormClndr = nullptr;


C_FormClndr::C_FormClndr(Ui::C_Form_Clndr *iFrm) {
    clsTxtEdtMetaClndr = iFrm->textEditClndr;
    clsSttDateTime = new QDateTime(dMetaClndrStart);
    const T_Core_RefDateTime* pRefDateTime = C_Core_MetaClndr::RefDateTime();
    clsRefDateTime = new QDateTime(QDate(pRefDateTime->Year, pRefDateTime->Month, pRefDateTime->Day),
                                   QTime(pRefDateTime->Hour, pRefDateTime->Month, pRefDateTime->Second));
    clsMetaClndr = new C_Core_MetaClndr(C_FormClndr::fDateTimeDiffMil(*clsSttDateTime, QDateTime::currentDateTime()),
                                        C_FormClndr::fDateTimeDiffMil(*clsRefDateTime, QDateTime::currentDateTime()));
    C_UI_RefreshThrd* pRfrshThrd = new C_UI_RefreshThrd(1L);
    connect(pRfrshThrd, &C_UI_RefreshThrd::resultReady, [=]() {
        clsMetaClndr->Refersh();
    });
    pRfrshThrd->start();
}

void C_FormClndr::Refrash(void) {
    QString QStrClndr = QString::fromStdString(clsMetaClndr->GetCalendar());
    // clsMetaClndr->Refersh();
    clsTxtEdtMetaClndr->setText(QStrClndr);
    return;
}

QString C_FormClndr::TransFromGeli(const QDateTime& iGeli) {
    QString QStrMetaClndr = "";
    uint64_t AfterAjMil = 0U;
    uint64_t BeginMil = fDateTimeDiffMil(*clsSttDateTime, iGeli);
    if(iGeli < *clsSttDateTime) {
        QStrMetaClndr = "-";
    } else if(iGeli > *clsRefDateTime) {
        AfterAjMil = clsRefDateTime->msecsTo(iGeli);
    }
    QStrMetaClndr += C_Core_MetaClndr::TransFromMil(BeginMil, AfterAjMil);
    return QStrMetaClndr;
}

uint64_t C_FormClndr::fDateTimeDiffMil(const QDateTime& iDateTimeA,
                                       const QDateTime& iDateTimeB) {
    uint64_t DateTimeDiffMil = 0U;
    if(iDateTimeA > iDateTimeB) {
        DateTimeDiffMil = static_cast<uint64_t>(iDateTimeB.msecsTo(iDateTimeA));
    } else {
        DateTimeDiffMil = static_cast<uint64_t>(iDateTimeA.msecsTo(iDateTimeB));
    }
    return DateTimeDiffMil;
}

C_FormClndr::~C_FormClndr(void) {
    delete clsMetaClndr;
}


C_Form_Clndr::C_Form_Clndr(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::C_Form_Clndr)
{
    ui->setupUi(this);
    this->setWindowTitle(dStrFormNameClndr);
    ui->textEditClndr->setFont(QFont("Noto Sans", 23));
    ui->textEditGeliDate->setFont(QFont("Noto Sans", 23));
    ui->textEditGeliTime->setFont(QFont("Noto Sans", 23));
    ui->textEditStartClndr->setFont(QFont("Noto Sans", 23));
    ui->pushButtonTrans->setText("转换");
    ui->textEditGeliDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));
    ui->textEditGeliTime->setText(QTime::currentTime().toString("hh:mm:ss.zzz"));
    ui->textEditGeliDate->setMaximumWidth(this->width()/3.0);
    ui->textEditGeliTime->setMaximumWidth(this->width()/3.0);
    clsFormClndr = new C_FormClndr(ui);
    QTimer* pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, clsFormClndr, &C_FormClndr::Refrash);
    pTimer->start(1);
}

C_Form_Clndr::~C_Form_Clndr()
{
    delete clsFormClndr;
    delete ui;
}

void C_Form_Clndr::on_pushButtonTrans_clicked()
{
    bool Ok = false;
    QStringList QStrLstGeliDate = ui->textEditGeliDate->toPlainText().split("/");
    int32_t GeliYear = QStrLstGeliDate[0].toInt(&Ok);
    int32_t GeliMonth = QStrLstGeliDate[1].toInt(&Ok);
    int32_t GeliDay = QStrLstGeliDate[2].toInt(&Ok);
    QDate GeliDate = QDate(GeliYear, GeliMonth, GeliDay);
    QTime GeliTime = QTime::fromString(ui->textEditGeliTime->toPlainText(), "hh:mm:ss.zzz");
    if(false == GeliDate.isValid() || false == GeliTime.isValid()) {
        ui->textEditStartClndr->setText("日期或时间格式错误");
    } else {
        ui->textEditStartClndr->setText(clsFormClndr->TransFromGeli(QDateTime(GeliDate, GeliTime)));
    }
    return;
}

