#include "initial.h"

int BE_total = 0;
double e[N] = { 0 };
int F[N / 2] = { 0 };
int in_u[N / 2] = { 0 };
int G[N][N] = { 0 };
double SNR_dB = 0;
int c[N] = { 0 };
double L[N_log2 + 1][N] = { 0 };
double R[N_log2 + 1][N] = { 0 };
double L_temp[N][N] = { 0 };
int ch[N] = { 0 };
int u[N] = { 0 };
double r[N] ={0};