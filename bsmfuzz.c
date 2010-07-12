#include "bsmfuzz.h"

typedef token_t *(*generator_t)(void);

static generator_t generators[] = {
	gen_arbitrary,
	gen_file,
	gen_opaque,
	gen_path,
	gen_text,
	gen_zone
};

static int interrupted;

int bits;

static int
gen_record(struct token_buffer *tb, int num_tokens)
{
	token_t *tok;
	int i, j;

	/* Open record. */
	if (rec_open(tb) != 0)
		return (-1);

	/* Generate header token. */
	tok = gen_header();
	if (tok == NULL)
		return (-1);

	/* Append header to record. */
	if (rec_append(tb, tok) != 0)
		return (-1);

	for (i = 0; i < num_tokens; i++) {
		/* Choose a token generator. */
		j = random() % (sizeof(generators) / sizeof(generator_t));

		/* Generate token. */
		tok = generators[j]();
		if (tok == NULL) {
			i--;
			continue;
		}

		/* Append token to record. */
		if (rec_append(tb, tok) != 0)
			return (-1);
	}

	/* Close record. */
	if (rec_close(tb) != 0)
		return (-1);

	return (0);
}

static void
bail(int sig)
{

	/* Flag that we've been interrupted. */
	interrupted = 1;

	/* Mask further SIGINTs. */
	signal(SIGINT, SIG_IGN);
}

static void
usage(void)
{

	fprintf(stderr, "usage: bsmfuzz [-n tokens] [-N records] [-o file] "
		"[-s seed] -b bits\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	int ch, fd, num_records, num_tokens;
	uint64_t total_bytes, total_records;
	struct token_buffer tb;
	unsigned long seed;
	size_t bytes, left;

	/* Set default seed for random number generator. */
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		err(1, "Failed to open %s", DEVICE_RANDOM);

	for (left = sizeof(seed); left > 0; left -= bytes) {
		bytes = read(fd, &seed, left);
		if (bytes <= 0)
			err(1, "Failed to read from %s", DEVICE_RANDOM);
	}

	close(fd);

	/* Set other defaults. */
	/* XXX-MAK: There's got to be some nice way to detect 32 vs 64-bit. */
	bits = -1;
	fd = STDOUT_FILENO;
	num_records = -1;
	num_tokens = 1;

	/* Parse command line arguments. */
	while ((ch = getopt(argc, argv, "b:hn:N:o:s:")) != -1) {
		switch(ch) {
		case 'b':
			bits = atoi(optarg);
			if (bits != 32 && bits != 64)
				errx(1, "Bits must be 32 or 64.");
			break;

		case 'n':
			num_tokens = atoi(optarg);
			if (num_tokens <= 0)
				errx(1, "Number of tokens must be positive.");
			break;

		case 'N':
			num_records = atoi(optarg);
			if (num_records <= 0)
				errx(1, "Number of records must be positive.");
			break;

		case 'o':
			fd = open(optarg, O_CREAT|O_TRUNC|O_WRONLY);
			if (fd < 0)
				err(1, "Failed to open output file");
			break;

		case 's':
			if (sscanf(optarg, "%lx", &seed) != 1)
				err(1, "Failed to parse random seed");
			break;

		case 'h':
		case '?':
		default:
			usage();
		}
	}

	/* Ensure required arguments were provided. */
	if (bits == -1)
		usage();

	/* Seed the random number generator. */
	fprintf(stderr, "Random seed: %02lx\n", seed);
	srandom(seed);

	/* Set up signal handlers. */
	interrupted = 0;
	signal(SIGINT, bail);

	total_bytes = 0;
	total_records = 0;
	while (!interrupted) {
		/* Generate a random record. */
		if (gen_record(&tb, num_tokens) != 0)
			continue;

		/* Write record. */
		total_records++;
		total_bytes += tb.used;
		if (rec_write(&tb, fd) != 0) {
			errx(1, "Failed to write record.");
		}

		/* Check if we should generate infinite records. */
		if (num_records == -1)
			continue;

		/* Check if we've hit the record limit. */
		if (--num_records == 0)
			break;
	}

	/* Clean up. */
	close(fd);

	/* Print statistics. */
	fprintf(stderr, "Wrote %lu bytes, representing %lu audit records.\n",
	    total_bytes, total_records);

	return (0);
}
