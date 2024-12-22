

#ifndef CURS_FIGURESELECTORDIALOG_H
#define CURS_FIGURESELECTORDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class FigureSelectorDialog; }
QT_END_NAMESPACE

class FigureSelectorDialog : public QDialog {
Q_OBJECT

public:
    explicit FigureSelectorDialog(QWidget *parent = nullptr);

    ~FigureSelectorDialog() override;

    int getSelection() { return _selection; }

private:
    Ui::FigureSelectorDialog *ui;
    int _selection;
private slots:
    void OnOk();
};


#endif //CURS_FIGURESELECTORDIALOG_H
