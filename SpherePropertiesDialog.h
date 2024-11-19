#ifndef SPHEREPROPERTIESDIALOG_H
#define SPHEREPROPERTIESDIALOG_H

#include <QDialog>
#include "CurTypes.h"
#include "Sphere.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SpherePropertiesDialog; }
QT_END_NAMESPACE

class SpherePropertiesDialog : public QDialog {
    Q_OBJECT

    public:
    explicit SpherePropertiesDialog(Sphere * sphere, QWidget *parent = nullptr);
    Sphere * sphere;

    ~SpherePropertiesDialog() override;

private:
    Ui::SpherePropertiesDialog *ui;
    private slots:
        void OnOk();
};

#endif //SPHEREPROPERTIESDIALOG_H
