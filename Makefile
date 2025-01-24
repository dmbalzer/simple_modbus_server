SRC += main.c
FLG += -std=c99
FLG += -Wall
LNK += -lmodbus
BIN += modbus_server

build:
	gcc $(SRC) $(FLG) $(LNK) -o $(BIN)

run:
	./$(BIN)
