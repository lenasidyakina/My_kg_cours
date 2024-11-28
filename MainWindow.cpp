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
#include <deque>

#include "SettingsDialog.h"
#include "FigureSelectorDialog.h"
#include "CubePropertiesDialog.h"
#include "SpherePropertiesDialog.h"
#include "CylinderPropertiesDialog.h"
#include "TetraderPropertiesDialog.h"

#include "MainWindow.h"

#include "Cylinder.h"
#include "TrajectoryPropertiesDialog.h"

QList<Figure*> MainWindow::figureList;

Trajectory * MainWindow::trajectory = new Trajectory(DEFAULT_TRAJECTORY_COLOR ,40,30);

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

    auto btnStop = new QPushButton(tr("Stop")); // Создаем кнопку Stop
    gbl->addWidget(btnStop); // Добавляем кнопку в интерфейс
    connect(btnStop, &QPushButton::released, this, &MainWindow::stopAnimation); // Привязываем слот

    auto btnChangeTrajectiry = new QPushButton(tr("Change trajectiry"));
    gbl->addWidget(btnChangeTrajectiry);
    connect(btnChangeTrajectiry, &QPushButton::released, this, &MainWindow::handleChangeTrajectiry);


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

void MainWindow::handleChangeTrajectiry() {
    //Trajectory *trajectory_new = new Trajectory(DEFAULT_FIGURE_COLOR,0,0); // Инициализация объекта
    TrajectoryPropertiesDialog * dlg = new TrajectoryPropertiesDialog(trajectory, this);
    if (dlg->exec() == QDialog::Accepted) {
        //trajectory = trajectory_new;
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

void MainWindow::stopAnimation() {
    if (animationTimer->isActive()) {
        animationTimer->stop(); // Останавливаем таймер
    }
}

Point3D sphericalToCartesian_r(float radius, float theta, float phi) {
    Point3D result;

    // Преобразование из сферических координат в декартовы
    result.x = radius * sin(theta) * cos(phi);
    result.y = radius * sin(theta) * sin(phi);
    result.z = radius * cos(theta);

    return result;
}

Point3D normalize(const Point3D& v) {
    float magnitude = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (magnitude == 0.0f) {
        throw std::runtime_error("Cannot normalize a zero-length vector.");
    }
    return {v.x / magnitude, v.y / magnitude, v.z / magnitude};
}

Point3D crossProduct_1(const Point3D& a, const Point3D& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float magnitude(const Point3D& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


// Метод обновления сцены
void MainWindow::updateScene() {
    // Пробегаем по всем фигурам и обновляем их
    for (auto figure : figureList) {
        if (auto cube = dynamic_cast<Cube*>(figure)) {
            // Обновление углов для анимации
            cube->phi += 0.01f;
            if (cube->phi >= 2 * M_PI) cube->phi -= 2 * M_PI;

            // Параметры для траектории и куба
            float sphereRadius = 100;
            float edge = cube->EdgeLength;
            float halfEdge = edge / 2.0f;

            // Вычисляем точку на траектории
            Point3D bottomCenter = sphericalToCartesian_r(sphereRadius, cube->theta, cube->phi);

            // Центр куба (включая его половину стороны)
            Point3D cubeCenter = {
                bottomCenter.x,
                bottomCenter.y,
                bottomCenter.z + halfEdge
            };

            // Нормаль к поверхности
            Point3D normal = normalize(cubeCenter);
            Point3D upVector(0, 1, 0); // Направление вверх

            // Вычисляем тангенциальный вектор (перпендикулярно нормали)
            Point3D tangentVector = crossProduct_1(normal, upVector);

            if (magnitude(tangentVector) < 1e-6) {
                upVector = {1, 0, 0}; // Если тангенциальный вектор близок к нулю, выбираем другой вектор
                tangentVector = crossProduct_1(normal, upVector);
            }

            // Нормализуем все векторы
            tangentVector = normalize(tangentVector);
            upVector = normalize(crossProduct_1(tangentVector, normal));

            // Обновляем координаты и ориентируем куб
            cube->updateVertices(normal, tangentVector, upVector, cubeCenter);
        }
    }

    // Запрос на перерисовку панели
    mainPanel->invalidate(); // Вызов перерисовки
}




