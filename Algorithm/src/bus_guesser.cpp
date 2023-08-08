#include <iostream>
#include "vector.h"
#include "list.h"

Vector<int, 50> times;
Vector<bool, 50> marks;

void read_input() {
	int min; std::cin >> min;
	while (min != -1) {
		times.push_back(min);
		std::cin >> min;
	}
	std::cout << std::endl;
}

void preprocess_times() {
	/*TODO: 
		1. Convert all times to minutes
		2. Sort the array
		3. Substract the minimum element from all elements
	*/

	int temp = times[0];
	for (int i = 0; i < times.size(); ++i) {
		times[i] -= temp;
		marks.push_back(false);
	}
}

void subtract_element(int i, Vector<int, 50>& arr) {
	int temp = arr[i];
	for (int i = 0; i < arr.size(); ++i) arr[i] -= temp;
}

int main() {
	Vector<int, 50> buses;
	Vector<int, 50> temparr;
	Vector<Vector<int, 10>, 50> firstbuses;

	read_input();
	for (int i = 0; i < times.size(); ++i) {
		marks.push_back(false);
	}

	//preprocess_times();

	for (int i = 0; i < times.size(); ++i) {
		temparr = times;
		if (marks[i]) continue;
		subtract_element(i, temparr);
		int maxoccurance = 0; int period = 0;

		std::cout << "Times: " << temparr << std::endl;
		std::cout << "Checked: " << marks << std::endl << std::endl;
		
		for (int j = i + 1; j < temparr.size(); ++j) {
			if (marks[j]) continue;
			int occurance = 1;
			
			for (int k = j + 1; k < temparr.size(); ++k) {
				if (temparr[j] * (occurance + 1) < temparr[k]) break;
				if (marks[k]) continue;
				if (temparr[k] % temparr[j] == 0 && temparr[j] * (occurance+1) == temparr[k]) {
					occurance++;
				}
			}


			if (occurance > maxoccurance) { maxoccurance = occurance; period = temparr[j]; }
		}

		int occurance = 0;
		for (int k = i; k < temparr.size(); ++k) {
			if (occurance * period < temparr[i]) break;
			if (marks[k]) continue;
			if (occurance * period == temparr[k]) {
				marks[k] = true;
				occurance++;
			}
		}

		period = period == 0 ? times[i] : period;

		int index = -1;
		for (int k = 0; k < buses.size(); ++k) {
			if (buses[k] == period) index = k;
		}

		if (index != -1) {
			firstbuses[index].push_back(i);
		} else {
			buses.push_back(period);
			firstbuses.push_back(Vector<int, 10>());
			firstbuses[firstbuses.size() - 1].push_back(i);
		}
	}

	std::cout << "BUS COUNT: " << buses.size() << std::endl;
	std::cout << "PERIODS: " << buses << std::endl;

	std::cout << "GROUPS: " << std::endl;

	for (int i = 0; i < firstbuses.size(); ++i) {
		
		for (int k = 0; k < firstbuses[i].size(); ++k) {
			int occurance = 0;
			for (int j = 0; j < times.size(); ++j) {
				if (times[firstbuses[i][k]] + occurance * buses[i] == times[j]) {
					std::cout << times[j] << ", ";
					occurance++;
				}
			}
			
		}

		std::cout << std::endl;
	}


	return 1;
}