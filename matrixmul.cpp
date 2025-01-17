#include "matrixmul.h"

#ifdef solution1
void matrixmul_1(hls::stream<axis_data> &in_A, hls::stream<axis_data> &in_B, hls::stream<axis_data> &out_C) {
    #pragma HLS INTERFACE ap_ctrl_none port=return
    #pragma HLS INTERFACE axis register both port=in_A
    #pragma HLS INTERFACE axis register both port=in_B
    #pragma HLS INTERFACE axis register both port=out_C

    Mat_Dtype input_A[MATSIZE][MATSIZE];
    Mat_Dtype input_B[MATSIZE][MATSIZE];
    Mat_Dtype output_C[MATSIZE][MATSIZE];
    int row, col, index;
    axis_data local_stream;

    // Read Matrix A
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            local_stream = in_A.read();
            input_A[row][col] = local_stream.data;
        }
    }

    // Read Matrix B
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            local_stream = in_B.read();
            input_B[row][col] = local_stream.data;
        }
    }

    // Perform Matrix Multiplication
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            Mat_Dtype res = 0;
            for (index = 0; index < MATSIZE; index++) {
                res += input_A[row][index] * input_B[index][col];
            }
            output_C[row][col] = res;
        }
    }

    // Write Output Matrix
    for (row = 0; row < MATSIZE; row++) {
        for (col = 0; col < MATSIZE; col++) {
            local_stream.data = output_C[row][col];
            local_stream.last = (row == MATSIZE - 1 && col == MATSIZE - 1);
            out_C.write(local_stream);
        }
    }
}
#endif
