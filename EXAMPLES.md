# MicrogradCpp Examples

This guide walks through the provided examples, showing different usage patterns and when to use each one.

## Quick Start

If you're new to MicrogradCpp, start with **easy.cpp** → **easy_adam.cpp** → **easy_df.cpp** → **main.cpp**.

---

## 1. easy.cpp - Minimal Example

**Use this if:** You want the simplest possible example using the in-memory Iris dataset.

**What it demonstrates:**
- Loading the Iris dataset with `get_iris()`
- Creating an MLP model
- Training with the default SGD optimizer

**Key code:**
```cpp
DatasetType dataset = get_iris();
shuffle(dataset);

MLP model(4, {10, 10, 3});
train_eval(dataset, 0.8, model, 0.01, 100);
```

**Architecture:** 4 inputs → [10, 10] hidden layers → 3 outputs
**Optimizer:** SGD
**Learning rate:** 0.01
**Epochs:** 100

**Compile:**
```bash
g++ -std=c++17 -Iinclude -O2 -o easy easy.cpp && ./easy
# or
make run  # (if Makefile is configured for easy.cpp)
```

---

## 2. easy_adam.cpp - Adam Optimizer Example

**Use this if:** You want to see how to use the Adam optimizer instead of SGD.

**What it demonstrates:**
- Same setup as easy.cpp
- Extracting model parameters with `model.parameters()`
- Initializing and using the `AdamOptimizer`

**Key code:**
```cpp
MLP model(4, {10, 10, 3});
auto params = model.parameters();
AdamOptimizer optimizer(params, 0.01);

train_eval(dataset, 0.8, model, optimizer, 100);
```

**Differences from easy.cpp:**
- Uses Adam optimizer (often converges faster)
- Same hyperparameters for fair comparison

**Compile:**
```bash
g++ -std=c++17 -Iinclude -O2 -o easy_adam easy_adam.cpp && ./easy_adam
```

---

## 3. easy_df.cpp - DataFrame API

**Use this if:** You want to load data from a CSV file and use data preprocessing features.

**What it demonstrates:**
- Loading CSV with `DataFrame::from_csv()`
- Data normalization with `df.normalize()`
- Categorical encoding with `df.encode_column()`
- Shuffling with `df.shuffle()`

**Key code:**
```cpp
DataFrame df;
df.from_csv("./data/iris.csv");
df.normalize();
df.encode_column("variety");
df.shuffle();

MLP model(4, {10, 10, 3});
train_eval(df, 0.8, model, 0.01, 100);
```

**Benefits:**
- Automated data preprocessing
- Works with any CSV (not just Iris)
- Normalization improves training stability

**Compile:**
```bash
g++ -std=c++17 -Iinclude -O2 -o easy_df easy_df.cpp && ./easy_df
```

---

## 4. main.cpp - Advanced Example

**Use this if:** You want a complete example with interactive configuration and Adam optimizer.

**What it demonstrates:**
- DataFrame preprocessing
- Adam optimizer usage
- Interactive configuration (user enters epoch count)

**Key code:**
```cpp
DataFrame df;
df.from_csv("./data/iris.csv");
df.normalize();
df.encode_column("variety");
df.shuffle();

MLP model(4, {10, 10, 3});
auto params = model.parameters();

int epochs;
std::cout << "Enter number of epochs: ";
std::cin >> epochs;

AdamOptimizer optimizer(params, 0.01);
train_eval(df, 0.8, model, optimizer, epochs);
```

**Includes:**
- Complete data preprocessing pipeline
- Adam optimizer (often better convergence)
- Interactive input for experimentation

**Compile:**
```bash
g++ -std=c++17 -Iinclude -O2 -o main main.cpp && ./main
# or
make run  # (builds and runs main.cpp)
```

---

## Comparison Table

| Example | Dataset | Optimizer | Preprocessing | Interactive |
|---------|---------|-----------|---|---|
| easy.cpp | In-memory | SGD | No | No |
| easy_adam.cpp | In-memory | Adam | No | No |
| easy_df.cpp | CSV | SGD | Yes | No |
| main.cpp | CSV | Adam | Yes | Yes |

---

## Standardized Hyperparameters

All examples use consistent settings for fair comparison:

- **Model architecture:** 4 inputs → [10, 10] hidden → 3 outputs
- **Learning rate:** 0.01
- **Epochs:** 100 (configurable in main.cpp)
- **Train/test split:** 80/20

To experiment with different hyperparameters, modify the values in your chosen example.

---

## Next Steps

After running these examples:

1. **Modify hyperparameters** in easy.cpp and observe the effects
2. **Compare SGD vs Adam** (easy.cpp vs easy_adam.cpp)
3. **Try different architectures:** Change `{10, 10}` to `{16, 16}` or `{5, 5}`
4. **Load your own CSV:** Replace the Iris dataset path in easy_df.cpp
5. **Explore the library:** Check the `include/` directory for more features
