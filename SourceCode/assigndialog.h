#ifndef ASSIGNDIALOG_H
#define ASSIGNDIALOG_H

#include <QDialog>

namespace Ui {
class assignDialog;
}

class assignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit assignDialog(QWidget *parent = nullptr);
    ~assignDialog();

private:
    Ui::assignDialog *ui;
};

#endif // ASSIGNDIALOG_H
