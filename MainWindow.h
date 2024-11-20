//
// Created by Ivan Sidyakin on 06.11.2024.
//

#ifndef CURS_MAINWINDOW_H
#define CURS_MAINWINDOW_H


#include <QMainWindow>
#include <QListWidget>

#include "RenderArea.h"
#include "Figure.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    static QList<Figure*> & FigureList() { return figureList; }

private:
    RenderArea * mainPanel;
    static QList<Figure*> figureList;
    QListWidget * figureListWigdet;
    void updateScene();
    QTimer *animationTimer;
private slots:
    void handleSettings();
    void handleNewFigure();
    void handleEditFigure();
    void handleRemoveFigure();
    void startAnimation();

    void drawTrajectory();
};


#endif //CURS_MAINWINDOW_H
