

#include <stdio.h>

int square(int x)
{
    return x*x;
}


int main()
{
    int (*kwadrat)(int) = square;
    printf("%d\n", kwadrat(2));

    //double (*power)(double, int) = (void*)(kwadrat+1);
    //double z = power(3.4, 3);

    return 0;
}
