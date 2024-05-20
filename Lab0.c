#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          // Para O_RDWR e O_SYNC
#include <sys/mman.h>
#include <unistd.h>         // Para usleep
#include "../../LabExemples/auxFiles/address_map_arm.h"

int main(void)
{
    volatile int *LEDR_ptr; // Ponteiro para o endereço virtual dos LEDs vermelhos
    int fd = -1;            // Descritor de arquivo usado para abrir /dev/mem
    void *LW_virtual;       // Ponteiro para o endereço virtual da ponte de memória leve

    // Abre o /dev/mem para fornecer acesso aos endereços físicos
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return -1;
    }

    // Obtém um mapeamento de endereços físicos para endereços virtuais
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return -1;
    }

    // Configura o ponteiro de endereço virtual para a porta de I/O
    LEDR_ptr = (volatile int *)(LW_virtual + LEDR_BASE);

    // Inicializa o valor dos LEDs
    *LEDR_ptr = 0x1;

    while (1) {
        // Move os LEDs para a direita
        for (; *LEDR_ptr < 0x200; *LEDR_ptr <<= 1) {
            usleep(100000); // Espera 100 milissegundos
        }

        // Move os LEDs para a esquerda
        for (; *LEDR_ptr > 0x1; *LEDR_ptr >>= 1) {
            usleep(100000); // Espera 100 milissegundos
        }
    }

    // Unmap memory and close the file descriptor (não é alcançado neste loop infinito)
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}
