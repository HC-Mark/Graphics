#include "mainwindow.h"
#include <QApplication>
#include "globalincludes.h"
#include "ray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //test ray
//    glm::mat4 test = glm::mat4(2.0);
//    test[3] = glm::vec4(0,0,0,1);
//    Point3f p(0,0,0);
//    Vector3f d(1,0,0);
//    Ray r(p,d);
//    Ray n = r.trans_ray(test);
//    std::cout << "test\n" << std::endl;
    return a.exec();
}
