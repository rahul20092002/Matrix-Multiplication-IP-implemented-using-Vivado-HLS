#include "matrixmul.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void matrix_bm(double input_A[MATSIZE][MATSIZE], double input_B[MATSIZE][MATSIZE], double output_C[MATSIZE][MATSIZE]);

int main() {
    double input_A[MATSIZE][MATSIZE];
    double input_B[MATSIZE][MATSIZE];
    double output_C_BM[MATSIZE][MATSIZE], output_C_HW[MATSIZE][MATSIZE];
    int row, col;

    // Generate random input matrices
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            input_A[row][col] = rand() % 100;  // Random numbers between 0 and 99
            input_B[row][col] = rand() % 100;  // Random numbers between 0 and 99
        }
    }

    // Software matrix multiplication
    matrix_bm(input_A, input_B, output_C_BM);

    axis_data local_stream;
    hls::stream<axis_data> in_A, in_B, out_C;

    // Write input_A to AXI stream
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            local_stream.data = input_A[row][col];
            local_stream.last = (row == MATSIZE - 1 && col == MATSIZE - 1);
            in_A.write(local_stream);
        }
    }

    // Write input_B to AXI stream
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            local_stream.data = input_B[row][col];
            local_stream.last = (row == MATSIZE - 1 && col == MATSIZE - 1);
            in_B.write(local_stream);
        }
    }

#ifdef solution1
    // Call the hardware function
    matrixmul_1(in_A, in_B, out_C);
#endif

    // Read output_C from AXI stream
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            local_stream = out_C.read();
            output_C_HW[row][col] = local_stream.data;
        }
    }

    // Verify the results
    int error_count = 0;
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            if (fabs(output_C_BM[row][col] - output_C_HW[row][col]) > 1e-6) {
                printf("Mismatch at [%d][%d]: BM=%.2f, HW=%.2f\n", row, col, output_C_BM[row][col], output_C_HW[row][col]);
                error_count++;
            }
        }
    }

    if (error_count == 0) {
        printf("Matrix multiplication passed!\n");
    } else {
        printf("Matrix multiplication failed with %d errors.\n", error_count);
    }

    return 0;
}

// Software matrix multiplication (Behavioral Model)
void matrix_bm(double input_A[MATSIZE][MATSIZE], double input_B[MATSIZE][MATSIZE], double output_C[MATSIZE][MATSIZE]) {
    int row, col, index;
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            double res = 0;
            for (index = 0; index < MATSIZE; index++) {
                res += input_A[row][index] * input_B[index][col];
            }
            output_C[row][col] = res;
        }
    }
}
