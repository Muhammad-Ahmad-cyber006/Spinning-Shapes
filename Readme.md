# Spinning Shapes

Three 3D shapes spinning in the terminal written in C. No libraries, no graphics API, just math and ASCII characters.

## The one idea behind all of them

Take a dot in 3D space, do math on it, it shows up on the 2D screen. Do that for every dot on the shape 60 times a second and your eye sees a spinning shape. Everything else is just details of that idea.

## Spinning Cube

The starting point. Covers the core pipeline that every shape after it reuses.

Concepts it clears:
- Linear algebra: rotation matrices Rx Ry Rz to spin a point around X Y Z axes
- Perspective projection: divide x and y by z so far things look smaller
- Z-buffer: stores 1/z per pixel so the closer point always draws on top
- 1D array as 2D grid: index = x + width * y, used for both pixel and depth buffer

## Spinning Pyramid

Same pipeline as the cube. The only new problem is that a pyramid face is a triangle and a normal square loop goes outside the triangle edges.

Concepts it clears:
- Barycentric coordinates: any point inside a triangle is a weighted mix of its 3 corners, P = u*A + t*B + s*C where u+t+s always equals 1, keeping s within 1 minus t is what stops the loop from going outside
- Why everything in 3D is triangles: 3 points are always flat, 4 are not, so the square base splits into 2 triangles

## Spinning Donut

Same pipeline again with two things the others did not have.

Concepts it clears:
- Torus parametric surface: two angles, i sweeps around the tube and j sweeps that around the big ring, together they reach every point on the surface
- Lambertian lighting: dot product of the surface normal with a light direction gives a number that picks a character from a palette going dark to bright, actual lighting math instead of faking it

## Build

```bash
gcc -O2 -o cube cube.c -lm && ./cube
gcc -O2 -o pyramid pyramid.c -lm && ./pyramid
gcc -O2 -o donut donut.c -lm && ./donut
```

Ctrl+C to exit. Needs GCC and a Unix terminal.