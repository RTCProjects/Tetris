#include "logic.h"

uint8_t CLogic::glass_x_size = 12;
uint8_t CLogic::glass_y_size = 26;

CLogic::CLogic()
{
    //figure generator
    figure = new CFigure();

    //create game map
    glass = new uint8_t*[this->glass_x_size];
    prev_glass = new uint8_t*[this->glass_y_size];

    for(int i = 0;i<this->glass_x_size;i++) {
        glass[i] = new uint8_t[this->glass_y_size];
        prev_glass[i] = new uint8_t[this->glass_y_size];
    }
    logic_state = E_LOGIC_STOP;
}

void CLogic::Copy(uint8_t **src, uint8_t **dst)
{
    for(int i = 0; i < CLogic::glass_x_size; i++)
        for(int j = 0; j < CLogic::glass_y_size; j++)
            dst[i][j] = src[i][j];
}

int CLogic::GetGlassValue(uint8_t x,uint8_t y) const
{
    if(x >= glass_x_size || y >= glass_y_size)
        return -1;
    
    return glass[x][y];
}

void CLogic::update()
{
    /*
    1. Генерируем новую фигуру
    2. Начинаем скролить фигуру вниз
    3. Каждыйй тик скролла проверяем на коллизии
    */

   switch(logic_state)
   {
       case E_LOGIC_GENERATE:
       {
           if(TryGameOver() < 0) {
               logic_state = E_LOGIC_GAMEOVER;
               break;
           }
           ClearLine();
           Copy(glass,prev_glass);

           figure->Generate();
            
           logic_state = E_LOGIC_GAME;
       }break;

       case E_LOGIC_GAME:
       {
           //фиксируем текущее состояние поля
           Copy(prev_glass, glass);
           
            for(int i = 0;i<figure->GetSize().GetX();i++) {
                for(int j = 0;j<figure->GetSize().GetY();j++) {
                    int pixel_x = figure->GetPosition().GetX() + i;
                    int pixel_y = figure->GetPosition().GetY() + j;
                    
                    if(pixel_y < 0)
                        continue;

                    if(figure->GetData(i, j) == 0)
                        continue;
                    
                    glass[pixel_x][pixel_y] = figure->GetData(i, j);
                }
            }

            //проверяем коллизию с дном
            if(figure->GetPosition().GetY() >= CLogic::glass_y_size - figure->GetSize().GetY()) {
                logic_state = E_LOGIC_GENERATE;
                break;
            }

            int fig_posx = figure->GetPosition().GetX();
            int fig_posy = figure->GetPosition().GetY();
            int fig_sizex = figure->GetSize().GetX();
            int fig_sizey = figure->GetSize().GetY();
            //поиск коллизий

            int find_j = 0;


            /*
            Производим обход массива данных фигуры:
                - фиксируем столбец
                - идем по столбцу снизу фигуры вверх
                - если нашли заполненный элемент в матрице фигуры
                - то фиксируем его местоположение и переходим к следующему столбцу

                - сразу смотрим на фигуры в банке, на пиксель вниз по горизонтали
                - если есть заполнение в банке - то фиксируем состояние и генерируем новую фигуру 
            */
            for(int i = 0;i < fig_sizex; i++) {
                find_j = 0;
                for(int j = fig_sizey - 1;j>=0; j--) {
                    if(figure->GetData(i,j)){      
                        if(find_j)
                            continue;              
                        if(glass[fig_posx + i][fig_posy + j + 1]){
                            logic_state = E_LOGIC_GENERATE;
                            break;
                        } 
                        find_j = 1;
                    }
                }
            }

            figure->MoveDown();
           
           /*
            Проверяем коллизию фигуры. Если снизу нет коллизий с цветными
            ячейками в стакане или с дном стакана, то двигаем фигуру
           */
       }break;

       default:
       break;
   }
}

int CLogic::MoveRight()
{
    if(figure->GetPosition().GetX() + figure->GetSize().GetX() >= CLogic::glass_x_size )
        return -1;

    if(RightCollision() < 0)
        return -1;

    figure->MoveRight();

    return 0;
}

