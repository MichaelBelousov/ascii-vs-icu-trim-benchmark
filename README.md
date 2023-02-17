Haven't tested performance on ascii only strings, that is possibly much more performant

Run:

```sh
make && ./test
```

Last Results:

```
$ ./test
 > TrimTestBentleyAsciiTrim [██████████████████████████████████████████████████] 100% [00m:01s<00m:00s]
 ✓ TrimTestBentleyAsciiTrim
    Configuration
      3 warmup runs, 78492 benchmark runs, 20 iterations per run
    Execution Time
      Average       329.60 ns
      Fastest       159.00 ns (-170.60 ns, -51.76 %)
      Slowest        21.07 us (20.74 us, 6292.50 %)
      Lowest RSD    164.70 ns ± 0.55% (41st run)
    Performance
      Average    3033936.28 iterations/s
      Fastest    6289308.18 iterations/s (-3255371.90 iterations/s, -107.30 %)
      Slowest      47460.84 iterations/s (2986475.43 iterations/s, 98.44 %)

 > TrimTestIcuTrim [██████████████████████████████████████████████████] 100% [00m:01s<00m:00s]
 ✓ TrimTestIcuTrim
    Configuration
      3 warmup runs, 52246 benchmark runs, 20 iterations per run
    Execution Time
      Average       738.07 ns
      Fastest       404.00 ns (-334.07 ns, -45.26 %)
      Slowest        68.58 us (67.84 us, 9191.54 %)
      Lowest RSD    423.05 ns ± 0.41% (176th run)
    Performance
      Average    1354886.15 iterations/s
      Fastest    2475247.52 iterations/s (-1120361.37 iterations/s, -82.69 %)
      Slowest      14581.94 iterations/s (1340304.21 iterations/s, 98.92 %)
```
