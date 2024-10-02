# ChaosInterview
A solution to the interview problem at Chaos

## Solution Layout
The code is split into two parts: 
- `lighting` - a standalone library implementing the algorithm, as well as input and output file operations
  - `include` - provides the public headers
  - `src` - provides the private implementation headers
  - `test` - creates a random scene withs 250 000 lights
  - `app` - an executable, demonstrating the usage of the library
- `gui` - a Qt GUI, serving as a wrapper of the library, providing the user graphical means to select the input file and visualize the results.

## Notes
- The current solution does not return exact values. I attempted to improve the accuracy in several ways:
  - the library classes take a parameter - a floating point data type, which allows for experimenting with the performance and accuracy of different data types and finding a balance between accuracy and performance
  - attempting two different algorithms - one using `atan2()` (which was my initial idea) and the second using vectors and dot products
  - using Kahan summation
  - multiplying the coordinates by a `MULTIPLIER`, hoping this could lead to reduced loss of accuracy
  - avoiding subtraction errors of numbers with negligible difference (catastrophic cancellation) by reformulating the calculation of the dot product and the distance. 
- Qt uses `lighting` as an external library. Modify the `gui.pro` file to change the path to the library, if needed. 
- I was curious if the algorithm could be made faster and experimented with a modified version of my ray tracer's BVH. If I had done it from scratch, I would have used a bottom-up approach using heuristics, instead of a top-down approach. I didn't achieve improvements in performance, so I didn't use it, but I left it for completeness.