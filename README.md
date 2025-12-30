# Ray Tracing Series

## Build

```bash
cmake -B build
cmake --build build -j${nproc}
```

# Notes

## PPM image format

create `ppm` image

```bash
cmake --build build -j${nproc}
./build/main > image.ppm
```

![image.ppm](image.jpg)

```txt
P3
256 256
255
0 0 0
1 0 0
2 0 0
3 0 0
4 0 0
5 0 0
6 0 0
7 0 0
8 0 0
9 0 0
10 0 0
11 0 0
12 0 0
...
```
