# 🪨 Soil Classification System (C++20)

A comprehensive **soil classification program** written in **modern C++20**, implementing **ASTM, AASHTO, MIT, and BSCS** standards.  
The program reads **sieve analysis data**, calculates key parameters (D10, D30, D60, CU, CC), Atterberg limits (LL, PL, PI), and classifies the soil accordingly.  

---

## ⚙️ Features

- 📜 **CSV or manual data input** for sieve analysis  
- 📏 **Automatic calculation** of D₁₀, D₃₀, D₆₀ via linear interpolation  
- 📈 Computes **Uniformity (CU)** and **Curvature (CC)** coefficients  
- 💧 Calculates **Atterberg limits**: Liquid Limit (LL), Plastic Limit (PL), Plasticity Index (PI)  
- 🧱 Supports multiple **naming/classification systems**:
  - ASTM (Unified Soil Classification System)
  - AASHTO
  - MIT
  - BSCS (British System)
- 💾 **Data saving** in CSV format  
- 💻 100% **standard C++20 STL** — no external libraries  
- 🧩 Fully **object-oriented** design (using class `Soil`)

---

## 🧮 Example Usage

### 🧰 1. Create an object
```cpp
#include "Soil.h"

int main() {
    Soil soil(System::ASTM);
    soil.InsertData("soil.csv"); // or soil.Add_Data() for manual input
    soil.Classify();
    soil.Print_Data();
    soil.SaveFile_CSV();
}
```
