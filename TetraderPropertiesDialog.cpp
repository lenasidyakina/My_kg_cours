
#include "TetraderPropertiesDialog.h"
#include "ui_TetraderPropertiesDialog.h"
#include "QDialogButtonBox"


TetraderPropertiesDialog::TetraderPropertiesDialog(Tetrader * tetrader, QWidget *parent) :
        QDialog(parent), ui(new Ui::TetraderPropertiesDialog) {
    ui->setupUi(this);
    this->tetrader = tetrader;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&TetraderPropertiesDialog::OnOk);
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
    int len = s.toUInt(&ok, 10);
    if (ok)
        tetrader->Height = len;
    accept();
}

