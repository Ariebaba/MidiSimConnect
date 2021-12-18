#ifndef DIALOGASSIGN_H
#define DIALOGASSIGN_H

#include <QDialog>

namespace Ui {
class DialogAssign;
}

class DialogAssign : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAssign(QWidget *parent = nullptr);
    ~DialogAssign();
    void prepCommand(QString);
signals:
    void fillChecker(QString);
    void commandChange(QString,QString,QString);
    void statusSignal(QString,int);
public slots:
    void midiSlot(QString, QString, QString,QString,QString);
    void assignSlot(QString);

private slots:
    void on_btn_AssignCancel_clicked();
    void on_btn_Assign_clicked();
    void on_radioButton_btn_toggled(bool checked);
    void on_radioButton_rel_toggled(bool checked);
    void on_lineEdit_code_textChanged(const QString &arg1);
    void on_btn_Delete_clicked();

private:
    Ui::DialogAssign *ui;
};

#endif // DIALOGASSIGN_H
