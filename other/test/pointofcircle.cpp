#include<iostream>
using namespace std;
#include"point.h"
void Point::setX(int x)
{
    m_X = x;
}
int Point::getX()
{
    return m_X;
}
void Point::setY(int y)
{
    m_Y = y;
}
int Point::getY()
{
    return m_Y;
}

void Circle::setR(int r)
{
    m_R = r;
}
int Circle::getR()
{
    return m_R;
}
void Circle::setCenter(Point center)
{
    m_Center = center;
}
Point Circle::getCenter()
{
    return m_Center;
}
void isInCircle(Circle &c,Point &p)
{
    int distance = (c.getCenter().getX()-p.getX())*(c.getCenter().getX()-p.getX())+
    (c.getCenter().getY()-p.getY())*(c.getCenter().getY()-p.getY());
    
    int Rdistance = c.getR()*c.getR();

    if(distance == Rdistance)
    {
        cout<<"the point is on the circle"<<endl;
    }
    if(distance > Rdistance)
    {
        cout<<"the point is out of the circle"<<endl;
    }if(distance < Rdistance)
    {
        cout<<"the point is in the circle"<<endl;
    }
    

}

int main()
{
    Circle c;
    c.setR(2);
    //嵌套创建流程如下
    //先创建子类
    Point center;
    center.setX(2);
    center.setY(0);
    //用子类填补母类
    c.setCenter(center);

    Point p;
    p.setX(2);
    p.setY(2);

    isInCircle(c,p);


    system("pause");
    return 0;
}