CFLAGS=-Wall -Wno-main -g -Isrc -Iunity -Igen -Icmock

all: test run

test:
	ruby test/bl_formatter_test.rb

bl_test: src/binary_logger.o test/bl_environment_test.o test/test.o test/test2.o
	gcc $(CFLAGS) -o $@ $^

run: bl_test
	find test -name "*.c" | ruby scripts/bl_map.rb > bl_map.yaml
	./bl_test > trace.bin
	ruby scripts/bl_formatter.rb bl_map.yaml trace.bin

.PHONY: test

clean:
	rm -f src/*.o test/*.o bl_test bl_map.yaml trace.bin
	rm -f *~ */*~
