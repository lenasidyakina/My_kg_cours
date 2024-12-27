//
// Created by Ivan Sidyakin on 08.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include <QColorDialog>
#include <QSettings>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "SettingsDialog.h"
#include "cmake-build-debug/qtest_autogen/include/ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->btnPickLine, &QPushButton::released, this, &SettingsDialog::PickLineColor);
    connect(ui->btnPickBack, &QPushButton::released, this, &SettingsDialog::PickBackColor);

    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);

    QString bkColor = settings.value("background-color", "#000000").toString();
    ui->edtBackColor->setText(bkColor);
    ui->bgndColor->setStyleSheet("{color:" + bkColor + "}");
    QString frColor = settings.value("globe-color", "#FFFFFF").toString();
    ui->edtLineColor->setText(frColor);
    ui->edtCentexX->setText(settings.value("globe-center-x", "0").toString());
    ui->edtCenterY->setText(settings.value("globe-center-y", "0").toString());
    ui->edtRadius->setText(settings.value("globe-radius", "100").toString());
    ui->edtSpeed->setText(settings.value("train-speed", "1").toString());
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::OnOk() {
    QString fn = SETTINGS_FILE;
    QSettings settings(fn, QSettings::IniFormat);
    settings.setValue("background-color", ui->edtBackColor->text());
    settings.setValue("globe-color", ui->edtLineColor->text());
    bool ok;
    QString s = ui->edtCentexX->text();
    s.toInt(&ok);
    if (!ok) {
        QMessageBox::critical(0, "Error", "X position should be negative or positive integer");
        return;
    }
    settings.setValue("globe-center-x", s);
    s = ui->edtCenterY->text();
    s.toInt(&ok);
    if (!ok) {
        QMessageBox::critical(0, "Error", "Y position should be negative or positive integer");
        return;
    }
    settings.setValue("globe-center-y", s);

    s = ui->edtRadius->text();
    uint v =s.toUInt(&ok);
    if (!ok || (v < 50) || (v > 500))  {
        QMessageBox::critical(0, "Error", "Radius should be integer between 50 and 500");
        return;
    }

    settings.setValue("globe-radius", s);

    s = ui->edtSpeed->text();
    v =s.toUInt(&ok);
    if (!ok || (v < 1) || (v > 90))  {
        QMessageBox::critical(0, "Error", "Train speed should be integer between 1 and 90");
        return;
    }
    settings.setValue("train-speed", s);
    accept();
}

void SettingsDialog::PickLineColor()
{
    QColorDialog * dlg = new QColorDialog();
    if (dlg->exec() == QDialog::Accepted) {
        int r, g, b;
        dlg->currentColor().getRgb(&r, &g, &b);
        ui->edtLineColor->setText(QString( "#%1%2%3" )
                                          .arg( r, 2, 16, QLatin1Char('0') )
                                          .arg( g, 2, 16, QLatin1Char('0') )
                                          .arg( b, 2, 16, QLatin1Char('0') ).toUpper());
    }
    delete dlg;
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
