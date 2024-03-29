#include <kernel/task.h>
#include <inc/x86.h>

#define ctx_switch(ts) \
  do { env_pop_tf(&((ts)->tf)); } while(0)

/* TODO: Lab5
* Implement a simple round-robin scheduler (Start with the next one)
*
* 1. You have to remember the task you picked last time.
*
* 2. If the next task is in TASK_RUNNABLE state, choose
*    it.
*
* 3. After your choice set cur_task to the picked task
*    and set its state, remind_ticks, and change page
*    directory to its pgdir.
*
* 4. CONTEXT SWITCH, leverage the macro ctx_switch(ts)
*    Please make sure you understand the mechanism.
*/
void sched_yield(void)
{
	extern Task tasks[];
	extern Task *cur_task;
    int next_task_id = -1;

    /* Find next runnable task. */
    for(int i = (cur_task - tasks + 1) % NR_TASKS ; i != (cur_task - tasks) ; i == NR_TASKS - 1 ? (i = 0) : (++i)) {
        if(tasks[i].state == TASK_RUNNABLE) {
            next_task_id = i;
            break;
        }
    }

    /* Use current task if not found. */
    if(next_task_id == -1)
        next_task_id = cur_task - tasks;
    cur_task = &(tasks[next_task_id]);
    cur_task->remind_ticks = TIME_QUANT;
    cur_task->state = TASK_RUNNING;

    lcr3(PADDR(cur_task->pgdir));
    ctx_switch(cur_task);
}
