#include <iostream>
#include <vector>
#include <chrono>
using namespace std; 

int main() 
{
	
	vector<vector<double>> A(512, vector<double>(512));
	vector<vector<double>> B(512, vector<double>(512));
	vector<vector<double>> C(512, vector<double>(512));
	
	int N = 512;
	const int b = 16;	//b is the block size
	for (int i = 0; i < N; i++) {
   		for (int j = 0; j < N; j++) {
        		A[i][j] = i + j;
        		B[i][j] = i - j;
    		}
	}
	auto start = std::chrono::high_resolution_clock::now();
	
	for(int ii = 0; ii < N; ii+=b) {
		for(int jj = 0; jj < N; jj+=b) {
			for(int kk = 0; kk < N; kk+=b) {
				
				for(int i = ii; i< ii + b;i++ ) {
					for(int j = jj; j< jj+b; j++) {
						for(int k = kk; k < kk + b; k++) {
							C[i][j] += A[i][k] * B[k][j];
						}
					}
				}
						
			}
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double, std::milli>(stop-start);

	cout << duration.count() << endl;

	return 0;
}
