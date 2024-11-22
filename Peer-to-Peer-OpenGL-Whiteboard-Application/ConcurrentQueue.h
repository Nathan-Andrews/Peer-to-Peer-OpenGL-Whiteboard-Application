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

        std::vector<T> Snapshot() const {
            std::lock_guard<std::mutex> lock(mtx);
            std::vector<T> snapshot;
            std::queue<T> temp = queue;
            while (!temp.empty()) {
                snapshot.push_back(temp.front());
                temp.pop();
            }
            return snapshot;
        }

        void Iterate(std::function<void(const T&)> func) const {
            auto snapshot = Snapshot();
            for (const T& item : snapshot) {
                func(item);
            }
        }
        
        bool erase(const T& value) {
            std::lock_guard<std::mutex> lock(mtx);

            // Temporary queue to hold non-matching elements
            std::queue<T> tempQueue;
            bool found = false;

            while (!queue.empty()) {
                if (!found && queue.front() == value) {
                    found = true; // Mark as found and skip the element
                    queue.pop();
                } else {
                    tempQueue.push(std::move(queue.front()));
                    queue.pop();
                }
            }

            // Swap back the remaining elements
            queue.swap(tempQueue);

            return found;
        }
};