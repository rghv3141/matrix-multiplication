# Overview
This project implements and benchmarks three different approaches to square matrix multiplication of size 512 × 512 (double):
- Naive triple-loop implementation
- Cache-blocked (tiled) implementation
- SIMD intrinsics using AVX2

## Benchmark + Explanation

### Naive Triple Loop

```text
assessment/matrix-mul » perf stat ./triple-loop
163.921

 Performance counter stats for './triple-loop':

                 0      context-switches:u               #      0.0 cs/sec  cs_per_second     
                 0      cpu-migrations:u                 #      0.0 migrations/sec  migrations_per_second
             1,689      page-faults:u                    #   9919.2 faults/sec  page_faults_per_second
            170.28 msec task-clock:u                     #      0.9 CPUs  CPUs_utilized       
           272,204      branch-misses:u                  #      0.2 %  branch_miss_rate         (49.35%)
       135,868,400      branches:u                       #    797.9 M/sec  branch_frequency     (49.89%)
       455,524,868      cpu-cycles:u                     #      2.7 GHz  cycles_frequency       (66.96%)
     1,211,377,761      instructions:u                   #      2.7 instructions  insn_per_cycle  (50.65%)
         2,832,650      stalled-cycles-frontend:u        #     0.01 frontend_cycles_idle        (50.11%)

       0.171411148 seconds time elapsed

       0.162742000 seconds user
       0.005929000 seconds sys
```
This implementation takes ~160 ms for this matrix multiplication.

**Why it is slow?**

The main bottleneck is cache inefficiency, C++ stores matices in row-major order so each row is contiguous in memory and fits well with cache lines
While A[i][k] is accessed sequentially and has good locality, B[k][j] is accessed column-wise:

```
B[0][j], B[1][j], B[2][j], ...
```

This causes each access to likely hit a different cache line, leading to frequent cache misses and poor reuse of loaded data.
As a result, the CPU spends more time waiting on memory than performing computations, making the algorithm memory-bound.

### Cache-Blocked(tiled) version

```text
assessment/matrix-mul » perf stat ./blocked-mul    
90.6798
2.93202e+12

 Performance counter stats for './blocked-mul':

                 0      context-switches:u               #      0.0 cs/sec  cs_per_second     
                 0      cpu-migrations:u                 #      0.0 migrations/sec  migrations_per_second
             1,689      page-faults:u                    #  17384.6 faults/sec  page_faults_per_second
             97.16 msec task-clock:u                     #      0.9 CPUs  CPUs_utilized       
            57,026      branch-misses:u                  #      0.6 %  branch_miss_rate         (50.37%)
        10,200,372      branches:u                       #    105.0 M/sec  branch_frequency     (50.57%)
       254,306,939      cpu-cycles:u                     #      2.6 GHz  cycles_frequency       (67.01%)
       652,537,640      instructions:u                   #      2.6 instructions  insn_per_cycle  (49.63%)
           899,390      stalled-cycles-frontend:u        #     0.00 frontend_cycles_idle        (49.43%)

       0.098154861 seconds time elapsed

       0.092702000 seconds user
       0.003950000 seconds sys
```

This implementation of Matrix multiplication in my case almost took half the cpu cycles and instructions which reduces runtime to ~90ms

**Wwhy is it faster?**
Blocking improves cache locality by dividing the matrix into smaller tiles that fit into L1/L2 cache.
Instead of repeatedly accessing full rows and columns, the algorithm reuses data within a tile before it is evicted from cache. This significantly reduces cache misses and improves cache line utilization.

### SIMD intrinsics version using AVX2

```text
assessment/matrix-mul » perf stat ./avx2-mul   
53.5501

 Performance counter stats for './avx2-mul':

                 0      context-switches:u               #      0.0 cs/sec  cs_per_second     
                 0      cpu-migrations:u                 #      0.0 migrations/sec  migrations_per_second
             1,690      page-faults:u                    #  28212.7 faults/sec  page_faults_per_second
             59.90 msec task-clock:u                     #      0.9 CPUs  CPUs_utilized       
            81,054      branch-misses:u                  #      0.2 %  branch_miss_rate         (49.97%)
        34,406,008      branches:u                       #    574.4 M/sec  branch_frequency     (49.86%)
       151,322,781      cpu-cycles:u                     #      2.5 GHz  cycles_frequency       (66.53%)
       240,437,840      instructions:u                   #      1.6 instructions  insn_per_cycle  (50.03%)
         1,412,503      stalled-cycles-frontend:u        #     0.01 frontend_cycles_idle        (50.14%)

       0.061293435 seconds time elapsed

       0.055290000 seconds user
       0.004937000 seconds sys
```

**On my system this version is clearly the fastest** algorithm between the three and it takes about ~50 ms.

This version uses AVX2 SIMD instructions, which operate on 4 double values per instruction using 256-bit registers.
Instead of processing one value at a time, the CPU processes four values in parallel, significantly reducing the total number of instructions executed.

