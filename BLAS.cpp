//
//  BLAS.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "BLAS.h"
#include <iostream>

// Inplace bias addition
void addBias( float *inout, const float* bias, int32_t ch, int32_t n)
{
    for (int32_t c = 0; c < ch; c++) {
        for (int32_t i = 0; i < n; i++) {
            inout[c*n+i] += bias[c];
        }
    }
}

// Fill array with a number
void fill(int32_t N, float ALPHA, float *X)
{
    for(int32_t i = 0; i < N; ++i) X[i] = ALPHA;
}

// Matrix multiplication C += ALPHA*A*B
void gemm_nn(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc)
{
    for(int32_t i = 0; i < M; ++i){
        for(int32_t k = 0; k < K; ++k){
            float A_PART = ALPHA*A[i*lda+k];
            if( A_PART != 0.f )
            {
                for(int32_t j = 0; j < N; ++j){
                    C[i*ldc+j] += A_PART*B[k*ldb+j];
                }
            }
        }
    }
}

// Matrix multiplication C += ALPHA*A*B^T
void gemm_nt(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc)
{
    for(int32_t i = 0; i < M; ++i){
        for(int32_t j = 0; j < N; ++j){
            float sum = 0;
            for(int32_t k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

// Matrix multiplication C += ALPHA*A^T*B
void gemm_tn(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc)
{
    for(int32_t i = 0; i < M; ++i){
        for(int32_t k = 0; k < K; ++k){
            float A_PART = ALPHA*A[k*lda+i];
            if( A_PART != 0.f )
            {
                for(int32_t j = 0; j < N; ++j){
                    C[i*ldc+j] += A_PART*B[k*ldb+j];
                }
            }
        }
    }
}

// Matrix multiplication C += ALPHA*A^T*B^T
void gemm_tt(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc)
{
    for(int32_t i = 0; i < M; ++i){
        for(int32_t j = 0; j < N; ++j){
            float sum = 0;
            for(int32_t k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}

// Generic Metrix Multiplication C = BETA*C + ALPHA*A^TA*B^TB
void gemm(bool TA, bool TB, int32_t M, int32_t N, int32_t K, float ALPHA,
          const float *A, int32_t lda,
          const float *B, int32_t ldb,
          float BETA,
          float *C, int32_t ldc)
{
    if( BETA != 1.f )
    {
        for(int32_t i = 0; i < M; ++i){
            for(int32_t j = 0; j < N; ++j){
                C[i*ldc + j] *= BETA;
            }
        }
    }
    if(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    else if(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    else if(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    else
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

// Affine normalization: out = gamma * (in-mean)/(sqrt(var)) + beta
// Note: 1/sqrt(var) is computed in advance
void batchNorm(float *x, const float *mean, const float *variance, const float *gamma, const float *beta, int32_t filters, int32_t spatial)
{
    for(int32_t f = 0; f < filters; ++f){
        float denum = gamma[f]*variance[f];
        for(int32_t i = 0; i < spatial; ++i){
            int index = f*spatial + i;
            x[index] = (x[index] - mean[f])*denum+beta[f];
        }
    }
}

// Non-affine normalization: out = (in-mean)/(sqrt(var))
// Note: 1/sqrt(var) is computed in advance
void batchNorm(float *x, const float *mean, const float *variance, int32_t filters, int32_t spatial)
{
    for(int32_t f = 0; f < filters; ++f){
        for(int32_t i = 0; i < spatial; ++i){
            int index = filters*spatial + f*spatial + i;
            x[index] = (x[index] - mean[f])*variance[f];
        }
    }
}

// Matrix creation for Convolutional layers
void im2col(const float* data_im,
            int32_t channels, int32_t height, int32_t width,
            int32_t ksize, int32_t stride, int32_t pad, int32_t dilation, float* data_col)
{
    int32_t height_col = (height + 2 * pad - (dilation * (ksize - 1) + 1)) / stride + 1;
    int32_t width_col = (width + 2 * pad - (dilation * (ksize - 1) + 1)) / stride + 1;
    
    int32_t channels_col = channels * ksize * ksize;
    for (int32_t c = 0; c < channels_col; ++c) {
        int32_t w_offset = ( c % ksize )*dilation;
        int32_t h_offset = ((c / ksize) % ksize)*dilation;
        int32_t c_im = c / ksize / ksize;
        for (int32_t h = 0; h < height_col; ++h) {
            for (int32_t w = 0; w < width_col; ++w) {
                int32_t im_row = h_offset + h * stride - pad;
                int32_t im_col = w_offset + w * stride - pad;
                int32_t col_index = (c * height_col + h) * width_col + w;
                if (im_row >= 0 && im_col >= 0 && im_row < height && im_col < width)
                    data_col[col_index] = data_im[im_col + width*(im_row + height*c_im)];
                else
                    data_col[col_index] = 0;
            }
        }
    }
}

// Image creation for transposed convolution
void col2im(const float* data_col,
            int32_t channels, int32_t height, int32_t width,
            int32_t ksize, int32_t stride, int32_t pad, int32_t outpad, float* data_im)
{
    int32_t height_col = (height + 2*pad - ksize) / stride + 1;
    int32_t width_col = (width + 2*pad - ksize) / stride + 1;
    
    int32_t channels_col = channels * ksize * ksize;
    for (int32_t c = 0; c < channels_col; ++c) {
        int32_t w_offset = c % ksize;
        int32_t h_offset = (c / ksize) % ksize;
        int32_t c_im = c / ksize / ksize;
        for (int32_t h = 0; h < height_col; ++h) {
            for (int32_t w = 0; w < width_col; ++w) {
                int32_t im_row = h_offset + h * stride - pad;
                int32_t im_col = w_offset + w * stride - pad;
                int32_t col_index = (c * height_col + h) * width_col + w;
                if (im_row >= 0 && im_col >= 0 && im_row < height-outpad && im_col < width-outpad)
                    data_im[im_col + (width)*(im_row + (height)*c_im)] += data_col[col_index];
            }
        }
    }
}

// Reorder neurons:
// If forward: (WxHxC)->(W/stride x H/Stride x C*stride^2)
// If backward: (WxHxC)->(W*stride x H/*tride x C/stride^2)
void reorg(const float *x, int32_t w, int32_t h, int32_t c, int32_t stride, bool forward, float *out)
{
    int32_t dScale = stride*stride;
    
    for(int32_t k = 0; k < c; ++k){
        for(int32_t j = 0; j < h; ++j){
            for(int32_t i = 0; i < w; ++i){
                int32_t in_index  = i + w*(j + h*k);
                int32_t c2 = k / dScale;
                int32_t offset = k % dScale;
                int32_t w2 = i*stride + offset % stride;
                int32_t h2 = j*stride + offset / stride;
                int32_t out_index = w2 + w*stride*(h2 + h*stride*c2);
                if(forward) out[out_index] = x[in_index];
                else out[in_index] = x[out_index];
            }
        }
    }
}

// Concatenate
void concat(const float *x, const float *y, int32_t n1, int32_t n2, float *out)
{
    for (int32_t i = 0; i < n1; i++) {
        out[ i ] = x[ i ];
    }
    for (int32_t i = 0; i < n2; i++) {
        out[ n1 + i ] = y[ i ];
    }
}

// Shortcut
// Out += add
void shortcut( int32_t w, int32_t h, int32_t ch, const float *add, float *out)
{
    for(int32_t k = 0; k < ch; ++k){
        for(int32_t j = 0; j < h; ++j){
            for(int32_t i = 0; i < w; ++i){
                int32_t out_index = i + w*(j + h*k);
                out[out_index] += add[out_index];
            }
        }
    }
}
