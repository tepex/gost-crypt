# $ make && sudo make install
# $ make OPTFLAGS="-DARCH_MODE=32"

#CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS) # TODO: проверить порядок параметров для Linux
CFLAGS= -O2 -Wall -Wextra -DNO_DEBUG=true $(OPTFLAGS) # TODO: проверить порядок параметров для Linux

LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

LIB_PATH==app/src/main/jni/
TEST_PATH=app/src/test/jni/

LIBTEST=build/lib_tests

TARGET=build/libgost.a
SO_TARGET=$(patsubst %.a,%.dylib,$(TARGET)) # TODO: поменять расширение на .so для Linux

SOURCES=$(wildcard app/src/main/jni/**/*.c app/src/main/jni/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SOURCES=$(wildcard app/src/test/jni/**/*.c app/src/test/jni/*.c)
TEST_OBJECTS=$(patsubst %.c,%,$(TEST_SOURCES))


# The Target Build
all: $(TARGET) $(SO_TARGET) tests 

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra -DNO_DEBUG=false $(OPTFLAGS)  # TODO: проверить порядок параметров для Linux
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build

# The Unit Tests
tests: $(TEST_OBJECTS) move
	sh ./runtests.sh

move:
	@mv app/src/test/jni/lib_tests $(LIBTEST)
	@mv app/src/main/jni/*.o build/
	
valgrind:
	VALGRIND="valgrind --log-file=tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f $(TEST_PATH)tests.log
	rm -f $(TEST_PATH)*.o
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
