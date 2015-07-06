#ifndef __POINT_HH__
#define __POINT_HH__

template <typename T>
class Point
{
public:
    T x;
    T y;

public:
    Point(T pX, T pY)
        : x(pX), y(pY)
    { }

    T GetX()
    {
        return x;
    }

    void setX(T pX)
    {
        x = pX;
    }

    T GetY()
    {
        return y;
    }

    void setY(T pY)
    {
        x = pY;
    }
};

#endif // __POINT_HH__