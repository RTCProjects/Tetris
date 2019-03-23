#ifndef _FIGURE_H
#define _FIGURE_H

#include <random>
#include <chrono>
#include "string.h"
#include "vector.h"

#define BLOCK_SIZE  4

typedef enum
{
    E_TYPE_POINT = 0,
    E_TYPE_RECT,
    E_TYPE_LINE,
    E_TYPE_Z,
    E_TYPE_L,
    E_TYPE_T
}e_figtype;

typedef enum
{
    E_NON_ROTATEBLE = 0,
    E_ANGLE_0,
    E_ANGLE_90,
    E_ANGLE_180,
    E_ANGLE_270,   
}e_figangle;

typedef struct
{
    CVector         size;
    e_figangle      angle;
    e_figtype       type;
    uint8_t         data[BLOCK_SIZE][BLOCK_SIZE];
}t_figure_data;

class CFigure
{
public:
    /*
    - тип фигуры
    - размер фигуры
    - блок данных фигуры
    */
    CFigure();
    ~CFigure();

    void    Generate(void);
    void    MoveLeft(void);
    void    MoveRight(void);
    void    MoveDown(void);
    void    Rotate(void);
    void    BuildFigureByIndex(uint8_t index);

    //Getters
    CVector         GetSize()       { return v_size; }
    CVector         GetPosition()   { return v_pos; }
    uint8_t         GetData(uint8_t x, uint8_t y)       { return data[x][y]; }
    uint8_t         GetIndex()      { return rnd_index; }
    

private:
    uint8_t GetRndValue(uint8_t from, uint8_t to);
    //void    BuildFigure(t_figure_data fig_data);
    int     GameOverTry(void);

    CVector v_pos;
    CVector v_size;

    e_figangle  angle;
    e_figtype   type;

    uint8_t rnd_color;
    uint8_t rnd_index;
    uint8_t data[BLOCK_SIZE][BLOCK_SIZE];    
};

#endif