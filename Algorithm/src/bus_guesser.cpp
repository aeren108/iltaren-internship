#include <iostream>
#include "vector.h"
#include "list.h"

#define MAX_BUS 50
#define MAX_GROUP 300

#define ERROR_RATE 0.1f
#define RATE_OF_CHANGE 0.05f

#define MAX_GROUP_DISTANCE 250

#define DEBUG false

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

void subtract_element(int n, Vector<int, MAX_BUS>& arr) {
	int temp = arr[n];
	for (int i = 0; i < arr.size(); ++i) 
		arr[i] -= temp;
}

int abs(int x) { if (x < 0) return -x; return x; }

double average_increasing_rate(Vector<int, MAX_BUS>& group) {
	double sum = 0;
	for (int i = 2; i < group.size(); ++i)
		sum += (times[group[i]] - times[group[i - 1]]) - (times[group[i - 1]] - times[group[i - 2]]);

	return sum / group.size();
}

int average_period(Vector<int, MAX_BUS>& group) {
	int sum = 0;
	for (int i = 1; i < group.size(); ++i)
		sum += times[group[i]] - times[group[i - 1]];
	
	return ((float)sum / group.size() + 0.5f);
}

void extract_increasing_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups) {
	Vector<bool, MAX_BUS> marks; for (int i = 0; i < MAX_BUS; ++i) marks.push_back(false);
	
	for (int i = 0; i < times.size(); ++i) {
		if (marks[i]) continue;

		int maxlen = 0;
		int decr_maxlen = 0;
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		if (DEBUG) {
			std::cout << "Times: " << temparr << std::endl;
			std::cout << "Checked: " << marks << std::endl;
		}

		for (int j = i + 1; j < temparr.size(); ++j) {
			if (marks[j]) continue;

			int est_period = temparr[j];
			int prev = temparr[j];

			int length = 1;

			Vector<int, MAX_BUS> visited_tmp;
			visited_tmp.push_back(i);
			visited_tmp.push_back(j);

			//Look for an increasing pattern
			for (int k = j + 1; k < temparr.size(); ++k) {
				if (marks[k]) continue;

				int max_expected_time = prev + est_period + est_period * RATE_OF_CHANGE;
				int min_expected_time = prev + est_period;

				if (temparr[k] > max_expected_time) break; //There is no possible value for rest of the array
				else if (temparr[k] >= min_expected_time) {
					length++;
					visited_tmp.push_back(k);
					est_period = temparr[k] - prev;
					prev = temparr[k];
				}
			}

			if (length > maxlen) {
				maxlen = length;
				visited = visited_tmp;
				period = times[visited[1]] - times[visited[0]];
			}
		}

		if (maxlen < 2) continue;

		for (int j = 0; j < visited.size(); ++j) {
			if (DEBUG) std::cout << times[visited[j]] << ", ";
			marks[visited[j]] = true;
		} if (DEBUG) std::cout << std::endl << std::endl;

		groups.push_back(visited);
	}
}

void extract_decreasing_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups) {
	Vector<bool, MAX_BUS> marks; for (int i = 0; i < MAX_BUS; ++i) marks.push_back(false);

	for (int i = 0; i < times.size(); ++i) {
		if (marks[i]) continue;

		int maxlen = 0;
		int decr_maxlen = 0;
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		if (DEBUG) {
			std::cout << "Times: " << temparr << std::endl;
			std::cout << "Checked: " << marks << std::endl;
		}

		for (int j = i + 1; j < temparr.size(); ++j) {
			if (marks[j]) continue;

			int est_period = temparr[j];
			int prev = temparr[j];

			int length = 1;

			Vector<int, MAX_BUS> visited_tmp;
			visited_tmp.push_back(i);
			visited_tmp.push_back(j);

			//Look for a decreasing pattern
			for (int k = j + 1; k < temparr.size(); ++k) {
				if (marks[k]) continue;
				
				int min_expected_time = prev + est_period - est_period * RATE_OF_CHANGE;
				int max_expected_time = prev + est_period;

				if (temparr[k] > max_expected_time) break; //There is no possible value for rest of the array
				else if (temparr[k] >= min_expected_time) {
					length++;
					visited_tmp.push_back(k);
					est_period = temparr[k] - prev;
					prev = temparr[k];
				}
			}

			if (length > maxlen) {
				maxlen = length;
				visited = visited_tmp;
				period = times[visited[1]] - times[visited[0]];
			}
		}

		if (maxlen == 0) continue;

		for (int j = 0; j < visited.size(); ++j) {
			if (DEBUG) std::cout << times[visited[j]] << ", ";
			marks[visited[j]] = true;
		} if (DEBUG) std::cout << std::endl << std::endl;

		groups.push_back(visited);
	}
}

