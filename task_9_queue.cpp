#include <queue>
#include <thread>
#include <mutex>
#include <set>
#include <iostream>
#include <condition_variable>

struct ThreadSafeQueue {
private:
    std::queue<int> q_;
    std::mutex mx_;
    std::condition_variable cv_;
public:
    void Push(int value) {
        {
            std::scoped_lock(mx_);
            q_.push(value);
        }
        cv_.notify_one();
    }
    
    int Front() {
        std::unique_lock lock(mx_);
        cv_.wait(lock, [this] {
            return !q_.empty();
        });
        return q_.front();
    }

    int Pop() {
        std::unique_lock lock(mx_);
        cv_.wait(lock, [this] {
            return !q_.empty();
        });
        int temp = q_.front();
        q_.pop();
        return temp;
    }
};


namespace ThreadSafeQueueWorking {
    ThreadSafeQueue q;
    std::set<int> st;
    void Pusher() {
        for (int i = 0; i < 1000; ++i) {
            q.Push(i);
        }
    }

    void Reader() {
        for (int i = 0; i < 1000; ++i) {
            st.insert(q.Pop());
            if (*st.begin() == -1) {
                st.erase(st.begin());
            }
        }
    }
}


int main() {
    std::thread th3(ThreadSafeQueueWorking::Reader);
    std::thread th2(ThreadSafeQueueWorking::Pusher);
    
    th2.join();
    th3.join();

    std::cout << (ThreadSafeQueueWorking::st.size() == 1000);
}
