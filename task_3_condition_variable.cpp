#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <iostream>


namespace id_printer {
    std::mutex condition_mx;
    std::condition_variable cv;
    std::vector<int32_t> buffer(10);
    bool produced = false;

    void Produce () {
        condition_mx.lock();
        
        for (int8_t i = 0; i < 10; ++i) {
            buffer[i] = 100500 * i;
            std::cout << "Producer just buffered: " << buffer[i] << '\n';
        }

        produced = true;
        condition_mx.unlock();
        cv.notify_all();
    }

    void Consumer () {
        std::unique_lock<std::mutex> lck(condition_mx);
        cv.wait(lck, []{return produced;});

        for (int8_t i = 0; i < 10; ++i) {
            std::cout << "Consumer just got: " << buffer[i] << '\n';
        }
    }
}


namespace second_variant {
    std::mutex condition_mx;
    std::condition_variable cv;
    std::vector<int32_t> buffer(10);

    void Produce () {
        for (int8_t i = 0; i < 10; ++i) {
            condition_mx.lock();
            buffer[i] = i + 1;
            std::cout << "Producer just buffered: " << buffer[i] << '\n';
            condition_mx.unlock();
            cv.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void Consumer () {
        std::unique_lock<std::mutex> lck(condition_mx);
        for (int8_t i = 0; i < 10; ++i) {
            cv.wait(lck);
            std::cout << "Consumer: ";
            for (uint8_t j = 0; j < 10; ++j) {
                std::cout << buffer[j] << ' ';
            }
            std::cout << '\n';
        }
    }
}


int main() {
    std::thread consumer(id_printer::Consumer);
    std::thread producer(id_printer::Produce);

    consumer.join();
    producer.join();

    std::cout << "---------------------------------------------------------------------\n";

    std::thread consumer2(second_variant::Consumer);
    std::thread producer2(second_variant::Produce);

    consumer2.join();
    producer2.join();
}