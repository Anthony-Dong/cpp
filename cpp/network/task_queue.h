#pragma once

#include <functional>
#include <vector>
#include <list>
#include <thread>
#include <cassert>

namespace cpp::network::utils {

struct TaskQueue {
    void AddTask(const std::function<void()> &foo) {
        assert(foo != nullptr);
        tasks.push_back(new Task(foo));
        cleanTask();
    }

    ~TaskQueue() {
        for (const auto &item : tasks) {
            delete item;
        }
    }

private:
    void cleanTask() {
        std::vector<Task *> closed_tasks{};
        for (const auto &task : tasks) {
            if (task->isDone()) {
                closed_tasks.push_back(task);
            }
        }
        for (const auto &task : closed_tasks) {
            tasks.remove(task);
            delete task;
        }
    }

private:
    struct Task {
        explicit Task(const std::function<void()> &foo) {
            assert(foo != nullptr);
            thread_ = std::thread([async_func = foo, this]() {
                async_func();
                is_done_ = true;
            });
        };

        ~Task() {
            if (thread_.joinable()) {
                thread_.join();
            }
        }

        bool isDone() {
            return is_done_;
        }

    private:
        bool is_done_{};
        std::thread thread_{};
    };

private:
    std::list<Task *> tasks;
};

} // namespace cpp::network::utils