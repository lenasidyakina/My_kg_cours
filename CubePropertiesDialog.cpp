//
// Created by Ivan Sidyakin on 08.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CubePropertiesDialog.h" resolved

#include "CubePropertiesDialog.h"
#include "ui_CubePropertiesDialog.h"
#include "QDialogButtonBox"
#include <QColorDialog>
#include <QPushButton>
#include <QMessageBox>


CubePropertiesDialog::CubePropertiesDialog(Cube * cube, QWidget *parent) :
        QDialog(parent), ui(new Ui::CubePropertiesDialog) {
    ui->setupUi(this);
    this->cube = cube;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&CubePropertiesDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->btnColor, &QPushButton::released, this, &CubePropertiesDialog::PickLineColor);

    ui->edtEdge->setText(QString("%1").arg(cube->EdgeLength));
    ui->edtColor->setText(QString("#%1").arg(cube->color, 6, 16, QLatin1Char('0')).toUpper());

}

CubePropertiesDialog::~CubePropertiesDialog() {
    delete ui;
}

void CubePropertiesDialog::OnOk() {
    QString s = ui->edtColor->text();
    if (!s.isEmpty())
        cube->color = Figure::ColorToInt(s);
    s = ui->edtEdge->text();
    bool ok;
    int len = s.toUInt(&ok, 10);
    if (!ok || len < 10 || len > 500) {
        QMessageBox::critical(0, "Error", "Cube edge should be integer in range from 10 to 500");
        return;
    }
    cube->EdgeLength = len;
    accept();
}

void CubePropertiesDialog::PickLineColor()
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