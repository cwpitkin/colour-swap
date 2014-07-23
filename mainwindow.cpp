
#include <QtGui>
#include <QtOpenGL>
#include <QDebug>
#include <QPalette>
#include <stdio.h>

#include "mainwindow.h"



MainWindow::MainWindow()
{
    //Sets up GUI
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);


    sourceLabel = new QLabel;
    sourceLabel->setBackgroundRole(QPalette::Base);
    sourceLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    sourceLabel->setScaledContents(true);

    sourceArea = new QScrollArea;
    sourceArea->setBackgroundRole(QPalette::Dark);
    sourceArea->setWidget(sourceLabel);
    sourceArea->setWidgetResizable(true);

    resultLabel = new QLabel;
    resultLabel->setBackgroundRole(QPalette::Base);
    resultLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    resultLabel->setScaledContents(true);

    resultArea = new QScrollArea;
    resultArea->setBackgroundRole(QPalette::Dark);
    resultArea->setWidget(resultLabel);
    resultArea->setWidgetResizable(true);

    colorB = new QRadioButton("Color");
    drawB = new QRadioButton("Area");
    scribbleB = new QRadioButton("Scribble");
    satB = new QPushButton("Resaturate");
    invertB = new QPushButton("Swap Saturate");

    hLabel = new QLabel("10");
    hRLabel = new QLabel("Hue range:");
    sLabel = new QLabel("0.35");
    sRLabel = new QLabel("Saturation range:");
    tHLabel = new QLabel("Target hue:");
    hueLabel = new QLabel("0");
    tSLabel = new QLabel("Target Saturation");
    satLabel = new QLabel("0");


    hRangeSlider = new QSlider(Qt::Horizontal);
    hRangeSlider->setRange(0,360);
    hRangeSlider->setTickInterval(360/8);
    hRangeSlider->setTickPosition(QSlider::TicksBelow);
    hRangeSlider->setMinimumWidth(150);
    hRangeSlider->setValue(10);

    sRangeSlider = new QSlider(Qt::Horizontal);
    sRangeSlider->setRange(0,100);
    sRangeSlider->setTickInterval(100/8);
    sRangeSlider->setTickPosition(QSlider::TicksBelow);
    sRangeSlider->setValue(35);

    hSlider = new QSlider(Qt::Horizontal);
    hSlider->setRange(0,360);
    hSlider->setTickInterval(360/8);
    hSlider->setTickPosition(QSlider::TicksBelow);
    hSlider->setMinimumWidth(150);

    sSlider = new QSlider(Qt::Horizontal);
    sSlider->setRange(0,100);
    sSlider->setTickInterval(100/8);
    sSlider->setTickPosition(QSlider::TicksBelow);


    createActions();
    createMenus();

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addWidget(sourceArea);
    centralLayout->addWidget(resultArea);

    QVBoxLayout *sideLayout = new QVBoxLayout;
    sideLayout->setAlignment(Qt::AlignTop);
    sideLayout->setSizeConstraint(QLayout::SizeConstraint());

    colorFrame = new QFrame;
    colorFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    colorFrame->setLineWidth(2);
    colorFrame->setAutoFillBackground(true);
    colorFrame->setMinimumHeight(100);
    colorFrame->setMinimumWidth(100);

    QPalette pal = colorFrame->palette();
    pal.setColor(colorFrame->backgroundRole(), QColor(0,0,0));
    colorFrame->setPalette(pal);

    targetFrame = new QFrame;
    targetFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    targetFrame->setLineWidth(2);
    targetFrame->setAutoFillBackground(true);
    targetFrame->setMinimumHeight(100);
    targetFrame->setMinimumWidth(100);

    QPalette pal2 = targetFrame->palette();
    pal2.setColor(targetFrame->backgroundRole(), QColor(255,255,255));
    targetFrame->setPalette(pal2);

    refImage = NULL;
    newImage = NULL;
    tempImage = NULL;

    sideLayout->addWidget(colorFrame);
    sideLayout->addWidget(hRangeSlider);
    sideLayout->addWidget(hRLabel);
    sideLayout->addWidget(hLabel);
    sideLayout->addWidget(sRangeSlider);
    sideLayout->addWidget(sRLabel);
    sideLayout->addWidget(sLabel);
    sideLayout->addWidget(colorB);
    sideLayout->addWidget(drawB);
    sideLayout->addWidget(scribbleB);
    sideLayout->addWidget(targetFrame);
    sideLayout->addWidget(hSlider);
    sideLayout->addWidget(tHLabel);
    sideLayout->addWidget(hueLabel);
    sideLayout->addWidget(sSlider);
    sideLayout->addWidget(tSLabel);
    sideLayout->addWidget(satLabel);
    sideLayout->addWidget(satB);
    sideLayout->addWidget(invertB);

    connect(hSlider, SIGNAL(valueChanged(int)), this, SLOT(hChanged()));
    connect(sSlider, SIGNAL(valueChanged(int)), this, SLOT(hChanged()));
    connect(satB, SIGNAL(clicked()), this, SLOT(saturate()));
    connect(invertB, SIGNAL(clicked()), this, SLOT(swapSaturate()));
    connect(hRangeSlider, SIGNAL(valueChanged(int)), hLabel, SLOT(setNum(int)));
    connect(sRangeSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSValue(int)));
    connect(hSlider, SIGNAL(valueChanged(int)), hueLabel, SLOT(setNum(int)));
    connect(sSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSatValue(int)));

    centralLayout->addLayout(sideLayout);

    centralWidget->setLayout(centralLayout);


    setWindowTitle(tr("CSC 205 Assignment 4"));
    resize(800, 600);
}



