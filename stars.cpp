//
//  stars.cpp
//  solarsystem
//

#include "stars.hpp"
#include <cmath>

constexpr auto PI = 3.1415926536;

Star::Star(GLfloat radius, GLfloat distance,
    GLfloat speed, GLfloat selfSpeed,
    Star* parent, GLfloat rgbColor[3]) {
    this->radius = radius;
    this->selfSpeed = selfSpeed;
    this->alphaSelf = this->alpha = 0;
    this->distance = distance;

    rgbaColor[0] = rgbColor[0];
    rgbaColor[1] = rgbColor[1];
    rgbaColor[2] = rgbColor[2];
 
    // for (int i = 0; i < 4; i++)
    //     this->rgbaColor[i] = 1.0f;

    this->parentStar = parent;
    if (speed > 0)
        this->speed = 360.0f / speed; //在初始化速度变量时将其转化为角速度，角速度（度/天） = 360/公转周期（天）
    else
        this->speed = 0.0f;
}

//行星的绘制
void Star::update(long timeSpan) {
    alpha += timeSpan * speed;  // 更新公转角度
    alphaSelf += selfSpeed;     // 更新自转角度
}

void Star::drawStar() {

    //glEnable函数用于激活 OpenGL 中提供的各种功能，
    //传入的参数是OpenGL 内部的宏，提供诸如光源、雾化、抖动等效果
    glEnable(GL_LINE_SMOOTH); // 执行后，过虑线段的锯齿
    glEnable(GL_BLEND); // 启用颜色混合,例如实现半透明效果

    int n = 1440;

    // 保存 OpenGL 当前的矩阵环境
    glPushMatrix();
    {
        // 公转

        // 如果是行星，且距离不为0，那么向原点平移一个半径
        // 这部分用于处理卫星
        if (parentStar != 0 && parentStar->distance > 0) {
            //将绘制的图形沿 z 轴旋转 alpha
            glRotatef(parentStar->alpha, 0, 0, 1);
            // x 轴方向上平移 distance, y, z 方向不变
            glTranslatef(parentStar->distance, 0.0, 0.0);
            //将绘制的图形沿 z 轴旋转 alpha
        }
        // 绘制运行轨道
        // glBegin() 和 glEnd()：当我们要进行图形绘制时，需要在开始绘制前和绘制完成后调用这两个函数
        // glBegin()指定了绘制图形的类型，例如 GL_POINTS 表示绘制点、 GL_LINES 表示绘制依次画出的点及他们之间的连线、
        // GL_TRIANGLES 则是在每绘制的三个点中完成一个三角形、GL_POLYGON 则是绘制一个从第一个点到第 n 个点的多边形
        glBegin(GL_LINES);
        //绘制圆时用多边形来模拟
        for (int i = 0; i < n; ++i)
            // glvertex--OpenGL中一系列画点函数共通名
            // 2--函数有两个参数
            // f--两个参数都是浮点型
            glVertex2f(distance * cos(2 * PI * i / n),
                       distance * sin(2 * PI * i / n));
        glEnd();

        // 绕 z 轴旋转 alpha
        glRotatef(alpha, 0, 0, 1);
        // x 轴方向平移 distance, y, z 方向不变
        glTranslatef(distance, 0.0, 0.0);

        // 自转
        glRotatef(alphaSelf, 0, 0, 1);


        // 绘制行星颜色
        glColor3f(rgbaColor[0], rgbaColor[1], rgbaColor[2]);
        glutSolidSphere(radius, 40, 32); //绘制球体，星球半径、经线条数、纬线条数
    }
    // 恢复绘制前的矩阵环境
    glPopMatrix();

}

Planet::Planet(GLfloat radius, GLfloat distance,
    GLfloat speed, GLfloat selfSpeed,
    Star* parent, GLfloat rgbColor[3]) :
    Star(radius, distance, speed, selfSpeed, parent, rgbColor) {
    ;
}

void Planet::drawPlanet() {

    // 设置一个物体的材质一般有五个属性需要设置：
    // 1.多次反射后追踪在环境中遗留的光照强度;
    // 2.漫反射后的光照强度;
    // 3.镜面反射后的光照强度;
    // 4.OpenGL 中不发光物体发出的微弱且不影像其他物体的光照强度;
    // 5.镜面指数，指越小，表示材质越粗糙，点光源发射的光线照射后，产生较大的亮点；相反，值越大，材质越像镜面，产生较小的亮点。

    // 设置材质 OpenGL 提供了两个版本的函数：
    // void glMaterialf(GLenum face, GLenum pname, TYPE param);
    // void glMaterialfv(GLenum face, GLenum pname, TYPE * param);
    // 其差异在于，镜面指数只需要设置一个数值，这时只需要使用 glMaterialf；
    // 而其他的材质设置都需要设置多个值，这是需要使用数组进行设置，使用带指针向量参数的版本 glMaterialfv

    GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    GLfloat mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat mat_emission[] = { rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3] };
    GLfloat mat_shininess = 90.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); // GL_FRONT表示显示模式将适用于物体的前向面（也就是物体能看到的面）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

LightPlanet::LightPlanet(GLfloat radius, GLfloat distance, GLfloat speed,
    GLfloat selfSpeed, Star* parentStar, GLfloat rgbColor[3]) :
    Planet(radius, distance, speed, selfSpeed, parentStar, rgbColor) {
    ;
}

void LightPlanet::drawLight() {

    // 这里的位置由四个值来表示，(x,y,z,w) 
    // 其中当 w为0时，表示该光源位于无限远，而 x,y,z 便指定了这个无限远光源的方向
    // 当 w 不为0时，表示 位置性光源，其位置为 (x/w, y/w, z/w)
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 指定零号光源的位置
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);  // 表示各种光线照射到该材质上，经过很多次反射后追踪遗留在环境中的光线强度
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);  // 漫反射后的光照强度
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); // 镜面反射后的光照强度

}