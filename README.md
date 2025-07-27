# Evaluation_AG
Performance Evaluation of a Genetic Algorithm Based on Parameter Configuration in the 0-1 Knapsack Problem


# 🎯 Evaluation_AG  
**Parameter Configuration Evaluation in Genetic Algorithms for the 0-1 Knapsack Problem**

This repository is part of an academic research project that explores how the **parameter configuration of a Genetic Algorithm (GA)** affects its performance when solving the **0-1 Knapsack Problem**.

---

## 📂 Contents

- `AG.c`: main C program that implements the Genetic Algorithm.  
- `*.txt`: benchmark datasets from [Universidad del Cauca](http://artemisa.unicauca.edu.co/~johnyortega/instances_01_KP/).  
- `Enfoque_de_Evaluación_para_la_Configuración_de_Parámetros_en_Algoritmos_Genéticos_Un_Caso_del_Problema_de_la_Mochila_0_1.ipynb`: Jupyter Notebook used for **data analysis and visualization** of the results.

---

## ⚙️ About the Implementation

The **Genetic Algorithm** is implemented in C and supports:

- Tournament selection  
- One-point crossover  
- Bitwise mutation  
- Multiple mutation rates and tournament sizes  
- Evaluation over several benchmark datasets  
- GAP calculation based on known optimal values

Each configuration is evaluated across multiple executions to compute average GAP (%).

---

## 📊 Data Visualization

The Python notebook processes the output of the C program and generates:

- Comparative GAP plots  
- Parameter performance trends  
- Summary tables for each dataset and configuration

This helps **analyze the effect of mutation rates and selection pressure** on solution quality.

---

## 📁 Benchmark Datasets

The benchmark instances used in this study are **openly published** by:

> 📌 Universidad del Cauca (Colombia)  
> 🔗 http://artemisa.unicauca.edu.co/~johnyortega/instances_01_KP/

All dataset credits go to **Prof. Johny Ortega** and collaborators.  
These instances are widely used for evaluating algorithms on the 0-1 Knapsack Problem.

---

## ▶️ How to Compile and Run

```bash
gcc AG.c -o ag
./ag

```

Make sure the dataset files (e.g., f1_l-d_kp_10_269.txt) are in the same directory.


---
## 📄 License

This repository is released under the MIT License.
