#include "bsmfuzz.h"

token_t *
gen_header(void)
{
	struct timeval tm;
	token_t *tok;

	/* Choose the time. */
	/* XXX-MAK: Should use symbolic constants. */
	tm.tv_sec = random() % 4294967295;
	tm.tv_usec = random() % 1000000;

	/* Create token. */
	if (bits == 32)
		tok = au_to_header32_tm(0, 0, 0, tm);
	else
		tok = au_to_header64_tm(0, 0, 0, tm);

	return (tok);
}
