# Compilador
CC = gcc

# Includes da raylib
INCLUDES = -I/usr/local/include

# Bibliotecas da raylib
LIBPATH = -L/usr/local/lib

# Flags
CFLAGS = -Wall -Wextra -std=c11

# Bibliotecas necessárias
LIBS = $(LIBPATH) -lraylib -lm -lpthread -ldl -lrt -lX11

# Nome do executável
TARGET = game

# Arquivos fonte
SRC = $(wildcard *.c)

# Regra principal
all: $(TARGET)

# Compilação
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS)

# Executar
run: all
	./$(TARGET)


# Limpeza
clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean