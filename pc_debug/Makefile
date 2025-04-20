# Директории
SRCDIR = ../Core/Src
INCDIR = ../Core/Inc
INCDIR2 = Inc
OBJDIR = obj
BINDIR = bin

# Компилятор и флаги
CC = gcc.exe
CFLAGS = -Wall -g -std=c99 -I$(INCDIR) -I$(INCDIR2) 


# Файлы
SOURCES	=	main.c \
			Src/i2c.c \
			$(SRCDIR)/rgbw_driver.c

OBJECTS =	$(OBJDIR)/main.o	\
			$(OBJDIR)/i2c.o		\
			$(OBJDIR)/rgbw_driver.o

TARGET =	$(BINDIR)/main.exe

# Правила
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/main.o: main.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/i2c.o: Src/i2c.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/rgbw_driver.o: $(SRCDIR)/rgbw_driver.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Дополнительно
.PHONY: all clean