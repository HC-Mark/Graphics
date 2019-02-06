#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>
#include "globalincludes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void DisplayQImage(QImage &i);
    //void keyPressEvent(QKeyEvent *e);

private slots:

    void on_actionSave_Image_triggered();

    void on_actionQuit_Esc_triggered();

private:
    Ui::MainWindow *ui;

    //This is used to display the QImage produced by RenderScene in the GUI
    QGraphicsScene graphics_scene;
    //This is the image rendered by your program when it loads a scene
    QImage rendered_image;
};

#endif // MAINWINDOW_H
