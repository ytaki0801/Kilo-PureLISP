NAME = kplisp

$(NAME): $(NAME).o kilo-BYOTE130.o PureLISP.o
	cc -o $@ *.o

PureLISP.o: PureLISP.c 
	cc -c -DPURE_LISP_LIB PureLISP.c

kilo-BYOTE130.o: kilo-BYOTE130.c kilo-BYOTE130.h
	cc -c -DPURE_LISP_LIB kilo-BYOTE130.c

clean:
	rm -f $(NAME) *.o a.out

