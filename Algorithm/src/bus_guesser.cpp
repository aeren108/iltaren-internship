#include <iostream>
#include "vector.h"
#include "list.h"

#define ERROR_RATE 0.1

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
		3. Subtract the minimum element from all elements
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

int abs(int x) {
	if (x < 0) return -x; return x;
}

int main() {
	Vector<int, 50> periods;
	Vector<Vector<int, 10>, 50> groups;

	read_input();
	for (int i = 0; i < times.size(); ++i) {
		marks.push_back(false);
	}

	//preprocess_times();

	for (int i = 0; i < times.size(); ++i) { //Find a pattern for i'th element
		if (marks[i]) continue;
		
		int maxlen = 0; int period = 0;
		Vector<int, 50> temparr = times;
		
		subtract_element(i, temparr);

		std::cout << "Times: " << temparr << std::endl;
		std::cout << "Checked: " << marks << std::endl;
		Vector<int, 50> visited; visited.push_back(i);
		for (int j = i + 1; j < temparr.size(); ++j) { //Check for the pattern with maximum length
			if (marks[j]) continue;

			//TODO: Find the max length for period [min_period, max_period]

			int est_period = temparr[j];
			int min_period = est_period * 0.81, max_period = est_period * 1.21;

			for (est_period = min_period; est_period < max_period; ++est_period) {
				int length = 1;
				Vector<int, 50> visited_tmp; visited_tmp.push_back(j);
				//Measure the pattern length for period with jth item (estimated period <- temparr[j])
				for (int k = j + 1; k < temparr.size(); ++k) {
					int expected_time = (length + 1) * est_period;
					int err = est_period / 10.0f; //+0.5 for rounding to nearest integer

					if (temparr[k] > expected_time + err) break; //There is no possible value for rest of the array
					else if (temparr[k] <= expected_time + err && temparr[k] >= expected_time - err) {
						length++;
						//est_period = (est_period * (length - 1) + temparr[k] / length) / length;
						visited_tmp.push_back(k);
					}
				}

				//Set the period for the pattern with maximum length
				if (length > maxlen) { maxlen = length; visited = visited_tmp; period = est_period; }
			}

			//visited = visited_tmp;
		}

		std::cout << "Max length for i=" << i << ": " << maxlen << std::endl << std::endl;

		// Mark visited elements
		for (int j = 0; j < visited.size(); ++j) marks[visited[j]] = true;
		
		/*int length = 0;
		for (int j = i; j < temparr.size(); ++j) {
			if (marks[j]) continue;
			int expected_time = length * period;
			float err = period / 8.0f;

			if (temparr[j] > expected_time + err) break;
			else if (temparr[j] <= expected_time + err && temparr[j] >= expected_time - err) {
				marks[j] = true;
				length++;
			}
		}*/

		period = period == 0 ? times[i] : period;

		int index = -1; //Find the pattern group if it already exists
		for (int j = 0; j < periods.size(); ++j) {
			int err = period / 10.0f > periods[j] / 10.0f ? period / 10.0f + 0.5f : periods[j] / 10.0f + 0.5f;
			if (period <= periods[j] + err && period >= periods[j] - err) index = j;
		}

		if (index != -1) { // If a group with same period  exist, push it to the existing group
			groups[index].push_back(i);
		} else { //If it does not exist push new
			periods.push_back(period);
			groups.push_back(Vector<int, 10>());
			groups[groups.size() - 1].push_back(i);
		}
	}

	std::cout << "BUS COUNT: " << periods.size() << std::endl;
	std::cout << "PERIODS: " << periods << std::endl;

	std::cout << "GROUPS: " << std::endl;

	for (int i = 0; i < groups.size(); ++i) {
		
		for (int k = 0; k < groups[i].size(); ++k) {
			int length = 0;
			for (int j = 0; j < times.size(); ++j) {
				int expected_time = times[groups[i][k]] + length * periods[i];
				float err = periods[i] / 10.0f;
				if (abs(times[j] - expected_time) <= err) {
					std::cout << times[j] << ", ";
					length++;
				}
			}
			
		}

		std::cout << std::endl;
	}

	return 1;
}