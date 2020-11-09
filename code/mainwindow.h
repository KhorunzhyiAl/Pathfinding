#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QFrame>
#include <QStyle>
#include <QTimer>
#include "global.h"
#include "mygraphicsview.h"
#include "graphicsmap.h"
#include "dijkstrasalgorithm.h"
#include "astaralgorithm.h"

#include "mazegenerationalgorithm.h"
#include "randommazealgorithm.h"
#include "recursivedivisionmazealgorithm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void onPathfindingFinished();

private slots:
    // mainWindow events
    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    // main buttons click
    void on_pushButtonRun_clicked();
    void on_pushButtonClear_clicked();

    void on_pushButtonAlgorithm_clicked();
    void on_pushButtonInfo_clicked();
    void on_pushButtonCreateMaze_clicked();

    // submenu buttons click
    void on_pushButtonDijkAlg_clicked();
    void on_pushButtonAStarAlg_clicked();
    void on_pushButtonRandomMaze_clicked();
    void on_pushButtonRecursiveDivision_clicked();
    void on_pushButtonRecDivVertical_clicked();
    void on_pushButtonRecDivHorizontal_clicked();

    // bottom menu buttons click
    void on_pushButtonDraw_clicked();
    void on_pushButtonDrag_clicked();
    void on_horizontalSliderAlgorithmSpeed_valueChanged(int value);

    // other
    void hideMenu();
    void draw();
    void onMazeFinished();

    void on_pushButtonNextStep_clicked();

private:
    Ui::MainWindow *ui;

    MyGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QTimer *sceneUpdateTimer;

    GraphicsMap *gm;

    QPropertyAnimation *openMenuAnim;
    QPropertyAnimation *closeMenuAnim;
    QFrame *openedMenu = nullptr;

    PathfindingAlgorithm *algorithm = nullptr;
    MazeGenerationAlgorithm *mazeAlgorithm = nullptr;

    void openMenuFrame();
    void closeMenuFrame();
    void hideAllSubmenues();
    void switchMenu(QFrame *menu);

    void setAlgorithm(PathfindingAlgorithm *newAlgorithm);
    void setAndRunMazeAlgorithm(MazeGenerationAlgorithm *newAlgorithm);
};
#endif // MAINWINDOW_H
