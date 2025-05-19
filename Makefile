# Pastas
OBJ_DIR = obj
SRC_DIR = src

# Ficheiros fonte e objeto
OBJ_FILE = $(OBJ_DIR)/Grafos.o
SRC_FILES = $(SRC_DIR)/Menu.c $(SRC_DIR)/Listas_Ligadas.c

# Nome do executável
EXE_NAME = "Gestor de Estruturas"

# Regra principal
$(EXE_NAME): $(SRC_FILES) $(OBJ_FILE)
	gcc $(SRC_FILES) $(OBJ_FILE) -o $(EXE_NAME)

$(OBJ_FILE): $(SRC_DIR)/Grafos.c | $(OBJ_DIR)
	gcc -c $(SRC_DIR)/Grafos.c -o $(OBJ_FILE)

# Criar a pasta obj se não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpar ficheiros compilados
clean:
	rm -f $(OBJ_FILE) $(EXE_NAME)
	rmdir $(OBJ_DIR)