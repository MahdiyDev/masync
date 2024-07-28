#include <stdio.h>
#include "../async.h"

void print_hello(Task *task, resolve_func res, reject_func rej)
{
    res(task, "Hello, World!");
}

int main(int argc, char** argv)
{
    AsyncState* state = async_init();
    Task* hello_message_task = async_func(state, print_hello);

    printf("Some thing to do...\n");

    printf("Awaited Message: %s\n", (char*)await(hello_message_task).resolve);

    async_close(state);

    return 0;
}
