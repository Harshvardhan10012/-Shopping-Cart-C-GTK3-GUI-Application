# 🛒 Shopping Cart — C & GTK3 GUI Application

A simple and interactive **Shopping Cart** desktop application built in **C** using the **GTK3 graphical toolkit**.  
It demonstrates modular C programming, event-driven GUI development, and payment simulation — all in one clean project.

---

## 📚 Table of Contents
- [🚀 Features](#-features)
- [📁 Project Structure](#-project-structure)
- [🧰 Requirements](#-requirements)
- [⚙️ Build Instructions](#️-build-instructions)
- [▶️ Run the Application](#️-run-the-application)
- [🧠 Code Overview](#-code-overview)
- [🧩 Future Enhancements](#-future-enhancements)
- [👨‍💻 Author](#-author)
- [🪪 License](#-license)

---

## 🚀 Features

✅ **Product Catalog** – Display a list of items with names and prices.  
➕ **Add / Remove Items** – Manage items dynamically in the cart.  
💰 **Payment Simulation** – Checkout with fake payment confirmation.  
🪶 **Clean UI** – Minimal and modern GTK3 interface.  
🧩 **Modular Code** – UI, cart, and payment logic are cleanly separated.  

---

## 📁 Project Structure

shopping_cart/
│
├── main.c # Entry point of the app
├── ui.c # Handles all GTK UI design and event signals
├── ui.h
├── cart.c # Shopping cart logic
├── cart.h
├── payment.c # Simulates payment process
├── payment.h
├── Makefile # Build automation file
└── README.md # Project documentation


---

## 🧰 Requirements

This project runs on **Windows**, **Linux**, or **macOS** with GTK3 installed.

### 🔹 For Windows (MSYS2 / MinGW UCRT64)

1. Download & install **[MSYS2](https://www.msys2.org/)**  
2. Open **MSYS2 MinGW UCRT64** terminal  
3. Run these commands:

   ```bash
   pacman -Syu
   pacman -S mingw-w64-ucrt-x86_64-gtk3
   pacman -S mingw-w64-ucrt-x86_64-pkgconf
   pacman -S make

🔹 For Linux (Ubuntu/Debian)
    sudo apt update
    sudo apt install build-essential libgtk-3-dev pkg-config make

⚙️ Build Instructions
🧱 Using make
   make


▶️ Run the Application

Run from the MSYS2 (UCRT64) terminal:

./shopping_cart.exe 

A GTK window will open showing the product catalog and shopping interface.


| File                      | Responsibility                                                      |
| ------------------------- | ------------------------------------------------------------------- |
| **main.c**                | Initializes GTK and launches the UI                                 |
| **ui.c / ui.h**           | Builds the graphical interface and event callbacks                  |
| **cart.c / cart.h**       | Contains functions for adding/removing items and calculating totals |
| **payment.c / payment.h** | Handles simulated payment processing and confirmation dialog        |


🧩 Future Enhancements

✨ Integrate real payment APIs (Stripe/PayPal sandbox)
📦 Store cart data in JSON or SQLite database
🌐 Add backend integration for inventory
💡 Apply discounts, taxes, and promotional codes
🎨 Improve UI design with custom GTK themes



👨‍💻 Author
Harshvardhan

Built with ❤️ in C using GTK3.
