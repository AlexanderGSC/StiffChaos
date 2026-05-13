## STIFFCHAOS
### ROSENBROCK INTEGRATION ENGINE FOR STIFF SYSTEMS AND CHAOTIC DYNAMICS

StiffChaos-C is a solver based on the 3-stage Rosenbrock method (ROS3), this solver is engineered for the deterministic resolution of stiff Ordinary Differential Equations (ODEs) and multi-scale kinetics. Simulation of chaotic systems, with a visualisation of how the trajectory colonises the attractor space using Plotly:

![Halvorsen Attractor](gallery/halvorsen.gif)

### Features

- Implicit error control (using a two-stage embedded method)
- Dynamic step size calculated based on the error-to-tolerance ratio
- Zero-Heap Architecture constraint
- No dynamic memory allocation (`malloc/free`)
- Static memory footprint designed for bare-metal deployment and real-time embedded systems

---

### PROJECT STRUCTURE

```text
.
├── CMakeLists.txt      # System build configuration
├── include/            # Core headers (ros3.h, system.h)
├── solver/             # Numerical core implementation
├── models/             # Physical system definitions (Robertson, Lorenz)
├── tests/              # Execution entry points and validation
├── vis/                # Plotly-based data visualization
└── gallery/            # Validated simulation outputs
```

---

### BUILD AND EXECUTION PROTOCOL

Execution requires:
- CMake 3.10+
- Standard C99 compiler

```bash
mkdir -p build && cd build
cmake ..
make
./test_robertson
```

---

### The Robertson System

The system models kinetic reactions in which the reaction rate changes drastically, whilst obeying a linear conservation law:

```math
X + Y + Z = 1
```

The Robertson system is frequently used to assess the stability of time integrators, given its extreme stiffness and the ability to measure the actual numerical drift using only the values of the three variables.

### Differential Equations

```math
\frac{dX}{dt} = -0.04X + 10^4YZ
```

```math
\frac{dY}{dt} = 0.04X - 10^4YZ - 3 \cdot 10^7Y^2
```

```math
\frac{dZ}{dt} = 3 \cdot 10^7Y^2
```

### Initial Conditions

```math
[1.0,\ 0.0,\ 0.0]^T
```

### Conservation Invariant

```math
X + Y + Z = 1.0
```

Using Robertson’s stiff scheme as the primary method for assessing the stability of the method, a deviation of the order of $10^{-12}$ in the conservation invariant is observed after 10^{8} accepted steps.

---

### Execution Output

```text
(TIME 100000.0006)
X: 0.016926090730 Y: 0.000000068856 Z: 0.983073840415 SUM: 1.000000000000142109
H STEP: 1.000000e-03 ERR/TOL: 2.282618476728e-04
----------------------------------------------------------------------
FINAL RESULTS
Elapsed time: 18.3649 (secs)
Steps accepted: 100697447
Percentage steps accepted: 100.00
Steps rejected: 9
Percentage steps rejected: 0.00
Total simulated time (SUM h accepted): 100000.0006
NUMBER OF STEPS: 100697456
Max H_STEP used: 1.0000e-03
Min H_STEP used: 1.0000e-09
---------------------END OF SIMULATION--------------------------------------
```
