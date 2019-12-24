SRC_DIR = src
BIN_DIR = bin
BATS_FILE = test.bats
EXEC = automaton
TEST_EXEC = $(patsubst %.c,%,$(wildcard $(SRC_DIR)/test*.c))

.PHONY: exec bindir clean html source test testbats testbin testcunit

exec: source bindir
	cp $(SRC_DIR)/$(EXEC) $(BIN_DIR)

bindir:
	mkdir -p $(BIN_DIR)

clean:
	make clean -C $(SRC_DIR)
	rm -rf $(BIN_DIR)
	rm -f README.html

html:
	pandoc --metadata pagetitle="Cellular Automaton" \
			-f markdown -o README.html \
			-sc misc/github-pandoc.css \
			README.md

source:
	make -C $(SRC_DIR)

test: exec testbin testcunit testbats

testbats:
	bats $(BATS_FILE)

testbin: source bindir
	$(MAKE) test -C $(SRC_DIR)
	cp $(TEST_EXEC) $(BIN_DIR)

testcunit:
	set -e; \
	for test in `ls $(BIN_DIR)/test*` ; do \
		$$test; \
	done
