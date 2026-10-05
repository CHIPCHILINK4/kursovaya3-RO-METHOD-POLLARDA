#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using BASE = std::uint32_t;
using DBASE = std::uint64_t;

#define BASE_SIZE (sizeof(BASE) * 8)
#define BASE_MAX ((BASE)(-1))

class BiiigNum {
    std::vector<BASE> coef;
    void obr();

public:
    BiiigNum(int mlen = 1, int mode = 0);

    friend std::istream& operator>>(std::istream&, BiiigNum&);
    friend std::ostream& operator<<(std::ostream&, const BiiigNum&);

    bool operator==(const BiiigNum&);
    bool operator!=(const BiiigNum&);
    bool operator>(const BiiigNum&);
    bool operator>=(const BiiigNum&);
    bool operator<(const BiiigNum&);
    bool operator<=(const BiiigNum&);

    BiiigNum operator+(const BiiigNum&);
    BiiigNum& operator+=(const BiiigNum&);
    BiiigNum operator-(const BiiigNum&);
    BiiigNum& operator-=(const BiiigNum&);

    BiiigNum operator*(const BiiigNum&);
    BiiigNum& operator*=(const BiiigNum&);
    BiiigNum operator*(const BASE&);
    BiiigNum& operator*=(const BASE&);

    BiiigNum operator/(const BASE&);
    BiiigNum& operator/=(const BASE&);
    BASE operator%(BASE);
    BiiigNum& operator%=(BASE);

    BiiigNum operator/(const BiiigNum&);
    BiiigNum& operator/=(const BiiigNum&);
    BiiigNum operator%( BiiigNum&);
    BiiigNum& operator%=( BiiigNum&);

    friend void vivod(BiiigNum, BASE, std::string&);
    friend void vvod(BiiigNum&, BASE, std::string);

};