KPSRC = kilo-BYOTE130.c PureLISP.c kplisp.c
KPOBJ = $(KPSRC:%.c=%.o)

kplisp: $(KPOBJ)
	cc -o $@ $^

.c.o:
	cc -c -DPURE_LISP_LIB $<

kilo-BYOTE130.o: kilo-BYOTE130.h PureLISP.h
PureLISP.o: PureLISP.h
kplisp.o: kilo-BYOTE130.h PureLISP.h

plsh: PureLISP.c PureLISP.h
	cc -o $@ PureLISP.c

clean:
	rm -f *.o kplisp plsh a.out

