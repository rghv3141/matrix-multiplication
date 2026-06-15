#include <iostream> 
#include <vector>
#include <chrono>
using namespace std;

int main() 
{
	int vsize = 512;
	vector<vector<double>> A(512, vector<double>(512));
	vector<vector<double>> B(512, vector<double>(512));
	vector<vector<double>> C(512, vector<double>(512));

	for(int i = 0; i < vsize; i++) {
		for(int j = 0; j < vsize; j++) {
			A[i][j] = i + j;
			B[i][j] = i - j;
		}
	}
	auto start = std::chrono::high_resolution_clock::now();	
	for(int i = 0; i < vsize; i++) {
		for(int j = 0; j < vsize; j++) {
			for(int k = 0; k < vsize; k++) {
				C[i][j] += A[i][k] * B[k][j]; 
			}
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration<double, std::milli>(stop - start);

	cout << duration.count() << endl;
	
	return 0;
}
