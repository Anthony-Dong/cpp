#include <functional>
#include <iostream>
#include <list>

struct task {
    bool is_ready() const {
        return is_ready_();
    }
    explicit task(const std::function<bool()>& is_ready) : is_ready_(is_ready) {
    }

private:
    std::function<bool()> is_ready_;
};

template <typename T>
struct executor {

    void push(const T& task) {
        this->tasks.push_back(task);
    }

    void run() {
        for (const auto& item : this->tasks) {
            std::cout << item.is_ready() << "\n";
        }
    }

private:
    std::list<T> tasks;
};

void append(executor<task>& executors) {
    executors.push(task([]() -> bool {
        return true;
    }));
}
int main() {
    executor<task> executors;
    append(executors);
    executors.run();
}