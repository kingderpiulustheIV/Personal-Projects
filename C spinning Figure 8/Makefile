CC=gcc
SRC=figure8.c
BIN_DIR=bin
OUT=$(BIN_DIR)/figure8

default: $(OUT)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OUT): $(SRC) | $(BIN_DIR)
	$(CC) -o $(OUT) $(SRC) -lm

clean:
	rm -rf $(BIN_DIR)
