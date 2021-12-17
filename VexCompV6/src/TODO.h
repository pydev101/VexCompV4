/*

TODO:
1) Finish tracking for GPS, Inertial/Encoder systems
2) Ensure error functions return proper values
3) Usable move functions
4) Intellgent speed; Max accel, max speed, const speed, etc; Speed is returned as the output of robot class and unit conversian and mtor set occurs in intergration class
5) Slight auto adjustments to PID values to ensure it will reach target (If not moving and not within threashold; increase gain by 1.01 per cycle until it starts moving again)

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
--Cost = TotalTime of travel; TotalTime over Gain must be minimized; Gain can be adjusted if constant ossilation or minimum speed is outside because total time would still not be fully optimal and so it won't break algorithm
4) Nonhold move functions
--Allow it to lower arm, etc while moving to a target and then mvoe to the next target as soon as it arrives at the orignal



//TODO use gyro for autobalence
//TODO ensure PID valeus set correctly
//TODO raise arm to certain point when intake is activated
//TODO Vision sensor to turn to face a goal
//TODO bump swithc to know when goal is in the pot

*/