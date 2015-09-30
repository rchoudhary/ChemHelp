//
// Created by Rutvik Choudhary on 7/25/15.
//

#include "frac.h"

using namespace std;

const frac_t unit_frac = {1, 1};
const frac_t zero_frac = {0, 1};


int gcd(int a, int b)
{
    if (a == b) return a;
    while (!(a == 0 || b == 0))
    {
        if (a > b) a %= b;
        else if (b > a) b %= a;
        if (a == 0) return b;
        else if (b == 0) return a;
    }
    return 1;
}

int lcm(int a, int b)
{
    return (a * b) / gcd(a, b);
}

frac_t simplify(frac_t frac)
{
    int a = frac.num, b = frac.den;
    int _gcd = gcd(a, b);
    return {a / _gcd, b / _gcd};
}

frac_t operator+(frac_t a, frac_t b)
{
    return simplify({a.num * b.den + a.den * b.num, a.den * b.den});
}

frac_t operator+(frac_t a, int b)
{
    return simplify({a.num + a.den * b, a.den});
}

frac_t operator-(frac_t a, frac_t b)
{
    return simplify({a.num * b.den - a.den * b.num, a.den * b.den});
}

frac_t operator-(frac_t a, int b)
{
    return simplify({a.num - a.den * b, a.den});
}

frac_t operator*(frac_t a, frac_t b)
{
    return simplify({a.num * b.num, a.den * b.den});
}

frac_t operator*(frac_t a, int b)
{
    return simplify({a.num * b, a.den});
}

frac_t operator/(frac_t a, frac_t b)
{
    return simplify({a.num * b.den, a.den * b.num});
}

frac_t operator/(frac_t a, int b)
{
    return simplify({a.num, a.den * b});
}

ostream& operator<<(ostream& out, frac_t frac)
{
    out << frac.num;
    if (frac.den != 1)
        out << "/" << frac.den;
    return out;
}
