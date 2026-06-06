# 🧱 Soil Analyzer – دانه‌بندی خاک

A modular C++20 tool for sieve analysis and soil classification (ASTM, AASHTO, BSCS, MIT).

## ✨ Features
- Auto‑detection of sieve system
- D10, D30, D60, Cu, Cc calculation
- Classification by: ASTM, AASHTO, BSCS, MIT
- LL/PL input → PI
- File (CSV/TXT) or manual input
- Save results

## 🛠 Requirements
- C++20 compiler (GCC 11+, Clang 15+, MSVC 2022)
- CMake ≥ 3.16 (optional)

## 🚀 Build & Run

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
./SoilAnalyzer
```

## 📂 Project Structure

```
├── Analyze.cpp/h       # system matching
├── Input.cpp/h         # manual input
├── Insert.cpp/h        # file input
├── Sieve.cpp/h         # sieve data
├── Soil.cpp/h          # classification core
├── Screen.h            # console tools
└── SoilAnalyzer.cpp    # main & menu
```

## 📥 Input File Format (CSV/TXT)

```
4,4.75,92
8,2.36,85
...
```

## 📸 Sample Output

```
System         Classify
[ASTM]         SW

NO | SIZE  | PASSING
4  | 4.75  | 100
...

LL | PL | PI
35 | 20 | 15

D10   D30   D60   CC    CU
0.15  0.60  2.00  1.20  13.33
```


