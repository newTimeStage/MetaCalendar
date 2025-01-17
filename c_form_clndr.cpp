#include "c_form_clndr.h"
#include "ui_c_form_clndr.h"
#include <c_core_metaclndr.h>
#include <QTimer>
#include <QString>

#define cStrFormNameClndr    "元始历"


class C_FormClndr : public QObject
{
public:
    C_FormClndr(Ui::C_Form_Clndr *iFrm);
    void Refrash(void);
    ~C_FormClndr(void);
private:
    Ui::C_Form_Clndr *ui;
    C_Core_MetaClndr *clsMetaClndr = nullptr;
    static uint64_t fBeginToRefMilSec(const T_Core_RefDateTime* iRefDateTime);
    static uint64_t fAfterRefMilSec(const T_Core_RefDateTime* iRefDateTime);
};

static C_FormClndr * clsFormClndr = nullptr;


C_FormClndr::C_FormClndr(Ui::C_Form_Clndr *iFrm) {
    ui = iFrm;
    const T_Core_RefDateTime* pRefDateTime = C_Core_MetaClndr::RefDateTime();
    clsMetaClndr = new C_Core_MetaClndr(C_FormClndr::fBeginToRefMilSec(pRefDateTime),
                                        C_FormClndr::fAfterRefMilSec(pRefDateTime));
}

void C_FormClndr::Refrash(void) {
    clsMetaClndr->Refersh();
    QString QStrClndr = QString::fromStdString(clsMetaClndr->GetCalendar());
    ui->textEditClndr->setText(QStrClndr);
    return;
}

uint64_t C_FormClndr::fBeginToRefMilSec(const T_Core_RefDateTime* iRefDateTime) {
    // QDate BeginDate(-49911L, 10L, 14L);
    // QTime BeginTime(17L, 4L, 36L);
    QDateTime BeginDateTime(QDate(-49911L, 10L, 14L), QTime(0L, 0L, 0L));
    // QDate RefDate(iRefDateTime->Year, iRefDateTime->Month, iRefDateTime->Day);
    // QTime RefTime(iRefDateTime->Hour, iRefDateTime->Month, iRefDateTime->Second);
    QDateTime CurDateTime = QDateTime::currentDateTime();
    return static_cast<uint64_t>(BeginDateTime.msecsTo(CurDateTime));
}

uint64_t C_FormClndr::fAfterRefMilSec(const T_Core_RefDateTime* iRefDateTime) {
    QDateTime RefDateTime(QDate(iRefDateTime->Year, iRefDateTime->Month, iRefDateTime->Day),
                          QTime(iRefDateTime->Hour, iRefDateTime->Month, iRefDateTime->Second));
    QDateTime CurDateTime = QDateTime::currentDateTime();
    return static_cast<uint64_t>(RefDateTime.msecsTo(CurDateTime));
}

C_FormClndr::~C_FormClndr(void) {
    delete clsMetaClndr;
}


C_Form_Clndr::C_Form_Clndr(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::C_Form_Clndr)
{
    ui->setupUi(this);
    this->setWindowTitle(cStrFormNameClndr);
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
