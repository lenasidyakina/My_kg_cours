#include "CylinderPropertiesDialog.h"
#include "ui_CylinderPropertiesDialog.h"
#include "QDialogButtonBox"

CylinderPropertiesDialog::CylinderPropertiesDialog(Cylinder *cylinder, QWidget *parent) :
        QDialog(parent), ui(new Ui::CylinderPropertiesDialog) {
    ui->setupUi(this);
    this->cylinder = cylinder;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&CylinderPropertiesDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
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
    int len = s.toUInt(&ok, 10);
    if (ok)
        cylinder->BaseRadius = len;
    s = ui->edtHeight->text();
    len = s.toUInt(&ok, 10);
    if (ok)
        cylinder->Height = len;
    accept();
}