# PointInside3D
Code for determining whether or not a point is inside a polyhedron

This code was inspired by a series of articles publishing on the CodeProject website by John Jiyang Hou

http://www.codeproject.com/Members/John-Jiyang-Hou

The C++ code here was taken from that and amended so that it builds and runs on 64-bit Linux.

The Fortran code is my own work and implements a simpler variation over what is originally presented.
The code was built and tested with the Intel 16.0.1 compiler on 64-bit Linux (Mageia 4).

The main variations are as follows:

1) the centroid of the polyhedron is used to ensure that the normals to the triangular faces are outward;
2) the output is written with gnuplot in mind since there isn't a Linux version of the original viewer.


