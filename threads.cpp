#include <thread>
#include <iostream>
#include <mutex>


namespace id_printer {
    std::mutex mx;
    void print_id () {
        std::scoped_lock lck(mx);
        std::cout << std::this_thread::get_id() << '\n';
        return;
    }
}


int main() {
    std::thread th1(id_printer::print_id);
    std::thread th2(id_printer::print_id);
    std::thread th3(id_printer::print_id);
    std::thread th4(id_printer::print_id);
    std::thread th5(id_printer::print_id);
    std::thread th6(id_printer::print_id);
    std::thread th7(id_printer::print_id);
    std::thread th8(id_printer::print_id);

    th1.detach();
    th2.detach();
    th3.detach();
    th4.detach();
    th5.detach();
    th6.detach();
    th7.detach();
    th8.detach();


    std::scoped_lock lck(id_printer::mx);
    int x;
    std::cout << &x << '\n';
    {
        int x;
        std::cout << &x << '\n'; // (1)
    }
    int y;
    std::cout << &y << '\n'; // тот же, что и в (1)
}
