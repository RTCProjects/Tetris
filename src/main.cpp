#include "main.h"

static cairo_surface_t *surface = NULL;

CGame   *Game = NULL;

//Callback-s
extern "C"
{
    void
    destroy_callback (GtkWidget *object,
                     gpointer   user_data)
    {
        //CGame destructor call
        delete Game;    
        //Destroy GTK application
        gtk_main_quit();
    }

    gboolean
    startbutton_click(GtkButton *button,
                     gpointer   user_data)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
        
        Game->Start();

        return FALSE;
    }

    gboolean
    restartbutton_click(GtkButton *button,
                        gpointer   user_data)
    {   
        gtk_widget_hide(GTK_WIDGET(user_data));

        Game->Start();
        return FALSE;
    }
    

    gboolean
    exitbutton_click(GtkButton *button,
                     gpointer   user_data)
    {
        gint response = gtk_dialog_run (GTK_DIALOG(user_data));
        switch (response)
        {
            case GTK_RESPONSE_OK:
                gtk_widget_destroy(Game->getwindow());
                break;
        
            default:
                gtk_widget_hide(GTK_WIDGET(user_data));
                break;
        }
        return FALSE;
    }

    gboolean
    canvas_draw (GtkWidget    *widget,
                cairo_t *cr,
                gpointer      user_data)
    {
        Game->Draw(cr);
        return FALSE;
    }

    gboolean
    keyboard_input (GtkWidget *widget,
                    GdkEventKey  *event,
                    gpointer   user_data)
    {
        if(event->keyval == GDK_KEY_space || event->keyval == GDK_KEY_Up) {
            Game->Rotate();
            return TRUE;
        }
        if(event->keyval == GDK_KEY_Left) {
            Game->MoveLeft();
            return TRUE;
        }
        if(event->keyval == GDK_KEY_Right) {
            Game->MoveRight();
            return TRUE;
        }
        if(event->keyval == GDK_KEY_Down) {
            Game->MoveDown();
            return TRUE;
        }

        return FALSE;
    }

    gboolean
    canvas_configure (GtkWidget *widget,
                        GdkEvent  *event,
                        gpointer   user_data)
    {
        GtkAllocation allocation;

        if (surface)
            cairo_surface_destroy (surface);

        gtk_widget_get_allocation (widget, &allocation);
        surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                    CAIRO_CONTENT_COLOR,
                                                    allocation.width,
                                                    allocation.height);

        return TRUE;
    }
}

GtkWidget*
CGame::CreateWindow()
{
        error = NULL;

        /* тут загружаем файл с интерфейсом */
        builder = gtk_builder_new ();


        if (!gtk_builder_add_from_file (builder, "GladeUISrc//BasicTemplate.glade", &error))
        {
                /* загрузить файл не удалось */
                g_critical ("Не могу загрузить файл: %s", error->message);
                g_error_free (error);
        }
        
        /* помните, мы подключали сигналы вручную? теперь это происходит автоматически! */
        gtk_builder_connect_signals (builder, NULL);

        /* получаем виджет окна, чтобы его показать */
        window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
        if (!window)
        {
                /* что-то не так, наверное, ошиблись в имени */
                g_critical ("Ошибка при получении виджета окна");
                return NULL;
        }

        canvas = GTK_WIDGET (gtk_builder_get_object(builder, "maincanvas"));
        if(!canvas)
        {
            g_critical ("Ошибка при получении виджета канвы");
            return NULL;
        }

        gameoverdlg = GTK_WIDGET (gtk_builder_get_object(builder, "gameoverdialog"));
        if(!gameoverdlg)
        {
            g_critical("Ошибка при получении виджета диалога gameoverdialog");
            return NULL;
        }

        g_object_unref (builder);

        return window;
} 

CGame::CGame() : ticktime(250)
{
    gamelogic = new CLogic();

    gamethread = std::thread(gameloop,this);
    gamethread.detach();
}

void 
CGame::gameloop(void *args)
{
    CGame *current_game = (CGame*)args;

    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(current_game->ticktime));
        current_game->Update();
    }
}

void 
CGame::Draw(cairo_t *cr)
{
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_paint (cr);
    cairo_set_line_width(cr, 1);

    for(int i = 0; i < CLogic::glass_x_size; i++) {
        for(int j = 0; j<CLogic::glass_y_size; j++) {

            cairo_rectangle(cr,5 + i * 20,5 + j * 20, 20 , 20);
            cairo_set_source_rgb(cr, 1, 1, 1);
            cairo_stroke_preserve(cr);
            
            switch(gamelogic->GetGlassValue(i,j))
            {
                case 0: cairo_set_source_rgb(cr, 0, 0, 0); break;
                case 1: cairo_set_source_rgb(cr, 0.1, 0.1, 0.8); break;
                case 2: cairo_set_source_rgb(cr, 0.1, 0.8, 0.1); break;
                case 3: cairo_set_source_rgb(cr, 0.1, 0.8, 0.8); break;
                case 4: cairo_set_source_rgb(cr, 0.8, 0.1, 0.1); break;
                case 5: cairo_set_source_rgb(cr, 0.8, 0.1, 0.8); break;
                case 6: cairo_set_source_rgb(cr, 0.8, 0.8, 0.1); break;
                case 7: cairo_set_source_rgb(cr, 0.8, 0.8, 0.8); break;                   
            }      

            cairo_fill(cr);
        }
    }

            //check state
        if(gamelogic->GetLogicState() == E_LOGIC_GAMEOVER) {
            gint response = gtk_dialog_run(GTK_DIALOG(gameoverdlg));

            switch (response)
            {
                case GTK_RESPONSE_OK:
                    gtk_widget_hide(GTK_WIDGET(gameoverdlg));
                    break;
            
                default:
                    break;
            }
        }
}

void 
CGame::Update()
{
    //mtx.lock();
        //update gamelogic
        gamelogic->update();
        //update canvas
        gtk_widget_queue_draw(GTK_WIDGET(canvas));
    //mtx.unlock();
}

void
CGame::MoveRight()
{    
    if(gamelogic->MoveRight() != -1) 
        Update();   //Update game
}

void
CGame::MoveLeft()
{   
    if(gamelogic->MoveLeft() != -1)
        Update();   //Update game
}

void
CGame::MoveDown()
{
    gamelogic->MoveDown();
    Update();   //Update game
}

void
CGame::Rotate()
{
    gamelogic->Rotate();
    Update();
}
void
CGame::Start()
{
    gamelogic->Start();
}

CGame::~CGame()
{
    delete gamelogic;
}

int 
main(int argc,char *argv[])
{
    Game = new CGame();
    /* запускаем GTK+ */
    gtk_init (&argc, &argv);
    /* вызываем нашу функцию для создания окна */
    gtk_widget_show (Game->CreateWindow());
    /* передаём управление GTK+ */
    gtk_main ();
    
    return 0;
}