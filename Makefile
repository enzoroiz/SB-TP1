# Parametros de Ambiente

CC = gcc
 
CFLAGS = -Wall


# Parametros de Projeto

# TODO: altere a lista de forma que contenha todos os arquivos .c 
#       do seu projeto, que deve estar no diretório src/emulador
SRC_EMULADOR = main.c virtualmachine.c

OBJS_EMULADOR = $(SRC_EMULADOR:%.c=build/emulador/%.o)


# Executaveis

all: bin/emulador

bin/emulador: $(OBJS_EMULADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CC) $(CFLAGS) $(OBJS_EMULADOR) -o bin/emulador


# Objetos

build/%.o: src/%.c src/%.h
	@echo "- Compilando objeto \"$@\""
	@$(CC) -c $(CFLAGS) $< -o $@

build/%.o: src/%.c
	@echo "- Compilando objeto \"$@\""
	@$(CC) -c $(CFLAGS) $< -o $@


# Auxiliares

clean:
	rm -f bin/* $(OBJS_EMULADOR)

