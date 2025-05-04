# Директории
SRCDIR = ../Core/Src
INCDIR = ../Core/Inc
OBJDIR = obj
BINDIR = bin

# Компилятор и флаги
CC = gcc.exe
CFLAGS = -Wall -g -std=c99 -DPC_DEBUG -I$(INCDIR)

ifdef DEBUG
	CFLAGS += -DDEBUG
endif

# --- Исходные файлы (без путей) ---
SOURCES	=	main.c \
			rgbw_driver.c \
			i2c_hw.c \

# --- Относительные пути к исходникам и объектам ---
SRC_PATHS = $(addprefix $(SRCDIR)/, $(SOURCES))
OBJ_PATHS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC_PATHS))

TARGET = $(BINDIR)/main.exe

# Правила
all: $(TARGET)

echo_vars:
	@echo "SOURCES: $(SRC_PATHS)"
	@echo "OBJ_PATHS: $(OBJ_PATHS)"

$(TARGET): $(OBJ_PATHS)
	@echo "GET_EXE: $@"
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "GET_OBJs: $@"
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaninig: $@"
	rm -rf $(OBJDIR) $(BINDIR)

# Дополнительно
.PHONY: all clean