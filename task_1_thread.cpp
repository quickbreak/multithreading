#include <thread>
#include <vector>
#include <iostream>
#include <mutex>

int res;

void SummWithoutMutex(const std::vector<int>&a, int l, int r) {
    for (int i = l; i < r; ++i) {
        res += a[i];
    }
}

void WithoutMutex() {
    std::vector<int>a(12001);
    a.reserve(12011);
    for (int i = 1; i <= 12000; ++i) {
        a[i] = i;
    }
    // (1 + 12000) * 12000 / 2 = 12001 * 6000 = 72 006 000
    res = 0;
    std::thread th1(SummWithoutMutex, a, 1, 3001);
    std::thread th2(SummWithoutMutex, a, 3001, 6001);
    std::thread th3(SummWithoutMutex, a, 6001, 9001);
    std::thread th4(SummWithoutMutex, a, 9001, 12001);
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

std::mutex summ_mx;

void SummWithMutex(const std::vector<int>&a, int l, int r) {
    for (int i = l; i < r; ++i) {
        std::lock_guard lck(summ_mx);
        res += a[i];
    }
}

void WithMutex() {
    std::vector<int>a(12001);
    a.reserve(12011);
    for (int i = 1; i <= 12000; ++i) {
        a[i] = i;
    }
    // (1 + 12000) * 12000 / 2 = 12001 * 6000 = 72 006 000
    res = 0;
    std::thread th1(SummWithMutex, a, 1, 3001);
    std::thread th2(SummWithMutex, a, 3001, 6001);
    std::thread th3(SummWithMutex, a, 6001, 9001);
    std::thread th4(SummWithMutex, a, 9001, 12001);
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    std::cout << res << '\n'; 
    /* запуски:
    сколько ни запускал, вывод 72006000
    вывод: примитивы синхронизации - тема
    */
}


int main() {
    WithoutMutex();
    WithMutex();
}
