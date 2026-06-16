# Generalized Diffusion Models

## Overview

This project contains implementations of two graph diffusion models:

1. **GLT1DT** – Code for Generalized Diffusion Model
2. **GSLT1DT** – Code for Generalized Single Threshold Diffusion Model

Both models are implemented in C++ and support batch execution over multiple datasets using a Python script.

---

## Project Structure

```
.
├── GLT1DT/                      # Generalized Diffusion Model
│   ├── DiffusionData.cpp
│   ├── DiffusionData.h
│   ├── Graph.cpp
│   ├── Graph.h
│   ├── Main.cpp
│   ├── Dataset/                 # Input datasets
│   ├── filelist                 # List of dataset files
│   └── runall.py                # Batch execution script
│
├── GSLT1DT/                     # Generalized Single Threshold Diffusion Model
│   ├── DiffusionData.cpp
│   ├── DiffusionData.h
│   ├── Graph.cpp
│   ├── Graph.h
│   ├── Main.cpp
│   ├── Dataset/                 # Input datasets
│   ├── filelist                 # List of dataset files
│   └── runall.py                # Batch execution script
│
└── README.md
```

---

## Directories

### 1. GLT1DT

Implements the **Generalized Diffusion Model**, which captures information propagation under varying levels of trust and mistrust among individuals in a network.

### 2. GSLT1DT

Implements the **Generalized Single Threshold Diffusion Model**, a simplified version where each node uses a single threshold to determine activation.

---

## Source Files

Both directories contain the following files:

| File | Description |
|--------|-------------|
| `DiffusionData.h` | Declarations of diffusion-related data structures. |
| `DiffusionData.cpp` | Implementation of diffusion data handling functions. |
| `Graph.h` | Graph class definitions. |
| `Graph.cpp` | Graph operations and algorithms. |
| `Main.cpp` | Main driver program. |

---

## Compilation

Navigate to the desired model directory and compile all source files.

### GLT1DT

```bash
cd GLT1DT
g++ *.cpp -o diffusion
```

### GSLT1DT

```bash
cd GSLT1DT
g++ *.cpp -o diffusion
```

---

## Dataset Files

All input datasets should be placed inside the corresponding

```
Dataset/
```

directory.

The file

```
filelist
```

contains the names of the datasets to be processed.

---

## Running Experiments

### Single Execution

After compilation:

```bash
./diffusion
```

---

### Batch Execution

To execute the program on all datasets listed in `filelist`:

```bash
python3 runall.py filelist
```

---

## Requirements

- GNU C++ Compiler (`g++`)
- Python 3.x

---

## Example Workflow

### For GLT1DT

```bash
cd GLT1DT
g++ *.cpp -o diffusion
python3 runall.py filelist
```

### For GSLT1DT

```bash
cd GSLT1DT
g++ *.cpp -o diffusion
python3 runall.py filelist
```

---

## Models Included

| Directory | Model |
|------------|-------|
| `GLT1DT` | Generalized Diffusion Model |
| `GSLT1DT` | Generalized Single Threshold Diffusion Model |

---

## Notes

- Each model has its own dataset directory and execution script.
- Batch experiments are controlled through the `filelist` file.
- The code is designed for conducting diffusion and information propagation experiments on network datasets.
