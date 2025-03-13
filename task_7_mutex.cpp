#include <thread>
#include <mutex>
#include <iostream>
#include <vector>


namespace files {
    std::mutex mx;
        
    void worker() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::scoped_lock lck(mx);
        std::cout << "Work is done. ((c) " << std::this_thread::get_id() << ")\n";
    }
}


int main() {
    std::vector<std::thread>threads;
    for (int i = 0; i < 33; ++i) {
        // threads.push_back(std::thread(files::worker));
        threads.emplace_back(files::worker);
    }

    for(auto& thr: threads) {
        thr.join();
    }
}
