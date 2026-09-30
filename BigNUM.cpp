#include "BigNUM.h"

#include <algorithm>
#include <cstdlib>
#include <string>

using namespace std;

BiiigNum::BiiigNum(int mlen, int mode) {
    if (mlen < 0) {
        mlen = -mlen;
    }
    if (mlen == 0) {
        mlen = 1;
    }

    switch (mode)
    {
    case 2:
        coef = vector<BASE>(mlen);
        for (size_t i = 0; i < coef.size(); i++) {
            coef[i] = (BASE)((rand() << 16) + rand());//используя 2 ренда сделать<<
        }
        obr();
        break;

    default:
        coef = vector<BASE>(mlen, 0);
        break;
    }

}


void BiiigNum::obr() {
    for (int i = coef.size() - 1; i > 0; i--) {
        if (coef[i] == 0) {
            coef.pop_back();
        }
        else {
            i = 0;
        }
    }
}

istream& operator >> (istream& is, BiiigNum& bignum) {
    string str;
    bignum.coef.clear();
    if (!(is >> str)) {
        bignum.coef.push_back(0);
        return is;
    }

    int i = str.find_first_not_of('0');
    if (i > 0) {
        str = str.substr(i);
    }

    bignum.coef.resize((str.size() + (sizeof(BASE) * 2) - 1) / (sizeof(BASE) * 2), 0);

    i = str.size() - 1;

    for (int j = 0; j < bignum.coef.size() && i >= 0; i--) {

        int k = 0;
        for (; k < BASE_SIZE && i >= 0; i--) {

            char val = str[i];
            int n;
            if (val >= '0' && val <= '9') {
                n = val - '0';
            }
            else if (val >= 'a' && val <= 'f') {
                n = val - 'a' + 10;
            }
            else if (val >= 'A' && val <= 'F') {
                n = val - 'A' + 10;
            }
            else {
                n = 0;
                cout << "ERRRRRRRRRR\n";
            }
            bignum.coef[j] |= (n << k);
            k += 4;

            if (k >= BASE_SIZE) {
                k = 0;
                j++;
            }

        }
    }


    return is;
}

ostream& operator << (ostream& os, const BiiigNum& bignum) {

    int i = 0;
    int k = (BASE_SIZE - 4);
    string vivod(bignum.coef.size() * sizeof(BASE) * 2, '0');
    for (int j = bignum.coef.size() - 1; j >= 0; )
    {
        char tmp = (bignum.coef[j] >> k) & (0xf);
        if (tmp >= 0 && tmp <= 9) {
            vivod[i] = (char)(tmp + '0');
        }
        else if (tmp >= 10 && tmp <= 15) {
            vivod[i] = (char)(tmp - 10 + 'a');
        }
        i++; k -= 4;
        if (k < 0) {
            k = BASE_SIZE - 4; j--;
        }
    }

    i = vivod.find_first_not_of('0');
    if (i > 0) {
        os << vivod.substr(i);
    }
    else {
        os << vivod;
    }
    return os;
}

bool BiiigNum::operator == (const BiiigNum& num) {
    // Находим реальную длину (без ведущих нулей)
    int sz1 = coef.size();
    while (sz1 > 1 && coef[sz1 - 1] == 0) sz1--;
    int sz2 = num.coef.size();
    while (sz2 > 1 && num.coef[sz2 - 1] == 0) sz2--;
    if (sz1 != sz2) return false;
    for (int i = sz1 - 1; i >= 0; i--) {
        if (coef[i] != num.coef[i]) {
            return false;
        }
    }
    return true;
}

bool BiiigNum::operator != (const BiiigNum& num) {
    return !(*this == num);
}

bool BiiigNum::operator > (const BiiigNum& num) {
    int sz1 = coef.size();
    while (sz1 > 1 && coef[sz1 - 1] == 0) sz1--;
    int sz2 = num.coef.size();
    while (sz2 > 1 && num.coef[sz2 - 1] == 0) sz2--;
    if (sz1 > sz2) return true;
    if (sz1 < sz2) return false;
    for (int i = sz1 - 1; i >= 0; i--) {
        if (coef[i] < num.coef[i]) return false;
        if (coef[i] > num.coef[i]) return true;
    }
    return false;
}

bool BiiigNum::operator >= (const BiiigNum& num) {
    if (!(*this < num)) {
        return true;
    }
    return false;
}

