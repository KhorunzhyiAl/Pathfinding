#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    // create and set overlapping widgets
    this->graphicsView = new MyGraphicsView();
    this->graphicsView->setScene(scene);

    ui->gridLayoutForOverlapingWidgets->addWidget(graphicsView, 0, 0, Qt::AlignRight);
    this->graphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    int w = ui->gridLayoutForOverlapingWidgets->geometry().width();
//    int h = ui->gridLayoutForOverlapingWidgets->geometry().height();
    this->graphicsView->setMinimumSize(w,0);

    this->graphicsView->lower();
    ui->MenuContainer->lower();


    ui->gridLayoutForOverlapingWidgets->removeWidget(ui->MenuContainer);
    ui->gridLayoutForOverlapingWidgets->addWidget(ui->MenuContainer, 0, 0, Qt::AlignLeft);

    ui->gridLayoutForOverlapingWidgets->removeWidget(ui->BottomMenuContainer);
    ui->gridLayoutForOverlapingWidgets->addWidget(ui->BottomMenuContainer, 0, 0, Qt::AlignRight | Qt::AlignBottom);


    // initialize map and connect to graphicsview
    gm = new GraphicsMap();
    connect(graphicsView, SIGNAL(mousePressedInDrawMode(QPoint, Qt::MouseButton)),
            gm, SLOT(mousePressed(QPoint, Qt::MouseButton)));
    connect(graphicsView, SIGNAL(mouseMovedInDrawMode(QPoint)),
            gm, SLOT(mouseMovedWhilePressed(QPoint)));
    connect(graphicsView, SIGNAL(mouseReleasedInDrawMode()),
            gm, SLOT(mouseReleased()));



    // create and set scene update timer
    sceneUpdateTimer = new QTimer(this);
    sceneUpdateTimer->setInterval(10);
    connect(sceneUpdateTimer, SIGNAL(timeout()), this, SLOT(draw()));
    sceneUpdateTimer->start();

    // Create menu animations
    openMenuAnim = new QPropertyAnimation(this);
    openMenuAnim->setPropertyName("maximumWidth");
    openMenuAnim->setStartValue(0);
    openMenuAnim->setEndValue(300);
    openMenuAnim->setDuration(100);
    openMenuAnim->setTargetObject(ui->SecondMenuFrame);

    closeMenuAnim = new QPropertyAnimation(this);
    closeMenuAnim->setPropertyName("maximumWidth");
    closeMenuAnim->setStartValue(300);
    closeMenuAnim->setEndValue(0);
    closeMenuAnim->setDuration(100);
    closeMenuAnim->setTargetObject(ui->SecondMenuFrame);
    connect(closeMenuAnim, SIGNAL(finished()), this, SLOT(hideMenu()));

    ui->InfoMenu->setHidden(1);
    ui->AlgorithmsMenu->setHidden(1);
    openedMenu = nullptr;


    // set dijkstra's algorithm by defoult.
    this->on_pushButtonDijkAlg_clicked();

    ui->horizontalSliderAlgorithmSpeed->setValue(7);

    this->on_pushButtonDraw_clicked();
    this->hideMenu();

    qRegisterMetaType<NODE>("NODE");
}

MainWindow::~MainWindow()
{
    delete gm;
    delete ui;
    delete algorithm;
    delete mazeAlgorithm;
}


/*
 \===============================================================================================\
 \=================================== MAINWINDOW EVENTS =========================================\
 \===============================================================================================\
*/
void MainWindow::showEvent(QShowEvent *)
{
    int w = ui->gridLayoutForOverlapingWidgets->geometry().width();
    int h = ui->gridLayoutForOverlapingWidgets->geometry().height();
    this->graphicsView->setMinimumSize(w,h);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int w = ui->gridLayoutForOverlapingWidgets->geometry().width();
//    int h = ui->gridLayoutForOverlapingWidgets->geometry().height();
    this->graphicsView->setMinimumSize(w,0);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key::Key_Shift) {
        on_pushButtonDrag_clicked();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key::Key_Shift) {
        on_pushButtonDraw_clicked();
    }
}



