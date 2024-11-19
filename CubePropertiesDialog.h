//
// Created by Ivan Sidyakin on 08.11.2024.
//

#ifndef CURS_CUBEPROPERTIESDIALOG_H
#define CURS_CUBEPROPERTIESDIALOG_H

#include <QDialog>
#include "CurTypes.h"
#include "Cube.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CubePropertiesDialog; }
QT_END_NAMESPACE

class CubePropertiesDialog : public QDialog {
Q_OBJECT

public:
    explicit CubePropertiesDialog(Cube *cube, QWidget *parent = nullptr);
    Cube * cube;

    ~CubePropertiesDialog() override;

private:
    Ui::CubePropertiesDialog *ui;
private slots:
    void OnOk();
};


#endif //CURS_CUBEPROPERTIESDIALOG_H
