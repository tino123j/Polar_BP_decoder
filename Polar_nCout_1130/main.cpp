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
	seed = (unsigned)time(NULL);	 // ���o�ɶ��ǦC
	srand(seed);					// �H�ɶ��ǦC��üƺؤl*/
	unsigned int finish = time(NULL);
	unsigned int finish2 = 0;


	G_array();										// ��XG�x�}[N][N]
	for (int i = 0; i < 10; i++) {					// ���j�鬰�n�]��SNR�d��
		finish2 = time(NULL);
		SNR_dB = i;
		BE_total = 0;								// �C��SNR BE_total��l��0   **BE_total���ѽX��o�쪺�X �P ���J������ ���ۦP���ƶq 
		Forzen();									// ��X frozen ����l F=[1,2,3,5...]
		for (int a = 0; a < run_times ; a++) {		// �C��SNR"�n�]������"
			// 1.���ͶüưT��
			message();
			
			// 2.�N�T���[�W���l���Ϳ�J�X
			message_add_frozen();
			
			// 3.�s�X
			Encoding();
			
			// 4.BPSK����
			Modulatio_BPSK();
			
			//-------------------------
			// 5.AWGN�q�D-(�����������T)
			awgn(SNR_dB);
			
			// 6.r=e+c
			for(int i=0;i<N;i++){
				r[i] = e[i] + c[i];
			}
			
			
			//-------------------------
			// 7.BP�ѽX
			BPdecoding();
			
			// 8.�ѽ���
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