/*
 \===============================================================================================\
 \===================================== MAIN MENU BUTTONS =======================================\
 \===============================================================================================\
*/
void MainWindow::on_pushButtonRun_clicked()
{
    if (algorithm == nullptr) return;

    if (algorithm->IsInProcess()) {
        algorithm->Stop();
        gm->vm->unlockChanges();
        gm->vm->clearPath();
        ui->pushButtonRun->setText("Run");

        on_pushButtonDraw_clicked();
    } else {
        if (mazeAlgorithm != nullptr) {
            mazeAlgorithm->stop();
        }

        gm->vm->clearPath();
        gm->vm->lockForUser();
        gm->vm->isInstantUpdateMode = false;
        algorithm->Start();
        ui->pushButtonRun->setText("Stop");

        on_pushButtonDrag_clicked();

    }
}

void MainWindow::on_pushButtonClear_clicked()
{
    if (mazeAlgorithm != nullptr) {
        mazeAlgorithm->stop();
    }

    gm->vm->clear();
    algorithm->Stop();
    gm->vm->unlockChanges();
    this->onPathfindingFinished();
}




void MainWindow::on_pushButtonInfo_clicked()
{
    switchMenu(ui->InfoMenu);
//    ui->pushButtonInfo->setStyleSheet(Styles::blackButton);
}

void MainWindow::on_pushButtonAlgorithm_clicked()
{
    switchMenu(ui->AlgorithmsMenu);
//    ui->pushButtonAlgorithm->setStyleSheet(Styles::blackButton);
}

void MainWindow::on_pushButtonCreateMaze_clicked()
{
    if (mazeAlgorithm != nullptr && mazeAlgorithm->IsWorking()) {
        mazeAlgorithm->stop();
        ui->pushButtonCreateMaze->setText("Create Maze");
    } else {
        this->switchMenu(ui->mazeMenu);
    //    ui->pushButtonCreateMaze->setStyleSheet(Styles::blackButton);
    }
}



/*
 \===============================================================================================\
 \=================================== SUBMENU BUTTONS ===========================================\
 \===============================================================================================\
*/
void MainWindow::on_pushButtonDijkAlg_clicked()
{
    setAlgorithm(new DijkstrasAlgorithm(gm->vm));
    ui->labelAlgorithmInfo->setText("Dijkstra's algorithm");
}

void MainWindow::on_pushButtonAStarAlg_clicked()
{
    setAlgorithm(new AStarAlgorithm(gm->vm));
    ui->labelAlgorithmInfo->setText("A* algorithm");
}

void MainWindow::on_pushButtonRandomMaze_clicked()
{
    setAndRunMazeAlgorithm(new RandomMazeAlgorithm(gm->vm));
}

void MainWindow::on_pushButtonRecursiveDivision_clicked()
{
    setAndRunMazeAlgorithm(new RecursiveDivisionMazeAlgorithm(gm->vm, RecursiveDivisionMazeAlgorithm::Type::NORMAL));
}

void MainWindow::on_pushButtonRecDivVertical_clicked()
{
    setAndRunMazeAlgorithm(new RecursiveDivisionMazeAlgorithm(gm->vm, RecursiveDivisionMazeAlgorithm::Type::VERTICAL));
}

void MainWindow::on_pushButtonRecDivHorizontal_clicked()
{
    setAndRunMazeAlgorithm(new RecursiveDivisionMazeAlgorithm(gm->vm, RecursiveDivisionMazeAlgorithm::Type::HORIZONTAL));
}



/*
 \===============================================================================================\
 \================================== BUTTOM MENU BUTTONS ========================================\
 \===============================================================================================\
*/
void MainWindow::on_pushButtonDraw_clicked()
{
    graphicsView->setDragMode(QGraphicsView::DragMode::NoDrag);

    ui->pushButtonDraw->setStyleSheet(Styles::pressedButton);
    ui->pushButtonDrag->setStyleSheet(Styles::unpressedButton);
}

void MainWindow::on_pushButtonDrag_clicked()
{
    graphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);

    ui->pushButtonDrag->setStyleSheet(Styles::pressedButton);
    ui->pushButtonDraw->setStyleSheet(Styles::unpressedButton);
}

