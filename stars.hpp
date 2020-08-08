//
//  stars.hpp
//  solarsystem
//
//hpp,其实质就是将.cpp的实现代码混入.h头文件当中，定义与实现都包含在同一文件，
//则该类的调用者只需要include该hpp文件即可，无需再将cpp加入到project中进行编译
//
//#ifndef是if not define的简写,是一种宏定义判断，作用是防止多重定义
//在头文件中使用，防止头文件被多重调用
//比如你有两个C文件，这两个C文件都include了同一个头文件
//而编译时，这两个C文件要一同编译成一个可运行文件，于是问题来了，大量的声明冲突
#ifndef stars_hpp //先测试stars_hpp是否被宏定义过
#define stars_hpp //如果stars_hpp没有被宏定义过，定义stars_hpp，并编译程序段

#include <GL/glut.h>

//为了编程实现上的方便，对现实世界的实际编程模型做一些前提假设:
//1.星球的运行轨道为圆形
//2.自转速度保持相同
//3.每次刷新绘制的时候假设时间经过了一天

//普通能够自转并绕某个点公转的星球(Star)
class Star {
public:
    // 星球的半径
    GLfloat radius;
    // 星球的公转、自传速度
    GLfloat speed, selfSpeed;
    // 星球的中心与父节点星球中心的距离
    GLfloat distance;
    // 星球的颜色
    GLfloat rgbaColor[4] = { 1.0f,1.0f,1.0f,1.0f };

    // 父节点星球
    Star* parentStar;

    // 构造函数，构造一颗星球时必须提供
    // 星球半径、与父节点星球的距离、公转速度、自转速度、绕行(父节点)星球
    Star(GLfloat radius, GLfloat distance,
        GLfloat speed, GLfloat selfSpeed,
        Star* parentStar, GLfloat rgbColor[3]);
    // 对一般的星球的移动、旋转等活动进行绘制
    void drawStar();
    // 虚函数(virtual)是指一个类中你希望重载的成员函数 ，当你用一个基类指针或引用
    // 指向一个继承类对象的时候，调用一个虚函数时, 实际调用的是继承类的版本
    // 提供默认实现，负责调用 drawStar()
    virtual void draw() { drawStar(); }
    // 参数为每次刷新画面时的时间跨度
    virtual void update(long timeSpan);
protected:
    GLfloat alphaSelf; //自转角度
    GLfloat alpha; //公转角度
};

// 具有特殊材质的星球(Planet)
class Planet : public Star {
public:
    // 构造函数
    Planet(GLfloat radius, GLfloat distance,
        GLfloat speed, GLfloat selfSpeed,
        Star* parentStar, GLfloat rgbColor[3]);
    // 增加对具备自身材质的行星绘制材质
    void drawPlanet();
    // 继续向其子类开放重写功能
    virtual void draw() { drawPlanet(); drawStar(); }
};

//能够发光的星球(LightPlanet)
class LightPlanet : public Planet {
public:
    LightPlanet(GLfloat Radius, GLfloat Distance,
        GLfloat Speed, GLfloat SelfSpeed,
        Star* ParentStar, GLfloat rgbColor[3]);
    // 增加对提供光源的恒星绘制光照
    void drawLight();
    virtual void draw() { drawLight(); drawPlanet(); drawStar(); }
};

#endif /* star_hpp */