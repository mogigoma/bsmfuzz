#include "bsmfuzz.h"

token_t *
gen_path(void)
{
	char buf[MAXPATHLEN + 1];
	token_t *tok;

	/* Choose a random path. */
	get_path(buf, sizeof(buf));

	/* Create token. */
	tok = au_to_path(buf);

	return (tok);
}
