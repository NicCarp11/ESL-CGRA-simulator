#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY_PER_BANK 32768  // 32KB per banco di memoria
#define WORD_SIZE 4  // 32 bit = 4 byte
#define DIMENSIONS_COUNT 24  // Aggiornato per riflettere il nuovo numero di dimensioni

unsigned long calculate_memory_usage(int rows, int cols, int channels, int filters) {
    return (unsigned long)((rows * cols * channels * WORD_SIZE) + (3 * 3 * channels * filters * WORD_SIZE) + (filters * (rows-2) * (cols -2) * WORD_SIZE));
}

int select_im2col_value(int channels) {
    if (channels == 16) return 679;
    if (channels > 16 && channels <= 32) return 1444;
    switch (channels) {
        case 48: return 2181;
        case 64: return 2903;
        case 80: return 3611;
        case 96: return 4312;
        case 112: return 5018;
        case 128: return 5731;
        case 144: return 6390;
        default: return 0; // Valore di default nel caso non corrisponda a nessuno dei casi specifici
    }
}
int select_im2col_value_out(int filters) {
    if (filters == 16) return 6278;
    if (filters > 16 && filters <= 32) return 12639;
    switch (filters) {
        case 48: return 18939;
        case 64: return 25235;
        case 80: return 31528;
        case 96: return 37756;
        case 112: return 43822;
        case 128: return 49871;
        case 144: return 55927;
        default: return 0; // Valore di default nel caso non corrisponda a nessuno dei casi specifici
    }
}
int conv_direc(int filters) {
    if (filters == 16) return 6994;
    if (filters > 16 && filters <= 32) return 13717;
    switch (filters) {
        case 48: return 20554;
        case 64: return 27390;
        case 80: return 34229;
        case 96: return 41173;
        case 112: return 48007;
        case 128: return 54896;
        case 144: return 61848;
        default: return 0; // Valore di default nel caso non corrisponda a nessuno dei casi specifici
    }
}
int conv_ws(int rows, int channels, int filters) {
    switch (rows) {
        case 16: return 2871;
        case 17: return 3276;
        case 18: return 3709;
        case 19: return 4175;
        case 20: return 4665;
        case 21: return 5177;
        case 22: return 5728;
        case 23: return 6295;
        case 24: return 6891;
        case 25: return 7523;
        case 26: return 8176;
        case 27: return 8851;
        case 28: return 9575;
        case 29: return 10307;
        case 30: return 11065;
        case 31: return 11864;
        case 32: return 12679;
        case 48: return 29218;
        case 64: return 51914;
        default: return 0; // Valore di default nel caso non corrisponda a nessuno dei casi specifici
    }
}

void generate_combinations(int max_memory_banks, FILE *file) {
    unsigned long max_memory = max_memory_banks * MAX_MEMORY_PER_BANK;
    int dimensions[DIMENSIONS_COUNT] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 48, 64, 80, 96, 112, 128, 144};
    int i, k, l;

    fprintf(file, "Rows & Cols, Channels, Filters, Memory Usage,,WS-1-ex, WS-(MAC/CYCLE),IM2COL-IC-1EX ,IM2C IC  (MAC/CYCLE), IM2C OC 1 EX, IM2C OC  (MAC/CYCLE), CONV OC 1 EX, CONV OC  (MAC/CYCLE)\n");
    

    for (i = 0; i < DIMENSIONS_COUNT; i++) {
        int rows = dimensions[i];
        int cols = dimensions[i]; // Usa lo stesso valore per le colonne
        for (k = 0; k < DIMENSIONS_COUNT; k++) {
            for (l = 0; l < DIMENSIONS_COUNT; l++) {
                int channels = dimensions[k];
                int filters = dimensions[l];
                unsigned long memory_usage = calculate_memory_usage(rows, cols, channels, filters);
                int im2col_value = select_im2col_value(channels);
                int im2col_value_output = select_im2col_value_out(filters);
                int conv_value_output = conv_direc(filters);
                int ws_value = conv_ws(rows,channels,filters);
                if (memory_usage <= max_memory) {
                    fprintf(file, "%d, %d, %d, %lukB, ,%d, ,%d , ,%d, ,%d, ,\n",
                           rows, channels, filters, memory_usage / 1024, ws_value, im2col_value,im2col_value_output, conv_value_output);
                }
            }
        }
    }
}

int main() {
    int max_memory_banks;
    FILE *file;

    printf("Enter the maximum number of memory banks: ");
    scanf("%d", &max_memory_banks);

    file = fopen("output.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    generate_combinations(max_memory_banks, file);
    fclose(file);

    printf("Combinations have been written to output.txt\n");

    return 0;
}
