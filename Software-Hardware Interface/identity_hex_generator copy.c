


#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *file = fopen("../program.hex", "w");

    if (file == NULL) {
        perror("Could not open ../program.hex");
        return EXIT_FAILURE;
    }

    for (int value = 0; value <= 0xFF; value++) {
        fprintf(file, "%02X\n", value);
    }

    fclose(file);

    printf("Generated ../program.hex with 256 identity bytes.\n");
    return EXIT_SUCCESS;
}