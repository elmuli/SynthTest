C = gcc
CFLAGS = -g -W -MMD
LDLIBS = -lSDL3 -lm -lportmidi

TARGET = LineTest
SRCS = LineTest.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(C) $(OBJS) -o $@ $(LDLIBS)

%.o: %.c
	$(C) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) || exit 0
