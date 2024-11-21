#include "Source.h"

// threadsafe queue class
template <typename T>
class ConcurrentQueue {
    private:
        std::queue<T> queue;
        mutable std::mutex mtx;
        std::condition_variable cv;

    public:
        void Add(const T& item) {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(item);
            cv.notify_one();
        }

        std::optional<T> TryTake() {
            std::unique_lock<std::mutex> lock(mtx);
            if (queue.empty()) {
                return std::nullopt;
            }
            T item = std::move(queue.front());
            queue.pop();
            return item;
        }

        T Take() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !queue.empty(); });
            T item = std::move(queue.front());
            queue.pop();
            return item;
        }

        bool Empty() const {
            std::lock_guard<std::mutex> lock(mtx);
            return queue.empty();
        }

        size_t Size() const {
            std::lock_guard<std::mutex> lock(mtx);
            return queue.size();
        }
};