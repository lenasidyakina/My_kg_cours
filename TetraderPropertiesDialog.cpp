//
// Created by sidya on 19.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TetraderPropertiesDialog.h" resolved

#include "TetraderPropertiesDialog.h"
#include "ui_TetraderPropertiesDialog.h"
#include "QDialogButtonBox"
#include <QColorDialog>
#include <QPushButton>
#include <QMessageBox>

TetraderPropertiesDialog::TetraderPropertiesDialog(Tetrader * tetrader, QWidget *parent) :
        QDialog(parent), ui(new Ui::TetraderPropertiesDialog) {
    ui->setupUi(this);
    this->tetrader = tetrader;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&TetraderPropertiesDialog::OnOk);
    connect(ui->btnColor, &QPushButton::released, this, &TetraderPropertiesDialog::PickLineColor);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    ui->edtHeight->setText(QString("%1").arg(tetrader->Height));
    ui->edtColor->setText(QString("#%1").arg(tetrader->color, 6, 16, QLatin1Char('0')).toUpper());

}

TetraderPropertiesDialog::~TetraderPropertiesDialog() {
    delete ui;
}

void TetraderPropertiesDialog::OnOk() {
    QString s = ui->edtColor->text();
    if (!s.isEmpty())
        tetrader->color = Figure::ColorToInt(s);
    s = ui->edtHeight->text();
    bool ok;
    int height = s.toUInt(&ok, 10);
    if (!ok ||  height < 10 || height > 500) {
        QMessageBox::critical(0, "Error", "Tetraeder height should be integer in range from 10 to 500");
        return;
    }
    if (ok)
        tetrader->Height = height;
    accept();
}

void TetraderPropertiesDialog::PickLineColor()
{
    QColorDialog * dlg = new QColorDialog();
    if (dlg->exec() == QDialog::Accepted) {
        int r, g, b;
        dlg->currentColor().getRgb(&r, &g, &b);
        ui->edtColor->setText(QString( "#%1%2%3" )
                                      .arg( r, 2, 16, QLatin1Char('0') )
                                      .arg( g, 2, 16, QLatin1Char('0') )
                                      .arg( b, 2, 16, QLatin1Char('0') ).toUpper());
    }
    delete dlg;
}

