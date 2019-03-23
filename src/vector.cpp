#include "vector.h"

CVector::CVector(int _x, int _y) 
{
    this->x = _x;
    this->y = _y;
}

void CVector::Set(int _x, int _y) 
{
    this->x = _x;
    this->y = _y;
}

CVector CVector::Get()
{
    return CVector(this->x, this->y);
}

int CVector::GetX()
{
    return x;
}

int CVector::GetY()
{
    return y;
}

CVector& CVector::operator=(const CVector &vector)
{
    if(this == &vector)
        return *this;

    this->x = vector.x;
    this->y = vector.y;

    return *this;
}