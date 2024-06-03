#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "async.h"

void print_hello(Task *task, resolve_func res, reject_func rej)
{
	srand(time(NULL));
	int r = rand();
	if (r % 2 == 0) {
		rej(task, "Random number is not even");
		return;
	}
    res(task, "Hello, World!");
}

int main(int argc, char** argv)
{
    AsyncState* state = async_init();
    Task* hello_message_task = async_func(state, print_hello);

    printf("Some thing to do...\n");

	TaskResponse hello_response = await(hello_message_task);

	if (hello_response.reject != NULL) {
		printf("Async Error Message: %s\n", (char*)hello_response.reject);
		return 1;
	}

    printf("Awaited Message: %s\n", (char*)hello_response.resolve);

    async_close(state);

    return 0;
}
