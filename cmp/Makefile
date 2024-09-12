run: build
	@./main
	@rm -rf main

build:
	@gcc main.c vec/vec.c mtx/mtx_dense.c sle/sle_dense.c -lm -fopenmp -o main

