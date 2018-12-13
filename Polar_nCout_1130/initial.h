#pragma once

#define N 8
#define N_log2 3
#define run_times 100000
#define code_rate 0.5
#define iteration 60


extern int BE_total;
extern double e[N];
extern int F[N / 2];
extern int in_u[N / 2];
extern int G[N][N];
extern double SNR_dB;
extern int c[N];
extern double L[N_log2 + 1][N];
extern double R[N_log2 + 1][N];
extern double L_temp[N][N];
extern int ch[N];
extern int u[N];
extern double r[N];