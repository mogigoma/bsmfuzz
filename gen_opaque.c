#include "bsmfuzz.h"

token_t *
gen_opaque(void)
{
	token_t *tok;
	int i, size;
	char *buf;

	/*
	 * Choose the length of the data. Note that the length is limited here
	 * to a less than what it can actually hold (2^16 - 1). The reason for
	 * this is that the maximum record length is the significantly less.
	 */
	/* XXX-MAK: Should use symbolic constants. */
	size = random() % 20000;

	/* Generate the random data. */
	buf = malloc(size);
	if (buf == NULL)
		err(1, "[malloc]");

	for (i = 0; i < size; i++)
		/* XXX-MAK: Should use symbolic constants. */
		buf[i] = random() % 256;

	/* Create token. */
	tok = au_to_opaque(buf, size);

	/* Clean up. */
	free(buf);

	return (tok);
}
