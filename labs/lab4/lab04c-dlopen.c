#include <stdio.h>
#include <dlfcn.h>


int main()
{
    void* lib = dlopen("./libsquare.so", RTLD_LAZY);
    if (!lib) return 1;
    double (*square)(double) = dlsym(lib, "square");
    if (!square) return 1;
    printf("%f\n", square(5.0));
    dlclose(lib);
    return 0;
}

//  gcc lab04c-dlopen.c
