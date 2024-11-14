#pragma once//防止头文件重复包含
#include<iostream>
using namespace std;
//设置中只要声明不要实践，函数声明和变量声明
class Point
{
    public:
        void setX(int x);
        int getX();
        void setY(int y);
        int getY();
    private:
        int m_X;
        int m_Y;
};

class Circle
{
public:
    void setR(int r);
    int getR();
    void setCenter(Point center);
    Point getCenter();
private:
    int m_R;
    Point m_Center;


};