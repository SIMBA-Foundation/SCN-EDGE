#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <common/atomic.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <libservalctrl/message_channel.h>

typedef struct message {
	atomic_t refcount;
    channel_addr_t from;
    socklen_t from_len;
	unsigned int length;
    unsigned int alloc_len;
	unsigned char data[0];
} message_t;

static inline void message_hold(struct message *m)
{
	atomic_inc(&m->refcount);
}

static inline void message_put(struct message *m)
{
	if (atomic_dec_and_test(&m->refcount))
		free(m);
}

message_t *message_alloc(const void *data, size_t len);

#endif /* MESSAGE_H_ */
