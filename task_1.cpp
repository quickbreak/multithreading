#include <thread>
#include <vector>
#include <iostream>

int res;

void Summ(const std::vector<int>&a, int l, int r) {
    for (int i = l; i < r; ++i) {
        res += a[i];
    }
}

int main() {
    std::vector<int>a(12001);
    a.reserve(12011);
    for (int i = 1; i <= 12000; ++i) {
        a[i] = i;
    }
    // (1 + 12000) * 12000 / 2 = 12001 * 6000 = 72 006 000
    res = 0;
    std::thread th1(Summ, a, 1, 3001);
    std::thread th2(Summ, a, 3001, 6001);
    std::thread th3(Summ, a, 6001, 9001);
    std::thread th4(Summ, a, 9001, 12001);
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    std::cout << res << '\n'; 
    /* запуски:
    72006000
    67591307
    48176676
    вывод: хорошо, потоки делают своё дело, но нужны примитивы синхронизации
    */
}