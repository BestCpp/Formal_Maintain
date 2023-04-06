
#include <cstdio>
#include <thread>
#include <mutex>
using namespace std;
#include <windows.h>
#include <easyx.h>


void Rotate()//旋转
{
    settextcolor(WHITE);
    char buffer[4] = { '|','/','-','\\' };//注意转义字符

    int index = 0;
    while (true)
    {
        char puts[2] = { 0 };
        puts[0] = buffer[index++];
        outtextxy(10, 10, puts);
        index %= 4;
        Sleep(100);
    }

}

void Progress()//进度条
{
    while (true)
    {
        char buffer[102] = { 0 };
        int cnt = 0;
        while (cnt <= 100)
        {
            outtextxy(10, 40, buffer);
            fflush(stdout);
            buffer[cnt++] = '#';
            Sleep(50);
        }
        cleardevice();
    }

}

int main()
{
    initgraph(1024, 480);
    /*   Rotate();
       Progress();*/
    thread t1(Rotate);
    //Sleep(2);
    thread t2(Progress);

    t1.join();
    t2.join();
    while (true);
    return 0;
}