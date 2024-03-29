/* See COPYRIGHT for copyright information. */

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/assert.h>

extern int vprintk(const char *fmt, va_list ap);

const char *panicstr = NULL;

/*
 * Panic is called on unresolvable fatal errors.
 * It prints "panic: mesg", and then enters the kernel monitor.
 */
void
_panic(const char *file, int line, const char *fmt,...)
{
	va_list ap;

	if (panicstr)
		goto dead;
	panicstr = fmt;

	// Be extra sure that the machine is in as reasonable state
	__asm __volatile("cli; cld");

	va_start(ap, fmt);
	printk("kernel panic at %s:%d: ", file, line);
	vprintk(fmt, ap);
	printk("\n");
	va_end(ap);

dead:
	while(1);
}

/* like panic, but don't */
void
_warn(const char *file, int line, const char *fmt,...)
{
	va_list ap;

	va_start(ap, fmt);
	printk("kernel warning at %s:%d: ", file, line);
	vprintk(fmt, ap);
	printk("\n");
	va_end(ap);
}
