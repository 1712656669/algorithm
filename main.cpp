//
//  main.cpp
//  solarsystem
//
// OpenGL（英语：Open Graphics Library，译名：开放图形库或者“开放式图形库”）
// 是用于渲染2D、3D矢量图形的跨语言、跨平台的应用程序编程接口（API）
// GLUT (OpenGL Utility Toolkit) 是一个处理 OpenGL 程序的工具库，主要负责处理与底层操作系统的调用及 I/O 操作
// 使用 GLUT 的基本头文件
#include <GL/glut.h>
#include "solarsystem.hpp"

// 创建图形窗口的基本宏
constexpr auto WINDOW_X_POS = 50;
constexpr auto WINDOW_Y_POS = 50;
constexpr auto WIDTH = 700;
constexpr auto HEIGHT = 700;

SolarSystem solarsystem;

// 用于注册 GLUT 的回调
// 应用程序（application program）会时常通过api调用库里所预先备好的函数
// 但是有些库函数（library function）却要求应用先传给它一个函数，好在合适的时候调用，以完成目标任务
// 这个被传入的、后又被调用的函数就称为回调函数
void onDisplay(void) { //显示刷新
    solarsystem.onDisplay();
}
void onUpdate(void) { //空闲刷新
    solarsystem.onUpdate();
}
void onKeyboard(unsigned char key, int x, int y) { //键盘事件处理
    solarsystem.onKeyboard(key, x, y);
}


int main(int argc, char* argv[]) {
    // argc用来统计程序运行时发送给main函数的命令行参数的个数，在VS中默认值为1
    // argv[]，为字符串数组，用来存放指向的字符串参数的指针数组，每一个元素指向一个参数

    // 对 GLUT 进行初始化，并处理所有的命令行参数
    glutInit(&argc, argv);
    // 这个函数指定了使用 RGBA 模式颜色索引模式。另外还可以
    // 指定是使用单缓冲还是双缓冲窗口。这里我们使用 RGBA 和 双缓冲窗口
    // RGBA中A(Alpha)代表不透明度，完全透明0%，完全不透明100%
    // 单缓冲，是将所有的绘图指令在窗口上执行，直接在窗口上绘图，绘图效率比较慢
    // 双缓冲，是将绘图指令在一个缓冲区完成，绘图非常的快，在绘图指令完成之后，
    // 再通过交换指令把完成的图形立即显示在屏幕上，进而避免出现绘图的不完整，效率高
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    // 设置窗口被创建时左上角位于屏幕上的位置
    glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS);
    // 设置窗口被创建时的宽高, 为了简便起见
    glutInitWindowSize(WIDTH, HEIGHT);
    // 创建一个窗口，输入的字符串为窗口的标题
    glutCreateWindow("SolarSystem");

    // glutDisplayFunc 的函数原型为 glutDisplayFunc(void (*func)(void))
    // 这是一个回调函数，每当 GLUT 确定一个窗口的内容需要更新显示的时候,
    // glutDisplayFunc 注册的回调函数就会被执行.
    //
    // glutIdleFunc(void (*func)(void)) 将指定一个函数，用于处理当事件循环
    // 处于空闲的时候，就执行这个函数。这个回调函数接受一个函数指针作为它的唯一参数
    //
    // glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
    // 会将键盘上的键与一个函数关联，当这个键被按下或者释放时，函数就会调用
    //
    // 因此下面的三行实际上是在向 GLUT 注册关键的三个回调函数
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onUpdate);
    glutKeyboardFunc(onKeyboard);

    // 进入GLUT事件处理循环，让所有与事件有关的函数调用无限循环
    glutMainLoop();
    return 0;

}