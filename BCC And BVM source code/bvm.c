#include <stdio.h>
#include <stdlib.h>

void run(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    unsigned char memory = {0};
    unsigned char *ptr = &memory;
    
    int ip = 0;
    unsigned char instruction;
    fseek(file, 512, SEEK_SET);
    while (fread(&instruction, 1, 1, file)) {
        switch (instruction) {
            case 0x3A: ptr++; break;
            case 0x3B : ptr--; break;
            case 0x4C: (*ptr)++; break;
            case 0x4D: (*ptr)--; break;
            case 0x5E: putchar(*ptr); break;
            case 0x5F: *ptr = getchar(); break;
            case 0xA7: {
                unsigned char temp = ip;
                fread(&instruction, 1, 1, file);
                if (instruction == 0xA8) {
                    while (*ptr != 0) {
                        ip = temp;
                        fread(&instruction, 1, 1, file);
                    }
                }
                break;
            }
            case 0xA8: ip = (int)(ptr - memory) - 1; break;
            case 0x2A: (*ptr) = 0;
            
            default: printf("Unknown Format."); exit(1); break; 
        }
        ip++;
    }

    fclose(file);
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <bytecode.bc>\n", argv);
        return EXIT_FAILURE;
    }

    run(argv[1]);
    
    return EXIT_SUCCESS;
}
