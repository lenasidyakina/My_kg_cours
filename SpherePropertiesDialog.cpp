
#include "SpherePropertiesDialog.h"
#include "ui_SpherePropertiesDialog.h"
#include "QDialogButtonBox"


SpherePropertiesDialog::SpherePropertiesDialog(Sphere * sphere, QWidget *parent) :
        QDialog(parent), ui(new Ui::SpherePropertiesDialog) {
    ui->setupUi(this);
    this->sphere = sphere;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&SpherePropertiesDialog::OnOk);
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
    int len = s.toUInt(&ok, 10);
    if (ok)
        sphere->RadiusLength = len;
    accept();
}