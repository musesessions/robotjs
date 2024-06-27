CC = gcc -mmacosx-version-min=10.11
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm -framework Carbon -framework CoreFoundation -framework ApplicationServices -framework IOKit

# List of files to exclude
EXCLUDE_FILES = MMBitmap.c bitmap_find.c color_find.c png_io.c str_io.c xdisplay.c bmp_io.c zlib_util.c screengrab.c pasteboard.c io.c MMPointArray.c UTHashTable.c base64.c

# Find all .c files, then filter out the excluded files
SRCS = $(filter-out $(EXCLUDE_FILES), $(wildcard *.c))
OBJS = $(SRCS:.c=.o)
TARGET = target/robotjs

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p target
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	codesign -s 'Developer ID Application: Muse App LLC (P99CTB3BP8)' --timestamp -f --options=runtime $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Print the source files being compiled
print-sources:
	@echo "Compiling these source files:"
	@echo $(SRCS)

# NOTE: to notarize, run
# ```
# xcrun notarytool submit 'robotjs' --apple-id getmuseapp@gmail.com --team-id P99CTB3BP8 --password 'jwcv-niux-buhi-hatx' --verbose --wait
# ```
