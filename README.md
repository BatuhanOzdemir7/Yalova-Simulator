# C Based Console Strategy Game 

This is a simple, text-based grand strategy game written entirely in C. The game runs in the console and uses ASCII characters to represent the map, cities, and the player.

The objective is to manage your starting city, build an economy, raise an army, and conquer the other cities on the map.

## Core Features

* **ASCII Map Exploration:** Navigate the game world using `WASD` keys.
* **City Management:**
    * Control cities you own.
    * Set tax rates (`vergi`) to manage your budget (`butce`) and public happiness (`memnun`).
    * High taxes increase income but lower happiness, which can lead to desertions!
* **Military & Conquest:**
    * Recruit soldiers (`asker`) using your city's budget.
    * Declare war on neighboring cities.
    * A simple, probabilistic combat system determines the winner based on relative army sizes.
* **Dynamic World (AI):**
    * The game state updates every 5 seconds (using `time.h`).
    * AI-controlled cities autonomously manage their own economies, collect taxes, and build armies to challenge you.

## Included Towns
*Ç --- Çınarcık
*Y --- Yalova
*S --- Samanlı
*E --- Elmalık
*T --- Termal
*A --- Altınova
*K --- Kocaeli
*F --- Fıstıklı
*B --- Bursa

## How to Play

1.  Compile and run the program. (Note: The use of `conio.h` and `system("cls")` makes this primarily compatible with Windows).
2.  Enter a single character to represent your player.
3.  Move around the map (roads are marked as `=`) using **W, A, S, D**.
4.  Move onto a city (represented by a capital letter like 'Ç', 'Y', 'B') to interact with it.
5.  The first city you enter will become your capital.
6.  From a city you control, you will have options:
    * **1:** Change the tax rate.
    * **2:** Recruit a soldier (costs 25 budget).
    * **3:** Declare war on another city.
7.  Press **Q** to quit the game.

## Technical Details

This project is a practical example of fundamental C programming concepts, including:
* **Data Structures:** Using `struct` to manage the complex state of each city.
* **Game Loop:** A main `while(1)` loop handles user input (`getch()`) for movement.
* **Time-based Events:** Using `time.h` and `difftime()` to create a "game tick" that updates the world state (budgets, AI actions) independently of player input.
* **Console I/O:** Manipulating the console with `system("cls")` and `putchar()` to create a simple graphical interface.
* **State Management:** Tracking player ownership (`bizimMi`) and dynamic game variables like happiness and army size.