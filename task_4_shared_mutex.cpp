#include <thread>
#include <shared_mutex>
#include <iostream>
#include <mutex>


namespace read_write {
    std::shared_mutex shared_mx;
    int variable = 0;
   
    int Read() {
        std::shared_lock lck(shared_mx);
        return variable;
    }

    void Write(const int& x) {
        std::unique_lock lck(shared_mx);
        variable = x;
    }
}



int main() {
    std::thread th1(read_write::Write, 13);
    std::thread th2(read_write::Read);
    std::thread th3(read_write::Read);
    std::thread th4(read_write::Read);
    
    std::thread th5(read_write::Write, 14);
    std::thread th6(read_write::Read);
    std::thread th7(read_write::Read);
    std::thread th8(read_write::Read);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
}
