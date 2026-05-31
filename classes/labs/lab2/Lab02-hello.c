#include <stdio.h>

// double my_abs(double x) {
//     if (x >= 0.0) return x;
//     else return -x;
// }

// sum 0,...,n-1
long long my_sum(long long n)
{
    long long sum = 0;
    for (long long i=0; i<n; ++i)
        sum += i;
    return sum;
}


int main() {
    //double w = -0.5;
    //double z = my_abs(w);
    //printf("%f\n", z);
    long long res = my_sum(10000000LL);
    printf("%lld\n", res);
    return 0;
}
