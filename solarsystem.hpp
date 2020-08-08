//
// solarsystem.hpp
// solarsystem
//
#include <GL/glut.h>

#include "stars.hpp"

constexpr auto STARS_NUM = 10;

class SolarSystem {

public:

    SolarSystem();
    ~SolarSystem();

    void onDisplay();
    void onUpdate();
    void onKeyboard(unsigned char key, int x, int y);

private:
    Star* stars[STARS_NUM];

    // 定义观察视角的参数
    //相当于头（摄像机）在 OpenGL 世界坐标中的坐标位置
    GLdouble viewX, viewY, viewZ;
    //相当于头所看（摄像机所拍）物体的坐标位置
    GLdouble centerX, centerY, centerZ;
    //相当于头顶（摄像机顶部）朝上的方向向量（因为我们可以歪着头观察一个物体）
    GLdouble upX, upY, upZ;
};