# Problem 015 - File Storage Solution Summary

## Final Result
- **Status**: ✅ All Test Cases Passed (Accepted)
- **Score**: 9/9
- **Submissions Used**: 3 out of 7 allowed
- **Final Submission ID**: 752497

## Problem Analysis
The problem required implementing a file-based key-value database with:
- Insert operation: add (index, value) pairs
- Delete operation: remove specific (index, value) pairs
- Find operation: retrieve all values for a given index in ascending order
- Persistence across multiple runs
- Memory constraint: 5-6 MiB
- Time constraint: 500-16000 ms per test case
- Disk space: 1024 MiB
- File count: 20 files max

## Solution Evolution

### Attempt 1 (Submission 752493) - Score: 4/9
**Approach**: Load all entries into vector, sort, save back on each operation
**Issues**:
- Time Limit Exceeded on harder test cases
- O(n) file I/O for every operation was too slow
- Repeated sorting on every operation

### Attempt 2 (Submission 752494) - Score: 6/9
**Approach**: Use `map<string, set<int>>` for in-memory operations, file I/O only at start/end
**Improvements**:
- Eliminated TLE issues by doing all operations in memory
- File I/O reduced to 2 operations (load at start, save at end)
**Issues**:
- Memory Limit Exceeded on some test cases (5.8 MB > 5-6 MB limit)
- `map` and `set` have significant overhead

### Attempt 3 (Submission 752497) - Score: 9/9 ✅
**Approach**: Use `vector<Record>` for in-memory operations, file I/O only at start/end
**Key Optimizations**:
- Replaced `map<string, set<int>>` with simple `vector<Record>`
- Record struct is only 69 bytes (65 for index + 4 for int value)
- Much lower memory overhead compared to map/set containers
- Still maintains efficient operations for the given data sizes
- Load from file at program start, save at program end

**Final Implementation Details**:
- Record structure: 65-byte char array for index + 4-byte int for value
- Linear search for insert/delete (acceptable for 100K entries)
- Linear scan for find + sort results (acceptable for query results)
- Memory usage: ~5.9 MB max (within 6 MB limit)
- Time: Max 8.7 seconds (within 16 second limit)

## Key Insights
1. **Memory vs Time Trade-off**: The problem required balancing memory efficiency with time efficiency
2. **Container Overhead**: STL containers like `map` and `set` have significant memory overhead
3. **Batch I/O**: Loading at start and saving at end is much more efficient than repeated I/O
4. **Simple is Better**: A simple vector with linear search outperformed complex data structures due to lower memory overhead

## Performance Metrics (Final Submission)
- Maximum Memory: 5.898 MB (within 6 MB limit)
- Maximum Time: 8.673 seconds (within 16 second limit)
- All 29 test points passed across 9 test groups
- Handles up to 100,000 operations efficiently
