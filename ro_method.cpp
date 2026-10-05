#include "BigNUM.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;


BiiigNum make_num_dec(const string& s) {// простое число в большое
    BiiigNum num;
    vvod(num, 10, s);  
    return num;
}
BiiigNum ZERO = make_num_dec("0");
BiiigNum ONE = make_num_dec("1");

BiiigNum abs_diff( BiiigNum& a,  BiiigNum& b) { //разность по модулю тк нас не интересует знак
    if (a > b) {
        return a - b;
    } else {
        return b - a;
    }
}

BiiigNum check_primes( BiiigNum& n) {//простое ли взял пока базовую проверку
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    
    for (int p : primes) {
        BiiigNum divisor = make_num_dec(to_string(p));
        BiiigNum rem = n % divisor;
        if (rem == ZERO) {
            return divisor;
        }
    }
    return ONE; // базовых делителей не найдено
}
struct RunResult {
    bool success;
    double time_ms;
    int iterations;
    BiiigNum divisor;
};

BiiigNum gcd(BiiigNum a, BiiigNum b) {// алгоритм евклида с сайта URL:https://labex.io/ru/tutorials/cpp-how-to-implement-efficient-gcd-451087
    while (b != 0) {
        BiiigNum temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

BiiigNum f_poly(BiiigNum x, int degree, BiiigNum c, BiiigNum n){
    BiiigNum res = x;
    for (int i = 1; i < degree; ++i) {
        res = res * x;
    }
    return (res + c) % n;
}

RunResult pollard_rho(const BiiigNum& n, int degree, BiiigNum c_val) {
    BiiigNum x = make_num_dec("2");
    BiiigNum y = make_num_dec("2");
    BiiigNum d = ONE;
    // BiiigNum c = make_num_dec(to_string(c_val));

    
    int max_iter = 3000000; // Защита от зависания

    auto start = chrono::high_resolution_clock::now();

    for (size_t iterations = 0; d == ONE && iterations < max_iter; iterations++) {
        x = f_poly(x, degree, c_val, n);          
        y = f_poly(y, degree, c_val, n);          
        y = f_poly(y, degree, c_val, n);          

        BiiigNum diff = abs_diff(x, y);
        d = gcd(diff, n);
        // cout << "Iteration: " << iterations + 1 << ", x: " << x << ", y: " << y << ", |x - y|: " << diff << ", gcd(|x - y|, n): " << d << endl;
        // if (iterations % 1000 == 0) {
            // cout << "Iteration: " << iterations + 1 << ", x: " << x << ", y: " << y << ", |x - y|: " << diff << ", gcd(|x - y|, n): " << d << endl;
        // }
    }

    auto end = high_resolution_clock::now();
    double time_ms = duration<double, milli>(end - start).count();

    RunResult res;
    res.time_ms = time_ms;

    if (!(d == ONE) && !(d == n)) {
        res.success = true;
        res.divisor = d;
    } else {
        res.success = false;
    }
    return res;
}


int main(){
    setlocale(LC_ALL, "Russian");
    // получим число функцию ее надо будет сплитануть после чего взять поэлементно, наверное запихнуть в вектор. потом запустить аогоритм 10 раз , каждый раз выдавать время и делитель если не получилось вмето делителя '-'
    // to do 
    //протестировать в базе на значении x^2 + 1.
    // написать скрипт на питоне который будет читать функцию с аргументами из файла и отправлять методу и ждать когда будет доступ к значениям чтобы сохранить их и переносить значения из выхода в эксель
    // сравнить выход скрипта и выход ручками, возможно подправить.
    string num_str,c_s_val;
    int degree;
    
    // Читаем параметры из stdin (их передаст Python)
    if (!(cin >> num_str >> degree >> c_s_val)) {
        return 1;
    }
    string s;
    cout<< "переводим его в большое число\n";
    BiiigNum n = make_num_dec(num_str);
    BiiigNum c_val = make_num_dec(c_s_val);


    // BiiigNum small_div = check_primes(n);
    // if (!(small_div == ONE)) {
        // cout << num_str << "," << degree << "," << c_val << ",0,";
        // vivod(small_div, 10, s);
        // cout << ",0.00,SMALL_PRIME" << endl;
        // return 0;
    // }

    for (int i = 1; i <= 1; i++) {
        cout << "Запуск алгоритма с параметрами: число = " << num_str << ", степень = " << degree << ", c = " << c_val << ", попытка = " << i << endl;
        RunResult res = pollard_rho(n, degree, c_val);
        
        // Формат вывода: Число,Степень,c,Попытка,Делитель,Время,Статус
        cout << num_str << "," << degree << "," << c_val << "," << i << ",";
        
        if (res.success) {
            vivod(res.divisor, 10, s);
            cout << "," << fixed << setprecision(2) << res.time_ms << ",SUCCESS" << endl;
        } else {
            cout << "-," << fixed << setprecision(2) << res.time_ms << ",FAIL" << endl;
        }
    }

    // cout << "дата, функция, число, делитель который нашли";
}