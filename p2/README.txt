Hey yo~ welcome to the cherno engine manual, I learned some structure of the renderring from my favorate Youtuber "The Cherno"'s OpenGL play list https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

This renderring engine used plugin of 
1.GLEW and 
2.GLFW as the set up of opengl framework
3.GLM library for using glm::vec3 and glm::mat4 structs to store vertex and matrix.
4.ImGUI library for simple UI handling of the tranform of the entities

Manual of the engine:

1.Start: click Chreno.sln and press F5 to run the app in (x86) setting (in Visual Studio)

2.Camera: There are total 4 cameras on the scene which are:
  - Top Left: Projective
  - Top Right: Orthographic - XY plane
  - Down Left: Orthographic - XZ plane
  - Down Right: Orthographic - YZ plane

3.Entities: The engine will load 3 object defined in file (Assets/Objs/Objects.txt), you can easily switch between them by click button

4.Property Window: The Transform Editor window is an override window from ImGUI plugin where you can edit the transform properties of the 3 entities.

  -To switch between entities, click the buttons below Entity List

  -To Edit position, euler angle or scale of the selected entity: drag the bar on the Transform editor

5.Save scene: Hit SPACE button to save the scene which will record current vertices of all entities on the scene into (Assets/Objs/Objects.txt)
  
  -When start the engine again, properties saved last time will be loaded
  (Since in this project, the vertices are saved instead of tranform, every time the engine restart, the transform will be restart to 0
   However, the relative shape of the objects remain the same)