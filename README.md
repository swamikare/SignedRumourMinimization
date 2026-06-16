# Generalized Diffusion Models and Rumor Minimization in Signed Social Networks

## Description

This repository contains implementations of generalized information diffusion models and corresponding rumor minimization algorithms for signed social networks with varying levels of trust and mistrust among individuals.

The repository includes two diffusion models:

- **GLT1DT** – Generalized Diffusion Model
- **GSLT1DT** – Generalized Single Threshold Diffusion Model

In addition to simulating information propagation, the code provides algorithms for **rumor minimization**, where the objective is to limit the spread of misinformation by strategically selecting influential nodes for positive information dissemination.

The implementation supports experimentation on multiple datasets and provides automated batch execution facilities.

---

# Dataset Information

The repository contains separate directories for each diffusion model.

```
.
├── GLT1DT/
│   ├── Dataset/
│   ├── filelist
│   ├── runall.py
│   ├── DiffusionData.cpp
│   ├── DiffusionData.h
│   ├── Graph.cpp
│   ├── Graph.h
│   └── Main.cpp
│
├── GSLT1DT/
│   ├── Dataset/
│   ├── filelist
│   ├── runall.py
│   ├── DiffusionData.cpp
│   ├── DiffusionData.h
│   ├── Graph.cpp
│   ├── Graph.h
│   └── Main.cpp
│
└── README.md
```

### Dataset Directory

Each model contains a `Dataset/` directory that stores the network datasets used in the experiments.

### Dataset List

The file

```
filelist
```

contains the names of the datasets that will be processed during batch execution.

---

# Code Information

The repository consists of two independent implementations.

## 1. GLT1DT – Generalized Diffusion Model

This model captures information diffusion in signed social networks under varying levels of trust and mistrust between connected individuals.

The implementation also includes algorithms for rumor minimization based on the Generalized Diffusion Model.

---

## 2. GSLT1DT – Generalized Single Threshold Diffusion Model

This model employs a single activation threshold for each node while accounting for varying trust and mistrust relationships.

The implementation includes rumor minimization algorithms based on the Generalized Single Threshold Diffusion Model.

---

## Source Files

| File | Description |
|--------|-------------|
| `DiffusionData.h` | Definitions of data structures and declarations related to diffusion processes. |
| `DiffusionData.cpp` | Implementation of diffusion-related operations. |
| `Graph.h` | Graph class definitions and function declarations. |
| `Graph.cpp` | Implementation of graph algorithms and network operations. |
| `Main.cpp` | Main driver program for executing diffusion and rumor minimization experiments. |
| `runall.py` | Python script for running experiments on multiple datasets. |
| `filelist` | List of dataset files used during batch execution. |

---

# Usage Instructions

## Step 1: Compile the Source Code

Navigate to the desired model directory.

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

## Step 2: Prepare the Datasets

Place all input datasets inside the corresponding

```
Dataset/
```

directory.

Specify the dataset names in

```
filelist
```

---

## Step 3: Execute the Program

### Single Execution

```bash
./diffusion
```

### Batch Execution

Run experiments for all datasets listed in `filelist`.

```bash
python3 runall.py filelist
```

---

# Requirements

## Software Requirements

- GNU C++ Compiler (g++)
- Python 3.x

## Operating System

The code has been developed and tested on Linux-based systems and should also compile on macOS and Windows systems supporting GCC.

---

# Methodology

The proposed framework consists of the following stages.

### 1. Network Construction

- Read the input graph from the dataset.
- Construct a signed social network representation.

### 2. Trust–Mistrust Modeling

- Associate varying levels of trust and mistrust with the relationships between individuals.

### 3. Rumor Propagation

- Initialize rumor seed nodes.
- Simulate rumor diffusion using the underlying diffusion model.

### 4. Positive Information Propagation

- Select positive seed nodes using the implemented rumor minimization strategies.
- Propagate positive information throughout the network.

### 5. Node Activation

- Determine node activation according to the diffusion model.
- In GSLT1DT, each node uses a single activation threshold.

### 6. Rumor Minimization

- Evaluate the effectiveness of positive seeds in suppressing rumor spread.
- Compute the number of activated nodes and diffusion statistics.

### 7. Experimental Evaluation

- Repeat the process for all datasets specified in `filelist`.
- Compare the performance of rumor minimization approaches under different diffusion settings.

---

# License

This repository is intended for academic and research purposes.

Users are free to use, modify, and redistribute the code provided that appropriate credit is given to the original authors.

---

# Models Included

| Directory | Description |
|------------|-------------|
| **GLT1DT** | Generalized Diffusion Model and Rumor Minimization |
| **GSLT1DT** | Generalized Single Threshold Diffusion Model and Rumor Minimization |

---

# Keywords

Information Diffusion, Rumor Minimization, Signed Social Networks, Trust and Mistrust, Influence Propagation, Threshold Models, Social Network Analysis, Network Science.
