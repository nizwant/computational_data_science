#include <iostream>
using namespace std;

template <class T>
class Vector
{
private:
    size_t size;
    T* data;

public:
    Vector(int size=3) {  // constructor
        cout << "constructor(" << size << ")\n";
        this->size = size;
        data = new T[size]; // this->data = ..
    }

    Vector(const Vector<T>& y) {  // copy constructor
        //// shallow copy created by default:
        //size = y.size;
        //data = y.data;
        cout << "constructor(copy)\n";
        size = y.size;
        data = new T[size];
        for (size_t i=0; i<size; ++i) data[i] = y.data[i];
    }

    Vector& operator=(const Vector<T>& y) {  // assignment
        delete [] data;
        size = y.size;
        data = new T[size];
        for (size_t i=0; i<size; ++i) data[i] = y.data[i];
        return *this;
    }

    ~Vector() {  // destructor
        cout << "destructor()\n";
        delete [] data;
    }

    Vector<T> operator+(const Vector<T>& y) {
        Vector<T> out(size);  // this->size
        for (size_t i=0; i<size; ++i)
            out.data[i] = data[i]+y.data[i];
        return out;
    }

    //template <class S> friend void print(Vector<S> &v);  // print can access private fields

    inline size_t get_size() { return size; }

    inline T& operator[](size_t i) { return data[i]; }

};


template <class T>
void print(Vector<T> &v) {
    for (size_t i=0; i<v.get_size(); ++i)
        cout << v[i] << " ";
    cout << endl;
}

/*  needs a friend...
template <class T>
void print(Vector<T> &v) {
    for (size_t i=0; i<v.size; ++i)
        cout << v.data[i] << " ";
    cout << endl;
}*/


int main()
{
    Vector<double> v(5);  // passes 5 to the constructor
    v[3] = 6.1;
    Vector<double> w = v;  // w(v)
    w[3] = 1.2;
    Vector<double> z(v+w);

    print(z);

    z = v+z;

    //Vector<int>* x = new Vector<int>(3);
    //delete x;
    return 0;
}
