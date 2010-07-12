#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>

#include "token_buffer.h"

int
rec_append(struct token_buffer *tb, token_t *tok)
{
	size_t len;

	/* Ensure we have a valid arguments. */
	if (tb == NULL || tok == NULL)
		return (-1);

	/* Ensure there's enough space left. */
	if (tb->used >= MAX_AUDIT_RECORD_SIZE)
		return (-1);

	/* Append token to record. */
	len = MAX_AUDIT_RECORD_SIZE - tb->used;
	if (au_close_token(tok, &tb->data[tb->used], &len) != 0)
		return (-1);

	/* Update record size. */
	tb->used += len;

	return (0);
}

int
rec_close(struct token_buffer *tb)
{
	uint32_t len;

	/* Ensure we have a valid arguments. */
	if (tb == NULL)
		return (-1);

	/* Append the record trailer. */
	rec_append(tb, au_to_trailer(0));

	/*
	 * XXX-MAK: There's got to be a better approach to setting the record
	 * length. On the upside, this hack works for all four header types.
	 */
	len = htonl(tb->used);
	memmove(&tb->data[1], &len, sizeof(len));
	memmove(&tb->data[tb->used - 4], &len, sizeof(len));

	return (0);
}

int
rec_open(struct token_buffer *tb)
{

	/* Ensure we have a valid arguments. */
	if (tb == NULL)
		return (-1);

	tb->used = 0;

	return (0);
}

int
rec_write(struct token_buffer *tb, int fd)
{
	size_t i, len;

	/* Ensure we have a valid arguments. */
	if (tb == NULL || fd < 0)
		return (-1);

	/* Write buffer. */
	for (i = 0; i < tb->used; i += len) {
		len = write(fd, &tb->data[i], tb->used - i);
		if (len < 0)
			return (-1);
	}

	return (0);
}
