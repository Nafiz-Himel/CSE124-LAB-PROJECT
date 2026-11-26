CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c
TARGET = bin/app.exe

all: $(TARGET)

$(TARGET): $(SRC)
	@if not exist bin mkdir bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	@if exist bin\\app.exe del /Q bin\\app.exe

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
