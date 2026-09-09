CC       = cc
CFLAGS   = -O2 -Wall -Wextra
LDLIBS   = -lX11
TARGET   = wmempty
SRC      = wmempty.c
PREFIX   = /usr/local

all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)
install: $(TARGET)
	install -Dm755 $(TARGET) $(PREFIX)/bin/$(TARGET)
clean:
	rm -f $(TARGET)
.PHONY: all install clean
