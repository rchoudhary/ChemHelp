//
// Created by Rutvik Choudhary on 7/25/15.
//

#pragma once

#include <iostream>

// simple fraction-type object
struct frac_t
{
    int num, den;
};

// global fraction objects that repr. 1/1 and 0/1 respectively
extern const frac_t unit_frac;
extern const frac_t zero_frac;

// finds the gcd and lcm of two numbers
int gcd(int a, int b);

int lcm(int a, int b);

// simplifies a fraction into the lowest possible form
frac_t simplify(frac_t frac);

// common mathematical operators
frac_t operator+(frac_t a, frac_t b);

frac_t operator+(frac_t a, int b);

frac_t operator-(frac_t a, frac_t b);

frac_t operator-(frac_t a, int b);

frac_t operator*(frac_t a, frac_t b);

frac_t operator*(frac_t a, int b);

frac_t operator/(frac_t a, frac_t b);

frac_t operator/(frac_t a, int b);

std::ostream& operator<<(std::ostream& out, frac_t frac);
