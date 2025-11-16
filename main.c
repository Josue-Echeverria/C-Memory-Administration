#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hole {
    size_t start;          // starting address of the free block
    size_t size;           // length of the block
    struct hole *next;     // next hole in sorted-by-address order
} Hole;


void alloc_memory(char *mem, int index, int size, char *name) {
    // Implementation of memory allocation
    for (int i = 0; i < size; i++) {
        mem[index + i] = *name; // Mark memory as used
    }
}

int get_table_index(int table[1024][2], int page_number) {
    for (int i = 0; i < 1024; i++) {
        if (table[i][0] == page_number) {
            return i;
        }
    }
    return -1;
}

int get_free_table_index(int table[1024][2]) {
    for (int i = 0; i < 1024; i++) {
        if (table[i][0] == -1) {
            return i;
        }
    }
    return -1;
}

int get_best_fit_index(Hole *free_list, int size, char *mem, char name) {
    Hole *prev = NULL;
    Hole *cur  = free_list;

    while (cur) {
        if (cur->size >= size) {
            alloc_memory(mem, cur->start, size, &name); // Mark memory as used
            // If the hole is larger than needed, split it
            if (cur->size > size) {
                cur->start += size;
                cur->size  -= size;
            } else { // Exact fit, remove hole from free list
                if (prev) {
                    prev->next = cur->next;
                } else {
                    free_list = cur->next;
                }
                free(cur);
            }
            return cur->start; // Return starting index of allocated memory
        }
        prev = cur;
        cur = cur->next;
    }
}

int get_worst_fit_index(int table[1024][3], int size) {
    int worst_index = -1;
    int worst_size = -1;

    for (int i = 0; i < 1024; i++) {
        if (table[i][0] != -1 && table[i][2] >= size) {
            if (table[i][2] > worst_size) {
                worst_size = table[i][2];
                worst_index = i;
            }
        }
    }
    return worst_index;
}

int get_first_fit_index(int table[1024][3], int size) {
    for (int i = 0; i < 1024; i++) {
        if (table[i][0] != -1 && table[i][2] >= size) {
            return i;
        }
    }
    return -1;
}

int complete_instruction(char *ins[], Hole *free_list, int table[1024][3], char *mem) {
    if (strcmp(ins[0], "ALLOC") == 0) {
        printf("%s %s %s\n", ins[0], ins[1], ins[2]);
        int free_index = get_free_table_index(table);

        int get_best_index = get_best_fit_index(free_list, atoi(ins[2]), mem);


        table[free_index][0] = atoi(ins[1]); 
        table[free_index][1] = 
        table[free_index][2] = atoi(ins[2]); 
        return 1;
    }
    if (strcmp(ins[0], "FREE") == 0) {
        printf("%s %s\n", ins[0], ins[1]);
        return 2;
    };
    if (strcmp(ins[0], "PRINT") == 0) {
        printf("%s\n", ins[0]);
        return 3;
    }
    if (strcmp(ins[0], "REALLOC") == 0) {
        printf("%s %s %s\n", ins[0], ins[1], ins[2]);
        return 4;
    }
    return -1;
}


int main(int argc, char *argv[]) {
    char *mem = malloc(1024000);
    Hole *free_list = NULL;
    Hole *h = malloc(sizeof(Hole));
    h->start = 0;
    h->size = 1024000;
    h->next = NULL;
    free_list = h;

    int table[1024][3];
    for (int i = 0; i < 1024; i++) {
        table[i][0] = -1;
        table[i][1] = -1;
        table[i][2] = -1;
    }

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_entrada>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo '%s'\n", argv[1]);
        return 1;
    }


    char line[256];
    int line_number = 1;
    char word[256];
    int word_index = 0;
    char *ins[3];
    int ins_index = 0;

    while (fgets(line, sizeof(line), file) != NULL) {

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\n') {
                word[word_index] = '\0';
                ins[ins_index++] = strdup(word);
                complete_instruction(ins, table, mem);
                word_index = 0;
                ins_index = 0;
                break;
            }
            if (line[i] == ' ') {
                word[word_index] = '\0';
                word_index = 0;
                ins[ins_index++] = strdup(word);
                continue;
            } 
            word[word_index++] = line[i];
        }
        word_index = 0;
        line_number++;
    }

    // Cerrar el archivo
    fclose(file);
    
    return 0;
}