#include <vector>
#include <iostream>
#include <string>

int compute_change(std::vector<int> &d, int size) {
	int k = (int)d.size();
	std::vector<std::vector<int> > table(k , std::vector<int>(size + 1));

	for ( int row = 0; row < k ; row++) 
		table[row][0]  = 0;	
	for ( int col = 0; col <= size ; col++) 
		table[0][col]  = col;

	for ( int row = 1 ; row < k; row++) {
		for ( int col = 1 ; col <= size; col++) {
			if ( col < d[row] )  
				table[row][col] = table[row - 1][col];
			else 
				table[row][col] = std::min(table[row - 1][col], 1 + table[row][col - d[row]]);
		}
	}
	return table[k - 1][size - 1];
}
	
int main ( int argc, char **argv) {
	int points[] = { 1, 2, 3, 17, 23, 42, 98};
	std::vector<int> d (points, points + sizeof(points) / sizeof(int));
	int solution =  compute_change(d,2349 );

	return 0;
}
