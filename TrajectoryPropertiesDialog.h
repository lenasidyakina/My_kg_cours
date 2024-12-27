//
// Created by sidya on 21.11.2024.
//

#ifndef TRAJECTORYPROPERTIESDIALOG_H
#define TRAJECTORYPROPERTIESDIALOG_H

#include <QDialog>

#include "Trajectory.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TrajectoryPropertiesDialog; }
QT_END_NAMESPACE

class TrajectoryPropertiesDialog : public QDialog {
Q_OBJECT

public:
    explicit TrajectoryPropertiesDialog(Trajectory * trajectory, QWidget *parent = nullptr);
    Trajectory * trajectory;

    ~TrajectoryPropertiesDialog() override;

void OnOk();

private:
    Ui::TrajectoryPropertiesDialog *ui;
};


#endif //TRAJECTORYPROPERTIESDIALOG_H
