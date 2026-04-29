# compilador
CC = gcc

# opções de flags de compilacao
CFLAGS = -Wall -Wextra -std=c11

# bibliotecas necessárias para Raylib
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

# aome do executável gerado
TARGET = a.out

# arquivo principal
SRC = main.c

# compila o programa
all: $(TARGET)

# cria o executavel
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# compila e executa (se ja estiver compilado só executa)
run: $(TARGET)
	./$(TARGET)

# remove o executável gerado
clean:
	rm -f $(TARGET)

# indica que estas regras não geram arquivos com esses nomes
.PHONY: all run clean