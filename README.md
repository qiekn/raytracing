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

![image.ppm](imgs/20251230-1.jpg)

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

## Sending rays to the scene

We got this blue-to-white gradient depending on ray Y coordinate.

![blue-to-white gradient](./imgs/20251230-2.jpg)

## A simiple red sphere

![red-sphere](./imgs/20251230-3.jpg)

## A sphere colored according to its normal vectors

![colored-sphere-with-normal-vector](./imgs/20251230-4.jpg)

## Normals-colored sphere with ground

![normal-colored](./imgs/20251230-5.jpg)

## Antialiasing

![after](./imgs/20251230-6.png)

## A diffuse sphere

![duffuse sphere](./imgs/20260101-1.jpg)

## Gamma correction

![gamma fix](./imgs/20260101-2.jpg)

## Metal

![shiny metal](./imgs/20260101-3.jpg)

![fuzz metal](./imgs/20260101-4.jpg)

## Glass

![always refracts](./imgs/20260101-5.jpg)

Total internal reflection: Air bubble sometimes refracts, someties reflects

![total internal reflection](./imgs/20260101-6.jpg)

Hollow glass sphere

![hollow glass sphere](./imgs/20260101-7.jpg)

## Camera fov

A wide-angle view

![wide-angle view](./imgs/20260101-8.jpg)

A distant view

![a distant view](./imgs/20260101-9.jpg)

Zooming in

![zooming in](./imgs/20260101-10.jpg)

## Defocus blur

![defocus blur](./imgs/20260101-11.jpg)
