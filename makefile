target:	test

test:	main.c lib1.h lib1.c
		gcc main.c lib1.c -o exe
		./exe

clean:
		rm test