bool BiiigNum::operator < (const BiiigNum& num) {
    int sz1 = coef.size();
    while (sz1 > 1 && coef[sz1 - 1] == 0) sz1--;
    int sz2 = num.coef.size();
    while (sz2 > 1 && num.coef[sz2 - 1] == 0) sz2--;
    if (sz1 < sz2) return true;
    if (sz1 > sz2) return false;
    for (int i = sz1 - 1; i >= 0; i--) {
        if (coef[i] > num.coef[i]) return false;
        if (coef[i] < num.coef[i]) return true;
    }
    return false;
}

bool BiiigNum::operator <= (const BiiigNum& num) {
    if (*this > num)
    {
        return false;
    }
    return true;
}


BiiigNum BiiigNum::operator + (const BiiigNum& num) {
    size_t min_sz = min(coef.size(), num.coef.size());
    size_t max_sz = max(coef.size(), num.coef.size());
    BiiigNum numres(max_sz, 1);
    unsigned int tmp2 = 0;
    int i = 0;
    for (; i < min_sz; i++) {
        unsigned long long int tmp = 0;
        tmp = (DBASE)tmp2 + (DBASE)coef[i] + (DBASE)num.coef[i];
        tmp2 = tmp >> BASE_SIZE;
        numres.coef[i] = (BASE)tmp;
    }

    if (i < coef.size()) {
        for (; i < coef.size(); i++) {
            unsigned long long int tmp = tmp2 + coef[i];
            tmp2 = tmp >> BASE_SIZE;
            numres.coef[i] = tmp;
        }
    }

    if (i < num.coef.size()) {
        for (; i < num.coef.size(); i++) {
            unsigned long long int tmp = tmp2 + num.coef[i];
            tmp2 = tmp >> BASE_SIZE;
            numres.coef[i] = tmp;
        }
    }

    if (tmp2) {
        numres.coef.push_back(tmp2);
    }

    return numres;
}

BiiigNum& BiiigNum::operator += (const BiiigNum& num) {
    size_t min_sz = min(coef.size(), num.coef.size());
    size_t max_sz = max(coef.size(), num.coef.size());

    coef.resize(max_sz, 0);

    unsigned  int tmp2 = 0;
    int i = 0;

    for (; i < min_sz; i++) {
        unsigned long long int tmp = 0;
        tmp = (DBASE)tmp2 + (DBASE)coef[i] + (DBASE)num.coef[i];
        tmp2 = tmp >> BASE_SIZE;
        coef[i] = (BASE)tmp;
    }
    if (i < coef.size()) {
        for (; i < coef.size(); i++) {
            unsigned long long int tmp = tmp2 + coef[i];
            tmp2 = tmp >> BASE_SIZE;
            coef[i] = (BASE)tmp;
        }
    }
    if (i < num.coef.size()) {
        for (; i < num.coef.size(); i++) {
            unsigned long long int tmp = tmp2 + num.coef[i];
            tmp2 = tmp >> BASE_SIZE;
            coef[i] = (BASE)tmp;
        }
    }

    if (tmp2) {
        coef.push_back(tmp2);
    }

    return *this;
}


BiiigNum BiiigNum::operator - (const BiiigNum& num) {

    if (*this < num) {
        cout << "ERR first is small1" << endl;
        return BiiigNum();
    }
    BiiigNum numres(coef.size(), 1);

    unsigned long long int tmp;
    unsigned int tmp2 = 0;
    int i = 0;

    for (; i < num.coef.size(); i++) {
        tmp = (DBASE)((DBASE)1 << BASE_SIZE) | (DBASE)coef[i];
        tmp = tmp - num.coef[i] - tmp2;
        numres.coef[i] = (BASE)tmp;
        tmp2 = !(tmp >> BASE_SIZE);
        /*tmp = (unsigned long long)coef[i] - tmp2 - num.coef[i];
        if (tmp >> BASE_SIZE ) {
            tmp2 = 1;
        }
        else {
            tmp2 = 0;
        }
        numres.coef[i] = (BASE)(tmp);*/
    }

    if (i < coef.size()) {
        for (; i < coef.size(); i++) {
            tmp = (DBASE)((DBASE)1 << BASE_SIZE) | (DBASE)coef[i];
            tmp = tmp - tmp2;
            numres.coef[i] = (BASE)tmp;
            tmp2 = !(tmp >> BASE_SIZE);
            /*tmp = (unsigned long long)coef[i] - tmp2;
            if (tmp >> BASE_SIZE) {
                tmp2 = 1;
            }
            else {
                tmp2 = 0;
            }
            numres.coef[i] = (BASE)(tmp);*/
        }
    }
    numres.obr();


    return numres;
}

