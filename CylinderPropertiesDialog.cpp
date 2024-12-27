#include "CylinderPropertiesDialog.h"
#include "ui_CylinderPropertiesDialog.h"
#include "QDialogButtonBox"
#include <QColorDialog>
#include <QPushButton>
#include <QMessageBox>

CylinderPropertiesDialog::CylinderPropertiesDialog(Cylinder *cylinder, QWidget *parent) :
        QDialog(parent), ui(new Ui::CylinderPropertiesDialog) {
    ui->setupUi(this);
    this->cylinder = cylinder;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&CylinderPropertiesDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->btnColor, &QPushButton::released, this, &CylinderPropertiesDialog::PickLineColor);

    ui->edtBaseRadius->setText(QString("%1").arg(cylinder->BaseRadius));
    ui->edtHeight->setText(QString("%1").arg(cylinder->Height));
    ui->edtColor->setText(QString("#%1").arg(cylinder->color, 6, 16, QLatin1Char('0')).toUpper());

}

CylinderPropertiesDialog::~CylinderPropertiesDialog() {
    delete ui;
}

void CylinderPropertiesDialog::OnOk() {
    QString s = ui->edtColor->text();
    if (!s.isEmpty())
        cylinder->color = Figure::ColorToInt(s);
    s = ui->edtBaseRadius->text();
    bool ok;
    int radius = s.toUInt(&ok, 10);
    if (!ok || radius < 10 || radius > 500) {
        QMessageBox::critical(0, "Error", "Cylinder radius should be integer in range from 10 to 500");
        return;
    }
    s = ui->edtHeight->text();
    int height = s.toUInt(&ok, 10);
    if (!ok ||  height < 10 || height > 500) {
        QMessageBox::critical(0, "Error", "Cylinder height should be integer in range from 10 to 500");
        return;
    }
    cylinder->BaseRadius = radius;
    cylinder->Height = height;
    accept();
}

void CylinderPropertiesDialog::PickLineColor()
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