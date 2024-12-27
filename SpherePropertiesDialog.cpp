
#include "SpherePropertiesDialog.h"
#include "ui_SpherePropertiesDialog.h"
#include "QDialogButtonBox"
#include <QColorDialog>
#include <QMessageBox>


SpherePropertiesDialog::SpherePropertiesDialog(Sphere * sphere, QWidget *parent) :
        QDialog(parent), ui(new Ui::SpherePropertiesDialog) {
    ui->setupUi(this);
    this->sphere = sphere;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&SpherePropertiesDialog::OnOk);
    connect(ui->btnColor, &QPushButton::released, this, &SpherePropertiesDialog::PickLineColor);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    ui->edtRadius->setText(QString("%1").arg(sphere->RadiusLength));
    //ui->edtSpeed->setText(QString("%1").arg(sphere->RadiusLength));
    ui->edtColor->setText(QString("#%1").arg(sphere->color, 6, 16, QLatin1Char('0')).toUpper());

}

SpherePropertiesDialog::~SpherePropertiesDialog() {
    delete ui;
}

void SpherePropertiesDialog::OnOk() {
    QString s = ui->edtColor->text();
    if (!s.isEmpty())
        sphere->color = Figure::ColorToInt(s);
    s = ui->edtRadius->text();
    bool ok;
    int radius = s.toUInt(&ok, 10);
    if (!ok ||  radius < 10 || radius > 500) {
        QMessageBox::critical(0, "Error", "Sphere radius should be integer in range from 10 to 500");
        return;
    }
    sphere->RadiusLength = radius;
    accept();
}

void SpherePropertiesDialog::PickLineColor()
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