BiiigNum& BiiigNum::operator -= (const BiiigNum& num) {

    if (*this < num) {
        cout << "ERR first is small2 " << endl;
        return *this;
    }

    DBASE tmp;
    BASE tmp2 = 0;
    int i = 0;

    for (; i < num.coef.size(); i++) {
        tmp = (DBASE)((DBASE)1 << BASE_SIZE) | (DBASE)coef[i];
        tmp = tmp - (BASE)num.coef[i] - tmp2;
        coef[i] = (BASE)tmp;
        tmp2 = !(tmp >> BASE_SIZE);
        /*tmp = (DBASE)(coef[i] - tmp2 - num.coef[i]);
        if (tmp >> BASE_SIZE) {
            tmp2 = 1;
        }
        else{
            tmp2 = 0;
        }
        coef[i] = (BASE)(tmp);*/
    }

    if (i < coef.size()) {
        for (; i < coef.size(); i++) {
            tmp = (DBASE)((DBASE)1 << BASE_SIZE) | (DBASE)coef[i];
            tmp = tmp - tmp2;
            coef[i] = (BASE)tmp;
            tmp2 = !(tmp >> BASE_SIZE);
            /*tmp = (unsigned long long)coef[i] - tmp2 ;
            if (tmp >> BASE_SIZE) {
                tmp2 = 1;
            }
            else
            {
                tmp2 = 0;
            }
            coef[i] = (BASE)(tmp);*/
        }
    }
    obr();


    return *this;
}

BiiigNum BiiigNum::operator * (const BiiigNum& num) {
    if (coef.size() == 1 && coef[0] == 0 || num.coef.size() == 1 && num.coef[0] == 0) {
        return BiiigNum(0);
    }

    BiiigNum numres(coef.size() + num.coef.size(), 1);

    DBASE tmp;
    BASE tmp2 = 0;
    int i = 0;
    int j = 0;

    for (; j < num.coef.size(); i = 0, j++)
    {
        tmp2 = 0;

        for (; i < coef.size(); i++) {
            tmp = (DBASE)coef[i] * (DBASE)num.coef[j] + (DBASE)tmp2 + (DBASE)numres.coef[i + j];
            numres.coef[i + j] = (BASE)tmp;
            tmp2 = (BASE)(tmp >> BASE_SIZE);
        }

        if (tmp2 != 0) {
            int idx = coef.size() + j;
            DBASE carry = tmp2;
            while (carry) {
                if (idx >= (int)numres.coef.size()) {
                    numres.coef.push_back(0);
                }
                DBASE s = (DBASE)numres.coef[idx] + carry;
                numres.coef[idx] = (BASE)s;
                carry = s >> BASE_SIZE;
                idx++;
            }
        }
    }

    numres.obr();
    return numres;
}

BiiigNum& BiiigNum::operator *= (const BiiigNum& num) {
    if (coef.size() == 1 && coef[0] == 0 || num.coef.size() == 1 && num.coef[0] == 0) {
        *this = BiiigNum(0);
        return *this;
    }

    BiiigNum numres(coef.size() + num.coef.size(), 1);

    DBASE tmp;
    BASE tmp2 = 0;
    int i = 0;
    int j = 0;

    for (; j < num.coef.size(); i = 0, j++)
    {
        tmp2 = 0;

        for (; i < coef.size(); i++) {
            tmp = (DBASE)coef[i] * (DBASE)num.coef[j] + (DBASE)tmp2 + (DBASE)numres.coef[i + j];
            numres.coef[i + j] = (BASE)tmp;
            tmp2 = (BASE)(tmp >> BASE_SIZE);
        }

        if (tmp2 != 0) {
            int idx = coef.size() + j;
            DBASE carry = tmp2;
            while (carry) {
                if (idx >= (int)numres.coef.size()) {
                    numres.coef.push_back(0);
                }
                DBASE s = (DBASE)numres.coef[idx] + carry;
                numres.coef[idx] = (BASE)s;
                carry = s >> BASE_SIZE;
                idx++;
            }
        }
    }

    numres.obr();
    *this = numres;
    return *this;
}

