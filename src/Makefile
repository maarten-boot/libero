CC := clang
CC_FLAGS := -pedantic-errors -std=c99 -Wall
FILES := lr.c lrcalc.c lrcode.c lrdump.c lreval.c lrfree.c lrglib.c lrlibr.c lrload.c lroptn.c lrsort.c lrsymb.c

all: clean indent lr

indent:
	indent  lr.c lrcalc.c lrcode.c lrdump.c lreval.c lrfree.c  \
	lrload.c lroptn.c lrsort.c lrsymb.c \
	*.h

tidy:
	clang-tidy *.h
	clang-tidy *.c

clean:
	rm -f *.o
	rm -f a.out
	rm -f lr

lr:
	$(CC) $(CC_FLAGS) $(FILES) -o lr
