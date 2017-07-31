#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>

using namespace std;


template<bool flag, int n1, int n2>
struct If_then_else {
    const static int Result;
};

template<int n1, int n2>
struct If_then_else<true, n1, n2> {
    const static int Result = n1;
};

template<int n1, int n2>
struct If_then_else<false, n1, n2> {
    const static int Result = n2;
};

template<typename T, T n1, T n2>
struct Select {
    const static T greater = If_then_else<(n1 > n2), n1, n2>::Result;
    const static T smaller = If_then_else<(n1 < n2), n1, n2>::Result;
    const static T equal = (n1 == n2);
};

struct Handle {
    static void check(int p) {
        if (p == 1) throw invalid_argument("Overflow occured!");
    }
};

struct Ignore {
    static void check(int p) {
        //todo nihuja
    }
};

template<int size, typename BaseType=unsigned int, class OverloadCheck=Handle>
class NSizeNatural : public OverloadCheck {
public:
    BaseType tab[size];

    NSizeNatural() {}

    NSizeNatural(BaseType x) {
        tab[0] = x;
        for (int i = 1; i < size; i++) {
            tab[i] = 0;
        }
    }

    NSizeNatural(BaseType tab[], unsigned int length) {
        int i = 0;
        for (; i < length; i++) {
            this->tab[i] = tab[i];
        }
        for (; i < size; i++) {
            this->tab[i] = 0;
        }

    }


    template<BaseType, int size1, int size2>
    friend bool operator==(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
                           const NSizeNatural<size2, BaseType, OverloadCheck> &right);

    template<BaseType, int size1, int size2>
    friend bool operator!=(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
                           const NSizeNatural<size2, BaseType, OverloadCheck> &right);

    template<BaseType, int size1, int size2>
    friend bool operator>(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
                          const NSizeNatural<size2, BaseType, OverloadCheck> &right);

    template<BaseType, int size1, int size2>
    friend bool operator<(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
                          const NSizeNatural<size2, BaseType, OverloadCheck> &right);

    template<int size1, BaseType>
    friend ostream &operator<<(ostream &, const NSizeNatural<size1, BaseType, OverloadCheck> &);


    template<BaseType, int size1, int size2>
    friend NSizeNatural<size, BaseType, OverloadCheck> operator+
            (const NSizeNatural<size1, BaseType, OverloadCheck> &left,
             const NSizeNatural<size2, BaseType, OverloadCheck> &right);

};


template<int size, typename BaseType=unsigned int, class OverloadCheck>
ostream &operator<<(ostream &stream, const NSizeNatural<size, BaseType, OverloadCheck> &n) {
    short one_basic_width = std::numeric_limits<BaseType>::digits / 4;
    stream << std::setfill('0') << std::setw(one_basic_width);
    for (int i = size - 1; i >= 0; i--) {
        if (n.tab[i] != 0)
            cout << (unsigned long long) n.tab[i];
    }
    return stream;
}

template<typename BaseType, int size1, int size2, class OverloadCheck>
bool operator==(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
                const NSizeNatural<size2, BaseType, OverloadCheck> &right) {
    if (size1 != size2) return false;
    else {
        int i = 0;
        while (i < size1 && left.tab[i] == right.tab[i]) i++;
        return i == size1;

    }
}


template<typename BaseType, int size1, int size2, class OverloadCheck>
bool operator!=(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
                const NSizeNatural<size2, BaseType, OverloadCheck> &right) {
    if (size1 != size2) return false;
    else {
        int i = 0;
        while (i < size1 && left.tab[i] == right.tab[i]) i++;
        return i != size1;

    }
}

template<typename BaseType, int size1, int size2, class OverloadCheck>
bool operator>(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
               const NSizeNatural<size2, BaseType, OverloadCheck> &right) {
    if (size1 < size2) return false;
    else if (size1 > size2) return true;
    else {
        int i = 0;
        while (i < size1 && left.tab[i] == right.tab[i]) i++;
        return left.tab[i] > right.tab[i];
    }
}

