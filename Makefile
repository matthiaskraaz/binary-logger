CFLAGS=-Wall -Wno-main -g -Isrc -Iunity -Igen -Icmock

.PHONY: all bl_formatter_test run_bl_demo bl_demo_map clean bl_demo_message

all: bl_formatter_test run_bl_demo

bl_formatter_test:
	@echo === Executing unit tests of bl_formatter.rb, the Ruby script that converts binary output from the device into human-readable messages
	ruby test/bl_formatter_test.rb

# Run demo of binary logger
run_bl_demo: bl_demo_message bl_demo bl_demo_map
	@echo === Executing the demo program
	./bl_demo > trace.bin
	@echo === Transforming binary output of demo program into human-readable messages
	ruby scripts/bl_formatter.rb bl_demo_map.yaml trace.bin

bl_demo_message:
	@echo === Building demo program

bl_demo_map:
	@echo === Building table of all log messages in sources of demo program
	find test -name "*.c" | ruby scripts/bl_map.rb > bl_demo_map.yaml

bl_demo: src/binary_logger.o test/bl_environment_test.o test/test.o test/test2.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo === Cleaning up
	rm -f src/*.o test/*.o bl_demo bl_demo_map.yaml trace.bin
	rm -f *~ */*~

