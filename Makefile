all: build

binaries:
	gcc 203005542_assignment1.c -o shell 
	./shell
build: binaries
	@echo "Building"


clean:
	@echo "Cleaning"
	rm shell

