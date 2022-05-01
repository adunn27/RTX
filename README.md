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
Then go to the output directory and click `RTX.exe` and it will output a `output.ppm` image file in the same directory.
You can also run it from the command line and give a different filename as the first argument to output to a different file.