void MainWindow::on_horizontalSliderAlgorithmSpeed_valueChanged(int value)
{
    if (algorithm != nullptr) {
        algorithm->SetSpeed(value);
        gm->setAnimationInterval(70 - (value * 70 / 15) + 10);
    }
}



/*
 \===============================================================================================\
 \======================================= OTHER SLOTS ===========================================\
 \===============================================================================================\
*/
void MainWindow::draw()
{
    scene->clear();
    QPixmap pix = QPixmap::fromImage(*(gm->getImage()),
                                     Qt::AvoidDither |
                                     Qt::NoFormatConversion |
                                     Qt::NoOpaqueDetection);
    scene->addPixmap(pix);
}

void MainWindow::onMazeFinished()
{
    ui->pushButtonCreateMaze->setText("Create Maze");
}

void MainWindow::onPathfindingFinished()
{
    ui->pushButtonRun->setText("Run");
    on_pushButtonDraw_clicked();
}


/*
 \===============================================================================================\
 \===================================== OTHER FUNCTIONS =========================================\
 \===============================================================================================\
*/
void MainWindow::hideMenu()
{
    ui->SecondMenuFrame->setHidden(1);
    openedMenu = nullptr;
    ui->MenuContainer->lower();
}

void MainWindow::openMenuFrame()
{
    ui->SecondMenuFrame->setHidden(0);
    closeMenuAnim->stop();
    openMenuAnim->start();
    ui->MenuContainer->raise();
}

void MainWindow::closeMenuFrame()
{
    openMenuAnim->stop();
    closeMenuAnim->start();
    openedMenu = nullptr;

}

void MainWindow::hideAllSubmenues()
{
    for (int i = 0; i < ui->SecondMenuFrame->layout()->count(); i++) {
        ui->SecondMenuFrame->layout()->itemAt(i)->widget()->hide();
    }
}

void MainWindow::switchMenu(QFrame *menu)
{
    if (openedMenu != menu) {
        // open menu

        hideAllSubmenues();
        menu->setHidden(0);

        if (openedMenu == nullptr)
            openMenuFrame();

        openedMenu = menu;
    } else {
        // close menu
        closeMenuFrame();
    }


//    for (int i = 0; i < ui->MainMenuFrame->layout()->count(); i++) {
//        ui->MainMenuFrame->layout()->itemAt(i)->widget()->setStyleSheet(Styles::blueButton);
//    }
}

void MainWindow::setAlgorithm(PathfindingAlgorithm *newAlgorithm)
{
//    ui->SecondMenuFrame->hide();
    closeMenuFrame();

    if (algorithm != nullptr) {
        delete algorithm;
        gm->vm->clearPath();
        gm->vm->unlockChanges();
        ui->pushButtonRun->setText("Run");
    }

    algorithm = newAlgorithm;

    ui->horizontalSliderAlgorithmSpeed->setValue(7);
    on_horizontalSliderAlgorithmSpeed_valueChanged(7);

    connect(algorithm, SIGNAL(finished()), gm, SLOT(onPathfindingFinished()));
    connect(algorithm, SIGNAL(finished()), gm, SLOT(onMapChanged()));
    connect(algorithm, SIGNAL(finished()), this, SLOT(onPathfindingFinished()));
}

void MainWindow::setAndRunMazeAlgorithm(MazeGenerationAlgorithm *newAlgorithm)
{
    this->on_pushButtonCreateMaze_clicked(); // to close the menu

    if (mazeAlgorithm != nullptr && mazeAlgorithm->IsWorking()) {
        mazeAlgorithm->stop();
    }

    delete mazeAlgorithm;
    this->mazeAlgorithm = newAlgorithm;

    connect(mazeAlgorithm, SIGNAL(finished()), this, SLOT(onMazeFinished()));

    mazeAlgorithm->start();

    ui->pushButtonCreateMaze->setText("Stop Making \nMaze");
}


void MainWindow::on_pushButtonNextStep_clicked()
{
    if (algorithm != nullptr) {
        algorithm->step();
    }
}
