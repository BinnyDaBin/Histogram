#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <vector>
#include <iostream> /*cout, cin*/
#include <omp.h>
#include <string.h>

using namespace std;

int main ()
{
	// Max and min number range
	float hi = 9.9999;
	float lo = 0.0000;
	// contains all the randomly generated data points
	vector<int> data(10000000, 0);
	// Number of data points for the histogram
	int n = 10000000;
	// Creates an array of 10 elements and initialeze it to 0
	int histo[10] = {}; 

	// Initializes the seed for the generator
	srand (time(NULL));

	for(int i = 0; i < n; ++i)
	{
		// Generates a random number between 0.0000 to 9.9999
		float histPoint = lo + static_cast<float>(rand()) /(static_cast<float>(RAND_MAX/(hi - lo)));
		data[i] = histPoint;
	}

	// Sequential
	double t1 = omp_get_wtime();
	// Creates the histogram
	for (int i = 0; i < n; ++i)
	{
		int b = (int) data[i];
		histo[b]++;
	}
	t1 = omp_get_wtime() - t1;
	cout << "Sequential Time: " << t1 << endl;

	// Prints the values
	for (int i = 0; i < 10; ++i)
	{
		cout << "bin " << i << ": " << histo[i] << endl;
	}

	// Reset histo
	memset(histo, 0, sizeof(histo));

	// Parallel Reduction
	double t2 = omp_get_wtime();
	#pragma omp parallel for reduction(+:histo[:10]) num_threads(2)
		// Creates the histogram
		for (int i = 0; i < n; ++i)
		{
			int b = (int) data[i];
			histo[b]++;
		}
	t2 = omp_get_wtime() - t2;
	cout << "Reduced Time: " << t2 << endl;

	// Prints the values
	for (int i = 0; i < 10; ++i)
	{
		cout << "bin " << i << ": " << histo[i] << endl;
	}

	// Calculate S and E
	double s = t1/t2;
	cout << "S = " << s << endl;
	double e = s/4; //hw.physicalcpu = 2
	cout << "e = " << e << endl;

	return 0;
}