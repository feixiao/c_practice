#include <stdio.h>
#include <unistd.h>


#include "cpu.h"
#include "memory.h"

int main() 
{
    int cpu = 0;
    printf("cpu usage : %f\n", cpu_percentage(100000));

    MemoryStatus status;
    mem_status(status );
    printf("mem usage : %f  %f\n", status.total_mem, status.used_mem);
    return 0;
}