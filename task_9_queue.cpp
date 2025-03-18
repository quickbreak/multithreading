#include <queue>
#include <thread>
#include <mutex>
#include <set>
#include <iostream>

struct ThreadSafeQueue {
private:
    std::queue<int> q_;
    std::mutex mx_;
public:
    void Push(int value) {
        std::scoped_lock(mx_);
        q_.push(value);
    }
    
    int Front() {
        std::scoped_lock(mx_);
        if (!q_.empty()) {
            return q_.front();
        } else {
            return -1;
        }
    }

    int Pop() {
        std::scoped_lock(mx_);
        int temp;
        if (!q_.empty()) {
            temp = q_.front();
            q_.pop();
        } else {
            temp = -1;
        }
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
    std::thread th2(ThreadSafeQueueWorking::Pusher);
    std::thread th3(ThreadSafeQueueWorking::Reader);
    
    th2.join();
    th3.join();

    std::cout << (ThreadSafeQueueWorking::st.size() == 1000);
}
