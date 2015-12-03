#include <exports.h>
#include <lock.h>
#include <syscall.h>
#include <bits/swi.h>


ssize_t C_SWI_handler(int swi_num, ssize_t code, size_t * stack) {
	int fd;
	char * buf;
	ssize_t count;
	ssize_t mutex;
    ssize_t dev;
    task_t* task;

	switch(swi_num) {
		case READ_SWI:
			fd = code;
			buf = (char *)stack[0];
			count = stack[1];
			return read_syscall(fd, buf, count);
		case WRITE_SWI:
			fd = code;
			buf = (char *)stack[0];
			count = stack[1];
			return write_syscall(fd, buf, count);
		case TIME_SWI:
			return time_syscall();
		case SLEEP_SWI:
			sleep_syscall(code);
			break;
		case CREATE_SWI:
            // Extract parameters
            task = (task_t*) code;
            count = stack[0];
            return task_create(task, count);
        case MUTEX_CREATE:
            return mutex_create();
        case MUTEX_LOCK:
            // Extract parameters
            mutex = code;
            return mutex_lock(mutex);
        case MUTEX_UNLOCK:
            // Extract parameters
            mutex = code;
            return mutex_unlock(mutex);
        case EVENT_WAIT:
            // Extract parameters
            dev = code;
            return event_wait(dev);
		default:
			/* invalid syscall, exit with 0x0badc0de */
			invalid_syscall(swi_num);
	}
	return 0;
}