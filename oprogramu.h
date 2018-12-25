#ifndef OPROGRAMU_H
#define OPROGRAMU_H

#include <QDialog>
#include <QIcon>
#include <QScrollArea>
#include <QGroupBox>

namespace Ui {
class Oprogramu;
}

class Oprogramu : public QDialog
{
    Q_OBJECT

public:
    explicit Oprogramu(QWidget *parent = nullptr);
    ~Oprogramu();

private:
    Ui::Oprogramu *ui;
};

#endif // OPROGRAMU_H
