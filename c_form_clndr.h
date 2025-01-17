#ifndef C_FORM_CLNDR_H
#define C_FORM_CLNDR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class C_Form_Clndr;
}
QT_END_NAMESPACE

class C_Form_Clndr : public QMainWindow
{
    Q_OBJECT

public:
    C_Form_Clndr(QWidget *parent = nullptr);

    ~C_Form_Clndr();

private:
    Ui::C_Form_Clndr *ui;
};
#endif // C_FORM_CLNDR_H
