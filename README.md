# RTX: Raytracer

A C++ RayTracer made following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html) books, both avalible at https://raytracing.github.io/

## Output Pictures

![Render of many spheres](/images/many_orbs.png)

![Render of a foggy scene](/images/final_scene.png)

![Render of Cornell Box](/images/cornell_box.png)

## Features:

- Anti-Aliasing
- Bounding-Volume Hierarchies
- Camera with adjustable position and attributes
- Checkered Texture
- Depth of Field
- Fog and Smoke
- Image Textures
- Lights
- Multiple Materials: 
  - Matte (Lambertian)
  - Metal with adjustable level of reflectiveness
  - Glass
- Multiple Object Shapes: Boxes and SPHERES!!
- Rotatable Objects

## How to use

Import the .sln file using Visual Studio and build the program.
Using Command Line (currently needs to be cmd not PowerShell due to the text encoding, but this is being worked on) go to its build directory and run 

```
.\RTX.exe > FILENAME.ppm
``` 

With `FILENAME` adjusted to your desired output filename
