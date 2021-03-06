include ../mk/Makefile

CFLAGS += -I../util -I../include
# The names of the tests to run.
TEST_NAMES = dynamic_array concurrent_linked_list concurrent_hash_table
OTHER_TESTS = malloc_test ser_malloc_test

# The extra include files
UTILS = example_util_gettime.h check_sort.h
UTIL_FILES = $(addprefix $(UTIL_DIR)/,$(UTILS))

TEST_TARGETS = $(addprefix test_, $(TEST_NAMES)) 
TARGETS = $(TEST_TARGETS)  $(OTHER_TESTS) 

VALGRIND_TARGETS =  $(addprefix ser_test_, $(TEST_NAMES))

.PHONY: all clean

default: $(TARGETS)

all: $(TARGETS) $(VALGRIND_TARGETS)


# Pattern rule for building tests cases.
test_%: %_test.cpp $(UTIL_FILES) $(NABBIT_DIR)/%.h
	$(CC) $< $(NABBIT_INCLUDES) $(CFLAGS) -o $@

malloc_test: malloc_test.cpp $(UTIL_FILES)
	$(CC) $< $(NABBIT_INCLUDES) $(CFLAGS) -o $@

# Serial elision of test cases.
ser_test_%: %_test.cpp $(UTIL_FILES) $(NABBIT_DIR)/%.h
	$(CC) $< $(NABBIT_INCLUDES) $(CFLAGS) $(CILK_SERIALIZE_FLAG) -o $@

ser_malloc_test: malloc_test.cpp $(UTIL_FILES)
	$(CC) $< $(NABBIT_INCLUDES) $(CFLAGS) $(CILK_SERIALIZE_FLAG) -o $@

clean:
	rm -f $(TARGET) $(TARGETS) $(VALGRIND_TARGETS) *.exe *.obj
