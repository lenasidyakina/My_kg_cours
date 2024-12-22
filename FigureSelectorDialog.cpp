
#include <QListWidgetItem>
#include "FigureSelectorDialog.h"
#include "ui_FigureSelectorDialog.h"

FigureSelectorDialog::FigureSelectorDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::FigureSelectorDialog) {
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &FigureSelectorDialog::OnOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    new QListWidgetItem(tr("Куб"), ui->listWidget);
    new QListWidgetItem(tr("Сфера"), ui->listWidget);
    new QListWidgetItem(tr("Пирамида"), ui->listWidget);
    new QListWidgetItem(tr("Цилиндр"), ui->listWidget);
    _selection = 0;
    ui->listWidget->setCurrentRow(0);

}

FigureSelectorDialog::~FigureSelectorDialog() {
    delete ui;
}

void FigureSelectorDialog::OnOk() {
    _selection = ui->listWidget->currentRow();
    accept();
}