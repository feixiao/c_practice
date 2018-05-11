#include <glib.h>
#include <stdio.h>
#include <strings.h>

GMainLoop* loop;
//当stdin有数据可读时被GSource调用的回调函数
gboolean callback(GIOChannel *channel)
{
    gchar* str;
    gsize len;
    //从stdin读取一行字符串
    g_io_channel_read_line(channel, &str, &len, NULL, NULL);
    //去掉回车键()
    while(len > 0 && (str[len-1] == '/r' || str[len-1] == '/n'))
        str[--len]='/0';
    //反转字符串
    for(;len;len--)
        g_print("%c",str[len-1]);
    g_print("/n");
    //判断结束符
    if(strcasecmp(str, "q") == 0){
        g_main_loop_quit(loop);
    }
    g_free(str);
}

void add_source(GMainContext *context)
{
    GIOChannel* channel;
    GSource* source;
    //这里我们监视stdin是否可读， stdin的fd默认等于1
    channel = g_io_channel_unix_new(1);
    //g_io_create_watch创建一个默认的io监视作用的GSource，下次再研究自定义GSource。参数G_IO_IN表示监视stdin的读取状态。
    source = g_io_create_watch(channel, G_IO_IN);
    g_io_channel_unref(channel);
    //设置stdin可读的时候调用的回调函数
    g_source_set_callback(source, (GSourceFunc)callback, channel, NULL);
    //把GSource附加到GMainContext
    g_source_attach(source, context);
    g_source_unref(source);
}

int main(int argc, char* argv[])
{
    GMainContext *context;
    if(g_thread_supported() == 0)
        g_thread_init(NULL);
    //新建一个GMainContext
    context = g_main_context_new();
    //然后把GSource附到这个Context上
    add_source(context);
    //把Context赋给GMainLoop
    loop = g_main_loop_new(context, FALSE);
    g_print("input string('q' to quit)/n");
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    //Context用完计数器减1
    g_main_context_unref(context);
    return 0;
}