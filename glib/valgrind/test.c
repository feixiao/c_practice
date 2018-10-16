#include <glib.h>
#include <stdio.h>

int count = 0;

typedef struct _Person
{
    int a;
    int b;
}Person;

int main(int argc, char* argv[])
{
    if(getenv("BYPASS_GLIB_POOLS") != NULL)
    {
        g_slice_set_config(G_SLICE_CONFIG_ALWAYS_MALLOC, TRUE);
    }
       
    Person* p1 = g_new(Person, 1);
     p1->a = 10;
    p1->b = 10;
    Person* p2 = g_new(Person, 1);
    p2->a = 20;
    p2->b = 20;
       
    GList* list = NULL;
    list = g_list_append(list, p1);
    list = g_list_append(list, p2);
    g_printf("sizeof(int) = %d/n", sizeof(int));

    list = g_list_delete_link(list,g_list_nth(list, 1));
    g_free(p1);
 //   g_free(p2);
    g_list_free(list);
    return 0;
}