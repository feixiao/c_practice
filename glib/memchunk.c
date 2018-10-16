#include <glib.h>

int main(int argc, char *argv[])
{
    GMemChunk *chunk;   //  定义内存块
    gchar *mem[10];     //  定义指向原子的指针数组
    gint i, j;
    //创建内存块  名称，原子的长度， 内存块的长度，类型
    chunk = g_mem_chunk_new("Test MemChunk", 5, 50, G_ALLOC_AND_FREE);
                
    for(i=0; i<10; i++)
    {
        //创建对象
        //mem[i] = g_chunk_new(gchar, chunk);
        mem[i] = (gchar*)g_mem_chunk_alloc(chunk);
        for(j=0; j<5; j++)
        {
            mem[i][j] = 'A' + j;//为内存块中的指针赋值
        }
    }
     
    g_mem_chunk_print(chunk);   //显示内存块信息
    for(i=0; i<10; i++)
    {
        g_print("%s\t",mem[i]);//显示内存块中的内容
    }
     
    for(i=0; i<10; i++)
    {
        g_mem_chunk_free(chunk,mem[i]); //释放所有分配的内存
    }
    g_mem_chunk_destroy(chunk);
}
