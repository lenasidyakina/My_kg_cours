//
// Created by Ivan Sidyakin on 06.11.2024.
//

#include <QVBoxLayout>
#include <QMenuBar>
#include <QGroupBox>
#include <QPushButton>
#include <QListWidget>
#include <QSettings>
#include <QMessageBox>


#include "SettingsDialog.h"
#include "FigureSelectorDialog.h"
#include "CubePropertiesDialog.h"
#include "SpherePropertiesDialog.h"
#include "CylinderPropertiesDialog.h"
#include "TetraderPropertiesDialog.h"

#include "MainWindow.h"

#include "Cylinder.h"

QList<Figure*> MainWindow::figureList;

MainWindow::MainWindow() {
    auto widget = new QWidget;
    setCentralWidget(widget);

    auto layout = new QHBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);


    mainPanel = new RenderArea(this);
    mainPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //updateScene();

    layout->addWidget(mainPanel);

    auto gb = new QGroupBox();
    auto gbl = new QVBoxLayout();
    figureListWigdet = new QListWidget();
    gbl->addWidget( figureListWigdet);
    gb->setMaximumWidth(200);
    auto btnNewFigure = new QPushButton(tr("New figure"));
    gbl->addWidget(btnNewFigure);
    connect(btnNewFigure, &QPushButton::released, this, &MainWindow::handleNewFigure);

    auto btnEditFigure = new QPushButton(tr("Edit figure"));
    gbl->addWidget(btnEditFigure);
    connect(btnEditFigure, &QPushButton::released, this, &MainWindow::handleEditFigure);

    auto btnRemoveFigure = new QPushButton(tr("Remove figure"));
    gbl->addWidget(btnRemoveFigure);
    connect(btnRemoveFigure, &QPushButton::released, this, &MainWindow::handleRemoveFigure);

    QPushButton * btnSettings = new QPushButton(tr("Settings"));
    gbl->addWidget(btnSettings);
    connect(btnSettings, &QPushButton::released, this, &MainWindow::handleSettings);


    gb->setLayout(gbl);
    layout->addWidget(gb);
    widget->setLayout(layout);


    setWindowTitle(tr("Cursovoj project"));
    setMinimumSize(480, 240);
    resize(640, 480);
}

void MainWindow::handleSettings() {
    auto dlg = new SettingsDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        updateScene();
    }
    delete dlg;
}

//https://doc.qt.io/qt-6/qtwidgets-painting-basicdrawing-example.html
void MainWindow::updateScene()
{
    mainPanel->invalidate();
}

void MainWindow::handleNewFigure() {
    FigureSelectorDialog * dlg = new FigureSelectorDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        int sel = dlg->getSelection();
        if (sel == FIGURE_CUBE) {
            Cube * cube = new Cube(DEFAULT_FIGURE_COLOR, DEFAULT_CUBE_EDGE_LENGTH);
            CubePropertiesDialog * cpd = new CubePropertiesDialog(cube);
            if (cpd->exec() == QDialog::Accepted) {
                figureList.append(cube); // delete ?
                figureListWigdet->addItem(cube->getName());
            }
            delete cpd;
        }
        else if (sel == FIGURE_SPHERE) {
            Sphere * sphere = new Sphere(DEFAULT_FIGURE_COLOR, DEFAULT_SPHERE_RADIUS_LENGTH);
            SpherePropertiesDialog * cpd = new SpherePropertiesDialog(sphere);
            if (cpd->exec() == QDialog::Accepted) {
                figureList.append(sphere);
                figureListWigdet->addItem(sphere->getName());
            }
            delete cpd;
        }
        else if (sel == FIGURE_PYRAMID) {
            Tetrader *tetrader = new Tetrader(DEFAULT_FIGURE_COLOR, DEFAULT_TETRADER_HEIGHT);
            TetraderPropertiesDialog * cpd = new TetraderPropertiesDialog(tetrader);
            if (cpd->exec() == QDialog::Accepted) {
                figureList.append(tetrader);
                figureListWigdet->addItem(tetrader->getName());
            }
            delete cpd;
        }
        else if (sel == FIGURE_CYLINDER) {
            Cylinder * cylinder = new Cylinder(DEFAULT_FIGURE_COLOR, DEFAULT_CYLINDER_BASE_RADIUS, DEFAULT_CYLINDER_HEIGHT);
            CylinderPropertiesDialog * cpd = new CylinderPropertiesDialog(cylinder);
            if (cpd->exec() == QDialog::Accepted) {
                figureList.append(cylinder);
                figureListWigdet->addItem(cylinder->getName());
            }
            delete cpd;
        }
    }
    delete dlg;
}

void MainWindow::handleEditFigure() {
    int row = figureListWigdet->currentRow();
    if (row < 0)
        return;
    int sel = figureList[row]->GetType();
    if (sel == FIGURE_CUBE) {
        CubePropertiesDialog * cpd = new CubePropertiesDialog((Cube *)figureList[row]);
        if (cpd->exec() == QDialog::Accepted) {
            figureListWigdet->item(row)->setText(((Cube *)figureList[row])->getName());
        }
        delete cpd;
    }
    else if (sel == FIGURE_SPHERE) {
        SpherePropertiesDialog * cpd = new SpherePropertiesDialog((Sphere *)figureList[row]);
        if (cpd->exec() == QDialog::Accepted) {
            figureListWigdet->item(row)->setText(((Sphere *)figureList[row])->getName());
        }
        delete cpd;
    }
    else if (sel == FIGURE_PYRAMID) {
        TetraderPropertiesDialog * cpd = new TetraderPropertiesDialog((Tetrader *)figureList[row]);
        if (cpd->exec() == QDialog::Accepted) {
            figureListWigdet->item(row)->setText(((Tetrader *)figureList[row])->getName());
        }
        delete cpd;
    }
    else if (sel == FIGURE_CYLINDER) {
        CylinderPropertiesDialog * cpd = new CylinderPropertiesDialog((Cylinder *)figureList[row]);
        if (cpd->exec() == QDialog::Accepted) {
            figureListWigdet->item(row)->setText(((Cylinder *)figureList[row])->getName());
        }
        delete cpd;
    }
}

void MainWindow::handleRemoveFigure() {
    int row = figureListWigdet->currentRow();
    if (row < 0)
        return;
    int ret = QMessageBox::warning(this, tr("Remove figure"), "Remove " + figureList[row]->getName() +
                                   tr(" A you sure?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        delete figureListWigdet->takeItem(figureListWigdet->currentRow());
        Figure * f = figureList[row];
        figureList.removeAt(row);
        delete f;
    }
}

