# CS411_ExtraCredit

## Run for sample files
- Compile program: g++ -std=c++11 project.cpp -o project
- Run: ```console ./project nation.csv REGIONKEY region.csv REGIONKEY HASH output.csv ``` OR ```console ./project nation.csv REGIONKEY region.csv REGIONKEY NESTED_LOOP output.csv ```

## Benchmark result
- Note: inorder to reduce the time, I've reduce the size of the dataset and only take 1/200 of the data
- Result of Nested loop (ms): 12731.6, 12452.4, 13045.5, 13838.3, 11811.6, 13722.7, 12402.9, 13725.2, 13754.9, 14069.9. Average: 13155.5
- Result of Hash (ms): 44.4356, 47.3201, 47.6274, 47.9785, 48.8465, 48.8206. 45.785, 45.4575, 47.6185, 47.3509. Average: 47.1241
- It is clear that hash join is much more efficient than nested loop join
