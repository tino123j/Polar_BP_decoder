#define _USE_MATH_DEFINES
#include <math.h>       /* tanh, log */
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include "function.h"
using namespace std;

void G_array() {
	int G0[N][N] = { 0 };
	G0[0][0] = 1;
	G0[0][1] = 0;
	G0[1][0] = 1;
	G0[1][1] = 1;
	for (int n = 1; n < (N_log2); n++) {
		for (int i = 0; i < pow(2, n + 1); i++) {
			for (int j = 0; j < pow(2, n + 1); j++) {
				
				if (j < pow(2, n) && i < pow(2, n)) {
					G[i][j] = G0[i][j];
				}
				else if (j >= pow(2, n) && i < pow(2, n)) {
					G[i][j] = 0;
				}
				else if (j < pow(2, n) && i >= pow(2, n)) {
					G[i][j] = G0[i - int(pow(2, n))][j];
				}
				else {
					G[i][j] = G0[i - int(pow(2, n))][j - int(pow(2, n))];
				}
			}
		}
		for (int a = 0; a < pow(2, n + 1); a++) {
			for (int b = 0; b < pow(2, n + 1); b++) {
				G0[a][b] = G[a][b];
			}
		}
	}
}

void Forzen() {										//根據論文中PCC0的公式

	double z[N] = { 0 };							//給定N個z
	//z[0] =exp(-pow(10, SNR_dB / 10));				//z[0] 初始值
	z[0] = 0.5;
	for (int j = 1; j <= (N_log2); j++) {			//以下根據公式	/
		int u = pow(2, j);							//				/
		for (int t = 0; t <= (u / 2) - 1; t++) {	//				/
			double T = z[t];						//				/
			z[t] = 2 * T - (T*T);					//				/
			z[u / 2 + t] = T * T;					//--------------
		}
	}
	int count = 0;
	for (int i = 0; i < N / 2; i++) {				//找出較大的K個z	/
		double max = 0;								//此處方法為		/
		int a = 0;									//
		for (int j = 0; j < N; j++) {				//從0~N每個都找一遍	/
			if (max < z[j]) {						//找出最大的		/
				max = z[j];							//丟出來設為-99		/
				a = j;								//重複K次			/
			}
		}
		z[a] = -99;
		count++;							//--------------------------/
	}

	int a = 0;
	for (int i = 0; i < N; i++) {			//丟出frozen的位子----------/
		if (int(z[i]) == -99) {				//把剛剛設為-99的"位子"		/
											//丟出來					/
			F[a] = i;						//即得到frozen bit的位子	/
			a = a + 1;						//--------------------------/

		}
	}

}

void message() {                               //產生亂數in_u 
	for (int i = 0; i < (N / 2); i++) {
		in_u[i] = rand() % 2;
	}
}

void message_add_frozen() {
	//----------將訊號與Frozen位子組合為u-----
	int in_u_a = 0;
	for (int j = 0; j < N; j++) {
		bool flag = false;
		for (int i = 0; i < N / 2; i++) {
			if (j == (F[i])) {
				flag = true;
				break;
			}
		}
		if (flag == true) {
			continue;
		}
		u[j] = in_u[in_u_a];
		in_u_a = in_u_a + 1;
	}

}

void Encoding() {

	//----------乘上生成矩陣--------
	for (int i = 0; i < N; i++) {    // 初始化c = 0
		c[i] = 0;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			c[i] = c[i] ^ (u[j] & G[j][i]);
		}
	}
}

void Modulatio_BPSK() {
	//----------BPSK----------
	for (int j = 0; j < N; j++) {
		if (c[j] == 0) {
			c[j] = 1;
		}
		else {
			c[j] = -1;
		}
	}
}

void awgn(double SNR_dB) {				// 參考 Box Muller的方法 模擬出高斯雜訊
	double snr = pow(10, SNR_dB / 10);
	//double std = sqrt(1 / ( 2 *snr ));//1
	double std = sqrt(1 / ( 2 * 0.5 *snr ));//2
	//double std = sqrt(1 / ( SNR_dB ));  //3
	//double std = sqrt(1 / ( 0.5 *snr ));//4
	//double std = sqrt(1 / ( 2 * 2 * 0.5 *snr ));//6
	double u = 0, v = 0, x = 0;
	for (int i = 0; i < N; i++)
	{
		u = rand() / (double)RAND_MAX;
		v = rand() / (double)RAND_MAX;
		x = sqrt(-2 * log(u)) * cos(2 * M_PI * v) * std;
		e[i] = x;

	}

}



