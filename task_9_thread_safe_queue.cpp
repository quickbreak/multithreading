#include <queue>
#include <thread>
#include <mutex>
#include <set>
#include <iostream>
#include <condition_variable>


template <typename T>
struct ThreadSafeQueue {
private:
    std::queue<T> q_;
    std::mutex mx_;
    std::condition_variable cv_;
public:
    void Push(T value) {
        {
            std::scoped_lock(mx_);
            q_.push(value);
        }
        cv_.notify_one();
    }
    
    T Front() {
        std::unique_lock lock(mx_);
        cv_.wait(lock, [this] {
            return !q_.empty();
        });
        return q_.front();
    }

    T Pop() {
        std::unique_lock lock(mx_);
        cv_.wait(lock, [this] {
            return !q_.empty();
        });
        T temp = q_.front();
        q_.pop();
        return temp;
    }
};


namespace ThreadSafeQueueWorking {
    ThreadSafeQueue<int> q;
    std::set<int> st;
    void Pusher() {
        for (int i = 0; i < 1000; ++i) {
            q.Push(i);
        }
    }

    void Reader() {
        for (int i = 0; i < 1000; ++i) {
            st.insert(q.Pop());
        }
    }
}


int main() {
    std::thread th3(ThreadSafeQueueWorking::Pusher);
    std::thread th2(ThreadSafeQueueWorking::Reader);
    
    th2.join();
    th3.join();

    std::cout << (ThreadSafeQueueWorking::st.size() == 1000);
}
