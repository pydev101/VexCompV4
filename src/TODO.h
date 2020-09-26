/*
Base Motor:
--Ensure that axis directions are proper
--Ensure that the +/- nature of set velocity function changes direction on that axis
--Find minVel value
--Intregrate opical shaft encoders and omni wheels

Adv Motor:
--See if RAW is the encoder count for motor (should be 900 per rotation for 18:1 gearbox) - If RAW is 900perRotation then swich AdvUnits from Deg to RAW
--Set Constants and if helpfull create conversion constants
--Create PID motor functions to move robot accuratly
--Axis
--Rotatation
--Intregrate omni wheels into encoders

Odom:
--Create movement functions that use Adv Motor to move
--Create virtual Grid

Auto Programs:
--Program section based auto game modes
--Create selection GUI





*/