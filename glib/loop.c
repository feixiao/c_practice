#include <glib.h>

// https://www.ibm.com/developerworks/cn/linux/l-glib/
//  gcc `pkg-config --cflags --libs glib-2.0 gthread-2.0` loop.c -o loop

static GMutex *mutex = NULL;
static gboolean t1_end = FALSE;
static gboolean t2_end = FALSE;

typedef struct _Arg {
    GMainLoop* loop;
    gint max;   
}Arg;

void run_1(Arg* arg){
    int i=0;
    for(i=0; i< arg->max; i++) {
        if(g_mutex_trylock(mutex) == FALSE) {
            g_print("%d : thread 2 locked mutex \n", i);
            g_mutex_unlock(mutex);
        }
        else
        {
            g_usleep(10);
        }
    }
    t1_end = TRUE;
}

void    run_2(Arg *arg)
{
    int i;
    for(i=0; i<arg->max; i++)
    {
        if(g_mutex_trylock(mutex) == FALSE)
        {
            g_print("%d : thread 1 locked mutex \n", i);
            g_mutex_unlock(mutex);
        }
        else
        {
            g_usleep(10);
        }
    }
    t2_end = TRUE;
}

// run_3检索前两个线程是否结束，如结束的话，则执行g_main_loop_quit退出事件循环。
void run_3(Arg *arg)
{
    for(;;)
    {
        if(t1_end && t2_end)
        {
            g_main_loop_quit(arg->loop);
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    GMainLoop *mloop;
    Arg *arg;
     
    if(!g_thread_supported())
        g_thread_init(NULL);
    mloop = g_main_loop_new(NULL, FALSE);
     
    arg = g_new(Arg, 1);
    arg->loop = mloop;
    arg->max = 11;
         
    mutex = g_mutex_new();
    g_thread_create(run_1, arg, TRUE, NULL);
    g_thread_create(run_2, arg, TRUE, NULL);
    g_thread_create(run_3, arg, TRUE, NULL);
    
    // 事件循环GMainLoop在用g_main_loop_new创建之后并不马上运行
    // 用g_main_loop_run运行后，还要用g_main_loop_quit退出，否则循环将一直运行下去
    g_main_loop_run(mloop);

    g_print("线程3退出事件循环\n");
    g_mutex_free(mutex);
    g_print("释放互斥对象\n"); 
    g_free(arg);
    g_print("释放参数所用的内存\n");
}