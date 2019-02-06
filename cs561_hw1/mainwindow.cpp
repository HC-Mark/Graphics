#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QImageWriter>
#include <QDebug>
#include <QPixmap>
#include "camera.h"
#include "scene.h"

#include "sphere.h"
#include "plane.h"


#define SCENE_WIDTH 400
#define SCENE_HEIGHT 400

using std::unique_ptr;
using std::vector;

//might need to cast ray
QColor cal_color(Scene& s, int x, int y) {
    Ray ray = s.camera.ray_cast(x,y);
    Intersection i1;
    glm::vec3 RGB_raw = glm::vec3(0,0,0); //background color -- do we need to run recursively?
    if(s.getIntersection(ray,i1)) {
        //RGB_raw = (i1.normal + glm::vec3(1,1,1)) * 0.5f;
        //RGB_raw = (i1.objectHit->material->base_col + glm::vec3(1,1,1)) * 0.5f;
        RGB_raw = i1.objectHit->material->GetScatteredColor(i1,ray,s,1); //at most 1
    }
    //make sure RGB doesn't go over 1
    RGB_raw.r = std::min(1.0f, RGB_raw.r);
    RGB_raw.b = std::min(1.0f, RGB_raw.b);
    RGB_raw.g = std::min(1.0f, RGB_raw.g);
    //glm::vec3 RGB_raw = (ray.direct + glm::vec3(1,1,1)) * 0.5f;
    QColor RGB(RGB_raw.r * 255,RGB_raw.g * 255,RGB_raw.b * 255);

    return RGB;

}
//each home work should have a render Scene function
QImage RenderScene_hw2() {
    QImage q_image(SCENE_WIDTH,SCENE_HEIGHT,QImage::Format_RGB32);
    //set color
    q_image.fill(Qt::black); //why can't use QColor

    //setup camera
    Point3f eye(0, 0.5, 12);
    Point3f ref(0, 0, 0);
    Vector3f worldUp(0, 1, 0);
    //initialize scene
    int fieldOfView = 50;
    int width = 400;
    int height = 400;
    float nearClip = 0.1;
    float farClip = 1000;
    Camera hw2(eye,ref,worldUp,fieldOfView,nearClip,farClip,width,height);

    //setup scene
    //color list
    Color3f white(glm::vec3(1,1,1));
    Color3f red(glm::vec3(1,0,0));
    Color3f green(glm::vec3(0,1,0));
    Color3f blue(glm::vec3(0,0,1));
    Color3f yellow(glm::vec3(1,1,0.25));
    Color3f purple(glm::vec3(0.75,0,0.75));
    Color3f mirr_2(glm::vec3(0.25,0.5,1));
    Color3f ref_2(glm::vec3(0.15,1,0.15));

    //walls
    glm::vec3 bw_trans(0,0,-5), bw_rot(0,0,0),bw_scale(10,10,1);
    glm::vec3 rw_trans(5,0,0), rw_rot(0,-90,0),rw_scale(10,10,1);
    glm::vec3 lw_trans(-5,0,0), lw_rot(0,90,0),lw_scale(10,10,1);
    glm::vec3 ce_trans(0,5,0), ce_rot(90,0,0),ce_scale(10,10,1);
    glm::vec3 fl_trans(0,-5,0), fl_rot(-90,0,0),fl_scale(10,10,1);
    //Material bw_m(white), rw_m(green),lw_m(red),ce_m(white);
    Transform bw(bw_trans,bw_rot,bw_scale), rw(rw_trans,rw_rot,rw_scale),lw(lw_trans,lw_rot,lw_scale);
    Transform fl(fl_trans,fl_rot,fl_scale), ce(ce_trans,ce_rot,ce_scale);
    Shape* b_wall = new Plane(bw);
    Shape* r_wall = new Plane(rw);
    Shape* l_wall = new Plane(lw);
    Shape* ceill = new Plane(ce);
    Shape* flor = new Plane(fl);

    Material* bw_m = new LambertianMaterial(white);
    Material* rw_m  = new LambertianMaterial(green);
    Material* lw_m = new LambertianMaterial(red);
    Material* ce_m = new LambertianMaterial(white);
    Material* fl_m = new LambertianMaterial(white);

    //construct primitive
    unique_ptr<Primitive> back_wall(new Primitive("back_wall", b_wall,bw_m));
    unique_ptr<Primitive> right_wall(new Primitive("right_wall", r_wall,rw_m));
    unique_ptr<Primitive> left_wall(new Primitive("left_wall", l_wall,lw_m));
    unique_ptr<Primitive> ceil(new Primitive("ceil", ceill,ce_m));
    unique_ptr<Primitive> floor(new Primitive("floor", flor,fl_m));

    vector<unique_ptr<Primitive>> p_list;
    vector<unique_ptr<Light>> l_list;

    p_list.push_back(std::move(back_wall));
    p_list.push_back(std::move(right_wall));
    p_list.push_back(std::move(left_wall));
    p_list.push_back(std::move(ceil));
    p_list.push_back(std::move(floor));

    //spheres
    //transform
    glm::vec3 ps_trans(-2,-5,0), ps_rot(0,0,0),ps_scale(5,5,5);//purple sphere
    glm::vec3 rs_trans(-3,0,2), rs_rot(0,0,0),rs_scale(4,4,4);//refractive specular
    glm::vec3 ab_trans(-3,0,2), ab_rot(0,0,0),ab_scale(2.5,2.5,2.5);//air bubble
    glm::vec3 rs2_trans(3,-2,-3), rs2_rot(0,0,0),rs2_scale(4,4,4);//refractive specular 2
    glm::vec3 wd_trans(3,-2,-3), wd_rot(0,0,0),wd_scale(1,1,1);//white diffuse
    glm::vec3 rs3_trans(3,3.5,-3), rs3_rot(0,0,0),rs3_scale(3,3,3);//refractive specular 3
    glm::vec3 yd_trans(0,0,0), yd_rot(0,0,0),yd_scale(2,2,2);//yellow diffuse
    glm::vec3 ms_trans(-3,3.5,-3), ms_rot(0,0,0),ms_scale(3,3,3);//mirror specular
    glm::vec3 ms2_trans(-3,3.5,-3), ms2_rot(0,0,0),ms2_scale(1,1,1);//mirror specular2

    Transform ps(ps_trans,ps_rot,ps_scale), rs(rs_trans,rs_rot,rs_scale),ab(ab_trans,ab_rot,ab_scale);
    Transform rs2(rs2_trans,rs2_rot,rs2_scale), wd(wd_trans,wd_rot,wd_scale),rs3(rs3_trans,rs3_rot,rs3_scale);
    Transform yd(yd_trans,yd_rot,yd_scale), ms(ms_trans,ms_rot,ms_scale),ms2(ms2_trans,ms2_rot,ms2_scale);

    //Shape
    Shape* p_s = new Sphere(ps);
    Shape* r_s = new Sphere(rs);
    Shape* a_b = new Sphere(ab);
    Shape* r_s2 = new Sphere(rs2);
    Shape* w_d = new Sphere(wd);
    Shape* r_s3 = new Sphere(rs3);
    Shape* y_d = new Sphere(yd);
    Shape* m_s = new Sphere(ms);
    Shape* m_s2 = new Sphere(ms2);

    //materials
    Material* ps_m = new PhongMaterial(purple,10.f);
    //Material* ps_m = new LambertianMaterial(purple);
//    Material* rs_m = new LambertianMaterial(white);
//    Material* ab_m = new LambertianMaterial(white);
//    Material* rs2_m = new LambertianMaterial(ref_2);
//    Material* rs2_m = new SpecularReflectionMaterial(ref_2,5);
//    Material* wd_m = new LambertianMaterial(white);
//    Material* rs3_m = new LambertianMaterial(white);
    Material* yd_m = new LambertianMaterial(yellow);
    Material* ms_m = new SpecularReflectionMaterial(mirr_2,5);
//    Material* ms2_m = new LambertianMaterial(mirr_2);

    //primitive
    unique_ptr<Primitive> purple_specular(new Primitive("purple_specular", p_s,ps_m));
    unique_ptr<Primitive> mirrored_specular(new Primitive("mirrored_specular", m_s,ms_m));
//    unique_ptr<Primitive> refractive_specular(new Primitive("refractive_specular", r_s,rs_m));
//    unique_ptr<Primitive> refractive_specular2(new Primitive("refractive_specular2", r_s2,rs2_m));
//    unique_ptr<Primitive> refractive_specular3(new Primitive("refractive_specular3", r_s3,rs3_m));
//    unique_ptr<Primitive> air_bubble(new Primitive("air_bubble", a_b,ab_m));
//    unique_ptr<Primitive> white_diffuse(new Primitive("white_diffuse", w_d,wd_m));
//    unique_ptr<Primitive> mirrored_specular2(new Primitive("mirrored_specular2", m_s2,ms2_m));
    unique_ptr<Primitive> yellow_diffuse(new Primitive("yellow_diffuse", y_d,yd_m));

    //add to list
    p_list.push_back(std::move(purple_specular));
//    p_list.push_back(std::move(refractive_specular));
//    p_list.push_back(std::move(refractive_specular2));
    p_list.push_back(std::move(mirrored_specular));
//    p_list.push_back(std::move(refractive_specular3));
    p_list.push_back(std::move(yellow_diffuse));


    //lights
    //cornell box 1
//    glm::vec3 l1_trans(0,4.9,0), l1_rot(45,0,0),l1_scale(0.5,0.5,0.5);
//    Transform l1(l1_trans,l1_rot,l1_scale);
//    Light* p1 = new PointLight(white,white,l1);
//    unique_ptr<Light> pl1(p1);
//    l_list.push_back(std::move(pl1));

    //cornell box 2
//    glm::vec3 l2_trans(-2,4.9,0), l2_rot(45,0,0),l2_scale(0.5,0.5,0.5);
//    Transform l2(l2_trans,l2_rot,l2_scale);
//    Light* p2 = new PointLight(glm::vec3(1,0.25,0.25),white,l2);
//    unique_ptr<Light> pl2(p2);
//    l_list.push_back(std::move(pl2));

//    glm::vec3 l3_trans(2,4.9,0), l3_rot(45,0,0),l3_scale(0.5,0.5,0.5);
//    Transform l3(l3_trans,l3_rot,l3_scale);
//    Light* p3 = new PointLight(glm::vec3(0.25,1,0.25),white,l3);
//    unique_ptr<Light> pl3(p3);
//    l_list.push_back(std::move(pl3));

    //reflective and refractive
    glm::vec3 l4_trans(0,4,5), l4_rot(45,0,0),l4_scale(0.5,0.5,0.5);
    Transform l4(l4_trans,l4_rot,l4_scale);
    Light* p4 = new PointLight(white,white,l4);
    unique_ptr<Light> pl4(p4);
    l_list.push_back(std::move(pl4));


    Scene scene(hw2,p_list,l_list);

    //traverse each pixel to get its color
    for(int x = 0; x < 400; ++x) {
        for(int y = 0; y < 400; ++y) {
            QColor RGB = cal_color(scene,x,y);
            q_image.setPixelColor(x,y,RGB);
        }
    }


    return q_image;
}