void BPdecoding() {
	for (int i = 0; i < N / 2; i++) {  //初始化L_temp=0
		for (int j = 0; j < N; j++) {
			L_temp[i][j] = 0;
		}
	}

	//----------初始化-------
double L_n[N]={ 0 };
double R_0[N]={ 0 };
	for (int i = 0; i < N / 2; i++) {  //初始化L,R=0
		for (int j = 0; j < N; j++) {
			L[i][j] = 0;
			R[i][j] = 0;
		}
	}
	//cout << "------initial-----" <<endl;
	for (int j = 0; j < N; j++) {
		L[N_log2][j] = ((r[j]) * 2) * pow(10, SNR_dB / 10);
		//L[N_log2][j] = (r[j]);
		L_n[j] = L[N_log2][j];

	}
	//cout << "//" << endl;
	for (int j = 0; j < N / 2; j++) {
		R[0][int(F[j])] = INFINITY;
		R_0[int(F[j])] = R[0][int(F[j])];
	}

	
	
	//-------------開始計算-----
	for (int a = 0; a < iteration; a++) {
		//-------------L----------------
		for (int i = N_log2; i > 0; i--) {
			for (int j = 1; j <= N / 2; j++) {
				L[(i)-1][(j)-1] = 2 * atanh(tanh(L[(i + 1) - 1][(2 * j - 1) - 1] / 2)*tanh((L[(i + 1) - 1][(2 * j) - 1] + R[(i)-1][(j + N / 2) - 1]) / 2));
				L[(i)-1][(j + N / 2) - 1] = 2 * atanh(tanh(R[(i)-1][(j)-1] / 2)*tanh(L[(i + 1) - 1][(2 * j - 1) - 1] / 2)) + L[(i + 1) - 1][(2 * j) - 1];
			}
		}


		
		int frozen = 0;
		for (int j = 0; j < N; j++) {  //把最左邊的L 丟給最左邊的R

			R[0][j] = R_0[j];
		}
		//------------R---------------
		for (int i = 1; i <= N_log2; i++) {
			for (int j = 1; j <= N / 2; j++) {
				R[(i + 1) - 1][(2 * j - 1) - 1] = 2 * atanh(tanh(R[(i)-1][(j)-1] / 2)*tanh((R[(i)-1][(j + N / 2) - 1] + L[(i + 1) - 1][(2 * j) - 1]) / 2));
				R[(i + 1) - 1][(2 * j) - 1] = 2 * atanh(tanh(R[(i)-1][(j)-1] / 2)*tanh(L[(i + 1) - 1][(2 * j - 1) - 1] / 2)) + R[(i)-1][(j + N / 2) - 1];

			}
		}


		//---------判斷本次迭代算出的值是否有效，無效(nan)時 讀取上次存的值 並跳出迭代-------
		int x = 0;
		for (int j = 0; j < N; j++) {
			if (isnan(R[N_log2][j]) == 0) {
				L[N_log2][j] =  L_n[j];//R[N_log2][j] +   //確認有效後 把最右邊的R丟給最右邊的L
				L_temp[0][j] = L[0][j];
			}
			else {
				x = x + 1;
				break;
			}
		}
		if (x != 0) {
			break;
		}
	}
}

void Demodulation() {
	//-------解調 判決-----

	for (int j = 0; j < N; j++) {
		if (L_temp[0][j] > 0) {
			ch[j] = 0;
		}
		else {
			ch[j] = 1;
		}

	}

}

void BER() {

	int BitError = 0;
	int d = 0;
	for (int j = 0; j < N; j++) {			//只選擇message的位子來做比對
		bool flag = false;
		for (int i = 0; i < N / 2; i++) {
			if (j == (F[i])) {
				flag = true;
				break;
			}
		}
		if (flag == true) {
			continue;
		}

		if (in_u[d] != ch[j]) {
			BitError = BitError + 1;
			BE_total = BE_total + 1;
		}
		d = d + 1;
	}

	//cout << "BitE = " << BitError << endl;
}
