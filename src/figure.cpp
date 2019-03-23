#include "figure.h"
#include "logic.h"

static const uint8_t figures_max = 16;
static const uint8_t colors_max = 7;

t_figure_data   figures_data[figures_max] = {
    {//1
        CVector(1,1),
        E_NON_ROTATEBLE,
        E_TYPE_POINT,
        {{1},{1}}
    },
    {//2
        CVector(2,2),
        E_NON_ROTATEBLE,
        E_TYPE_RECT,
        {{1,1},{1,1}}
    },
    {//3
        CVector(3,2),
        E_ANGLE_0,
        E_TYPE_Z,
        {{1,0},
         {1,1},
         {0,1}}
    },
    {//4
        CVector(2,3),
        E_ANGLE_90,
        E_TYPE_Z,
        {{1,1,0},
         {0,1,1}}
    },
    {//5
        CVector(3,2),
        E_ANGLE_180,
        E_TYPE_Z,
        {{0,1},
         {1,1},
         {1,0}}
    },
    {//6
        CVector(2,3),
        E_ANGLE_270,
        E_TYPE_Z,
        {{0,1,1},
         {1,1,0}}
    },
    {//7
        CVector(1,4),
        E_ANGLE_0,
        E_TYPE_LINE,
        {{1,1,1,1}}
    },
    {//8
        CVector(4,1),
        E_ANGLE_90,
        E_TYPE_LINE,
        {{1},
         {1},
         {1},
         {1}}
    },
    {//9
        CVector(3,2),
        E_ANGLE_0,
        E_TYPE_L,
        {{1,0},
         {1,0},
         {1,1}}
    },
    {//10
        CVector(2,3),
        E_ANGLE_90,
        E_TYPE_L,
        {{1,1,1},
         {1,0,0}}
    },
    {//11
        CVector(3,2),
        E_ANGLE_180,
        E_TYPE_L,
        {{1,1},
         {0,1},
         {0,1}}
    },
    {//12
        CVector(2,3),
        E_ANGLE_270,
        E_TYPE_L,
        {{0,0,1},
         {1,1,1}}
    },
    {//13
        CVector(3,2),
        E_ANGLE_0,
        E_TYPE_T,
        {{1,0},
         {1,1},
         {1,0}}
    },
    {//14
        CVector(2,3),
        E_ANGLE_90,
        E_TYPE_T,
        {{1,1,1},
         {0,1,0}}
    },
    {//15
        CVector(3,2),
        E_ANGLE_180,
        E_TYPE_T,
        {{0,1},
         {1,1},
         {0,1}}
    },
    {//16
        CVector(2,3),
        E_ANGLE_270,
        E_TYPE_T,
        {{0,1,0},
         {1,1,1}}
    }
    
};

CFigure::CFigure()
{
    v_pos.Set(0,0);
    v_size.Set(0,0);

    angle = E_ANGLE_0;
    type = E_TYPE_POINT;

    rnd_color = 0;
    rnd_index = 0;
}

CFigure::~CFigure()
{
    
}

void CFigure::MoveLeft()
{
    v_pos.Set(v_pos.GetX() - 1, v_pos.GetY());
}

void CFigure::MoveRight()
{
    v_pos.Set(v_pos.GetX() + 1, v_pos.GetY());
}

void CFigure::MoveDown()
{
    v_pos.Set(v_pos.GetX(),v_pos.GetY() + 1);
}

void CFigure::Rotate()
{
    /*
    Получить текущий тип фигуры и её угол
    Найти следующую фигуру такого же типа
    */
    uint8_t current_id = rnd_index; 
    uint8_t current_col = rnd_color;
    CVector current_pos = v_pos;

    //не вращаем симметричные фигуры
    if(figures_data[current_id].size.GetX() == figures_data[current_id].size.GetY())
        return;

    while(1) {
        if(figures_data[current_id].type == type && figures_data[current_id].angle != angle){
            
            BuildFigureByIndex(current_id);

            rnd_index = current_id;
            rnd_color = current_col;
            v_pos = current_pos;

            break;
        }

        current_id++;
        if(current_id >= figures_max)
            current_id = 0;
    }
}

uint8_t CFigure::GetRndValue(uint8_t from, uint8_t to)
{
    uint8_t result = 0;

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator {seed};

    std::uniform_int_distribution<int> uint_color10(from, to);
    result = uint_color10(generator);  

    return result; 
}

void CFigure::BuildFigureByIndex(uint8_t index)
{   
    t_figure_data fig_data = figures_data[index];

    //clear memory
    for(int i = 0; i < BLOCK_SIZE; i++)
        for(int j = 0; j < BLOCK_SIZE; j++)
            data[i][j] = 0;

        //copy data
    for(int i = 0; i < fig_data.size.GetX(); i++) {
        for(int j = 0; j < fig_data.size.GetY(); j++) {
            data[i][j] = fig_data.data[i][j];
            if(data[i][j])
                data[i][j] = rnd_color;
        }
    }

    v_size  = fig_data.size;
    type    = fig_data.type;
    angle   = fig_data.angle;

    v_pos = CVector(CLogic::glass_x_size / 2 - v_size.GetX() / 2,-v_size.GetY() );     
}

void CFigure::Generate()    
{
    rnd_index = 0;
    rnd_index = GetRndValue(1, figures_max) - 1;

    rnd_color = 0;
    rnd_color = GetRndValue(1, colors_max); 

    BuildFigureByIndex(rnd_index);    
}