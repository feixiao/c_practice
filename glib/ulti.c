#include <glib.h>

int main(int argc, char const *argv[])
{
    GRand* rand = NULL;
    GTimer* timer = NULL;
    gint n = 0;
    gint i = 0, j = 0;
    gint x = 0;
    rand = g_rand_new();    //创建随机数对象
    for(n=0; n<20; n++)
    {   //产生随机数并显示出来
        g_print("%d\t",g_rand_int_range(rand,1,100));
    }
    g_print("\n");
    g_rand_free(rand);  //释放随机数对象
    //创建计时器
    timer = g_timer_new();
    g_timer_start(timer);//开始计时
    for(i=0; i<10000; i++)
        for(j=0; j<3000; j++)
            x++;//累计
    g_timer_stop(timer);//计时结束
    //输出计时结果
    g_print("%ld\tall:%.2f seconds was used!\n",x,g_timer_elapsed(timer,NULL));
    return 0;
}
