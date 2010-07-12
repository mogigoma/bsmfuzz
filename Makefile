CC=gcc
CFLAGS=-Wall -g
LIBS=-l bsm
OBJS=bsmfuzz.o		\
     gen_arbitrary.o	\
     gen_file.o		\
     gen_header.o	\
     gen_opaque.o	\
     gen_path.o		\
     gen_text.o		\
     gen_zone.o		\
     token_buffer.o	\
     utils.o

all: bsmfuzz

bsmfuzz: $(OBJS)
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	@echo "[CC] $@"
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f bsmfuzz
	@rm -f $(OBJS)
	@rm -f *.core
