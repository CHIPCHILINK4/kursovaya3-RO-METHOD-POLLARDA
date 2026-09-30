#include "BigNUM.h"
#include <iostream>
#include <string>
using namespace std;


BiiigNum make_num_dec(const string& s) {// простое число в большое
    BiiigNum num;
    vvod(num, 10, s);  
    return num;
}

BiiigNum abs_diff( BiiigNum& a,  BiiigNum& b) { //разность по модулю тк нас не интересует знак
    if (a > b) {
        return a - b;
    } else {
        return b - a;
    }
}

bool check_primes(const BiiigNum& n) {//простое ли взял пока базовую проверку
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    BiiigNum zero = make_num_dec("0");
    
    for (int p : primes) {
        BiiigNum divisor = make_num_dec(to_string(p));
        BiiigNum rem = n % divisor;
        if (rem == zero) {
            return false;
        }
    }
    return true; // базовых делителей не найдено
}

BiiigNum gcd(BiiigNum a, BiiigNum b) {// алгоритм евклида с сайта URL:https://labex.io/ru/tutorials/cpp-how-to-implement-efficient-gcd-451087
    while (b != 0) {
        BiiigNum temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}



int main(){
    // получим число функцию ее надо будет сплитануть после чего взять поэлементно, наверное запихнуть в вектор. потом запустить аогоритм 10 раз , каждый раз выдавать время и делитель если не получилось вмето делителя '-'
    // to do 
    //протестировать в базе на значении x^2 + 1.
    // написать скрипт на питоне который будет читать функцию с аргументами из файла и отправлять методу и ждать когда будет доступ к значениям чтобы сохранить их и переносить значения из выхода в эксель
    // сравнить выход скрипта и выход ручками, возможно подправить.
    cout << "дата, функция, число, делитель который нашли";
}