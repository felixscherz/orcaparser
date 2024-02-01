HEADERS = -I. -I/opt/homebrew/Frameworks/Python.framework/Headers

test: a.out
	./a.out ch2o_soc_tddft.out

a.out: $(wildcard *.c)
	clang $(HEADERS) $< -o a.out

%.o : %.c
	clang -c $(HEADERS) $<
