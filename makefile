zstr.o : zstr.c
	gcc -Wall -Werror -std=c11 -c zstr.c
test_zstr: zstr.o
	gcc -Wall -Werror -std=c11 -o test_zstr test_zstr.c zstr.o
clean:
	rm test_zstr zstr.o
run:
	./test_zstr