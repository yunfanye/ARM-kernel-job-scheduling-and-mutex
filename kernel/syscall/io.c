/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>
#include "boundary.h"

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd, void *buf, size_t count)
{
	/* if count > SSIZE_MAX, the result is undefined */
	char c;
	size_t bytes_read = 0;
	char * bufp = buf;
	/* check if fd is stdin */
	if(fd != STDIN_FILENO) {
		return -EBADF;
	}
	/* check if the memory is writable, SDRAM */
	if(!((size_t)buf >= SDRAM_start && (size_t)buf + count <= SDRAM_end)) {
		return -EFAULT;
	}
	while(bytes_read < count) {
		c = getc();
		switch(c) {
			case DEL_CHAR:
			case '\b':
				/* delete last character */
				bytes_read--;
				/* puts "\b \b" */
				puts("\b \b");
				break;
			case '\n':
			case '\r':
				bufp[bytes_read] = '\n';
				bytes_read++;
				putc('\n');
			case EOT_CHAR:
				return bytes_read;
			default:
				bufp[bytes_read] = c;
				bytes_read++;
				putc(c);
				break; 
		}
	}
	return bytes_read;
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd, const void *buf, size_t count)
{
	/* if count > SSIZE_MAX, the result is undefined */
	size_t bytes_written;
	const char * bufp = buf;
	/* check if fd is stdout */
	if(fd != STDOUT_FILENO) {
		return -EBADF;
	}
	/* check if buf is readable, SDRAM or ROM */
	if(!((size_t)buf >= SDRAM_start && (size_t)buf + count <= SDRAM_end) &&
		!((size_t)buf >= ROM_start + 1 && (size_t)buf + count <= ROM_end)) {
		return -EFAULT;
	}
	for(bytes_written = 0; bytes_written < count; bytes_written++) {
		putc(bufp[bytes_written]);
	}
	return bytes_written;
}

