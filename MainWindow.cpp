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
#include <QTimer>


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

    auto btnSettings = new QPushButton(tr("Settings"));
    gbl->addWidget(btnSettings);
    connect(btnSettings, &QPushButton::released, this, &MainWindow::handleSettings);

    auto btnStart = new QPushButton(tr("Start"));
    gbl->addWidget(btnStart);
    connect(btnStart, &QPushButton::released, this, &MainWindow::startAnimation);


    gb->setLayout(gbl);
    layout->addWidget(gb);
    widget->setLayout(layout);


    setWindowTitle(tr("Cursovoj project"));
    setMinimumSize(480, 240);
    resize(640, 480);

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateScene);
}

void MainWindow::handleSettings() {
    auto dlg = new SettingsDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        updateScene();
    }
    delete dlg;
}

//https://doc.qt.io/qt-6/qtwidgets-painting-basicdrawing-example.html

void MainWindow::handleNewFigure() {
    FigureSelectorDialog * dlg = new FigureSelectorDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        int sel = dlg->getSelection();
        if (sel == FIGURE_CUBE) {
            Cube * cube = new Cube(DEFAULT_FIGURE_COLOR, DEFAULT_CUBE_EDGE_LENGTH, 0, 0);
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

// Новый слот для запуска анимации
void MainWindow::startAnimation() {
    if (animationTimer->isActive()) {
        animationTimer->stop(); // Останавливаем таймер, если он уже работает
    } else {
        animationTimer->start(16); // Запускаем таймер с интервалом ~16 мс (60 FPS)
    }
}

void MainWindow::drawTrajectory() {
    QPainter painter(mainPanel);
    painter.setRenderHint(QPainter::Antialiasing);

    // Центр сцены
    float centerX = mainPanel->width() / 2;
    float centerY = mainPanel->height() / 2;

    // Радиус сферы
    float sphereRadius =  100;

    // Цвет для траектории
    painter.setPen(QPen(QColor(255, 0, 0), 2));  // Красная линия

    // Отрисовываем окружность (траекторию)
    painter.drawEllipse(QPointF(centerX, centerY), sphereRadius, sphereRadius);
}

Point3D sphericalToCartesian_r(float radius, float theta, float phi) {
    Point3D result;

    // Преобразование из сферических координат в декартовы
    result.x = radius * sin(theta) * cos(phi);
    result.y = radius * sin(theta) * sin(phi);
    result.z = radius * cos(theta);

    return result;
}
// Метод обновления сцены
void MainWindow::updateScene() {
    drawTrajectory();  // Рисуем траекторию

    // Логика обновления координат куба на сфере
    for (auto figure : figureList) {
        if (auto cube = dynamic_cast<Cube*>(figure)) {
            // Увеличиваем угол phi для движения вдоль окружности
            cube->phi += 0.01f; // Скорость движения вдоль окружности
            if (cube->phi >= 2 * M_PI) cube->phi -= 2 * M_PI;

            // Фиксируем theta для плоскости сечения
            cube->theta = M_PI / 2; // Фиксированное значение для сечения

            // Радиус сферы
            float sphereRadius = 100;

            // Позиция середины нижней грани куба
            Point3D bottomCenter = sphericalToCartesian_r(sphereRadius, cube->theta, cube->phi);

            // Размер куба
            float edge = cube->EdgeLength;
            float halfEdge = edge / 2.0f;

            // Смещаем нижнюю грань куба, чтобы она касалась сферы
            // Мы смещаем точку вниз по оси Z на половину длины ребра куба

            // Обновляем позицию куба
            cube->position = bottomCenter;
        }
    }

    mainPanel->invalidate(); // Перерисовка сцены
}


