# 🧨 Minesweeper_Fall_2025

A classic **Minesweeper** game built in **C++ using SFML**, developed as part of a University of Florida computer science course. This project faithfully recreates the original Minesweeper experience while adding quality-of-life improvements for smoother and more enjoyable gameplay.

---

## 🎮 Features

- Classic Minesweeper gameplay  
- Variable board sizes  
- Mine counter and timer  
- Right-click flagging  
- Win and lose screens  
- Restart button  
- Debug mode to reveal all mines  
- Custom textures and sprites  

### ⭐ Gameplay Enhancements

- **Safe first click** — the first tile clicked is guaranteed not to be a mine  
- **Guaranteed large opening on first click**, ensuring a meaningful starting area  

---

## 🛠 Technical Details

- **Language:** C++  
- **Graphics Library:** SFML  
- **Platform:** Windows  

### Implementation Highlights

- Board implemented using a **2D structure of custom `Tile` objects**  
- Recursive flood-fill logic used to reveal adjacent empty tiles  
- Optimized tile-reveal logic with **no redundant checks** for better performance  
- Clean separation between game logic, rendering, and input handling  

---

## ▶️ How to Run

1. Download or clone the repository  
2. Navigate to the project directory  
3. Run the executable:  


> Ensure the `assets/` folder is in the same directory as the executable, as it contains all required textures and resources.

---

## 📁 Project Structure

Minesweeper_Fall_2025/
├── files/
│ └── (images, textures, fonts)
└── source files
  └── program.exe

---

## 🎓 Academic Context

This project was created for a **University of Florida Computer Science course** and focuses on applying object-oriented programming, recursion, and efficient state management in C++.

---

## 👤 Author

**Harishankar Rajesh**
