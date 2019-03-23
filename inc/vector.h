#ifndef _VECTOR_H
#define _VECTOR_H

#include <iostream>

class CVector
{
public:
    CVector() : x(0), y(0) {}
    CVector(int _x, int _y);

    void Set(int _x, int _y);
    
    CVector Get();
    int     GetX();
    int     GetY();

    CVector& operator=(const CVector &vector);

private:
    int x, y;
};

#endif