void extract_recurring_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups, Vector<Vector<int, MAX_GROUP>, MAX_GROUP>& groups2) {
	for (int i = 0; i < times.size(); ++i) {
		if (marks[i]) continue;
		
		int maxlen = 0;
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		if (DEBUG) {
			std::cout << "Times: " << temparr << std::endl;
			std::cout << "Checked: " << marks << std::endl;
		}

		for (int j = i + 1; j < temparr.size(); ++j) {
			if (marks[j]) continue;

			int est_period = temparr[j];
			int prev = temparr[j];
			int length = 1;

			Vector<int, MAX_BUS> visited_tmp;

			visited_tmp.push_back(i);
			visited_tmp.push_back(j);

			//Look for an increasing pattern
			for (int k = j + 1; k < temparr.size(); ++k) {
				if (marks[k]) continue;

				int max_expected_time = prev + est_period + est_period * RATE_OF_CHANGE;
				int min_expected_time = prev + est_period;

				if (temparr[k] > max_expected_time) break; //There is no possible value for rest of the array
				else if (temparr[k] >= min_expected_time) {
					length++;
					visited_tmp.push_back(k);
					est_period = temparr[k] - prev;
					prev = temparr[k];
				}
			}

			if (length >= 2) {
				groups.push_back(visited_tmp);
			}

			if (length > maxlen) {
				maxlen = length;
				visited = visited_tmp;
				period = times[visited[1]] - times[visited[0]];
			}

			//Clear visited tmp
			visited_tmp = Vector<int, MAX_BUS>();
			visited_tmp.push_back(i);
			visited_tmp.push_back(j);

			//Reset vars
			est_period = temparr[j];
			prev = temparr[j];
			length = 1;

			//TODO: Look for a decreasing pattern and compare with the increasing one. Then choose the longer one...

			for (int k = j + 1; k < temparr.size(); ++k) {
				if (marks[k]) continue;

				int min_expected_time = prev + est_period - est_period * RATE_OF_CHANGE;
				int max_expected_time = prev + est_period;

				if (temparr[k] > max_expected_time) break; //There is no possible value for rest of the array
				else if (temparr[k] >= min_expected_time) {
					length++;
					visited_tmp.push_back(k);
					est_period = temparr[k] - prev;
					prev = temparr[k];
				}
			}

			if (length >= 2) {
				bool equal = true;
				if (groups[groups.size() - 1].size() == visited_tmp.size()) {
					for (int k = 0; k < visited_tmp.size(); ++k) {
						if (visited_tmp[k] != groups[groups.size() - 1][k]) { equal = false; break; }
					}
				}
				else equal = false;
				if (!equal) groups.push_back(visited_tmp);
			}

			if (length > maxlen) {
				maxlen = length;
				visited = visited_tmp;
				period = times[visited[1]] - times[visited[0]];
			}
		}

		if (maxlen < 2) continue;

		for (int j = 0; j < visited.size(); ++j) {
			if (DEBUG) std::cout << times[visited[j]] << ", ";
			//marks[visited[j]] = true;
		} if (DEBUG) std::cout << std::endl << std::endl;

		//groups.push_back(visited);
	}

	std::cout << "GROUPS:" << std::endl;
	for (int i = 0; i < groups.size(); ++i) {
		std::cout << "index: " << i << " -- >";
		for (int j = 0; j < groups[i].size(); ++j) {
			std::cout << times[groups[i][j]] << ", ";
		}
		std::cout << std::endl;
	}
		
	Vector<int, MAX_GROUP> group_periods;
	Vector<bool, MAX_GROUP> group_marks;

	for (int i = 0; i < groups.size(); ++i) group_marks.push_back(false);
	
	// Find matching groups for the i'th group
	for (int i = 0; i < groups.size(); ++i) { 
		if (group_marks[i]) continue;
		
		int maxlen = 0;
		double avg_incr = average_increasing_rate(groups[i]);
		int avg_period = average_period(groups[i]);

		bool increasing = avg_incr > 0 ? true : false;

		if (DEBUG) std::cout << "Group checked: " << group_marks << std::endl;

		Vector<int, MAX_GROUP> visited_group;

		for (int j = i + 1; j < groups.size(); ++j) {
			if (DEBUG) std::cout << "AVERAGE PERIOD: " << average_period(groups[j]) << " AVERAGE INCREASING RATE: " << average_increasing_rate(groups[j]) << std::endl;
			bool increasing2 = average_increasing_rate(groups[j]) > 0 ? true : false;
			if (group_marks[j] || 
				groups[i].size() != groups[j].size() || 
				increasing != increasing2 ||
				avg_period > average_period(groups[j]) + avg_period * ERROR_RATE || 
				avg_period < average_period(groups[j]) - avg_period * ERROR_RATE)
				continue;
			
			int length = 1;

			Vector<int, MAX_GROUP> visited_group_tmp;
			visited_group_tmp.push_back(i);
			visited_group_tmp.push_back(j);

			int expected_diff = times[groups[j][0]] - times[groups[i][groups[i].size() - 1]];
			if (DEBUG) std::cout << "Expected diff: " << expected_diff  << "period j= "<< j << " ->" << average_period(groups[j]) << std::endl;
			if (expected_diff <= avg_period) continue;

			for (int k = j + 1; k < groups.size(); ++k) {
				if (group_marks[k] || groups[k].size() != groups[j].size()) continue;
				
				int diff = times[groups[k][0]] - times[groups[j][groups[j].size() - 1]];
				if (diff == expected_diff) {
					length++;
					visited_group_tmp.push_back(k);
				}
			}
			
			if (length > maxlen) {
				maxlen = length;
				visited_group = visited_group_tmp;
			}
		}

		if (maxlen < 2) continue;
		
		for (int j = 0; j < visited_group.size(); ++j) 
			group_marks[visited_group[j]] = true;
		
		group_periods.push_back(avg_incr);
		groups2.push_back(visited_group);
	}

	std::cout << "GROUP COUNT: " << groups2.size() << std::endl;
	for (int i = 0; i < groups2.size(); ++i) {
		std::cout << "AVERAGE INCREASING RATE:" << group_periods[i] << std::endl;
		for (int j = 0; j < groups2[i].size(); ++j) {
			for (int k = 0; k < groups[groups2[i][j]].size(); ++k) {
				std::cout << times[groups[groups2[i][j]][k]] << ", ";
			}
			std::cout << " | ";
		}
		
		std::cout << std::endl;
	}
}

void extract_recurring_distant_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups, Vector<Vector<int, MAX_GROUP>, MAX_GROUP>& groups2) {
	Vector<int, MAX_GROUP> periods;

	for (int i = 0; i < times.size(); ++i) {
		if (marks[i]) continue;

		int maxlen = 0;
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		if (DEBUG) {
			std::cout << "Times: " << temparr << std::endl;
			std::cout << "Checked: " << marks << std::endl;
		}

		for (int j = i + 1; j < temparr.size(); ++j) {
			if (marks[j]) continue;

			int est_period = temparr[j];
			int prev = temparr[j];
			int length = 1;

			Vector<int, MAX_BUS> visited_tmp;

			visited_tmp.push_back(i);
			visited_tmp.push_back(j);

			for (int k = j + 1; k < temparr.size(); ++k) {
				if (marks[k]) continue;

				int expected_time = (length + 1) * est_period;

				if (temparr[k] > expected_time) break; //There is no possible value for rest of the array
				else if (temparr[k] == expected_time) {
					length++;
					visited_tmp.push_back(k);
				}
			}

			if (length >= 2) {
				periods.push_back(times[visited_tmp[1]] - times[visited_tmp[0]]);
				groups.push_back(visited_tmp);
			}

		}
	}

	std::cout << "GROUPS:" << std::endl;
	for (int i = 0; i < groups.size(); ++i) {
		std::cout << "index: " << i << " period " << periods[i] << " -- >";
		for (int j = 0; j < groups[i].size(); ++j) {
			std::cout << times[groups[i][j]] << ", ";
		}
		std::cout << std::endl;
	}

	Vector<bool, MAX_GROUP> group_marks;

	for (int i = 0; i < groups.size(); ++i) group_marks.push_back(false);

	for (int i = 0; i < groups.size(); ++i) {
		if (group_marks[i]) continue;

		int period = times[groups[i][1]] - times[groups[i][0]];
		int maxlen = 0;

		Vector<int, MAX_GROUP> visited_group;

		for (int j = i + 1; j < groups.size(); ++j) {
			if (group_marks[j] || 
				groups[i].size() != groups[j].size()) continue;

			int dist = times[groups[j][0]] - times[groups[i][groups[i].size() - 1]];
			int length = 1;

			Vector<int, MAX_GROUP> visited_group_tmp;
			visited_group_tmp.push_back(i);
			visited_group_tmp.push_back(j);

			if (dist > MAX_GROUP_DISTANCE || dist < 0 || dist != periods[j]) continue;

			for (int k = j + 1; k < groups.size(); ++k) {
				if (group_marks[j] ||
					groups[i].size() != groups[k].size()) continue;

				dist = times[groups[k][0]] - times[groups[j][groups[j].size() - 1]];
				if (dist > MAX_GROUP_DISTANCE || dist < 0) break;
				if (dist == periods[k]) {
					length++;
					visited_group_tmp.push_back(k);
				}
			
			}

			if (length > maxlen) {
				maxlen = length;
				visited_group = visited_group_tmp;
			}

		}

		if (maxlen < 2) continue;

		for (int j = 0; j < visited_group.size(); ++j)
			group_marks[visited_group[j]] = true;

		groups2.push_back(visited_group);
	}

	std::cout << "GROUP COUNT: " << groups2.size() << std::endl;
	for (int i = 0; i < groups2.size(); ++i) {
		for (int j = 0; j < groups2[i].size(); ++j) {
			for (int k = 0; k < groups[groups2[i][j]].size(); ++k) {
				std::cout << times[groups[groups2[i][j]][k]] << ", ";
			}
			std::cout << " | ";
		}

		std::cout << std::endl;
	}

}

void extract_regular_groups(Vector<Vector<int, MAX_BUS>, MAX_GROUP>& groups, Vector<int, MAX_BUS>& periods) {
	for (int i = 0; i < times.size(); ++i) { //Find a pattern for i'th element
		if (marks[i]) continue;

		int maxlen = 0; 
		int period = 0;

		Vector<int, MAX_BUS> temparr = times;
		Vector<int, MAX_BUS> visited;

		subtract_element(i, temparr);

		if (DEBUG) {
			std::cout << "Times: " << temparr << std::endl;
			std::cout << "Checked: " << marks << std::endl;
		}

		for (int j = i + 1; j < temparr.size(); ++j) { //Check for the pattern with maximum length
			if (marks[j]) continue;

			int est_period = temparr[j];
			int min_period = est_period * (1 - ERROR_RATE);
			int max_period = est_period * (1 + ERROR_RATE);

			for (est_period = min_period; est_period <= max_period; ++est_period) { //Try all possible values for period
				int length = 0;
				Vector<int, MAX_BUS> visited_tmp;

				visited_tmp.push_back(i);

				//Measure the pattern length for period with jth item (estimated period <- temparr[j])
				for (int k = j; k < temparr.size(); ++k) {
					if (marks[k]) continue;

					int expected_time = (length + 1) * est_period;
					int err = est_period * ERROR_RATE;

					if (temparr[k] > expected_time + err) break; //There is no possible value for rest of the array
					else if (temparr[k] >= expected_time - err) {
						length++;
						visited_tmp.push_back(k);
					}
				}

				//Set the period for the pattern with maximum length
				if (length > maxlen) { 
					maxlen = length; 
					visited = visited_tmp; 
					period = est_period; 
				}
			}
		}

		if (DEBUG) std::cout << "Max length for i=" << i << ": " << maxlen << std::endl << "Period: " << period << " -> ";

		//Mark visited elements
		for (int j = 0; j < visited.size(); ++j) {
			if (DEBUG) std::cout << times[visited[j]] << ", ";
			marks[visited[j]] = true;
		} if (DEBUG) std::cout << std::endl << std::endl;

		period = period == 0 ? times[i] : period;
		periods.push_back(period);
		groups.push_back(visited);
	}
}

int main() {
	Vector<int, MAX_BUS> periods;
	Vector<Vector<int, MAX_BUS>, MAX_GROUP> main_groups_regular;

	Vector<Vector<int, MAX_BUS>, MAX_GROUP> main_groups_recurring;
	Vector<Vector<int, MAX_GROUP>, MAX_GROUP> recurring_groups;

	Vector<Vector<int, MAX_GROUP>, MAX_GROUP> recurring_distant_groups;
	Vector<Vector<int, MAX_BUS>, MAX_GROUP> main_distant_groups_recurring;

	read_input();
	for (int i = 0; i < times.size(); ++i)
		marks.push_back(false);

	//preprocess_times();

	extract_recurring_groups(main_groups_recurring, recurring_groups);

	//Clear marks and mark only recurring groups
	for (int i = 0; i < marks.size(); ++i) marks[i] = false;

	for (int i = 0; i < recurring_groups.size(); ++i) {
		for (int j = 0; j < recurring_groups[i].size(); ++j) {
			for (int k = 0; k < main_groups_recurring[recurring_groups[i][j]].size(); ++k) {
				marks[main_groups_recurring[recurring_groups[i][j]][k]] = true;
			}
		}
	}

	extract_recurring_distant_groups(main_distant_groups_recurring, recurring_distant_groups);

	for (int i = 0; i < recurring_distant_groups.size(); ++i) {
		for (int j = 0; j < recurring_distant_groups[i].size(); ++j) {
			for (int k = 0; k < main_distant_groups_recurring[recurring_distant_groups[i][j]].size(); ++k) {
				marks[main_distant_groups_recurring[recurring_distant_groups[i][j]][k]] = true;
			}
		}
	}

	std::cout << std::endl;

	extract_regular_groups(main_groups_regular, periods);

	std::cout << "BUS COUNT: " << periods.size() << std::endl;
	std::cout << "PERIODS: " << periods << std::endl;
	std::cout << "GROUPS: " << std::endl;

	for (int i = 0; i < main_groups_regular.size(); ++i) {
		for (int j = 0; j < main_groups_regular[i].size(); ++j) 
			std::cout << times[main_groups_regular[i][j]] << ", ";
		std::cout << std::endl;
	}

	return 1;
}