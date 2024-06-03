#include <stdio.h>
#include "async.h"

enum TaskPriorities {
    LOW = 0,
    MID,
    HIGH,
};

void hello_task_update(Task *task)
{
    static int a = 0;
    if (a == 1000) {
        finish_task(task);
        printf("Message: %s\n", (char*)await(task).resolve);
    } else {
        wait_task(task);
        a++;
    }
}

void some_task_update(Task *task)
{
    finish_task(task);
    printf("Message: %s\n", (char*)await(task).resolve);
}

void hello_func(Task *task, resolve_func res, reject_func rej)
{
    res(task, "Hello, World!");
}

void some_thing_func(Task *task, resolve_func res, reject_func rej)
{
    res(task, "Some thing else...");
}

int main(int argc, char** argv)
{
    AsyncState* state = async_init_priority(LOW, HIGH);

    Task* hello_message_task = async_func_priority(state, hello_func, LOW);
    set_task_update(state, hello_message_task, hello_task_update);
    Task* some_message_task = async_func_priority(state, some_thing_func, MID);
    set_task_update(state, some_message_task, some_task_update);

    while (!async_is_finished(state)) {
        async_update(state);
    }

    printf("Awaited Message: %s\n", (char*)await(hello_message_task).resolve);
    printf("Awaited Message: %s\n", (char*)await(some_message_task).resolve);

    async_close(state);

    return 0;
}
