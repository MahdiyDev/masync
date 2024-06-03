# Asynchronous programming in C programming language

An asynchronous library with a very simple implementation. Ability to manage priority.

# Features
- Delayed (asynchronous) function calls.
- Async function priority control
- Controlable event loop

# Build project
To build project:
```shell
make
```
It creates a folder called `build`. Example executables in the folder called `build/examples`.

# Examples
The await keyword allows you to pause execution until an asynchronous operation (like a promise) completes.

```c
#include <stdio.h>
#include "async.h"

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
```
There are more examples in the [examples folder](examples/).
