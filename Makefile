.PHONY: all
all:
	$(MAKE) -C src

.PHONY: clean
clean:
	$(MAKE) -C src clean

.PHONY: test
test:
	$(MAKE) -C src test

.PHONY: install
install:
	$(MAKE) -C src install