template<typename BaseType, int size1, int size2, class OverloadCheck>
bool operator<(const NSizeNatural<size1, BaseType, OverloadCheck> &left,
               const NSizeNatural<size2, BaseType, OverloadCheck> &right) {
    if (size1 < size2) return true;
    else if (size1 > size2) return false;
    else {
        int i = 0;
        while (i < size1 && left.tab[i] == right.tab[i]) i++;
        return left.tab[i] < right.tab[i];
    }
}

template<typename BaseType, int size1, int size2, class OverloadCheck>
NSizeNatural<Select<int, size1, size2>::greater, BaseType, OverloadCheck> operator+
        (const NSizeNatural<size1, BaseType, OverloadCheck> &left,
         const NSizeNatural<size2, BaseType, OverloadCheck> &right) {
    NSizeNatural<Select<int, size1, size2>::greater, BaseType, OverloadCheck> result(0);
    int per = 0;
    int smaller_size = Select<int, size1, size2>::smaller;

    int i = 0;
    for (; i < smaller_size; i++) {
        result.tab[i] = left.tab[i] + right.tab[i] + per;
        if (result.tab[i] < left.tab[i] || result.tab[i] < right.tab[i] ||
            (result.tab[i] == numeric_limits<BaseType>::max() && per == 1)) {
            per = 1;

        } else per = 0;
    }

    i--;
    if (size1 > size2) {
        while (++i < size1) {
            result.tab[i] = left.tab[i] + per;
            per = 0;

        }
    } else if (size1 < size2) {
        while (++i < size2) {
            result.tab[i] = right.tab[i] + per;
            per = 0;
        }
    }


    try {
        OverloadCheck::check(per);
    } catch (exception e) {
        cout << "Overflow occured! \n Press any key to continue...";
        cin.get();
    }

    return result;
}

template<class NSizeNatural>
void writeFibbonacci(int x) {
    cout << hex;
    NSizeNatural first(1);
    cout << "1: " << first << endl;
    NSizeNatural second(2);
    cout << "2: " << second << endl;
    NSizeNatural temp(0);

    for (int i = 2; i < x - 1; i++) {

        temp = first + second;
        cout << i + 1 << ": " << temp << endl;
        first = second;
        second = temp;
    }
};
namespace std {
    template<typename BaseType, int size, class Over>
    class numeric_limits<NSizeNatural<size, BaseType, Over>> {
    public:
        static NSizeNatural<size, BaseType, Over> max() {
            NSizeNatural<size, BaseType> temp(0);
            for (int i = 0; i < size; i++) {
                temp.tab[i] = std::numeric_limits<BaseType>::max();
            }
            return temp;
        }

        static NSizeNatural<size, BaseType, Over> min() {
            NSizeNatural<size, BaseType> temp(0);
            for (int i = 0; i < size; i++) {
                temp.tab[i] = 0;
            }
            return temp;
        }
    };
}

int main() {

    cout << std::hex;
    NSizeNatural<3, unsigned int, Ignore> a(101);
    NSizeNatural<3, unsigned int, Ignore> b(201);


    cout << a + b << endl;
    unsigned char cs[3] = {125, 250, 126};
    NSizeNatural<1, unsigned char, Ignore> ac(101);
    NSizeNatural<2, unsigned char, Ignore> bc(cs, 2);
    NSizeNatural<3, unsigned char, Ignore> cc(cs, 3);
    cout << cc << endl;
    cout << ac + bc << endl;
    cout << ac + cc + bc << endl;
    cout << (bc < cc) << endl;
    cout << (ac > cc) << endl;
    cout << (cc == cc) << endl;
    cout << std::numeric_limits<NSizeNatural<3, unsigned long>>::max() << endl;

    cout << NSizeNatural<5>(0) + std::numeric_limits<NSizeNatural<3> >::max() +
            std::numeric_limits<NSizeNatural<4> >::max();
    NSizeNatural<2, unsigned char, Handle> ta = std::numeric_limits<NSizeNatural<2, unsigned char> >::max();
    cout << ta << endl;
    cout << ta + ta;
    writeFibbonacci<NSizeNatural<4, unsigned char> >(46);
    writeFibbonacci<NSizeNatural<3, unsigned long long> >(200);


}


