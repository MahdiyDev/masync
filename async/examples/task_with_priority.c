#include <stdio.h>
#include "../async.h"

enum TaskPriorities {
    LOW = 0,
    MID,
    HIGH,
};

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
    Task* some_message_task = async_func_priority(state, some_thing_func, MID);

    printf("Awaited Message: %s\n", (char*)await(hello_message_task).resolve);
    printf("Awaited Message: %s\n", (char*)await(some_message_task).resolve);

    async_close(state);

    return 0;
}
