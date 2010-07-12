#include "bsmfuzz.h"

struct dirent *
get_dir_component(DIR *dir)
{
	struct dirent *file;
	long i, len;

	/* Count number of dirents. */
	for (len = 0; readdir(dir) != NULL; len++)
		;
	rewinddir(dir);

	/* Ignore "." and "..". */
	len -= 2;

	/* Empty directory. */
	if (len == 0)
		return (NULL);

	/* Choose a random dirent. */
	file = readdir(dir); /* "." */
	file = readdir(dir); /* ".." */
	for (i = random() % len; i >= 0 && file != NULL; i--)
		file = readdir(dir);

	return (file);
}

void
get_path(char *buf, int size)
{
	struct dirent *file;
	DIR *dir;

	/* Choose a random path on the system. */
	*buf = '\0';
	dir = opendir("/");
	while (strlen(buf) < size) {
		if (dir == NULL)
			break;

		/* Choose dirent. */
		file = get_dir_component(dir);
		if (file == NULL) {
			closedir(dir);
			break;
		}

		/* Append to path, if there's room. */
		if (strlen(buf) + 1 + file->d_namlen > size - 1)
			break;
		snprintf(buf, size, "%s/%s", buf, file->d_name);

		/* Only continue if we selected a directory. */
		if (file->d_type != DT_DIR)
			break;

		/* Open directory. */
		dir = opendir(buf);
	}
}
