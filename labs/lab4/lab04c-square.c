double square(double x)
{
    return x*x;
}


// no main - it's a library

// gcc -fPIC -c lab04c-square.c
// gcc -fPIC -shared lab04c-square.o -o libsquare.so

