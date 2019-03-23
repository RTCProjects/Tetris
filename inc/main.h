#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <thread>
#include <mutex>
#include <gtk/gtk.h>

#include "logic.h"

class CGame
{
public:
    //Constructor
    CGame();
    //Destructor
    ~CGame();
    //Method-s
    void        Draw(cairo_t *cr);
    void        Update();
    GtkWidget   *CreateWindow(void);
    void        MoveRight(void);
    void        MoveLeft(void);
    void        MoveDown(void);
    void        Rotate(void);
    void        Start(void);
    //Getters
    uint16_t getticktime(void) const { return ticktime; }
    GtkWidget *getwindow(void) const { return window; }
    //Setters
    void setyicktime(uint16_t value) { ticktime = value; }
    static void gameloop(void *args);
    
    
private:
    
    //Values
    uint16_t ticktime;
    //GTK
    GtkWidget   *window;
    GtkWidget   *canvas;
    GtkWidget   *gameoverdlg;
    GtkBuilder  *builder;
    GError      *error;
    //Application
    CLogic      *gamelogic;
    //Threads
    std::thread gamethread;
    std::mutex mtx; 


    
};

#endif