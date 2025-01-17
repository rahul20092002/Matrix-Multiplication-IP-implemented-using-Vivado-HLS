#include <hls_stream.h>
#include <ap_int.h>
#include <cmath>
#include <cstdlib>

#define MATSIZE 8
#define solution1

typedef float Mat_Dtype;

struct axis_data {
    Mat_Dtype data;
    ap_uint<1> last;
};

void matrixmul_1(hls::stream<axis_data> &in_A, hls::stream<axis_data> &in_B, hls::stream<axis_data> &out_C);
