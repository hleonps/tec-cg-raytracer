# Ray Tracing Engine

Created by Heiner León & Luis Alvarado

## Overview

This Ray Tracing engine was created as part of the course "IC8019 Introduction to Computer Graphics" at Costa Rican Institute of Technology (TEC).

Using <a href="http://cs.stanford.edu/people/eroberts/courses/soco/projects/1997-98/ray-tracing/types.html">Backward Ray technique</a> and <a href="http://web.eecs.umich.edu/~sugih/courses/eecs487/lectures/16-Phong+Shading.pdf">Phong shading</a>, it provides the necessary for any user to create a 3D scene using geometric primitives and light sources.

## Features

- Geometric Primitives
  - Spheres
  - Polygons
  - Cylinders
  - Cones
  - Disks
- Ligh sources
  - Attenuation factor.
  - Environment lighting.
- Material propieties for objects
  - Diffuse reflection.
  - Specular/Mirror reflection.
- Textures Mapping (There are some bugs).

## Usage

The scenes has to be described in a .3d file using own declarative language. Use the example in /examples for guidance.

Tested on macOS and Linux using gcc.

```
$ make
$ ./raytracing <file.3d>
```

The output is an AVS file. We recommend <a href="https://www.imagemagick.org/script/index.php">Image Magick</a> for viewing and exporting to other formats.
