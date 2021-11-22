CC=gcc
CFLAGS=-I. -g
CPPFLAGS=-I. -g -std=c++17
DEPS = gap_buffer.h document.h window.h log.h
OBJ = gap_buffer.o main.o document.o window.o log.o terminal.o status.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(CPPFLAGS)

default: $(OBJ)
	$(CC) -o wee $^ $(CPPFLAGS) -lncurses

clean:
	rm -f *.o *.out wee test/*.o test_wee

run: default
	./wee

test-build: $(OBJ) test/test_wee.o
	g++ $(CPPFLAGS) -o test_wee gap_buffer.o document.o window.o terminal.o \
		test/test_wee.o \
		-lgtest -lgtest_main -lpthread -lncurses

test-document: test-build
	./test_wee --gtest_filter=Document.*

test-document-io: test-build
	./test_wee --gtest_filter=DocumentIO.*

test-gap: test-build
	./test_wee --gtest_filter=Gap.*

test: test-gap test-document test-document-io
