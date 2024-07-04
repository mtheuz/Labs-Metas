#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "../../LabExemples/auxFiles/address_map_arm.h"

int main(void) {
    volatile int *KEY_ptr;
    int fd = -1;
    void *LW_virtual;

    // Abre /dev/mem
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERRO: não foi possível abrir \"/dev/mem\"...\n");
        return (-1);
    }

    // Mapeia a memória
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERRO: mmap() falhou...\n");
        close(fd);
        return (-1);
    }

    // Obtem o ponteiro para o endereço do botão
    KEY_ptr = (volatile int *)(LW_virtual + KEY_BASE);

    // Loop para testar os botões
    while (1) {
        int key_value = *KEY_ptr & 0xF; // Máscara para obter os 4 bits dos botões
        
        if (key_value != 0) {
            if (key_value & 0x1) printf("Botão 0 pressionado!\n");
            if (key_value & 0x2) printf("Botão 1 pressionado!\n");
            if (key_value & 0x4) printf("Botão 2 pressionado!\n");
            if (key_value & 0x8) printf("Botão 3 pressionado!\n");
            
            // Espera até que todos os botões sejam soltos
            while ((*KEY_ptr & 0xF) != 0);
        }
        usleep(100000); // Espera por 100ms
    }

    // Desmapeia a memória e fecha o arquivo
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}
