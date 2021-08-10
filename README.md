# QPU_Benchmarks

This repository contains benchmarks that measure the performance of the Quad Processing Unit (QPU) in the Raspberry Pi family.

## Device Information

The benchmarks are performed on the Raspberry Pi Zero W and the Raspberry Pi 3 Model B+. Both models contain Quad Processing Units (QPUs) that act as 16-way 32 bit SIMD processors. A SIMD processor can process multiple values with a single instruction. In this case, a QPU can process 16 values. A single QPU processes all 16 units in 4 clock cycles, with 4 units per clock cycle, and can sometimes deliver 2 results at a time.

### Raspberry Pi Zero W

The Raspberry Pi Zero W contains 12 QPUs (in 3 groups of 4), with each clocked at 250 MHz. This results in a theoretical maximum performance of 24 GFLOPS.

```bash
pi@raspberrypi:~ $ cat /proc/cpuinfo
processor : 0
model name : ARMv6-compatible processor rev 7 (v6l)
BogoMIPS : 697.95
Features : half thumb fastmult vfp edsp java tls 
CPU implementer : 0x41
CPU architecture: 7
CPU variant : 0x0
CPU part : 0xb76
CPU revision : 7

Hardware : BCM2835
Revision : 9000c1
Serial  : 000000008f6b5a2c
Model  : Raspberry Pi Zero W Rev 1.1
```

### Raspberry Pi 3 Model B+

The Raspberry Pi 3 Model B+ also contains 12 QPUs in the same grouping. The QPUs have higher clock speeds, with each clocked at 500 MHz. This results in a modest performance boost, with a theroetical maximum performance of 32 GFLOPS.

```bash
pi@raspberrypi:~ $ cat /proc/cpuinfo 
processor : 0
model name : ARMv7 Processor rev 4 (v7l)
BogoMIPS : 38.40
Features : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant : 0x0
CPU part : 0xd03
CPU revision : 4

processor : 1
model name : ARMv7 Processor rev 4 (v7l)
BogoMIPS : 38.40
Features : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant : 0x0
CPU part : 0xd03
CPU revision : 4

processor : 2
model name : ARMv7 Processor rev 4 (v7l)
BogoMIPS : 38.40
Features : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant : 0x0
CPU part : 0xd03
CPU revision : 4

processor : 3
model name : ARMv7 Processor rev 4 (v7l)
BogoMIPS : 38.40
Features : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant : 0x0
CPU part : 0xd03
CPU revision : 4

Hardware : BCM2835
Revision : a020d3
Serial  : 0000000026280030
Model  : Raspberry Pi 3 Model B Plus Rev 1.3
```

Each CPU core in the Raspberry Pi 3 Model B+ also contains a NEON SIMD Unit, which is simply an additional 16-way SIMD unit separate from each of the QPUs. It has its own instruction set and shares memory with the CPU.

## Benchmark Process

Currently, the only operation present in this benchmark is matrix multiplication. Matrix multiplication is achieved on the QPU through the use and modification of the [QMKL](https://github.com/Idein/qmkl) library. The benchmark outputs the time taken to multiply the matrices at various row and column sizes.

Additionally, for the Raspberry Pi 3 Model B+, the matrix multiplication performance on the QPU is compared with the performance of the NEON SIMD Unit, through the use of OpenBLAS, which utilizes the NEON SIMD unit.

### Requirements

- [QMKL](https://github.com/Idein/qmkl)

- [OpenBLAS (For the Raspberry Pi 3 Model B+)](https://www.openblas.net/)