//Creates file menu Actions
void MainWindow::createActions()
{
    openFileAct = new QAction(tr("&Open Reference Image File"), this);
    openFileAct->setShortcut(tr("Ctrl+O"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Close);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


}
//Creates file menu
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAct);


    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

}


//Opens image file from menu
void MainWindow::openFile(){


    filename = QFileDialog::getOpenFileName(this, tr("open file"), "/home",tr("Images (*.png *.tif *.bmp *.jpg)")  );

    if(!filename.isEmpty()){
        refImage = new QImage(filename);
        if(refImage->isNull()){
            QMessageBox::information(this, tr("Warning!"),
                                                  tr("Cannot load %1.").arg(filename));
            return;
        }
        sourceLabel->setPixmap(QPixmap::fromImage(*refImage));
        sourceLabel->adjustSize();
        greyImage();
    }

}
//Creates a grayscale image of the reference image
void MainWindow::greyImage() {
    int w = refImage->width();
    int h = refImage->height();
    newImage = new QImage(w,h,QImage::Format_RGB32);
    for(int i = 0; i < h;i++) {
        for(int j = 0; j < w;j++) {
            int grey = qGray(refImage->pixel(j,i));
            newImage->setPixel(j,i,qRgb(grey,grey,grey));
        }
    }
    resultLabel->setPixmap(QPixmap::fromImage(*newImage));
    resultLabel->adjustSize();
}
//This handles the possible mouse click events in the following order: Colour selection on reference image, ColourSelections on
//result image, draws red on result image, scribble saturation, right-click area saturation.
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && colorB->isChecked() && refImage != NULL && refClick(event->x(),event->y())){
        int x = event->pos().x() - 10;
        int y = event->pos().y() - 30;
        double xScale = (double)refImage->width()/sourceLabel->width();
        double yScale = (double)refImage->height()/sourceLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        refColor = refImage->pixel(j,i);
        setColorFrame(qRed(refColor),qGreen(refColor),qBlue(refColor));
    }
    if(event->button() == Qt::LeftButton && colorB->isChecked() && newImage != NULL && newClick(event->x(),event->y())){
        int x = event->pos().x() - sourceLabel->width()- 20;
        int y = event->pos().y() - 30;
        double xScale = (double)newImage->width()/resultLabel->width();
        double yScale = (double)newImage->height()/resultLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        refColor = refImage->pixel(j,i);
        setColorFrame(qRed(refColor),qGreen(refColor),qBlue(refColor));
    }
    if(event->button() == Qt::LeftButton && drawB->isChecked() && newImage != NULL && newClick(event->x(),event->y())) {
        tempImage = new QImage(newImage->width(),newImage->height(),QImage::Format_RGB32);
        for(int i = 0; i < newImage->height();i++) {
            for(int j = 0; j < newImage->width();j++) {
                tempImage->setPixel(j,i,QRgb(newImage->pixel(j,i)));
            }
        }
        boundaryImage = new QImage(newImage->width(),newImage->height(),QImage::Format_RGB32);
        for(int i = 0; i < newImage->height();i++) {
            for(int j = 0;j<newImage->width();j++) {
                int grey = qGray(refImage->pixel(j,i));
                boundaryImage->setPixel(j,i,qRgb(grey,grey,grey));
            }
        }
        int x = event->pos().x() - sourceLabel->width()- 20;
        int y = event->pos().y() - 30;
        double xScale = (double)newImage->width()/resultLabel->width();
        double yScale = (double)newImage->height()/resultLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        drawPoint(j,i);
        resultLabel->setPixmap(QPixmap::fromImage(*tempImage));
        resultLabel->adjustSize();
        lastPos.setX(j);
        lastPos.setY(i);
        wasRed = true;
    }
    if(event->button() == Qt::LeftButton && scribbleB->isChecked() && newImage != NULL && newClick(event->x(),event->y())) {
        int x = event->pos().x() - sourceLabel->width()- 20;
        int y = event->pos().y() - 30;
        double xScale = (double)newImage->width()/resultLabel->width();
        double yScale = (double)newImage->height()/resultLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        drawPoint2(j,i);
        resultLabel->setPixmap(QPixmap::fromImage(*newImage));
        resultLabel->adjustSize();
        lastPos.setX(j);
        lastPos.setY(i);
    }
    if(event->button() == Qt::RightButton && drawB->isChecked()&& newImage != NULL && newClick(event->x(),event->y()) && wasRed) {
        int x = event->pos().x() - sourceLabel->width()- 20;
        int y = event->pos().y() - 30;
        double xScale = (double)newImage->width()/resultLabel->width();
        double yScale = (double)newImage->height()/resultLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        boundaryFill(j,i);
        resultLabel->setPixmap(QPixmap::fromImage(*newImage));
        resultLabel->adjustSize();
        wasRed = false;
    }
}
// Handles mouse move events for red drawing and scribble saturation.
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton && drawB->isChecked()&& newImage != NULL && newClick(event->x(),event->y())&&wasRed) {
        int x = event->pos().x() - sourceLabel->width()- 20;
        int y = event->pos().y() - 30;
        double xScale = (double)newImage->width()/resultLabel->width();
        double yScale = (double)newImage->height()/resultLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        drawLine(j,lastPos.x(),i,lastPos.y());
        lastPos.setX(j);
        lastPos.setY(i);
        resultLabel->setPixmap(QPixmap::fromImage(*tempImage));
        resultLabel->adjustSize();
    }
    if(event->buttons() == Qt::LeftButton && scribbleB->isChecked()&& newImage != NULL && newClick(event->x(),event->y())) {
        int x = event->pos().x() - sourceLabel->width()- 20;
        int y = event->pos().y() - 30;
        double xScale = (double)newImage->width()/resultLabel->width();
        double yScale = (double)newImage->height()/resultLabel->height();
        int j = (double)x*xScale;
        int i = (double)y*yScale;
        drawLine(j,lastPos.x(),i,lastPos.y());
        lastPos.setX(j);
        lastPos.setY(i);
        resultLabel->setPixmap(QPixmap::fromImage(*newImage));
        resultLabel->adjustSize();
    }
}
// Checks if coordinates are on the reference image
bool MainWindow::refClick(int x, int y) {
    if(x<10) {
        return false;
    }
    if(x>sourceLabel->width()+10) {
        return false;
    }
    if(y<30) {
        return false;
    }
    if(y>sourceLabel->height()+30) {
        return false;
    }
    return true;
}
// Checks if coordinates are on result image
bool MainWindow::newClick(int x, int y) {
    if(x<sourceLabel->width()+20) {
        return false;
    }
    if(x>sourceLabel->width()*2+20) {
        return false;
    }
    if(y<30) {
        return false;
    }
    if(y>sourceLabel->height()+30) {
        return false;
    }
    return true;
}
// Finds instances of user selected colour and resaturates them into image based on user selected range.
void MainWindow::saturate() {
    float tarHue, tarSat, tarVal, refHue, refSat, refVal;
    getHsv(refColor,&tarHue,&tarSat,&tarVal);
    float satRange = (float)sRangeSlider->value()/100;
    int hueRange = hRangeSlider->value();
    for(int i = 0;i<refImage->height();i++) {
        for(int j = 0;j<refImage->width();j++) {
            QRgb color = refImage->pixel(j,i);
            getHsv(color,&refHue,&refSat,&refVal);
            if((refHue<=(tarHue + hueRange)&&(refHue>=(tarHue-hueRange)))&&((refSat<=(tarSat+satRange)))&&(refSat>=(tarSat-satRange))) {
                newImage->setPixel(j,i,color);
            }
        }
    }
    resultLabel->setPixmap(QPixmap::fromImage(*newImage));
    resultLabel->adjustSize();
}
// Finds instnaces of user selected colour and replaces anything within a user selected range of that with another user selected
// colour.
void MainWindow::swapSaturate() {
    float tarHue, tarSat, tarVal, refHue, refSat, refVal,r,g,b,newHue,newVal,newSat;
    getHsv(tarColor,&newHue,&newSat,&newVal);
    getHsv(refColor,&tarHue,&tarSat,&tarVal);
    float satRange = (float)sRangeSlider->value()/100;
    int hueRange = hRangeSlider->value();
    for(int i = 0;i<refImage->height();i++) {
        for(int j = 0;j<refImage->width();j++) {
            QRgb color = refImage->pixel(j,i);
            getHsv(color,&refHue,&refSat,&refVal);
            if((refHue<=(tarHue + hueRange)&&(refHue>=(tarHue-hueRange)))&&((refSat<=tarSat + satRange))&&(refSat>=tarSat - satRange)) {
                refHue = newHue;
                refSat = newSat;
                getRgb(&refHue,&refSat,&refVal,&r,&g,&b);
                newImage->setPixel(j,i,qRgb(r,g,b));
            }
        }
    }
    resultLabel->setPixmap(QPixmap::fromImage(*newImage));
    resultLabel->adjustSize();
}
// Calculates hue,saturation and value of given colour.
void MainWindow::getHsv(QRgb color, float *h, float *s, float *v) {
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);
    float maxC = b;
    if (maxC < r) {
        maxC = r;
    }
    if (maxC < g) {
        maxC = g;
    }
    float minC = b;
    if (minC > g) {
        minC = g;
    }
    if (minC > r) {
        minC = r;
    }
    *v = maxC;
    float delta = maxC - minC;
    if (maxC != 0) {
            *s = delta/maxC;
    } else {
        *s = 0;
    }
    if (delta == 0) {
        *h = 0;
    } else if (r == maxC) {
        *h = (g - b)/delta;
    } else if(g == maxC) {
        *h = 2 + (b - r)/delta;
    } else {
        *h = 4 + (r - g)/delta;
    }
    *h *= 60;
    if ( *h < 0) {
        *h += 360;
    }
}
// Sets the colour of the colour picker frame
void MainWindow::setColorFrame(int r, int g, int b)
{
    QPalette pal = colorFrame->palette();
    pal.setColor(colorFrame->backgroundRole(), QColor(r,g,b));
    colorFrame->setPalette(pal);

}
// Sets colour of colour replaces frame
void MainWindow::setTargetFrame(int r, int g, int b)
{
    QPalette pal = targetFrame->palette();
    pal.setColor(targetFrame->backgroundRole(), QColor(r,g,b));
    targetFrame->setPalette(pal);

}
// Draws a straight line between two given points
void MainWindow::drawLine(int x1, int x2, int y1, int y2) {
    boolean steep = abs(y2-y1) > abs(x2-x1);
    if (steep) {
        swap(x1,y1);
        swap(x2,y2);
    }
    if (x1 > x2) {
        swap(x1,x2);
        swap(y1,y2);
    }
    int deltax = x2 - x1;
    int deltay = abs(y2 - y1);
    int error = deltax/2;
    int ystep;
    int y = y1;
    if (y1 < y2) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if(drawB->isChecked()) {
            drawPoint(y,x);
            } else {
                drawPoint2(y,x);
            }
        } else {
            if(drawB->isChecked()) {
            drawPoint(x,y);
            } else {
                drawPoint2(x,y);
            }
        }
        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}
