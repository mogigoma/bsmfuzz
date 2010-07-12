#include "bsmfuzz.h"

token_t *
gen_file(void)
{
	char buf[MAXPATHLEN + 1];
	struct timeval tm;
	token_t *tok;

	/* Choose a path. */
	get_path(buf, sizeof(buf));

	/* Choose a time. */
	/* XXX-MAK: Should use symbolic constants. */
	tm.tv_sec = random() % 4294967295;
	tm.tv_usec = random() % 1000000;

	/* Create token. */
	tok = au_to_file(buf, tm);

	return (tok);
}
