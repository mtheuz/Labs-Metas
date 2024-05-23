#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <fcntl.h>       
#include <sys/mman.h>
#include <unistd.h>         
=======
#include <fcntl.h>          // Para O_RDWR e O_SYNC
#include <sys/mman.h>
#include <unistd.h>         // Para usleep
>>>>>>> d016f9529237151b5742d1ff194b94c62a63ad62
#include "../../LabExemples/auxFiles/address_map_arm.h"

int main(void)
{
<<<<<<< HEAD
    volatile int *LEDR_ptr; 
    int fd = -1;            
    void *LW_virtual;       
=======
    volatile int *LEDR_ptr; // Ponteiro para o endereço virtual dos LEDs vermelhos
    int fd = -1;            // Descritor de arquivo usado para abrir /dev/mem
    void *LW_virtual;       // Ponteiro para o endereço virtual da ponte de memória leve

    // Abre o /dev/mem para fornecer acesso aos endereços físicos
>>>>>>> d016f9529237151b5742d1ff194b94c62a63ad62
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return -1;
    }

<<<<<<< HEAD
=======
    // Obtém um mapeamento de endereços físicos para endereços virtuais
>>>>>>> d016f9529237151b5742d1ff194b94c62a63ad62
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return -1;
    }
<<<<<<< HEAD
    
    LEDR_ptr = (volatile int *)(LW_virtual + LEDR_BASE);
    *LEDR_ptr = 0x1;
    while (1) {

        for (; *LEDR_ptr < 0x200; *LEDR_ptr <<= 1) {
            usleep(100000); 
        }
        for (; *LEDR_ptr > 0x1; *LEDR_ptr >>= 1) {
            usleep(100000); 
        }
    }

=======

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
>>>>>>> d016f9529237151b5742d1ff194b94c62a63ad62
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}
