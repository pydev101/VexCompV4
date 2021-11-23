/*

TODO:
1) Finish tracking for GPS, Inertial/Encoder systems
2) Ensure error functions return proper values
3) Usable move functions
4) Slight auto adjustments to PID values to ensure it will reach target (If not moving and not within threashold; increase gain by 1.01 per cycle until it starts moving again)

Future Features
1) GUI Libary
--Components header file (Buttons, Text Boxes, Frames, etc)
--Class that handles events and passes it to the components, animates the screen, etc
2) Graphing
--Smart file writing; Quick write to buffer then a seperate thread that actually writes to a file so it doesn't slow the program
--Log target path, real path, velocity, error, targetSpeed etc into a CSV file for later analysis
--Ability to graph data on Brain Screen
3) Machine Learning
--Adjust gain values to ensure it reaches target and optimize velocity
4) Nonhold move functions
--Allow it to lower arm, etc while moving to a target and then mvoe to the next target as soon as it arrives at the orignal


*/