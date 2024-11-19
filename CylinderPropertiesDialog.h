

#ifndef CYLINDERPROPERTIESDIALOG_H
#define CYLINDERPROPERTIESDIALOG_H

#include "Cylinder.h"
#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class CylinderPropertiesDialog; }
QT_END_NAMESPACE

class CylinderPropertiesDialog : public QDialog {
Q_OBJECT

public:
    explicit CylinderPropertiesDialog(Cylinder * cylinder, QWidget *parent = nullptr);
    Cylinder * cylinder;
    ~CylinderPropertiesDialog() override;

void OnOk();

private:
    Ui::CylinderPropertiesDialog *ui;
};


#endif //CYLINDERPROPERTIESDIALOG_H
