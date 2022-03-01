"""
Commands:
GRAPH:GRID:[LABEL]:[lowXLim]/[highXLim]:[lowYLim]/[highYLim]; # Creates a new grid graph
POINT:[GRID_LABEL]:[X]:[Y]:[color]; #New point
VECTOR:[GRID_LABEL]:[X]:[Y]:[dX]:[dY]:[color]; #New vector

GRAPH:NEW:[LABEL] #New graph
ARRAY:[LABEL]; # New array
APPEND:[ARRAY_LABEL]:[newValue]; # Append to an array
PLOT:[ARRAY_LABEL]:[ARRAY_LABEL]:[LINE/POINT]; #Plot a scatter or line based on array values

# If any command is invalid in the set; The entire set is dropped
SET:START
[Command list]
SET:END


UPDATE:[GRID/GRAPH Label]:[ [GRID/GRAPH Label]:[ ... ] ] #Animate the graph based on new data
CLEAR:[GRID/GRAPH/ARRAY LABEL] #Clear a gid/graph/array object based on type of label given
"""