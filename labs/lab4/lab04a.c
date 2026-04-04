#include <stdio.h>


int main()
{
    int x=0, y=1, *w, z=2;

    printf("&x=%llu\n", (unsigned long long)&x);
    printf("&y=%llu\n", (unsigned long long)&y);
    printf("&w=%llu\n", (unsigned long long)&w);
    printf("&z=%llu\n", (unsigned long long)&z);

    w = &y;   // address of y
    printf(" y=%d\n", y);
    printf("*w=%d\n", *w);  // pointer dereference

    *w = 6;
    printf(" y=%d\n", y);

    w = &x;
    printf("*w=%d\n", *w);  // pointer dereference

    printf("*(w-1)=%d\n", *(w-1));  // pointer dereference

    //printf("%d\n", y);

    double t = 1.0;
    void* p = &t;
    printf("*p=%f\n", *(double*)p);
    printf("*p=%lld\n", *(long long*)p);
    printf("*p=%llx\n", *(long long*)p);

    return 0;
}
