#include "bsmfuzz.h"

static int print_vals[] = {
	AUP_BINARY,
	AUP_DECIMAL,
	AUP_HEX,
	AUP_OCTAL,
	AUP_STRING
};

static int type_vals[] = {
	AUR_BYTE,
	AUR_SHORT,
	AUR_INT32,
	AUR_INT64
};

token_t *
gen_arbitrary(void)
{
	int count, i, print, size, type;
	token_t *tok;
	uint8_t *buf;

	/* Choose the output format. */
	print = random() % (sizeof(print_vals) / sizeof(int));
	print = print_vals[print];

	/* Choose a data type. */
	type = random() % (sizeof(type_vals) / sizeof(int));
	type = type_vals[type];

	/* Determine the size associated with that type. */
	switch (type) {
	case AUR_BYTE:
		size = AUR_BYTE_SIZE;
		break;

	case AUR_SHORT:
		size = AUR_SHORT_SIZE;
		break;

	case AUR_INT32:
		size = AUR_INT32_SIZE;
		break;

	case AUR_INT64:
		size = AUR_INT64_SIZE;
		break;
	}

	/* Choose the length of the data. */
	/* XXX-MAK: Should use symbolic constants. */
	/* XXX-MAK: Any larger of a count and au_to_data() segfaults. */
	count = random() % (256 - 4);

	/* Generate the random data. */
	buf = malloc(count * size + 1000);
	if (buf == NULL)
		err(1, "[malloc]");

	for (i = 0; i < count * size; i++)
		/* XXX-MAK: Should use symbolic constants. */
		buf[i] = random() % 256;

	/* Create token. */
	tok = au_to_data(print, type, count, (char *) buf);

	/* Clean up. */
	free(buf);

	return (tok);
}
