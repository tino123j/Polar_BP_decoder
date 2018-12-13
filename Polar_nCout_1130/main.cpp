#define _USE_MATH_DEFINES
#include <stdio.h>      /* printf */
#include <math.h>       /* tanh, log */
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include "function.h"
#include "initial.h"

using namespace std;

int main() {
	unsigned seed;
	seed = (unsigned)time(NULL);	 // 取得時間序列
	srand(seed);					// 以時間序列當亂數種子*/
	unsigned int finish = time(NULL);
	unsigned int finish2 = 0;


	G_array();										// 算出G矩陣[N][N]
	for (int i = 0; i < 10; i++) {					// 此迴圈為要跑的SNR範圍
		finish2 = time(NULL);
		SNR_dB = i;
		BE_total = 0;								// 每次SNR BE_total初始為0   **BE_total為解碼後得到的碼 與 原輸入做比對後 不相同的數量 
		Forzen();									// 算出 frozen 的位子 F=[1,2,3,5...]
		for (int a = 0; a < run_times ; a++) {		// 每個SNR"要跑的次數"
			// 1.產生亂數訊息
			message();
			
			// 2.將訊息加上冗餘產生輸入碼
			message_add_frozen();
			
			// 3.編碼
			Encoding();
			
			// 4.BPSK調變
			Modulatio_BPSK();
			
			//-------------------------
			// 5.AWGN通道-(模擬高斯雜訊)
			awgn(SNR_dB);
			
			// 6.r=e+c
			for(int i=0;i<N;i++){
				r[i] = e[i] + c[i];
			}
			
			
			//-------------------------
			// 7.BP解碼
			BPdecoding();
			
			// 8.解調變
			Demodulation();
			
			// 9.BER
			BER();

		}
		cout << "SNR = " << SNR_dB << "  BE = " << BE_total << "  BER = " << double(BE_total) /(run_times*(N/2));
		finish = time(NULL);
		printf(" run time:%u \n", finish - finish2);
	}


	finish = time(NULL);
	printf("\nrun time:%u \n", finish - seed);
	cout << "runtimes = " << run_times <<endl;
	//	system("PAUSE");
	return 0;
}
