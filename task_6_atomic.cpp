#include <thread>
#include <atomic>
#include <iostream>


std::atomic<bool> flag = false;


void worker () {
    int time = 1000'000'000;
    while (time > 0 && !flag) {
        std::cout << "Время уходит!: " << time-- << '\n';
    }

    if (time > 0) {
        std::cout << "Выполнение прервано\n";
    } else {
        std::cout << "Выполнение закончено\n";
    }
}


int main() {
    std::thread th(worker);

    int x;
    std::cin >> x;
    flag = true;

    th.join();


    std::cout << x;
}