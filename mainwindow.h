
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtGui>
#include <QPalette>
#include <QRadioButton>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QSlider;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void openFile();
    void hChanged();
    void saturate();
    void swapSaturate();
    void updateSValue(int);
    void updateSatValue(int);

private:
    void createActions();
    void createMenus();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool refClick(int,int);
    bool newClick(int,int);
    void setColorFrame(int r, int g, int b);
    void setTargetFrame(int r, int g, int b);
    void greyImage();
    void reSat(QRgb);
    void getHsv(QRgb,float*,float*,float*);
    void drawLine(int,int,int,int);
    void drawPoint(int,int);
    void drawPoint2(int,int);
    void swap(int&,int&);
    void getRgb(float*,float*,float*,float*,float*,float*);
    void boundaryFill(int,int);
    bool boundaryCheck(int,int);
    void fillRight(int,int);

    QWidget *centralWidget;

    QLabel *hLabel;
    QLabel *hRLabel;
    QLabel *sLabel;
    QLabel *sRLabel;
    QLabel *tHLabel;
    QLabel *hueLabel;
    QLabel *tSLabel;
    QLabel *satLabel;

    QScrollArea *sourceArea;
    QLabel *sourceLabel;
    QScrollArea *resultArea;
    QLabel *resultLabel;

    QRadioButton *colorB;
    QRadioButton *drawB;
    QRadioButton *scribbleB;

    QPushButton *satB;
    QPushButton *invertB;

    QSlider *hSlider;
    QSlider *sSlider;
    QSlider *sRangeSlider;
    QSlider *hRangeSlider;

    QMenu *fileMenu;

    QAction *openFileAct;
    QAction *exitAct;

    QString filename;

    QImage *refImage;
    QImage *newImage;
    QImage *tempImage;
    QImage *boundaryImage;

    QRgb refColor;
    QRgb tarColor;

    QFrame *colorFrame;
    QFrame *targetFrame;

    QPoint lastPos;
    bool wasRed;
};

#endif