BiiigNum BiiigNum::operator * (const BASE& num) {
    if (num == 0 || coef.size() == 1 && coef[0] == 0) {
        return BiiigNum(0);
    }

    BiiigNum numres(coef.size() + 1, 1);

    DBASE tmp;
    BASE tmp2 = 0;
    int i = 0;

    for (; i < coef.size(); i++) {
        tmp = (DBASE)coef[i] * (DBASE)num + (DBASE)tmp2;
        numres.coef[i] = (BASE)tmp;
        tmp2 = (BASE)(tmp >> BASE_SIZE);
    }

    if (tmp2 != 0) {
        numres.coef[i] = tmp2;
    }
    numres.obr();
    return numres;
}

BiiigNum& BiiigNum::operator *= (const BASE& num) {

    if (num == 0 || coef.size() == 1 && coef[0] == 0) {
        *this = BiiigNum(0);
        return *this;
    }

    BiiigNum numres(coef.size() + 1, 1);

    DBASE tmp;
    BASE tmp2 = 0;
    int i = 0;

    for (; i < coef.size(); i++) {
        tmp = (DBASE)coef[i] * (DBASE)num + (DBASE)tmp2;
        numres.coef[i] = (BASE)tmp;
        tmp2 = (BASE)(tmp >> BASE_SIZE);
    }

    if (tmp2 != 0) {
        coef[i] = tmp2;
    }
    numres.obr();
    *this = numres;
    return *this;
}

BiiigNum BiiigNum::operator / (const BASE& num) {
    if (num == 0) {
        cout << "ERR: delenie na 0" << endl;
        return BiiigNum(0);
    }
    BiiigNum numres(coef.size(), 1);

    BASE r = 0;
    DBASE tmp;
    int i;
    for (i = coef.size() - 1; i >= 0; i--) {
        tmp = ((DBASE)r << BASE_SIZE) + (DBASE)coef[i];
        numres.coef[i] = (BASE)(tmp / num);
        r = tmp % num;
    }
    numres.obr();
    return numres;
}

BiiigNum& BiiigNum::operator /= (const BASE& num) {
    if (num == 0) {
        cout << "ERR: delenie na 0" << endl;
        *this = BiiigNum(0);
        return *this;
    }
    BiiigNum numres(coef.size(), 1);

    BASE r = 0;
    DBASE tmp;
    int i = 0;


    for (int i = coef.size() - 1; i >= 0; i--) {
        tmp = ((DBASE)r << BASE_SIZE) + (DBASE)coef[i];
        numres.coef[i] = (BASE)(tmp / num);
        r = tmp % num;
    }

    numres.obr();
    *this = numres;
    return *this;
}

BASE BiiigNum::operator % (const BASE num) {
    if (num == 0) {
        cout << "ERR: delenie na 0" << endl;
        return BASE(0);
    }
    BASE r = 0;
    DBASE tmp;
    int i = 0;
    for (int i = coef.size() - 1; i >= 0; i--) {
        tmp = ((DBASE)r << BASE_SIZE) + (DBASE)coef[i];
        r = tmp % num;
    }
    return r;
}

BiiigNum& BiiigNum::operator %= (const BASE num) {
    if (num == 0) {
        cout << "ERR: delenie na 0" << endl;
        *this = BiiigNum(0);
        return *this;
    }
    BASE r = 0;
    DBASE tmp;
    int i = 0;
    for (int i = coef.size() - 1; i >= 0; i--) {
        tmp = ((DBASE)r << BASE_SIZE) + (DBASE)coef[i];
        r = tmp % num;
    }
    coef.clear();
    coef.push_back(r);
    return *this;
}

void vvod(BiiigNum& num, BASE C, string str) {   // 10 ричка
    if (C > 36)
    {
        C = 36;
    }
    num.coef.clear();

    for (char ch : str) {
        int digit;
        if (ch >= '0' && ch <= '9') digit = ch - '0';
        else if (ch >= 'a' && ch <= 'z') digit = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'Z') digit = ch - 'A' + 10;
        else { cout << "ERR: invalid digit\n"; return; }

        if (digit >= C) { cout << "ERR: digit >= base\n"; return; }

        num = num * C;
        if (digit > 0) {
            BiiigNum add(1, 1);
            add.coef[0] = (BASE)digit;
            num += add;
        }
    }
}