int CLogic::MoveLeft()
{
    if(figure->GetPosition().GetX() <= 0 )
        return -1;

    if(LeftCollision() < 0)
        return -1;

    figure->MoveLeft();

    return 0;
}

int CLogic::Rotate()
{
    if(RotateCollision() < 0)
        return -1;

    figure->Rotate();

    return 0;
}

void CLogic::MoveDown()
{
    update();
}

int CLogic::LeftCollision()
{
    if(figure->GetPosition().GetX() == 0)
        return -1;

    for(int i = 0;i < figure->GetSize().GetY(); i++)
    {
        if(figure->GetData(0,i)) {
            if(glass[figure->GetPosition().GetX() - 1][i + figure->GetPosition().GetY()])
                return -1;
        }
    }
    return 0;
}

int CLogic::RightCollision()
{
    if(figure->GetPosition().GetX() + figure->GetSize().GetX() >= CLogic::glass_x_size)
        return -1;

    for(int i = 0;i < figure->GetSize().GetY(); i++)
    {
        if(figure->GetData(figure->GetSize().GetX() - 1,i)) {
            if(glass[figure->GetPosition().GetX() + figure->GetSize().GetX()][i + figure->GetPosition().GetY()])
                return -1;
        }
    }
    return 0;
}

int CLogic::RotateCollision()
{
    /*
        - Получаем копию текущей фигуры (TODO реализовать конструктор копирования)
        - Вращаем копию текущей фигуры
        - Позиционируем повернутую копию относительно позиции вращаемой фигур
        - Выпоняем следующие проверки
            1. Не выходят ли габариты фигуры за пределы стакана
            2. Не происходит ли наложение активных ячеек фигуры на активные ячейки стакана
    */
   CFigure before_rotate;

    before_rotate.BuildFigureByIndex(figure->GetIndex());
    before_rotate.Rotate();

    /*if(before_rotate.GetSize().GetX() + figure->GetPosition().GetX() >= CLogic::glass_x_size)
        return -1;
    */
    for(int i = 0; i < before_rotate.GetSize().GetX(); i++){
        for(int j = 0; j < before_rotate.GetSize().GetY(); j++){
            if(before_rotate.GetData(i,j))
            {
                if(figure->GetPosition().GetX() + i >= CLogic::glass_x_size - 1)
                    return -1;

                if(glass[i + before_rotate.GetPosition().GetX()][j + before_rotate.GetPosition().GetY()])
                    return -1;
            }
        }
    }

   return 0;
}

int CLogic::TryGameOver()
{
    for(int i = 0; i < CLogic::glass_x_size; i++) {
        if(glass[i][0] != 0)
            return -1;
    }
    return 0;
}

void CLogic::ClearLine()
{
    uint8_t block_cnt = 0;
    //перебираем каждую строку сверху стакана вниз
    for(int y = 0; y < CLogic::glass_y_size; y++) {
        block_cnt = 0;  //обнлуяем количество не пустых ячеек в текущей строке

        //получаем общее кол-во заполненных ячеек в строк
        for(int x = 0; x < CLogic::glass_x_size; x++) {
            if(glass[x][y] != 0) 
                block_cnt++;
        }
        //если строка заполнена
        if(block_cnt == CLogic::glass_x_size) {
            //обнуляем заполненную строку
            for(int i = 0;i<CLogic::glass_x_size;i++)
                glass[i][y] = 0;
            //опускаем каждый столбец вниз на одну строку
            for(int i = 0; i < CLogic::glass_x_size; i++) {
                for(int j = y; j >= 0; j--) {
                    glass[i][j] = glass[i][j - 1];
                }
            }
        }
    }
}

void CLogic::Start()
{
    ClearAll();
    logic_state = E_LOGIC_GENERATE;
}

void CLogic::ClearAll()
{
    for(int i = 0; i < CLogic::glass_x_size; i++) {
        for(int j = 0; j < CLogic::glass_y_size; j++) {
            glass[i][j] = 0;
            prev_glass[i][j] = 0;
        }
    }
}

CLogic::~CLogic()
{
    for(int i = 0; i < this->glass_x_size; ++i) {
        delete [] glass[i];
        delete [] prev_glass[i];
    }
    delete [] glass;
    delete [] prev_glass;
    delete figure;
}
