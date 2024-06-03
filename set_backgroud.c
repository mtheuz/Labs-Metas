#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "../../LabExemples/auxFiles/address_map_arm.h"

#define DATA_A_BASE  0x80
#define DATA_B_BASE  0x70
#define START_BASE 0xc0

int main(void)
{
    volatile int *DATA_A_PTR;   // Pointer to DATA_A
    volatile int *DATA_B_PTR;   // Pointer to DATA_B
    volatile int *START_PTR;    // Pointer to START

    int fd = -1;            
    void *LW_virtual;       


    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return -1;
    }

    // Map the Lightweight bridge
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return -1;
    }
    
    DATA_A_PTR = (volatile int *) (LW_virtual + DATA_A_BASE);
    DATA_B_PTR = (volatile int *) (LW_virtual + DATA_B_BASE);   
    START_PTR = (volatile int *) (LW_virtual + START_BASE);


    *START_PTR = 0;
    *DATA_A_PTR = 0b0;
    *DATA_B_PTR = 0x00000f1f;

    *DATA_A_ptr = 0b000010000; /*reg:00001 ; opcode: 0000*/ 
    *DATA_B_ptr = 0b100000111010000011101000001111;

    *START_PTR = 1;


    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}
