BIN := dwm_status
# CFLAGS := -Wall -Wextra -g

all: $(BIN)

%: %.c
	gcc $(CFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all $(BIN)

clean:
	rm $(BIN)
