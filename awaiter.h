#pragma once

#include <coroutine>

// Контролер очікування: вся логіка призупинення тут.
// Парне число   -> await_ready() == true  -> сопрограма працює далі.
// Непарне число -> await_ready() == false -> await_suspend() -> пауза.
class OddAwaiter
{
private:
    int value;

public:
    explicit OddAwaiter(int value) : value(value) {}

    bool await_ready() const noexcept { return value % 2 == 0; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    void await_resume() const noexcept {}
};
