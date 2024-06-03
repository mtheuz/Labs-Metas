#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "/home/aluno/TEC499/LabExemples/auxFiles/address_map_arm.h"

#define WRREG 0xc0
#define DATA_B 0x70
#define DATA_A 0x80
//#define OPCODE  

int main(void) {

    volatile int * WRREG_ptr;
    volatile int * DATA_A_ptr;
    volatile int * DATA_B_ptr;
    volatile int * VIDEO_IN_BASE_ptr;

    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge

    // Open /dev/mem to give access to physical addresses
    if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf ("ERROR: could not open \"/dev/mem\"...\n");
        return (-1);
    }

    // Get a mapping from physical addresses to virtual addresses
    LW_virtual = mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (-1);
    }

    WRREG_ptr = (int *) (LW_virtual + WRREG);
    DATA_A_ptr = (int *) (LW_virtual + DATA_A);
    DATA_B_ptr = (int *) (LW_virtual + DATA_B);
    
    /*
    int i;
    for(i = 0; i < 11; ++i) {    
        *WRREG_ptr = 0x00000000; 
        *DATA_A_ptr = 0x00000000; 
        *DATA_B_ptr = 0x00000f1f;
        *WRREG_ptr = 0x00000001; 

        sleep(1);

        *WRREG_ptr = 0x00000000; 
        *DATA_A_ptr = 0x00000000; 
        *DATA_B_ptr = 0x00000129;
        *WRREG_ptr = 0x00000001; 

        sleep(1);000001111
    }
    */

    //Define o valor do background
    *WRREG_ptr = 0x00000000; 
    *DATA_A_ptr = 0x00000000; 
    *DATA_B_ptr = 0x00000f1f;

    //Imprime sprite na tela
    *DATA_A_ptr = 0b000010000; /*reg:00001 ; opcode: 0000*/ 
    *DATA_B_ptr = 0b100000111110000011111000001111; /*sp: 1; coordX: 0000001010 ; coordY: 0000001010 ; offset: 000001010 */

    *DATA_A_ptr = 0b000010000; /*reg:00001 ; opcode: 0000*/ 
    *DATA_B_ptr = 0b100000111010000011101000001111; /*sp: 1; coordX: 0000001010 ; coordY: 0000001010 ; offset: 000001010 */

    //Imprime polígono na tela
    *DATA_A_ptr = 0b000100011; /*reg:00010 ; opcode: 0011*/ 
    *DATA_B_ptr = 0b10011000100000000011111000011111; /*forma: 1; B: 001 ; G: 100 ; R: 010 ; tamanho: 0001; ref_pointY: 000011111 ; ref_pointX: 000011111*/

    *WRREG_ptr = 0x00000001; 

    if (munmap (LW_virtual, LW_BRIDGE_SPAN) != 0) {
        printf("ERROR: munmap( failed...\n)");
        return (-1);
    }

    close(fd);

    return 0;
}