// Draws a red point on selected position
void MainWindow::drawPoint(int x, int y) {
    for(int i = x-1;i<=x+1;i++) {
        if(i>=0&&i<refImage->width()-1&&y>=0&&y<refImage->height()-1) {
            tempImage->setPixel(i,y,qRgb(255,0,0));
            boundaryImage->setPixel(i,y,qRgb(255,0,0));
        }
    }
    for(int j = y-1;j<=y+1;j++) {
        if(x>=0&&x<refImage->width()-1&&j>=0&&j<refImage->height()-1) {
            tempImage->setPixel(x,j,qRgb(255,0,0));
            boundaryImage->setPixel(x,j,qRgb(255,0,0));
        }
    }
}
// Draws a resaturated point on selected position
void MainWindow::drawPoint2(int x, int y) {
    for(int i = x-2;i<=x+2;i++) {
        if(i>=0&&i<refImage->width()-1&&y>=0&&y<refImage->height()) {
            newImage->setPixel(i,y,QRgb(refImage->pixel(i,y)));
        }
    }
    for(int j = y-2;j<=y+2;j++) {
        if(x>=0&&x<refImage->width()-1&&j>=0&&j<refImage->height()) {
            newImage->setPixel(x,j,QRgb(refImage->pixel(x,j)));
        }
    }
}
// Swaps two variables
void MainWindow::swap(int &x, int &y) {
    int temp = x;
    x = y;
    y = temp;
}
// Calculates rgb values from hsv values.
void MainWindow::getRgb(float *h, float *s, float *v, float *r, float *g, float *b) {
    int i;
    float f, p, q, t;
        *h /= 60;
        i = floor(*h);
        f = *h - i;
        p = *v * (1 - *s);
        q = *v * (1 - *s * f);
        t = *v * ( 1 - *s * ( 1 - f ) );
    switch (i) {
    case 0:
        *r = *v;
        *g = t;
        *b = p;
        break;
    case 1:
        *r = q;
        *g = *v;
        *b = p;
        break;
    case 2:
        *r = p;
        *g = *v;
        *b = t;
        break;
    case 3:
        *r = p;
        *g = q;
        *b = *v;
        break;
    case 4:
        *r = t;
        *g = p;
        *b = *v;
        break;
    case 5:
        *r = *v;
        *g = p;
        *b = q;
        break;
    case 6:
        *r = *v;
        *g = t;
        *b = p;
        break;
    }
}
// Changes colour of colour replaces frame when slider values change
void MainWindow::hChanged() {
    float h, s, v, r, g, b;
    h = (float)hSlider->value();
    s = (float)sSlider->value()/100;
    v = 255;
    getRgb(&h,&s,&v,&r,&g,&b);
    setTargetFrame(r,g,b);
    tarColor = qRgb(r,g,b);
}
// Implements a lazy version of the 'right-hand rule' flood fill by sticking to the borders and when a border is directly to the
// left of the pixel, every pixel until another border is found to the right of the pixel.
void MainWindow::boundaryFill(int x, int y) {
    while(!boundaryCheck(x-1,y)) {
        x = x - 1;
    }
    lastPos.setX(x);
    lastPos.setY(y);
    int last = 2;
    fillRight(x,y);
    do {
         if(last == 1) {
            if(!boundaryCheck(x,y-1)) {
                y = y - 1;
                last = 4;
            } else if(!boundaryCheck(x-1,y)) {
                x = x - 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 1;
            } else if(!boundaryCheck(x,y+1)) {
                y = y + 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 2;
            } else if(!boundaryCheck(x+1,y)) {
                x = x + 1;
                last = 3;
            }
        } else if(last == 2) {
            if(!boundaryCheck(x-1,y)) {
                x = x - 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 1;
            } else if(!boundaryCheck(x,y+1)) {
                y = y + 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 2;
            } else if(!boundaryCheck(x+1,y)) {
                x = x + 1;
                last = 3;
            } else if(!boundaryCheck(x,y-1)) {
                y = y - 1;
                last = 4;
            }
        } else if(last == 3) {
            if(!boundaryCheck(x,y+1)) {
                y = y + 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 2;
            } else if(!boundaryCheck(x+1,y)) {
                x = x + 1;
                last = 3;
            } else if(!boundaryCheck(x,y-1)) {
                y = y - 1;
                last = 4;
            } else if(!boundaryCheck(x-1,y)) {
                x = x - 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 1;
            }
        } else if(last == 4) {
            if(!boundaryCheck(x+1,y)) {
                x = x + 1;
                last = 3;
            } else if(!boundaryCheck(x,y-1)) {
                y = y - 1;
                last = 4;
            } else if(!boundaryCheck(x-1,y)) {
                x = x - 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 1;
            } else if(!boundaryCheck(x,y+1)) {
                y = y + 1;
                if(boundaryCheck(x-1,y)) {
                    fillRight(x,y);
                }
                last = 2;
            }
        }
    } while(x!=lastPos.x()||y!=lastPos.y());
}
// Checks to see if the given pixel is part of the boundary
bool MainWindow::boundaryCheck(int x, int y) {
    if(x<0) {
        return true;
    }if(x>=refImage->width()-1) {
        return true;
    }if (y<0) {
        return true;
    }if (y>=refImage->height()-1){
        return true;
    }if(qRed(boundaryImage->pixel(x,y))==255&&qGreen(boundaryImage->pixel(x,y))==0&&qBlue(boundaryImage->pixel(x,y))==0) {
        return true;
    } else {
        return false;
    }
}
// Resaturates every pixel to the right of the given pixel until a boundary is found.
void MainWindow::fillRight(int x, int y) {
    newImage->setPixel(x,y,QRgb(refImage->pixel(x,y)));
    while(!boundaryCheck(x+1,y)) {
       x = x + 1;
       newImage->setPixel(x,y,QRgb(refImage->pixel(x,y)));
    }
}

void MainWindow::updateSValue(int value)
{
    sLabel->setNum((double)value/100.0);
}

void MainWindow::updateSatValue(int value)
{
    satLabel->setNum((double)value/100.0);
}
