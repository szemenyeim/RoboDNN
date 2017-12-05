//
//  BLAS.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef BLAS_h
#define BLAS_h

#include <cstdint>


void im2col(const float* data_im,
            int32_t channels, int32_t height, int32_t width,
            int32_t ksize, int32_t stride, int32_t pad, int32_t dilation, float* data_col);

void col2im(const float* data_col,
            int32_t channels, int32_t height, int32_t width,
            int32_t ksize, int32_t stride, int32_t pad, int32_t outpad, float* data_im);


void reorg(const float *x, int32_t w, int32_t h, int32_t c, int32_t stride, bool forward, float *out);
void concat(const float *x, const float *y, int32_t n1, int32_t n2, float *out);
void shortcut( int32_t w, int32_t h, int32_t ch, const float *add, float *out);

void addBias( float *inout, const float* bias, int32_t ch, int32_t n);
void fill(int32_t N, float ALPHA, float *X);

void gemm(bool TA, bool TB, int32_t M, int32_t N, int32_t K, float ALPHA,
          const float *A, int32_t lda,
          const float *B, int32_t ldb,
          float BETA,
          float *C, int32_t ldc);

void gemm_nn(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc);
void gemm_nt(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc);
void gemm_tn(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc);
void gemm_tt(int32_t M, int32_t N, int32_t K, float ALPHA,
             const float *A, int32_t lda,
             const float *B, int32_t ldb,
             float *C, int32_t ldc);

void batchNorm(float *x, const float *mean, const float *variance, const float *gamma, const float *beta, int32_t filters, int32_t spatial);
void batchNorm(float *x, const float *mean, const float *variance, int32_t filters, int32_t spatial);

#endif /* BLAS_h */
