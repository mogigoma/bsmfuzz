#ifndef TOKEN_BUFFER_H_
#define TOKEN_BUFFER_H_

#include <bsm/libbsm.h>

#include <stdint.h>

struct token_buffer {
	uint8_t	 data[MAX_AUDIT_RECORD_SIZE];
	size_t	 used;
};

int	 rec_append(struct token_buffer *tb, token_t *tok);
int	 rec_close(struct token_buffer *tb);
int	 rec_open(struct token_buffer *tb);
int	 rec_write(struct token_buffer *tb, int fd);

#endif