void vivod(BiiigNum num, BASE C, string str) { //поточку в простую функцию с изменением С
    if (C < 2 || C > 36) {
        C = 35;
    }
    str.clear();
    int i = 0;
    BASE tmp = 0;
    str = string(num.coef.size() * BASE_SIZE, '0');

    for (; !(num.coef.size() == 1 && num.coef[0] == 0); i++)
    {
        if (C <= 10)
        {
            tmp = num % C;
            num /= C;
            str[i] = (char)(tmp + '0');

        }
        else
        {
            tmp = num % C;
            num /= C;
            if (tmp < 10)
            {
                str[i] = (char)(tmp + '0');
            }
            else
            {
                str[i] = (char)(tmp + 'a');
            }

        }

    }
    reverse(str.begin(), str.end());

    i = str.find_first_not_of('0');
    if (i > 0) {
        cout << str.substr(i);
    }
    else {
        cout << str;
    }
}


BiiigNum BiiigNum::operator / (const BiiigNum& num) {// частное
    if (num.coef.size() == 1 && num.coef[0] == 0) {
        cerr << "ERRRRRRRRRRRRRRRRR 0\n";
        return BiiigNum(1, 0);
    }
    else
    {
        const DBASE b = (DBASE)1 << BASE_SIZE; 

        BiiigNum U = *this;
        BiiigNum V = num;

        // Убираем ведущие нули
        V.obr();
        U.obr();

        if (V.coef.size() == 1) {
            return U / V.coef[0];
        }
        if (U < V) 
        { 
            return BiiigNum(); 
        }
        if (U == V) { 
            BiiigNum q; 
            q.coef[0] = 1; 
            return q; 
        }

        // D1. Нормализация
        BASE d = (BASE)(b / ((DBASE)V.coef.back() + 1));
        int old_U_size = U.coef.size();
        if (d > 1) {
            U = U * d;
            V = V * d;
        }
        if (U.coef.size() == old_U_size) {
            U.coef.push_back(0);
        }
        int n = V.coef.size();

        // D2. Начальная установка 
        int m = U.coef.size() - n - 1;
        if (m < 0) m = 0;
        BiiigNum q(m + 1, 1);
        /*/////////////////////////////////////////////////////////////////////
        while (U.coef.size() < n + m + 1) {
            U.coef.push_back(0);
        }*/
        n = V.coef.size();

        // D7. 
        for (int j = m; j >= 0; j--)
        {
            // D3. Вычисление q'
            DBASE q_tmp = ((DBASE)U.coef[j + n] * b + (DBASE)U.coef[j + n - 1]) / (DBASE)V.coef[n - 1];
            DBASE r_tmp = ((DBASE)U.coef[j + n] * b + (DBASE)U.coef[j + n - 1]) % (DBASE)V.coef[n - 1];


            int qwe = 0;
            // Проверка и корректировка q'
            while (n >= 2 && q_tmp * (DBASE)V.coef[n - 2] > r_tmp * b + (DBASE)U.coef[j + n - 2]) {
                q_tmp--;
                r_tmp += (DBASE)V.coef[n - 1];
                qwe++;
                if (r_tmp >= b) break;

            }
            if (qwe>2)
            {
                cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            }

            // D4. Умножить и вычесть: U[j..j+n] -= q' * V[0..n-1]
            BASE tmp3 = 0;  // перенос от умножения
            long long borrow_sub = 0;  // заём от вычитания
            for (int i = 0; i < n; i++) {
                DBASE t = q_tmp * (DBASE)V.coef[i] + tmp3;
                tmp3 = t >> BASE_SIZE;
                long long diff2 = (long long)U.coef[j + i] - (long long)((BASE)t) - borrow_sub;//разница в окне
                if (diff2 < 0) {
                    diff2 += (long long)b;
                    borrow_sub = 1;
                }
                else {
                    borrow_sub = 0;
                }
                U.coef[j + i] = (BASE)diff2;
            }
            long long diff = (long long)U.coef[j + n] - (long long)tmp3 - borrow_sub;
            bool negative = false; 
            if (diff < 0)
            {
                negative = true;//если q' слишком большой
            }
            U.coef[j + n] = (BASE)diff;

            // D5
            q.coef[j] = (BASE)q_tmp;

            // D6 
            if (negative) {
                q.coef[j]--;
                DBASE carry = 0;
                for (int i = 0; i < n; i++) {
                    DBASE s = (DBASE)U.coef[j + i] + (DBASE)V.coef[i] + carry;
                    carry = s >> BASE_SIZE;
                    U.coef[j + i] = (BASE)s;
                }
                U.coef[j + n] += (BASE)carry;
            }
        }
        q.obr();
        return q;
    }
    return BiiigNum();
}

