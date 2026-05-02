# Compilador
CC = gcc

# Opções de flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Bibliotecas necessárias para Raylib
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Nome do executável gerado
TARGET = a.out

# ARRUMADO: Pega todos os arquivos .c da pasta atual
SRC = $(wildcard *.c)

# Compila o programa
all: $(TARGET)

# Cria o executável
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Compila e executa
run: $(TARGET)
	./$(TARGET)

# Remove o executável gerado
clean:
	rm -f $(TARGET)

# Indica que estas regras não geram arquivos com esses nomes
.PHONY: all run clean