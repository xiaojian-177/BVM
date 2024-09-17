#include <stdio.h>
#include <stdlib.h>
#include <string.h>
size_t countBytes(const char* str) {
	    size_t count = 0;
	    while (*str) {
	        count++;
	        str++;
	    }
	    return count;
}
void compile(const char *src, const char *dest) {
    FILE *fin = fopen(src, "r");
    FILE *fout = fopen(dest, "wb");
    if (!fin || !fout) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    int c;
    
    int offset;
    char* stringHead = "BCC executeable File V1.0.0 BCC.";
    offset = countBytes(stringHead);
    fputs(stringHead, fout);
    
    for(int i=0;i<512 - offset;i++){
		fputc(0x00, fout);
		
	}
    while ((c = fgetc(fin)) != EOF) {
        switch (c) {
            case '>': fputc(0x3A, fout); break;
            case '<': fputc(0x3B, fout); break;
            case '+': fputc(0x4C, fout); break;
            case '-': fputc(0x4D, fout); break;
            case '.': fputc(0x5E, fout); break;
            case ',': fputc(0x5F, fout); break;
            case '[': {
                int depth = 1;
                while ((c = fgetc(fin)) != EOF && depth > 0) {
                    if (c == '[') depth++;
                    else if (c == ']') depth--;
                }
                fputc(0xA7, fout);
                fputc(c == ']' ? 0xA8 : 0x00, fout); // Check for matching ']'
                break;
            }
            case ']': break; // Handled by '[' case
            case 'c': fputc(0x2A, fout);
            
            default: continue;
            
        }
    }
    
    fclose(fin);
    fclose(fout);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source.b> <destination.bc>\n", argv);
        return EXIT_FAILURE;
    }

    compile(argv[1], argv[2]);
    return EXIT_SUCCESS;
    
}
