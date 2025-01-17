#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>
#include <vector>


namespace atomic {
    std::mutex mx;
    std::atomic<bool> flag = false;
    std::vector<int> a (40);
    int value = 40;

    void init () {
        for (int i = 0; i < a.size(); ++i) {
            a[i] = i + 1;
        }
    }

    void Find(int l, int r) {
        for (int i = l; i < r; ++i) {
            // std::scoped_lock lck(mx);
            if (flag) {
                std::scoped_lock lck(mx);
                std::cout << l << ' ' << r << " checked " << i - l << '\n';
                return;
            }
            // std::cout << std::this_thread::get_id() << " checked " << i << " " << a[i] << '\n';
            if (a[i] == value) {
                std::cout << "FOUND\n";
                flag = true;
            }
        }
        std::scoped_lock lck(mx);
        std::cout << l << ' ' << r << " checked all" << '\n';
    }
}


int main() {
    atomic::init();

    std::thread th1(atomic::Find, 0, 10);
    std::thread th2(atomic::Find, 10, 20);
    std::thread th3(atomic::Find, 20, 30);
    std::thread th4(atomic::Find, 30, 40);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
}
