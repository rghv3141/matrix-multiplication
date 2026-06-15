#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>
using namespace std;

int main()
{

        vector<vector<double>> A(512, vector<double>(512));
        vector<vector<double>> B(512, vector<double>(512));
        vector<vector<double>> C(512, vector<double>(512));

        int N = 512;
        const int b = 16;       //b is the block size
        for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                        A[i][j] = i + j;
                        B[i][j] = i - j;
                }
        }
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; i++) {
        	for (int j = 0; j < N; j += 4)
        	{
            		__m256d csum = _mm256_setzero_pd();

            		for (int k = 0; k < N; k++)
            		{
                		__m256d avec = _mm256_set1_pd(A[i][k]);

                		__m256d bvec = _mm256_loadu_pd(&B[k][j]);

                		csum = _mm256_fmadd_pd(avec,bvec,csum);
            		}		

            	_mm256_storeu_pd(&C[i][j], csum);
        	}
    	}
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(stop-start);

        cout << duration.count() << endl;

	return 0;
}
