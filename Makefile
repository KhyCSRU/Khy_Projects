#USE FOR COMMAND LINE PROJECY
all: build

binaries:
	gcc RUCassignment1.c -o shell 
	./shell
build: binaries
	@echo "Building"


clean:
	@echo "Cleaning"
	rm shell

