#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

enum NODE {
    empty = 0,
    start,
    finish,
    wall,
    visited,
    path
};


#define CELL_SIZE 64
#define X_NODES 100
#define Y_NODES 7


namespace Styles {
static QString blueButton =  "QPushButton {"
    "background-color: rgb(0, 174, 255);"
    "color: rgb(30, 30, 30);"
    "border: 0px;"
    "}"
    "QPushButton:hover {"
    "background-color: rgb(40, 40, 40);"
    "color: rgb(230,230,230);"
    "}";

static QString blackButton = "QPushButton {"
    "background-color: rgb(40, 40, 40);"
    "color: rgb(230,230,230);"
    "}"
    "QPushButton:hover {"
    "background-color: rgb(70, 70, 70);"
    "color: rgb(230,230,230);"
    "}";

static QString unpressedButton = "QPushButton {"
    "background-color: transparent;"
    "color: rgb(230, 230, 230);"
    "border: 0px;"
    "border-bottom: 4px solid;"
    "border-bottom-color: rgb(0, 174, 255);"
    "}"
    "QPushButton:hover {"
    "background-color: qlineargradient(spread:pad, x1:1, y1:0.1, x2:1, y2:0.9, stop:0 rgba(223, 223, 223, 0), stop:1 rgba(0, 174, 255, 255));"
    "}";

static QString pressedButton = " QPushButton {"
    "background-color: rgb(0, 174, 255);"
    "color: rgb(30, 30, 30);"
    "border: 0px;"
    "}"
    "QPushButton:hover {"
    "background-color: rgb(122, 222, 255);"
    "}";

}

#endif // GLOBAL_H
