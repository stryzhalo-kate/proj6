#pragma once

#include <coroutine>
#include <exception>
#include <utility>

// Тип, що повертає сопрограма. Володіє coroutine_handle і знищує кадр у деструкторі.
class Task
{
public:
    struct promise_type
    {
        std::exception_ptr exception;

        Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

private:
    std::coroutine_handle<promise_type> handle;

    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}

public:
    Task(const Task &) = delete;
    Task &operator=(const Task &) = delete;
    Task(Task &&other) noexcept : handle(std::exchange(other.handle, {})) {}
    ~Task()
    {
        if (handle)
            handle.destroy();
    }

    bool done() const { return !handle or handle.done(); }

    void resume()
    {
        if (done())
            return;
        handle.resume();
        if (handle.promise().exception)
            std::rethrow_exception(std::exchange(handle.promise().exception, nullptr));
    }
};
