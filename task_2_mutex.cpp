#include <thread>
#include <mutex>
#include <iostream>


uint32_t balance_ = 0;
std::mutex balance_mx_;

void SetMnogo (uint32_t money) {
    balance_mx_.lock();
    /*
    не зная о синхронизации:
    даже без задержки потока, если запустим код несколько раз, получим разные результаты: то 103050, то 52
    а если в операции этого типа действительно предусмотрены какие-то действия (что имитируется усыплением потока),
    ответ и вообще всегда будет не тот, который ожидаем
    */
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    balance_ = money;
    balance_mx_.unlock();
}

void SetMalo (uint32_t money) {
    balance_mx_.lock();
    balance_ = money;
    balance_mx_.unlock();
}

auto GetBalance() {
    return balance_;
}

void Process () {
    uint32_t money1 = 103050;
    uint32_t money2 = 52;
    std::thread th1(SetMnogo, money1);
    std::thread th2(SetMalo, money2);
    th1.join();
    th2.join();
    std::cout << GetBalance() << '\n';
}


int main() {
    Process();
}