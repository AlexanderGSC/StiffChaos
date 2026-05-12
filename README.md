## STIFFCHAOS
### ROSENBROCK INTEGRATION ENGINE FOR STIFF SYSTEMS AND CHAOTIC DYNAMICS
StiffChaos-C is a solver based on the 3-stage Rosenbrock method (ROS3), this engine is engineered for the deterministic resolution of stiff Ordinary Differential Equations (ODEs) and multi-scale chemical kinetics.

Operational Constraint: Zero-Heap Architecture. No dynamic memory allocation (malloc/free). Static memory footprint ensures reliability for bare-metal deployment and real-time embedded systems.

### PROJECT STRUCTURE
Plaintext
.
├── CMakeLists.txt      # System build configuration
├── include/            # Core headers (ros3.h, system.h)
├── solver/             # Numerical core implementation
├── models/             # Physical system definitions (Robertson, Lorenz)
├── tests/              # Execution entry points and validation
├── vis/                # Plotly-based data visualization
└── gallery/            # Validated simulation outputs

### BUILD AND EXECUTION PROTOCOL
Execution requires CMake 3.10+ and a standard C99 compiler.
Bash

mkdir -p build && cd build
cmake ..
make
./test_robertson
