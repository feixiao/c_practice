#include <stdio.h>
#include <dlfcn.h>

int main(void){
    void (*printhello)();
    void *phandle = NULL;
    char *perr = NULL;
    phandle = dlopen("./libfunc.so", RTLD_LAZY);
    if(!phandle){
        printf("Failed load library!\n");
    }
    perr = dlerror();
    if(perr != NULL){
        printf("%s\n", perr);
        return(0);
    }
//    printhello = dlsym(phandle, "func");
    printhello = (void (*)() )(dlsym(phandle, "func"));
    perr = dlerror();
    if(perr != NULL){
      printf("%s\n", perr);
      return(0);
    }    
    (*printhello)();
    dlclose(phandle);
    return 0;
} 