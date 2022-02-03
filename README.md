# Julia/Mandelbrot Visualization
This is my final project for graduation in my High School Computer Science class. Original version 1.0 is from that time, back in spring 2020. I've reviewed the code and rebuilt the libraries for Visual Studio 2022 and added these binaries to the repository. So this project should compile and run on any version of Visual Studio 2022.

## Prerequisites
This project is intended for building on Windows 10/11. To use the libraries and link them succesfully this project should be built using [Visual Studio 2022](https://visualstudio.microsoft.com/vs/). All the SFML libraries have been built using this IDE. If you wish to use another version of Visual Studio, you should recompile them yourself. Instructions are given on their website: [SFML-2.5.1 building](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php).

## Description
This is a Windows application, written using C++ language and OOP design. The fractals are rendered using GLSL. This application should be run on a dedicated GPU for the best performance.

The program renders a particular set of Julia-type fractals, defined using the formula fc(z) = z^2 + c. The Julia Set is the boundary of the set of complex points z such that their orbits are bounded (i.e., under repeated iteration of applying the function fc on z, we will get results in the same bounded area). The 'Julia Parameter' c (the red dot that appears on the screen) can be moved by clicking and dragging with the mouse. This action triggers the rendering of the Mandelbrot Set in the background, as a guide for choosing the position of the red dot. In short, the connectedness of the Julia Set is determined by the red dot: if it's in the interior of the Mandelbrot Set, then the fractal is connected; otherwise, it is disconnected (this is observed when there appear no filled interiors and the colors change drastically in swirls).

There are 4 different coloring schemes, which render the fractal using different colors and techniques (notice that we fill the interior of the set and color the exterior based on factors like distance or angle). There are also 10 different preset values of the parameter c, which render famous fractals.

If your GPU has the power, you can enable the Uber mode, which increases the accuracy and depth of iterations, but this can consume a lot of power.
