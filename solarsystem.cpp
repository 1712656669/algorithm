//
// solarsystem.cpp
// solarsystem
//

#include "solarsystem.hpp"

constexpr auto REST = 700;
#define REST_Y (-REST)
#define REST_Z (REST)

void SolarSystem::onDisplay() {

    // 清除 viewport 缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 清空并设置颜色缓存
    glClearColor(.7f, .7f, .7f, .1f);

    // 指定当前矩阵为投影矩阵
    // 这个函数接受三个不同的模式：GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理
    // GL_PROJECTION 会向 OpenGL 声明将进行投影操作，会把物体投影到一个平面上
    // 开启这个模式后要使用glLoadIdentity()把矩阵设置为单位矩阵，而后的操作如可以通过 gluPerspective 设置视景
    // GL_MODELVIEW 会向 OpenGL 声明接下来的语句将描绘一个以模型为基础的操作
    // 比如设置摄像机视角，开启后同样需要设置 OpenGL 的矩阵模式为单位矩阵
    // GL_TEXTURE 则是进行纹理相关的操作

    glMatrixMode(GL_PROJECTION);

    // 将指定的矩阵指定为单位矩阵
    glLoadIdentity();

    // 指定当前的观察视景体
    // 这个函数会创建一个对称的透视型视景体，在使用这个函数前需要将 OpenGL 的矩阵模式设置为 GL_PROJECTION
    // 在窗口中的画面是通过摄像机来捕捉的，捕捉的实际内容就是远平面上的内容
    // 显示的内容则是近平面上的内容，所以，这个函数需要四个参数：
    // 1.第一个参数为视角的大小
    // 2.第二个参数为实际窗口的横纵比，如图中 aspect=w/h
    // 3.第三个参数为近平面距离
    // 4.第四个参数则为远平面距离

    gluPerspective(75.0f, 1.0f, 1.0f, 40000000);

    // 指定当前矩阵为视景矩阵堆栈应用后的矩阵操作
    glMatrixMode(GL_MODELVIEW);
    // 指定当前的矩阵为单位矩阵
    glLoadIdentity();
    // 定义视图矩阵，并与当前矩阵相乘
    gluLookAt(viewX, viewY, viewZ, centerX, centerY, centerZ, upX, upY, upZ);

    // 设置第一个光源(0号光源)
    glEnable(GL_LIGHT0);
    // 启用光源
    glEnable(GL_LIGHTING);
    // 启用深度测试，根据坐标的远近自动隐藏被遮住的图形
    glEnable(GL_DEPTH_TEST);

    // 绘制星球
    for (int i = 0; i < STARS_NUM; i++)
        stars[i]->draw();

    // 我们在 main 函数中初始化显示模式时使用了 GLUT_DOUBLE
    // 需要使用 glutSwapBuffers 在绘制结束后实现双缓冲的缓冲区交换
    glutSwapBuffers();
}

constexpr auto TIMEPAST = 1; // 假设每次更新都经过了一天
void SolarSystem::onUpdate() {

    for (int i = 0; i < STARS_NUM; i++)
        stars[i]->update(TIMEPAST); // 更新星球的位置

    this->onDisplay(); // 刷新显示
}

constexpr auto OFFSET = 20;
void SolarSystem::onKeyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 'w': viewY += OFFSET; break; // 摄像机Y 轴位置增加 OFFSET
        case 's': viewZ += OFFSET; break;
        case 'S': viewZ -= OFFSET; break;
        case 'a': viewX -= OFFSET; break;
        case 'd': viewX += OFFSET; break;
        case 'x': viewY -= OFFSET; break;
        case 'r':
            viewX = 0; viewY = REST_Y; viewZ = REST_Z;
            centerX = centerY = centerZ = 0;
            upX = upY = 0; upZ = 1;
            break;
        default: break;
    }

}

// 星球半径
constexpr auto SUN_RADIUS = 48.74;
constexpr auto MER_RADIUS = 7.32;
constexpr auto VEN_RADIUS = 18.15;
constexpr auto EAR_RADIUS = 19.13;
constexpr auto MOO_RADIUS = 6.15;
constexpr auto MAR_RADIUS = 10.19;
constexpr auto JUP_RADIUS = 42.90;
constexpr auto SAT_RADIUS = 36.16;
constexpr auto URA_RADIUS = 25.56;
constexpr auto NEP_RADIUS = 24.78;

