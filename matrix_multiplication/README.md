# Benchmark Results

## SGEMM (Matrix Multiplication)

### Methodology
- Multiply two matrices together
  - Filled with random numbers
  - Increment rows and columns
    - Rows by 1
    - Columns by 64


### Preliminary Observations
- QPU operation times plateau and throw errors when incrementing after 1500 rows at 512 Columns
- ARM NEON operations are slower than the QPU until 512 Columns

### Raspberry Pi Zero W

![Raspberry Pi Zero W](https://raw.githubusercontent.com/kpanda254/QPU_Benchmarks/main/benchmark_results/Raspberry%20Pi%20Zero%20W.png)

### Raspberry Pi 3 Model B+

![Raspberry Pi 3 Model B+](https://raw.githubusercontent.com/kpanda254/QPU_Benchmarks/main/benchmark_results/Raspberry%20Pi%203%20Model%20B%2B.png)
