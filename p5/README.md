Hey yo~

The program uses Opengl for rendering pixel-buffers, no opengl math is used

For this assignment, I implemented 5 different surfaces which are:
1. Plane
2. Sphere
3. Triangle
4. Cube
5. Tetrahedron
(sadly I failed implement the epic ellipsoid intersaction)

Input:
The input xml I used in stored in the folder of RayTracer/inputs/scene1.xml

Sample pics:
There is already pictures for the final effect of scene1.xml stores in SampleOutput folder,
(there is another picture for rendering the random scene in skeleton code which I found really cool, so I just put it there)

Explaination of the scene:

A plane using phong material on the ground
A cube using dielectric material front
Three spheres using matel, dielectric and phong materials from left to right in the middle

A tetrahedron using phong material on the left behind
A cube using matel meterial on the right behind
A triangle using phong material on the left behind

Three lights: ambient, directional and point light, which result in 2 shadows on the ground (shadow cast will ignore dielectric material)



