CC=gcc
CFLAGS=-Wall -g
LIBS=-l bsm

all: bsmfuzz

bsmfuzz: bsmfuzz.c token_buffer.o gen_arbitrary.o gen_file.o gen_header.o \
	 gen_opaque.o gen_path.o gen_text.o gen_zone.o utils.o
	$(CC) $(CFLAGS) -o $@ $< *.o $(LIBS)

token_buffer.o: token_buffer.h token_buffer.c
	$(CC) $(CFLAGS) -c $<

gen_arbitrary.o: gen_arbitrary.c
	$(CC) $(CFLAGS) -c $<

gen_file.o: gen_file.c
	$(CC) $(CFLAGS) -c $<

gen_header.o: gen_header.c
	$(CC) $(CFLAGS) -c $<

gen_opaque.o: gen_opaque.c
	$(CC) $(CFLAGS) -c $<

gen_path.o: gen_path.c
	$(CC) $(CFLAGS) -c $<

gen_text.o: gen_text.c
	$(CC) $(CFLAGS) -c $<

gen_zone.o: gen_zone.c
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f bsmfuzz
	rm -f *.o
	rm -f *.core
