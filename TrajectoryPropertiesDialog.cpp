//
// Created by sidya on 21.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TrajectoryPropertiesDialog.h" resolved

#include "TrajectoryPropertiesDialog.h"

#include <QDialogButtonBox>

#include "Trajectory.h"
#include "ui_TrajectoryPropertiesDialog.h"
#include <QMessageBox>


TrajectoryPropertiesDialog::TrajectoryPropertiesDialog(Trajectory * trajectory, QWidget *parent) :
    QDialog(parent), ui(new Ui::TrajectoryPropertiesDialog) {
    ui->setupUi(this);
    this->trajectory = trajectory;
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,&TrajectoryPropertiesDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    ui->edtPhi->setText(QString("%1").arg(trajectory->phi));
    ui->edtTheta->setText(QString("%1").arg(trajectory->theta));
    ui->checkBox_draw->setChecked(true);
}

TrajectoryPropertiesDialog::~TrajectoryPropertiesDialog() {
    delete ui;
}

void TrajectoryPropertiesDialog::OnOk() {
    //QString s = ui->edtColor->text();
    //if (!s.isEmpty())
    trajectory->color = 0; // default is black
    QString s = ui->edtPhi->text();
    bool ok;
    int phi = s.toUInt(&ok, 10);
    if (!ok || phi < 0 || phi > 360)
    {
        QMessageBox::critical(0, "Error", "Phi angle should be integer between 0 and 360 degree");
        return;
    }
    s = ui->edtTheta->text();
    int theta = s.toUInt(&ok, 10);
    if (!ok || theta < 0 || theta > 360)
    {
        QMessageBox::critical(0, "Error", "Theta angle should be integer between 0 and 360 degree");
        return;
    }
    trajectory->phi = phi;
    trajectory->theta = theta;
    if (ui->checkBox_draw->isChecked())
        trajectory->flag = 1;
    else
        trajectory->flag = 0;
    accept();
}