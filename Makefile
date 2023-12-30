# HashTable implementation in C
SRC = main.c hashtable.c
OBJ = ${SRC:.c=.o}

hashtable: ${OBJ}
	${CC} -o $@ ${OBJ}

clean:
	rm -f hashtable ${OBJ}

.PHONY: clean
