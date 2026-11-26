CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/utils.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c src/section.c
TARGET = bin/app.exe

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	@rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run