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
      3 warmup runs, 65445 benchmark runs, 20 iterations per run
    Execution Time
      Average       679.37 ns
      Fastest       174.00 ns (-505.37 ns, -74.39 %)
      Slowest        21.23 us (20.55 us, 3025.24 %)
      Lowest RSD    869.40 ns ± 0.16% (21st run)
    Performance
      Average    1471946.84 iterations/s
      Fastest    5747126.44 iterations/s (-4275179.60 iterations/s, -290.44 %)
      Slowest      47098.72 iterations/s (1424848.12 iterations/s, 96.80 %)

 > TrimTestIcuTrim [██████████████████████████████████████████████████] 100% [00m:02s<00m:00s]
 ✓ TrimTestIcuTrim
    Configuration
      3 warmup runs, 100240 benchmark runs, 20 iterations per run
    Execution Time
      Average         1.55 us
      Fastest       569.00 ns (-984.97 ns, -63.38 %)
      Slowest       206.06 us (204.51 us, 13160.39 %)
      Lowest RSD    594.35 ns ± 0.54% (3258th run)
    Performance
      Average     643511.32 iterations/s
      Fastest    1757469.24 iterations/s (-1113957.92 iterations/s, -173.11 %)
      Slowest       4852.88 iterations/s (638658.44 iterations/s, 99.25 %)
```
