# compilador
CC = gcc

# flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# arquivos-fonte (incluindo o novo arquivo mapa.c)
SRC = main.c personagens.c movimentacao.c objetos.c mapa.c

# nome do executável
TARGET = jogo

# bibliotecas para Linux
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

# regra padrão (se digitar apenas 'make', ele compila)
all: $(TARGET)

# compila o jogo
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# compila e executa direto com 'make run'
run: $(TARGET)
	./$(TARGET)

# limpa o executável antigo
clean:
	rm -f $(TARGET)

.PHONY: all run clean