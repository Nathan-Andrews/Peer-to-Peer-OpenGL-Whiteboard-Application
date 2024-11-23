#include "Source.h"

// A thread-safe container for managing a queue with basic operations like Add, Take, and TryTake.
// This class ensures that all operations are performed safely in multithreaded environments.
template <typename T>
class ConcurrentContainer {
    private:
        // Internal queue to store elements of type T.
        std::queue<T> queue;

        // Mutex to ensure thread-safety for operations on the queue.
        mutable std::mutex mtx;

        // Condition variable to notify threads when an item is available to take.
        std::condition_variable cv;

    public:
        // Adds an item to the queue and notifies one waiting thread.
        void Add(const T& item) {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex for thread-safety.
            queue.push(item); // Add the item to the queue.
            cv.notify_one(); // Notify one waiting thread that an item is available.
        }

        // Tries to take an item from the queue without blocking. 
        // Returns std::nullopt if the queue is empty.
        std::optional<T> TryTake() {
            std::unique_lock<std::mutex> lock(mtx); // Lock the mutex for thread-safety.
            if (queue.empty()) {
                return std::nullopt; // Return null if the queue is empty.
            }
            T item = std::move(queue.front()); // Take the front item.
            queue.pop(); // Remove it from the queue.
            return item; // Return the taken item.
        }

        // Takes an item from the queue, blocking if the queue is empty until an item is available.
        T Take() {
            std::unique_lock<std::mutex> lock(mtx); // Lock the mutex for thread-safety.
            cv.wait(lock, [this] { return !queue.empty(); }); // Wait until the queue is not empty.
            T item = std::move(queue.front()); // Take the front item.
            queue.pop(); // Remove it from the queue.
            return item; // Return the taken item.
        }

        // Checks if the queue is empty.
        bool Empty() const {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex for thread-safety.
            return queue.empty(); // Return whether the queue is empty.
        }

        // Returns the number of items in the queue.
        size_t Size() const {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex for thread-safety.
            return queue.size(); // Return the size of the queue.
        }

        // Iterates over all items in the queue (snapshot is taken to avoid locking during iteration).
        // The provided function is applied to each item.
        void Iterate(std::function<void(const T&)> func) const {
            auto snapshot = Snapshot(); // Take a snapshot of the current queue contents.
            for (const T& item : snapshot) {
                func(item); // Apply the function to each item in the snapshot.
            }
        }
        
        // Erases the first occurrence of the specified value from the queue, returning true if found.
        bool Remove(const T& value) {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex for thread-safety.

            // Temporary queue to hold non-matching elements
            std::queue<T> tempQueue;
            bool found = false; // Flag to indicate if the element was found.

            // Iterate through the queue, copying all elements except the one to be erased.
            while (!queue.empty()) {
                if (!found && queue.front() == value) {
                    found = true; // Mark as found and skip the element.
                    queue.pop(); // Remove the element.
                } else {
                    tempQueue.push(std::move(queue.front())); // Keep other elements.
                    queue.pop(); // Remove the element from the original queue.
                }
            }

            // Swap back the remaining elements into the original queue.
            queue.swap(tempQueue);

            return found; // Return whether the element was found and removed.
        }

    private:
        // Creates a snapshot of the current queue contents. This method does not modify the queue.
        std::vector<T> Snapshot() const {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex for thread-safety.
            std::vector<T> snapshot; // Vector to hold the snapshot of the queue.
            std::queue<T> temp = queue; // Make a copy of the queue to iterate over.
            while (!temp.empty()) {
                snapshot.push_back(temp.front()); // Add each item to the snapshot.
                temp.pop(); // Remove the item from the copy.
            }
            return snapshot; // Return the snapshot.
        }
};