BiiigNum BiiigNum::operator % ( BiiigNum& num) {// остаток
    if (num.coef.size() == 1 && num.coef[0] == 0) {
        cerr << "ERRRRRRRRRRRRRRRRR 0\n";
        return BiiigNum(1, 0);
    }
    else
    {
        // Используем unsigned int для промежуточных вычислений чтобы избежать переполнения DBASE
        const DBASE b = (DBASE)1 << BASE_SIZE;

        BiiigNum U = *this;
        BiiigNum V = num;

        // Убираем ведущие нули
        V.obr();
        U.obr();

        if (V.coef.size() == 1) {
            BASE r = U % V.coef[0];
            BiiigNum res;
            res.coef[0] = r;
            return res;
        }
        if (U < V) {
            return U;
        }
        if (U == V) {
            return BiiigNum();
        }

        // D1. Нормализация
        BASE d = (BASE)(b / ((DBASE)V.coef.back() + 1));
        int old_U_size = U.coef.size();
        if (d > 1) {
            U = U * d;
            V = V * d;
        }
        if (U.coef.size() == old_U_size) {
            U.coef.push_back(0);
        }
        int n = V.coef.size();

        // D2. Начальная установка 
        int m = U.coef.size() - n - 1;
        if (m < 0) m = 0;
        //BiiigNum q(m + 1, 1);////////////////////можно убрать тк не надо

        while ((int)U.coef.size() < n + m + 1) {
            U.coef.push_back(0);
        }

        // D7. 
        for (int j = m; j >= 0; j--)
        {
            // D3. Вычисление q'
            DBASE q_tmp = ((DBASE)U.coef[j + n] * b + (DBASE)U.coef[j + n - 1]) / (DBASE)V.coef[n - 1];
            DBASE r_tmp = ((DBASE)U.coef[j + n] * b + (DBASE)U.coef[j + n - 1]) % (DBASE)V.coef[n - 1];

            while (n >= 2 && q_tmp * (DBASE)V.coef[n - 2] > r_tmp * b + (DBASE)U.coef[j + n - 2]) {
                q_tmp--;
                r_tmp += (DBASE)V.coef[n - 1];
                if (r_tmp >= b) break;
            }

            // D4. Умножить и вычесть
            BASE tmp3 = 0;//как в умножении
            long long borrow_sub = 0;
            for (int i = 0; i < n; i++) {
                DBASE t = q_tmp * (DBASE)V.coef[i] + tmp3;
                tmp3 = t >> BASE_SIZE;
                long long diff2 = (long long)U.coef[j + i] - (long long)((BASE)t) - borrow_sub;
                if (diff2 < 0) {
                    diff2 += (long long)b;
                    borrow_sub = 1;
                }
                else {
                    borrow_sub = 0;
                }
                U.coef[j + i] = (BASE)diff2;
            }
            long long diff = (long long)U.coef[j + n] - (long long)tmp3 - borrow_sub;
            bool negative = (diff < 0);
            U.coef[j + n] = (BASE)diff;

            // D5
            //q.coef[j] = (BASE)q_tmp;

            // D6
            if (negative) {
                //q[j]--;
                DBASE carry = 0;
                for (int i = 0; i < n; i++) {
                    DBASE s = (DBASE)U.coef[j + i] + (DBASE)V.coef[i] + carry;
                    carry = s >> BASE_SIZE;
                    U.coef[j + i] = (BASE)s;
                }
                U.coef[j + n] += (BASE)carry;
            }
        }
        // D8. Денормализация
        if (d > 1) {
            U = U / d;
        }

        U.obr();
        return U;
    }
    return BiiigNum();
}

BiiigNum& BiiigNum::operator /= (const BiiigNum& num) {
    *this = *this / num;
    return *this;
}

BiiigNum& BiiigNum::operator %= ( BiiigNum& num) {
    *this = *this % num;
    return *this;
}