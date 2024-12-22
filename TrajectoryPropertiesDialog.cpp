
#include "TrajectoryPropertiesDialog.h"

#include <QDialogButtonBox>

#include "Trajectory.h"
#include "ui_TrajectoryPropertiesDialog.h"


TrajectoryPropertiesDialog::TrajectoryPropertiesDialog(Trajectory * trajectory, QWidget *parent) :
    QDialog(parent), ui(new Ui::TrajectoryPropertiesDialog) {
    ui->setupUi(this);
    this->trajectory = trajectory;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&TrajectoryPropertiesDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    ui->edtPhi->setText(QString("%1").arg(trajectory->phi));
    ui->edtTheta->setText(QString("%1").arg(trajectory->theta));
    ui->edtColor->setText(QString("#%1").arg(trajectory->color, 6, 16, QLatin1Char('0')).toUpper());
}

TrajectoryPropertiesDialog::~TrajectoryPropertiesDialog() {
    delete ui;
}

void TrajectoryPropertiesDialog::OnOk() {
    QString s = ui->edtColor->text();
    if (!s.isEmpty())
        trajectory->color = Trajectory::ColorToInt(s);
    s = ui->edtPhi->text();
    bool ok;
    int len = s.toUInt(&ok, 10);
    if (ok)
        trajectory->phi = len;
    s = ui->edtTheta->text();
    len = s.toUInt(&ok, 10);
    if (ok)
        trajectory->theta = len;
    accept();
}