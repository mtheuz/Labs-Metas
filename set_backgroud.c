#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       
#include <sys/mman.h>
#include <unistd.h>         
#include "../../LabExemples/auxFiles/address_map_arm.h"

int main(void)
{
    volatile int *DATA_A_PTR;   // Opcode e endereçamento do Banco
    volatile int *DATA_B_PTR;   // Dados
    volatile int *START_PTR; // Controle modulo gerador de pulso

    int fd = -1;            
    void *LW_virtual;       

    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return -1;
    }

    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return -1;
    }
    
    DATA_A_PTR = (volatile int *) (LW_virtual + DATA_A);
    DATA_B_PTR = (volatile int *) (LW_virtual + DATA_B);   
    START_PTR = (volatile int *) (LW_virtual + START);

    *START_PTR = 0;
    *DATA_A_PTR = 0b0;
    *DATA_B_PTR = 0b0000011;
    *START_PTR = 1;
    *START_PTR = 0;

    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}