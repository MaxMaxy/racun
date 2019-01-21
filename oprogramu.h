#ifndef OPROGRAMU_H
#define OPROGRAMU_H

#include <QDialog>
#include <QIcon>

namespace Ui {
class Oprogramu;
}

class Oprogramu : public QDialog
{
    Q_OBJECT

public:
    explicit Oprogramu(QWidget *parent = nullptr);
    ~Oprogramu();

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

private:
    Ui::Oprogramu *ui;
};

#endif // OPROGRAMU_H
