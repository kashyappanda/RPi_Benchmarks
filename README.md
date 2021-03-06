# RPi_Benchmarks

This repository contains benchmarks that measure the performance and efficiency of the Raspberry Pi family.

## Device Information

The benchmarks are performed on the Raspberry Pi Zero W and the Raspberry Pi 3 Model B+. Both models contain Quad Processing Units (QPUs) that act as 16-way 32 bit [SIMD processors](https://en.wikipedia.org/wiki/SIMD). In this case, a QPU can process 16 values. A single QPU processes all 16 units in 4 clock cycles, with 4 units per clock cycle, and can sometimes deliver 2 results at a time.

### Raspberry Pi Zero W

The Raspberry Pi Zero W contains 12 QPUs (in 3 groups of 4), with each clocked at 250 MHz. This results in a theoretical maximum performance of 24 GFLOPS.

### Raspberry Pi 3 Model B+

The Raspberry Pi 3 Model B+ also contains 12 QPUs in the same grouping. The QPUs have higher clock speeds, with each clocked at 500 MHz. This results in a modest performance boost, with a theroetical maximum performance of 32 GFLOPS.

Each CPU core in the Raspberry Pi 3 Model B+ also contains a NEON SIMD Unit, which is simply an additional 16-way SIMD unit separate from each of the QPUs. It has its own instruction set and shares memory with the CPU.

## Benchmarks

### SGEMM (Matrix Multiplication)

Matrix multiplication is achieved on the QPU through the use and modification of [QMKL](https://github.com/Idein/qmkl), which is a Math Kernel Library for the QPU.

Additionally, for the Raspberry Pi 3 Model B+, the matrix multiplication benchmark is also run on the NEON SIMD Unit through the use of [OpenBLAS](https://www.openblas.net/).

## Requirements

- Running the benchmarks

  - [QMKL](https://github.com/Idein/qmkl)

  - [OpenBLAS (For the Raspberry Pi 3 Model B+)](https://www.openblas.net/)

- Viewing the results

  - [NumPy](https://numpy.org/)

  - [Pandas](https://pandas.pydata.org/)

## Next Steps
- Flops/Watt for matrix multiplication (Raspberry Pi 4)

- Add average inference time for Tensorflow Lite (Raspberry Pi 3 and 4)

- Benchmark [gradient boosting](https://github.com/szilard/benchm-ml) (Raspberry Pi 4)
