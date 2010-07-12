#include "bsmfuzz.h"

token_t *
gen_text(void)
{
	int acceptable, i, size;
	char *buf, ch;
	token_t *tok;

	/* Choose the length of the data. */
	/* XXX-MAK: Should use symbolic constants. */
	size = random() % 20000;

	/* Generate the random printable data. */
	buf = calloc(size, sizeof(char));
	if (buf == NULL)
		err(1, "[calloc]");

	for (i = 0; i < size - 1; i++) {
		acceptable = 0;
		while (!acceptable) {
			ch = random() % (' ' - '~' + 1) + ' ';

			/* XXX-MAK: This shouldn't be necessary. */
			if (ch != '&' &&
			    ch != '<' &&
			    ch != '>' &&
			    ch != '"' &&
			    ch != '\'')
				acceptable = 1;
		}
		buf[i] = ch;
	}
	buf[i] = '\0';

	/* Create token. */
	tok = au_to_text(buf);

	/* Clean up. */
	free(buf);

	return (tok);
}
