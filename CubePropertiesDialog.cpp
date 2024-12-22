
#include "CubePropertiesDialog.h"
#include "ui_CubePropertiesDialog.h"
#include "QDialogButtonBox"


CubePropertiesDialog::CubePropertiesDialog(Cube * cube, QWidget *parent) :
        QDialog(parent), ui(new Ui::CubePropertiesDialog) {
    ui->setupUi(this);
    this->cube = cube;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&CubePropertiesDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
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
    if (ok)
        cube->EdgeLength = len;
    accept();
}