Program instruction:

The code of the assignment 5 are in 
Engine.h Engine.cpp 	//data store and manipulation
Renderer.h Renderer.cpp	//algorithm
UI_SimpleEditor.h UI_SimpleEditor.cpp //UI

Running the application

1  - There are two curves showing on the screen
     Left: Bezier curve, Right: B-spline curve

2  - On the UI panel, you can create or delete individual curve

3  - To edit each curve, click the button of the curve name to open the edit panel

3.1 - Beizer curve input,
      you can manipulate the control points by 
     - push back
     - inserting
     - deleting
     - change the property of each points.

3.2- B-spline curve input
     you can do the same as in Beizer curve editor, in addition you can
     - change the degree (it will automatically correct you if the degree cannot be changed)
     - change the line connection type (if the line connect to the start and end point)
     - change the property of each knots

4  - Save and Load
   - Save: on the main panel, there is save button to save all curve data in the scene to Assets/Curves/curveData.txt
   - Load: same as save, the button read all data form the txt file
   - Reset button, this is for reset the scene to the begining status, as there are one Bezier curve on the left and one B-spline curve on thee right