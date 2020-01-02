# Monte Carlo Ray Tracer

Monte Carlo ray tracer with Path Tracing and Photon Mapping written in C++. 

This program was developed for the course [Advanced Global Illumination and Rendering (TNCG15)](https://liu.se/studieinfo/kurs/tncg15) at Linköpings Universitet. A report describing this work in more detail is available [here](report.pdf).

## Renders

The following images are renders of [scenes/hexagon_room.json](scenes/hexagon_room.json) and [scenes/oren_nayar_test.json](scenes/oren_nayar_test.json) produced by the program:

___

<h3 align="center">Path Traced, Scene IOR 1.0</h3>

![](renders/c1_64sqrtspp_report_4k_downscaled.png "Path Traced, Scene IOR 1")

___

<h3 align="center">Path Traced, Scene IOR 1.75</h3>

![](renders/c1_64sqrtspp_report_4k_flintglass_downscaled.png "Path Traced, Scene IOR 1.75")

___

<h3 align="center">Photon Mapped, Scene IOR 1.0</h3>

![](renders/c1_photon-map_report_2e6_250_16sqrtspp.png "Photon Mapped")

___

<h3 align="center">Path Traced, Skylit Oren-Nayar Spheres</h3>

![](renders/oren_nayar_test.png "Path Traced, Skylit Oren-Nayar Spheres")

## Compiling

Currently only Visual Studio 2019 project files are included. Compiling on other platforms should be straightforward however, just compile the *.cpp* files in [source](source) with [external/glm](external/glm) and [external/nlohmann](external/nlohmann) directories added to the include path. Requires a compiler with C++17 support.
