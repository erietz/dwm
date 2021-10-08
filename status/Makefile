BIN := dwm_status
# CFLAGS := -Wall -Wextra -g

all: $(BIN)

%: %.c
	gcc $(CFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	valgrind -s --leak-check=full --show-leak-kinds=all ./dwm_status

clean:
	rm $(BIN)
