#ifndef QEMU_DEBUG_H
#define QEMU_DEBUG_H

#define QEMU_DBG_IO 0xE9
#define dbg(s) qemu_debug(s)

void qemu_debug(const char *);

#endif
