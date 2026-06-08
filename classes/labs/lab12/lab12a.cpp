#include <iostream>

double square(double x) { return x*x; }
int square(int x) { return x*x; }


struct test { int data; };

test operator+(test x, test y) {
    test z;
    z.data = x.data + y.data;
    return z;
}




// z = x+y;

// ofstream& operator<<(ofstream& o, const std::string& s);
// ofstream& operator<<(ofstream& o, const double s);
// ofstream& operator<<(ofstream& o, const int s);



template <typename T, typename S>
T plus(T x, S y) { return x+y; }

template <typename T, int X>
T power(T x) {
    T y = x;
    for (int i=X-1; i>0; --i) y *= x;
    return y;
}



int& getdata(test& t) { return t.data; }

using namespace std;

int main()
{
    test z;
    getdata(z) = 6;

    power<double, 2>(2.0);
    power<double, 5>(2.0);

    cout << "hello " << square(3.0) << square(3) << endl;  // std::cout, std::endl;
    return 0;
}
