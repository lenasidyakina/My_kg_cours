

#ifndef CURS_SETTINGSDIALOG_H
#define CURS_SETTINGSDIALOG_H

#include <QDialog>
#include <QApplication>


#define SETTINGS_FILE (QApplication::applicationDirPath() + "./application.ini")


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

class SettingsDialog : public QDialog {
Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    ~SettingsDialog() override;
    static QString backgroundColor();

private:
    Ui::SettingsDialog *ui;
private slots:
    void OnOk();
    void PickLineColor();
    void PickBackColor();


};


#endif //CURS_SETTINGSDIALOG_H
