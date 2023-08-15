#include <iostream>
#include "vector.h"
#include "list.h"

#define MAX_BUS 50
#define MAX_GROUP 15

#define ERROR_RATE 0.1f
#define RATE_OF_CHANGE 0.05f

Vector<int, MAX_BUS> times;
Vector<bool, MAX_BUS> marks;

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

void subtract_element(int i, Vector<int, MAX_BUS>& arr) {
	int temp = arr[i];
	for (int i = 0; i < arr.size(); ++i) arr[i] -= temp;
}

int abs(int x) { if (x < 0) return -x; return x; }

void extract_recurring_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups, Vector<int, MAX_BUS>& periods) {
	for (int i = 0; i < times.size(); ++i) {
		if (marks[i]) continue;
		
		int maxlen = 0;
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		std::cout << "Times: " << temparr << std::endl;
		std::cout << "Checked: " << marks << std::endl;

		for (int j = i + 1; j < temparr.size(); ++j) {
			if (marks[j]) continue;

			int est_period = temparr[j];
			int length = 1;
			int prev = -1;

			Vector<int, MAX_BUS> visited_tmp;
			visited_tmp.push_back(i);
			visited_tmp.push_back(j);

			for (int k = j + 1; k < temparr.size(); ++k) {
				int max_expected_time = est_period + est_period * RATE_OF_CHANGE;

				if (temparr[k] > max_expected_time) break; //There is no possible value for rest of the array
				else {
					length++;
					visited_tmp.push_back(k);
					est_period = temparr[k] - prev;
					prev = temparr[k];
				}
			}

			if (length > maxlen) {
				maxlen = length;
				visited = visited_tmp;
				period =  times[visited[1]] - times[visited[0]];
			}
		}

		for (int j = 0; j < visited.size(); ++j) {
			std::cout << times[visited[j]] << ", ";
			marks[visited[j]] = true;
		} std::cout << std::endl << std::endl;

		periods.push_back(period);
		groups.push_back(visited);
	}

}

void extract_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups, Vector<int, MAX_BUS>& periods) {
	for (int i = 0; i < times.size(); ++i) { //Find a pattern for i'th element
		if (marks[i]) continue;

		int maxlen = 0; 
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		std::cout << "Times: " << temparr << std::endl;
		std::cout << "Checked: " << marks << std::endl;

		for (int j = i + 1; j < temparr.size(); ++j) { //Check for the pattern with maximum length
			if (marks[j]) continue;

			int est_period = temparr[j];
			int min_period = est_period * (1 - ERROR_RATE);
			int max_period = est_period * (1 + ERROR_RATE);

			for (est_period = min_period; est_period <= max_period; ++est_period) { //Try all possible values for period
				int length = 1;
				Vector<int, MAX_BUS> visited_tmp;

				visited_tmp.push_back(i);
				visited_tmp.push_back(j);

				//Measure the pattern length for period with jth item (estimated period <- temparr[j])
				for (int k = j + 1; k < temparr.size(); ++k) {
					if (marks[k]) continue;

					int expected_time = (length + 1) * est_period;
					int err = est_period * ERROR_RATE;

					if (temparr[k] > expected_time + err) break; //There is no possible value for rest of the array
					else if (temparr[k] <= expected_time + err && temparr[k] >= expected_time - err) {
						length++;
						visited_tmp.push_back(k);
					}
				}

				//Set the period for the pattern with maximum length
				if (length > maxlen) { maxlen = length; visited = visited_tmp; period = est_period; }
			}
		}

		std::cout << "Max length for i=" << i << ": " << maxlen << std::endl << "Period: " << period << " -> ";

		//Mark visited elements
		for (int j = 0; j < visited.size(); ++j) {
			std::cout << times[visited[j]] << ", ";
			marks[visited[j]] = true;
		} std::cout << std::endl << std::endl;

		period = period == 0 ? times[i] : period;
		periods.push_back(period);
		groups.push_back(visited);
	}
}

int main() {
	Vector<int, MAX_BUS> periods;
	Vector<Vector<int, MAX_BUS>, MAX_GROUP> groups;

	read_input();
	for (int i = 0; i < times.size(); ++i) {
		marks.push_back(false);
	}

	//preprocess_times();

	extract_recurring_groups(groups, periods);

	std::cout << "BUS COUNT: " << periods.size() << std::endl;
	std::cout << "PERIODS: " << periods << std::endl;
	std::cout << "GROUPS: " << std::endl;

	for (int i = 0; i < groups.size(); ++i) {
		for (int j = 0; j < groups[i].size(); ++j) 
			std::cout << times[groups[i][j]] << ", ";
		std::cout << std::endl;
	}

	return 1;
}