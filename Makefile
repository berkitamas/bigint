GCC=gcc

SOURCE=src/
BINARY=bin/

all: $(SOURCE)main.c $(SOURCE)rpn.c $(SOURCE)utils.c $(SOURCE)stack.c $(SOURCE)queue.c $(SOURCE)ops.c $(SOURCE)rpn.h $(SOURCE)utils.h $(SOURCE)stack.h $(SOURCE)queue.h $(SOURCE)ops.h
	mkdir -p bin
	$(GCC) -Wall $(SOURCE)main.c $(SOURCE)rpn.c $(SOURCE)utils.c $(SOURCE)stack.c $(SOURCE)queue.c $(SOURCE)ops.c -o $(BINARY)bigint

bare: $(SOURCE)main.c $(SOURCE)rpn.c $(SOURCE)utils.c $(SOURCE)stack.c $(SOURCE)queue.c $(SOURCE)ops.c $(SOURCE)rpn.h $(SOURCE)utils.h $(SOURCE)stack.h $(SOURCE)queue.h $(SOURCE)ops.h
	mkdir -p bin
	$(GCC) -Wall -DNOFORMAT $(SOURCE)main.c $(SOURCE)rpn.c $(SOURCE)utils.c $(SOURCE)stack.c $(SOURCE)queue.c $(SOURCE)ops.c -o $(BINARY)bigint

debug: $(SOURCE)main.c $(SOURCE)rpn.c $(SOURCE)utils.c $(SOURCE)stack.c $(SOURCE)queue.c $(SOURCE)ops.c $(SOURCE)rpn.h $(SOURCE)utils.h $(SOURCE)stack.h $(SOURCE)queue.h $(SOURCE)ops.h
	mkdir -p bin
	$(GCC) -Wall -DDEBUG -g $(SOURCE)main.c $(SOURCE)rpn.c $(SOURCE)utils.c $(SOURCE)stack.c $(SOURCE)queue.c $(SOURCE)ops.c -o $(BINARY)bigint

rebuild: clean all

clean:
	rm -rf bin