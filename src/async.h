#pragma once

#include <stdbool.h>

typedef struct Task Task;
typedef struct {
    void* resolve;
    void* reject;
} TaskResponse;
typedef struct AsyncState AsyncState;

typedef void (*resolve_func)(Task *task, void*);
typedef void (*reject_func)(Task *task, void*);
typedef void (*task_call_back)(Task *task, resolve_func resolve, reject_func reject); 
typedef void (*task_update)(Task *task); 

AsyncState* async_init();
AsyncState* async_init_priority(int priority_start, int priority_end);
void async_close(AsyncState* state);
bool async_is_finished(AsyncState* state);
Task* async_func(AsyncState* state, task_call_back call_back);
Task* async_func_priority(AsyncState* state, task_call_back call_back, int priority);
void async_update(AsyncState* state);

void set_task_update(AsyncState* state, Task *task, task_update update);
void finish_task(Task *task);
void wait_task(Task *task);
TaskResponse await(Task *task);
