TARGET = SampleCloudServer
BUILD_OUTPUT_DIR = ../output

test:
	@valgrind --leak-check=full --show-reachable=yes -v ${BUILD_OUTPUT_DIR}/${TARGET} 2>&1 | tee valgrind.log
	@grep "0 errors from 0 contexts" valgrind.log

.PHONY: test
