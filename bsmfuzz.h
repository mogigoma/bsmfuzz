#ifndef BSMFUZZ_H_
#define BSMFUZZ_H_

#include <sys/types.h>
#include <sys/uio.h>

#include <dirent.h>
#include <err.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "token_buffer.h"

#define DEVICE_RANDOM	"/dev/urandom"

extern int	 bits;

token_t		*gen_arbitrary(void);
token_t		*gen_file(void);
token_t		*gen_header(void);
token_t		*gen_opaque(void);
token_t		*gen_path(void);
token_t		*gen_text(void);
token_t		*gen_zone(void);

struct dirent	*get_dir_component(DIR *dir);
void		 get_path(char *buf, int size);

#endif
