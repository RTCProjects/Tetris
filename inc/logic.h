#ifndef _LOGIC_H
#define _LOGIC_H

#include <iostream>
#include "figure.h"

typedef enum
{
    E_LOGIC_STOP = 0,
    E_LOGIC_GENERATE,
    E_LOGIC_GAME,
    E_LOGIC_GAMEOVER
}e_logic_state;

class CLogic
{
public:
    CLogic();
    ~CLogic();

    static uint8_t glass_x_size;
    static uint8_t glass_y_size;

    int GetGlassValue(uint8_t x,uint8_t y) const;
    void update(void);
    int  MoveRight(void);
    int  MoveLeft(void);
    int  Rotate(void);
    void MoveDown(void);
    void Start(void);
    //getters
    e_logic_state GetLogicState(void) const { return logic_state; }
    
private:
    void Copy(uint8_t **src, uint8_t **dst);   
    void ClearAll();
    int  LeftCollision();
    int  RightCollision(); 
    int  TryGameOver(void);
    int  RotateCollision(void);
    void ClearLine();

    e_logic_state   logic_state;
    uint8_t         **glass;
    uint8_t         **prev_glass;



    CFigure *figure;
};

#endif