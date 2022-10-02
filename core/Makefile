TARGETS := x64

.PHONY: clean build

all: clean build

build:
	@$(foreach target,$(TARGETS),$(MAKE) -f Makefile.$(target) all;)

clean:
	@$(foreach target,$(TARGETS),$(MAKE) -f Makefile.$(target) clean;)
