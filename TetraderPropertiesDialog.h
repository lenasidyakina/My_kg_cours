//
// Created by sidya on 19.11.2024.
//

#ifndef TETRADERPROPERTIESDIALOG_H
#define TETRADERPROPERTIESDIALOG_H

#include <QDialog>
#include "Tetrader.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TetraderPropertiesDialog; }
QT_END_NAMESPACE

class TetraderPropertiesDialog : public QDialog {
Q_OBJECT

public:
    explicit TetraderPropertiesDialog(Tetrader *tetrader, QWidget *parent = nullptr);
    Tetrader * tetrader;
    ~TetraderPropertiesDialog() override;

private:
    Ui::TetraderPropertiesDialog *ui;
    private slots:
        void OnOk();
        void PickLineColor();
};


#endif //TETRADERPROPERTIESDIALOG_H
