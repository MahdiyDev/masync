#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "async.h"

#ifndef TASK_COUNT
    #define TASK_COUNT 64
#endif

typedef enum Status {
    ASYNC_INIT = 0,
    ASYNC_WAITING,
    ASYNC_FINISHED,
} Status;

typedef struct Task {
    void* resolve;
    void* reject;
    int priority;
    Status status;
    task_call_back call_back;
    task_update update;
} Task;

typedef struct Tasks {
    int index_priority;
    int taskCounter;
    Task* items;
} Tasks;

typedef struct AsyncState {
    Tasks* tasks;
    int task_count;
    int priority_start;
    int priority_end;

    bool is_finished;
} AsyncState;

void set_resolve(Task *task, void* resolve)
{
    task->resolve = resolve;
}

void set_reject(Task *task, void* reject)
{
    task->reject = reject;
}

void set_task_update(AsyncState* state, Task *task, task_update update)
{
    task->status = ASYNC_WAITING;
    task->update = update;
    state->is_finished = false;
}

TaskResponse await(Task *task)
{
    task->call_back(task, set_resolve, set_reject);
    return (TaskResponse) {
		.resolve = task->resolve,
		.reject = task->reject,
	};
}

AsyncState* async_init_priority(int priority_start, int priority_end)
{
    assert(priority_start <= priority_end);
    AsyncState* state = (AsyncState*)malloc(sizeof(AsyncState));

    state->priority_start = priority_start;
    state->priority_end = priority_end;

    int priority_diff = priority_end - priority_start;

    state->tasks = (Tasks*)malloc(priority_diff * sizeof(Tasks));
    state->task_count = priority_diff;

    for (int i = 0, j = priority_start; i <= priority_diff; i++) {
        state->tasks[i].taskCounter = 0;
        state->tasks[i].index_priority = j;
        state->tasks[i].items = (Task*)malloc(TASK_COUNT*sizeof(Task));
        for (int k = 0; k < TASK_COUNT; k++) {
            state->tasks[i].items[k].priority = priority_start;
            state->tasks[i].items[k].status = ASYNC_INIT;
            state->tasks[i].items[k].resolve = NULL;
            state->tasks[i].items[k].reject = NULL;
            state->tasks[i].items[k].call_back = NULL;
            state->tasks[i].items[k].update = NULL;
        }
        j++;
    }

    return state;
}

AsyncState* async_init()
{
    return async_init_priority(0, 0);
}

void async_close(AsyncState* state)
{
    free(state->tasks);
}

bool async_is_finished(AsyncState* state)
{
    state->is_finished = true;

    for (int i = 0; i <= state->task_count; i++) {
        for (int j = 0; j < state->tasks[i].taskCounter; j++) {
            if (state->tasks[i].items[j].status == ASYNC_WAITING) {
                state->is_finished = false;
            }
        }
    }

    return state->is_finished;
}

void async_update(AsyncState* state)
{
    for (int i = state->task_count; i >= 0; i--) {
        for (int j = 0; j < state->tasks[i].taskCounter; j++) {
            Task* currentTask = &state->tasks[i].items[j];
            if (currentTask->update != NULL && currentTask->status == ASYNC_WAITING) {
                currentTask->update(currentTask);
            }
        }
    }
}

void finish_task(Task *task)
{
    task->status = ASYNC_FINISHED;
}

void wait_task(Task *task)
{
    task->status = ASYNC_WAITING;
}

Task* async_func_priority(AsyncState* state, task_call_back call_back, int priority)
{
    assert(state->priority_start <= priority && state->priority_end >= priority);
    int priority_diff = state->priority_end - state->priority_start;
    int task_index = abs(state->priority_start - priority);

    Task* currentTask = &state->tasks[task_index].items[state->tasks[task_index].taskCounter];

    currentTask->call_back = call_back;
    state->tasks[task_index].taskCounter++;

    return currentTask;
}

Task* async_func(AsyncState* state, task_call_back call_back)
{
    return async_func_priority(state, call_back, state->priority_start);
}