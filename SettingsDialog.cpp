
#include <QColorDialog>
#include <QSettings>
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->btnPickLine, &QPushButton::released, this, &SettingsDialog::PickLineColor);
    connect(ui->btnPickBack, &QPushButton::released, this, &SettingsDialog::PickBackColor);

    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
    ui->edtBackColor->setText(backgroundColor());
    ui->bgndColor->setStyleSheet("{color:" + backgroundColor() + "}");

}


QString SettingsDialog::backgroundColor()
{
    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
    return settings.value("background-color", "#000000").toString();
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::OnOk() {
    QString fn = SETTINGS_FILE;
    QSettings settings(fn, QSettings::IniFormat);
    settings.setValue("background-color", ui->edtBackColor->text());
    accept();
}

void SettingsDialog::PickLineColor()
{

}

void SettingsDialog::PickBackColor()
{
    QColorDialog * dlg = new QColorDialog();
    if (dlg->exec() == QDialog::Accepted) {
        int r, g, b;
        dlg->currentColor().getRgb(&r, &g, &b);
        ui->edtBackColor->setText(QString( "#%1%2%3" )
        .arg( r, 2, 16, QLatin1Char('0') )
        .arg( g, 2, 16, QLatin1Char('0') )
        .arg( b, 2, 16, QLatin1Char('0') ).toUpper());
    }
    delete dlg;
}