//temporary render scene, all black image just for testing
QImage RenderScene()
{
    // TODO: Complete the various components of code that make up this function
    // It should return the rasterized image of the current scene

    // Notice that even though it should return a QImage this function compiles.
    // Remember, C++'s compiler is dumb (though it will at least warn you that
    // the function doesn't return anything).
    // BEWARE! If you use a function that is missing its return statement,
    // it will return garbage memory!


    QImage q_image(SCENE_WIDTH,SCENE_HEIGHT,QImage::Format_RGB32);
    //set color
    q_image.fill(Qt::black); //why can't use QColor

    Point3f eye(0, 0, 10);
    Point3f ref(0, 0, 0);
    Vector3f worldUp(0, 1, 0);
    //initialize scene
    int fieldOfView = 45;
    int width = 400;
    int height = 400;
    float nearClip = 0.1;
    float farClip = 1000;
    Camera test(eye,ref,worldUp,fieldOfView,nearClip,farClip,width,height);

    //initialize Sphere and its primitive
    //t1 for sphere
    //glm::vec3 s_trans(0,0,1), s_rot(0,0,45),s_scale(2,1,1);
    //glm::vec3 s2_trans(0.25,0,0), s2_rot(0,0,0),s2_scale(0.5,0.5,5);
    //glm::vec3 p_trans(0,-0.5,0), p_rot(90,0,0), p_scale(5,5,1);
    //Transform t1(s_trans,s_rot,s_scale), t2(p_trans,p_rot,p_scale), t3(s2_trans,s2_rot,s2_scale);
    Transform t1, t2;
    //Shape* s1 = new Sphere(t1);
    //Shape* s2 = new Sphere(t3);
    Shape* pl1 = new Plane(t2);
    //unique_ptr<Shape> s1(new Sphere(t1));
    //unique_ptr<Primitive> p1(new Primitive("Sphere1",s1));
    //unique_ptr<Primitive> p3(new Primitive("Sphere2",s2));
    //unique_ptr<Primitive> p2(new Primitive("Plane1", pl1));
    vector<unique_ptr<Primitive>> p_list;
    vector<unique_ptr<Light>> l_list;
    //p_list.push_back(std::move(p1));
    //p_list.push_back(std::move(p2));
    //p_list.push_back(std::move(p3));
    Scene scene(test,p_list,l_list);

    //traverse each pixel to get its color
    for(int x = 0; x < 400; ++x) {
        for(int y = 0; y < 400; ++y) {
            QColor RGB = cal_color(scene,x,y);
            q_image.setPixelColor(x,y,RGB);
        }
    }
    //set up color for each pixel
    return q_image;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->actionSave_Image, SIGNAL(clicked(bool)), this, SLOT(on_actionSave_Image_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_Image_triggered()
{
    //first render the image
    rendered_image = RenderScene_hw2();
    //rendered_image = RenderScene();
    DisplayQImage(rendered_image);
    QString filename = QFileDialog::getSaveFileName(0, QString("create Image"), QString("../.."), QString("*.png"));
    QString ext = filename.right(4);
    if(QString::compare(ext, QString(".png")) != 0)
    {
        filename.append(QString(".png"));
    }
    QImageWriter writer(filename);
    writer.setFormat("png");
    if(!writer.write(rendered_image))
    {
        qDebug() << writer.errorString();
    }
}

void MainWindow::DisplayQImage(QImage &i)
{
    QPixmap pixmap(QPixmap::fromImage(i));
    graphics_scene.addPixmap(pixmap);
    graphics_scene.setSceneRect(pixmap.rect());
    ui->scene_display->setScene(&graphics_scene);
}

void MainWindow::on_actionQuit_Esc_triggered()
{
    QApplication::exit();
}

