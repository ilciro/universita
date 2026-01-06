all: build critical shellcode

critical: build
	@sudo chown root buggy
	@sudo chmod 777 buggy
	@sudo chmod +s buggy

build: buggy.c
	@echo 0 | sudo tee /proc/sys/kernel/randomize_va_space > /dev/null
	@gcc -O0 -fno-unwind-tables -fno-builtin -fno-stack-protector -z execstack -U_FORTIFY_SOURCE -o buggy buggy.c

shellcode:
	@gcc -g -O0 -fno-unwind-tables -fno-builtin -fno-stack-protector -z execstack -U_FORTIFY_SOURCE -o sc sc.c


.PHONY: clean
clean:
	-unlink buggy
	-unlink sc