// 距太阳的距离
constexpr auto MER_DIS = 62.06;
constexpr auto VEN_DIS = 115.56;
constexpr auto EAR_DIS = 168.00;
constexpr auto MOO_DIS = 26.01;
constexpr auto MAR_DIS = 228.00;
constexpr auto JUP_DIS = 333.40;
constexpr auto SAT_DIS = 428.10;
constexpr auto URA_DIS = 848.00;
constexpr auto NEP_DIS = 949.10;

// 公转周期（天）
constexpr auto MER_SPEED = 87.0;
constexpr auto VEN_SPEED = 225.0;
constexpr auto EAR_SPEED = 365.0;
constexpr auto MOO_SPEED = 30.0;
constexpr auto MAR_SPEED = 687.0;
constexpr auto JUP_SPEED = 1298.4;
constexpr auto SAT_SPEED = 3225.6;
constexpr auto URA_SPEED = 3066.4;
constexpr auto NEP_SPEED = 6014.8;

// 自转速度
constexpr auto SELFROTATE = 3;

enum STARS {
    Sun,        // 太阳
    Mercury,    // 水星
    Venus,      // 金星
    Earth,      // 地球
    Moon,       // 月球
    Mars,       // 火星
    Jupiter,    // 木星
    Saturn,     // 土星
    Uranus,     // 天王星
    Neptune     // 海王星
};

// 为了方便操作数组，定义一个设置多位数组的宏
#define SET_VALUE_3(name, value0, value1, value2) \
                   ((name)[0])=(value0), ((name)[1])=(value1), ((name)[2])=(value2)


SolarSystem::SolarSystem() {

    // 定义视角
    //初始视角在 (x, -x, x) 处
    viewX = 0;
    viewY = REST_Y;
    viewZ = REST_Z;
    //所观察物体（太阳）的位置在 (0,0,0)
    centerX = centerY = centerZ = 0;
    upX = upY = 0;
    upZ = 1;

    // 太阳
    GLfloat rgbColor[3] = { 1, 0, 0 };
    stars[Sun] = new LightPlanet(SUN_RADIUS, 0, 0, SELFROTATE, 0, rgbColor);
    // 水星
    SET_VALUE_3(rgbColor, .2, .2, .5);
    stars[Mercury] = new Planet(MER_RADIUS, MER_DIS, MER_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 金星
    SET_VALUE_3(rgbColor, 1, .7, 0);
    stars[Venus] = new Planet(VEN_RADIUS, VEN_DIS, VEN_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 地球
    SET_VALUE_3(rgbColor, 0, 1, 0);
    stars[Earth] = new Planet(EAR_RADIUS, EAR_DIS, EAR_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 月亮
    SET_VALUE_3(rgbColor, 1, 1, 0);
    stars[Moon] = new Planet(MOO_RADIUS, MOO_DIS, MOO_SPEED, SELFROTATE, stars[Earth], rgbColor);
    // 火星
    SET_VALUE_3(rgbColor, 1, .5, .5);
    stars[Mars] = new Planet(MAR_RADIUS, MAR_DIS, MAR_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 木星
    SET_VALUE_3(rgbColor, 1, 1, .5);
    stars[Jupiter] = new Planet(JUP_RADIUS, JUP_DIS, JUP_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 土星
    SET_VALUE_3(rgbColor, .5, 1, .5);
    stars[Saturn] = new Planet(SAT_RADIUS, SAT_DIS, SAT_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 天王星
    SET_VALUE_3(rgbColor, .4, .4, .4);
    stars[Uranus] = new Planet(URA_RADIUS, URA_DIS, URA_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 海王星
    SET_VALUE_3(rgbColor, .5, .5, 1);
    stars[Neptune] = new Planet(NEP_RADIUS, NEP_DIS, NEP_SPEED, SELFROTATE, stars[Sun], rgbColor);

}

SolarSystem::~SolarSystem() {
    for (int i = 0; i < STARS_NUM; i++)
        delete stars[i];
}