#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1024000
#define MAX_FRAMES 1000 

// Estructura para llevar control de bloques asignados
typedef struct {
    int start;
    int size;   
    char name;   
    int is_free;   
} Frame;

// Variables globales
char *memory;
Frame frames[MAX_FRAMES]; 
int frame_count = 0;    
int allocation_strategy = 0; // 0: First Fit, 1: Best Fit, 2: Worst Fit

// Initialize memory
void init_memory() {
    memory = (char*)malloc(MEMORY_SIZE);
    if (memory == NULL) {
        printf("Error: Could not allocate memory\n");
        exit(1);
    }
    // Initialize all memory with '.'
    memset(memory, '.', MEMORY_SIZE);
}

// Find free space using First Fit
int find_free_space_first_fit(int size) {
    for (int i = 0; i <= MEMORY_SIZE - size; i++) {
        int found = 1;
        for (int j = 0; j < size; j++) {
            if (memory[i + j] != '.') {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    return -1; // No space available
}

int find_free_space_best_fit(int size) {
    int best_index = -1;
    int best_size = MEMORY_SIZE + 1;

    for (int i = 0; i <= MEMORY_SIZE - size; i++) {

        int block_size = 0;
        while (i + block_size < MEMORY_SIZE && memory[i + block_size] == '.') {
            block_size++;
        }
        if (size < block_size < best_size) {
            best_size = block_size;
            best_index = i;
        }
        i += block_size - 1; // Skip checked block
    }
    return best_index;
}

int find_free_space_worst_fit(int size) {
    int worst_index = -1;
    int worst_size = -1;

    for (int i = 0; i <= MEMORY_SIZE - size; i++) {
        int block_size = 0;
        while (i + block_size < MEMORY_SIZE && memory[i + block_size] == '.') {
            block_size++;
        }
        if (size < block_size > worst_size) {
            worst_size = block_size;
            worst_index = i;
        }
        i += block_size - 1; // Skip checked block
    }
    return worst_index;
}

// Simulate ALLOC
void my_malloc(char name, int size) {
    
    int start;
    if (allocation_strategy == 0) {
        start = find_free_space_first_fit(size);
    } else if (allocation_strategy == 1) {
        start = find_free_space_best_fit(size);
    } else {
        start = find_free_space_worst_fit(size);
    }
    
    if (start == -1) {
        printf("ALLOC %c %d: ERROR - Not enough space available\n", name, size);
        return;
    }
    
    // Mark memory as occupied
    for (int i = 0; i < size; i++) {
        memory[start + i] = name;
    }
    
    // Add to control table
    frames[frame_count].start = start;
    frames[frame_count].size = size;
    frames[frame_count].name = name;
    frames[frame_count].is_free = 0;
    frame_count++;
    
    printf("ALLOC %c %d: Allocated at position %d\n", name, size, start);
}

// Simulate free
void my_free(char name) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i].name == name && frames[i].is_free == 0) {
            // Mark memory as free
            for (int j = 0; j < frames[i].size; j++) {
                memory[frames[i].start + j] = '.';
            }
            frames[i].is_free = 1;
            printf("FREE %c: Released frame of %d bytes at position %d\n", 
                   name, frames[i].size, frames[i].start);
            return;
        }
    }
    printf("FREE %c: ERROR - frame not found or already freed\n", name);
}

// Simulate realloc
void my_realloc(char name, int new_size) {
    // Find the frame
    for (int i = 0; i < frame_count; i++) {
        if (frames[i].name == name && frames[i].is_free == 0) {
            int old_size = frames[i].size;
            int old_start = frames[i].start;
            
            // Free current frame
            my_free(name);
            
            // Try to allocate new size
            my_malloc(name, new_size);
            
            printf("REALLOC %c %d: Changed from %d bytes to %d bytes\n", 
                   name, new_size, old_size, new_size);
            return;
        }
    }
    printf("REALLOC %c %d: ERROR - frame not found\n", name, new_size);
}

// Show memory status
void print_memory() {
    printf("\nCurrently allocated frames:\n");
    printf("Name   | Start  | Size   | End\n");
    printf("-------|--------|--------|--------\n");
    
    for (int i = 0; i < frame_count; i++) {
        if (frames[i].is_free == 0) {
            printf("   %c   | %6d | %6d | %6d\n", 
                   frames[i].name, frames[i].start, frames[i].size, 
                   frames[i].start + frames[i].size - 1);
        }
    }
    printf("\n");
}

// Process command file
void process_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }
    
    char line[256];
    char command[20];
    char name;
    int size;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%s %c %d", command, &name, &size) == 3) {
            if (strcmp(command, "ALLOC") == 0) {
                my_malloc(name, size);
            } else if (strcmp(command, "REALLOC") == 0) {
                my_realloc(name, size);
            }
        } else if (sscanf(line, "%s %c", command, &name) == 2) {
            if (strcmp(command, "FREE") == 0) {
                my_free(name);
            }
        } else if (sscanf(line, "%s", command) == 1) {
            if (strcmp(command, "PRINT") == 0) {
                print_memory();
            } 
        }
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command_file>\n", argv[0]);
        return 1;
    }
    
    allocation_strategy = 0; // Default to First Fit
    if (argc == 3) {
        if (strcmp(argv[2], "best") == 0) {
            allocation_strategy = 1;
        } else if (strcmp(argv[2], "worst") == 0) {
            allocation_strategy = 2;
        }
    }
    init_memory();
    process_file(argv[1]);
        
    free(memory);
    
    return 0;
}