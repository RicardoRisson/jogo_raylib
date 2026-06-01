# Compilador
CC = gcc

# Opções de flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Bibliotecas necessárias para Raylib no Linux
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Nome do executável gerado (pode mudar para 'jogo' se preferir)
TARGET = a.out

# Pega todos os arquivos .c da pasta atual automaticamente
SRC = $(wildcard *.c)

# Regra padrão
all: $(TARGET)

# Cria o executável (Removido o $(INCLUDES) que estava sobrando)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Compila e executa
run: all
	./$(TARGET)

# Remove o executável gerado e arquivos objeto
clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean