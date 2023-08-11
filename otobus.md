# Bus Line Separation

### Problem Definition

You are stationed at a bus stop, recording the arrival times of buses as they pass by. However, you lack any information regarding the unique identifiers (such as bus numbers or names) of the different bus lines that utilize this stop. Your task is to determine the following information:

- **Number of Bus Lines:** Determine the total number of distinct bus lines that traverse the bus stop.

- **Arrival Periods:** Identify the time intervals during which each individual bus line tends to arrive at the bus stop.

### Solution

Assuming that arrival times are stored in a sorted array, we begin by traversing the array and examining each arrival time individually. Our aim is to find a recurring pattern starting from the current element of the traversal. If a recurring pattern is found which starts from the current element of the traversal, then all elements of the array belonging to that pattern is marked to be ignored in future.

```cpp
int[] times;
bool[] visited = {false};

for (int i = 0; i < times.size(); ++i) {
    if (visited[i]) continue;

    /* 
    ...
    Find a pattern starting from i'th element of the times array
    ...
    */

    visited[elements of the found pattern] = true;
}
```

When the case with array of arrival times is `[10, 25, 30, 50, 55, 70, 85]` considered there are two recurring patterns: First one starts from 10 and repeats with period 20, second one is starts with 25 and repeats with period 30.

At the first iteration, to find a pattern starting from the 0<sup>th</sup> element, value of the 0<sup>th</sup> element (10) is subtracted from all elements in the array, resulting with array below:

<p style="text-align: center;"><code>[0, 15, 20, 40, 45, 60, 75]</code></p>

Since each element of the array is the difference between 0<sup>th</sup> element and themselves, elements of the array are indeed values for the period of a possible recurring pattern starting from 0<sup>th</sup> element. 
Therefore, every possible value for the period is tried and the period with the longest recurring pattern is recorded.

```cpp
int[] times;
bool[] visited = {false};

for (int i = 0; i < times.size(); ++i) {
    if (visited[i]) continue;

    int temp_times[] = subtract_element(times, times[i]);
    int maxlength = 0, period;
    int maxpattern[];
    //Try all possible periods for i'th item
    for (int j = i + 1; j < times.size(); ++j) { 
        if (visited[j]) continue;
        int pred_period = temp_times[j] //Predicted period is temp_times[j]
        int pattern[];
        int length = 1;
        
        //Try to find a recurring pattern with period = temp_times[j]
        for (int k = j + 1; j < times.size(); ++k) {
            if (visited[k]) continue;
            int expected_time = (length + 1) * pred_period;
        
            //There is no possible value for rest of the array
            if (expected_time < temp_times[k]) break;
            
            else if (expected_time == temp_times[k]) {
                length++;
                pattern.push_back(k);
            }
        }

        if (length > maxlength) { 
            maxlength = length; 
            maxpattern = pattern; 
            period = pred_period; 
        }
    }

    visited[elements of the maxpattern] = true;
}
```

For the cases with buses do not arrive at the exact time they supposed to, an interval of values are tried for every possible period. Then again the value with the longest recurring value is chosen for the period. When buses deviating at most 10 percent of their period are considered, the innermost loop of the code becomes into:


```cpp
int est_period = temp_times[j];
int min_period = est_period * 0.9;
int max_period = est_period * 1.1;
int maxlength = 0, period, maxpattern[];

for (est_period = min_period; est_period <= max_period; ++est_period) {
    //Try to find a recurring pattern with period = est_period
    int length = 1, pattern[];

    for (int k = j + 1; j < times.size(); ++k) {
        if (visited[k]) continue;
        int expected_time = (length + 1) * est_period;
        int err = est_period * 0.1;

        //There is no possible value for rest of the array
        if (expected_time + err < temp_times[k]) break;
        
        else if (temp_times[k] <= expected_time + err && temp_times[k] >= expected_time - err) {
            length++;
            pattern.push_back(k);
        }
    }

    if (length > maxlength) { 
        maxlength = length; 
        maxpattern = pattern; 
        period = pred_period; 
    }
}
```

### Time Complexity

Time complexity of the algorithm is $O(n^3)$. Although, program traverses all possible period values for each element in the array of arrival times, iteration count at the worst case is the maximum value of an integer and it is constant.

### Tests
**Input:**
15 20 25 45 61 75 89 100 105 133 140 155 165 182 195 220 222 223 260 285 301 350 416 480 -1

**Periods:** 30, 40, 65

**Output:**<br>
BUS COUNT: 3<br>
PERIODS: 30 40 65

**Input:** 
0 15 20 25 43 46 62 65 74 89 94 100 112 129 134 136 141 158 173 180 188 194 210 235 247 249 282 282 310 329 368 428 -1

**Periods:** 23, 37, 47, 59

**Output:**<br>
BUS COUNT: 4<br>
PERIODS: 47 59 23 37

**Input:**
0 9 15 18 25 30 43 50 52 57 70 74 90 90 100 104 108 110 135 140 157 165 165 182 218 220 220 273 284 350 -1

**Periods:** 20, 30, 40, 55, 65

**Output:**<br>
BUS COUNT: 8<br>
PERIODS: 54 31 87 6 19 78 67 350

**Input:**
0 5 14 25 35 46 54 69 75 88 95 115 117 132 135 137 155 175 180 195 197 211 215 225 235 261 270 299 315 322 360 389 393 405 449 453 485 495 520 577 581 645 669 709 761 853 951 1034 -1

**Periods**: 20, 45, 64, 92

**Output:**<br>
BUS COUNT: 5<br>
PERIODS: 45 64 93 20 57
