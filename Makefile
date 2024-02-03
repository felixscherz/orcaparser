HEADERS = -I. -I/opt/homebrew/Frameworks/Python.framework/Headers

testpy: install
	python -m pytest -rP

test: a.out
	./a.out ch2o_soc_tddft.out

a.out: $(wildcard *.c)
	clang $(HEADERS) $< -o a.out

%.o : %.c
	clang -c $(HEADERS) $<


install: clean
	pip install .

clean:
	rm -rf ./build
	rm -rf ./orcaparser.egg-info
	rm -rf ./.pytest_cache/
	rm -rf ./orcaparser/__pycache__/
	rm -rf ./tests/__pycache__/
	rm -rf ./.pytest_cache/

