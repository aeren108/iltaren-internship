#include <iostream>
#include "vector.h"
#include "list.h"

#define ERROR_RATE 0.1f

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
	Vector<Vector<int, 50>, 10> g;

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
		Vector<int, 50> visited;
		for (int j = i + 1; j < temparr.size(); ++j) { //Check for the pattern with maximum length
			if (marks[j]) continue;

			int est_period = temparr[j];
			int min_period = est_period * (1 - ERROR_RATE);
			int max_period = est_period * (1 + ERROR_RATE);
			
			
			for (est_period = min_period; est_period <= max_period; ++est_period) { //Try all possible values for period
				int length = 1, real_length = 1; int missed_index = -1; int missed_order = -1;
				bool can_miss = true;
				Vector<int, 50> visited_tmp; 
				visited_tmp.push_back(i); visited_tmp.push_back(j);
				
				//Measure the pattern length for period with jth item (estimated period <- temparr[j])
				for (int k = j + 1; k < temparr.size(); ++k) {
					if (marks[k]) continue;

					int expected_time = (length + 1) * est_period;
					int err = est_period * ERROR_RATE;

					if (temparr[k] > expected_time + err) {
						if (!can_miss) break; //There is no possible value for rest of the array
						else {
							length++;
							missed_order = length;
							can_miss = false;
							k--;
							continue;
						}
					} 
					else if (temparr[k] <= expected_time + err && temparr[k] >= expected_time - err) {
						length++;
						real_length++;
						visited_tmp.push_back(k);
					}

				}

				if (missed_order == length - 1) { visited_tmp.remove(visited_tmp.size() - 1); length--; }
				else if (missed_order == length - 2) { visited_tmp.remove(visited_tmp.size() - 1); visited_tmp.remove(visited_tmp.size() - 1); length-=2; }
				if (length > maxlen) { maxlen = length; visited = visited_tmp; period = est_period; }
			}
		}

		std::cout << "Max length for i=" << i << ": " << maxlen << std::endl << "PERIOD: " << period << " -> ";

		//Mark visited elements
		for (int j = 0; j < visited.size(); ++j) { std::cout << times[visited[j]] << ", "; marks[visited[j]] = true; }
		std::cout << std::endl << std::endl;
		
		
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
		g.push_back(visited);
		period = period == 0 ? times[i] : period;
		periods.push_back(period);
		/*int index = -1; //Find the pattern group if it already exists
		for (int j = 0; j < periods.size(); ++j) {
			int err = period * ERROR_RATE < periods[j] * ERROR_RATE ? period * ERROR_RATE + 0.5f : periods[j] * ERROR_RATE + 0.5f;
			if (period <= periods[j] + err && period >= periods[j] - err) index = j;
		}

		if (index != -1) { //If a group with same period exists, push it to the existing group
			groups[index].push_back(i);
		} else { //If it does not exist push new
			periods.push_back(period);
			groups.push_back(Vector<int, 10>());
			groups[groups.size() - 1].push_back(i);
		}*/
	}

	std::cout << "BUS COUNT: " << periods.size() << std::endl;
	std::cout << "PERIODS: " << periods << std::endl;

	std::cout << "GROUPS: " << std::endl;

	/*for (int i = 0; i < groups.size(); ++i) {
		
		for (int k = 0; k < groups[i].size(); ++k) {
			int length = 0;

			for (int j = 0; j < times.size(); ++j) {
				int expected_time = times[groups[i][k]] + length * periods[i];
				int err = periods[i] * ERROR_RATE;

				if (abs(times[j] - expected_time) <= err) {
					std::cout << times[j] << ", ";
					length++;
				}
			}
			
		}

		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	for (int i = 0; i < g.size(); ++i) {
		for (int j = 0; j < g[i].size(); ++j) {
			std::cout << times[g[i][j]] << ", ";
		}
		//std::cout << g[i] << std::endl;
		std::cout << std::endl;
	}

	return 1;
}