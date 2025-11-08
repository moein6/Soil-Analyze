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

### 📁 1. file : soil_sample.csv

```
4.75,100
2.00,95
0.425,80
0.075,25
```

### 🧰 2. Create an object

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

### 🖥 Output : 
```
system    ,Classify
---------------
[ASTM] : SP (Poorly-graded sand)
[AASHTO] : A-1 to A-3 (Granular soils)
[MIT] : Coarse soil (sand/gravel)

Sieve      Passing
--------------------
4.75       100%
2          95%
0.425      80%
0.075      25%

LL        ,PL        ,PI
------------------------------
35        ,20        ,15

D10       ,D30       ,D60       ,CU        ,CC
--------------------------------------------------
0.12      ,0.30      ,0.75      ,6.25      ,1.88

Data saved to D:\Soil_allUS.csv

```

🪨 سیستم طبقه‌بندی خاک (C++20)

برنامه‌ای مدرن و شیءگرا به زبان ++C20 برای تحلیل و طبقه‌بندی خاک‌ها بر اساس استانداردهای
ASTM، AASHTO، MIT و BSCS.
این برنامه داده‌های آزمایش دانه‌بندی (sieve analysis) را از فایل CSV می‌خواند، پارامترهای مهمی مانند
D10، D30، D60، CU، CC و حدود اتربرگ (LL، PL، PI) را محاسبه کرده و نوع خاک را تعیین می‌کند.

⚙️ قابلیت‌ها

📜 خواندن داده از فایل CSV یا ورود دستی

📏 محاسبه خودکار D10، D30، D60، CU، CC

💧 محاسبه حدود اتربرگ (LL، PL، PI)

🧱 پشتیبانی از چند سیستم نام‌گذاری (ASTM, AASHTO, MIT, BSCS)

💾 ذخیره نتایج در فایل CSV

💻 نوشته‌شده فقط با کتابخانه‌های استاندارد
