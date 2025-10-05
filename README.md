# Typing Coach

An interactive C++ console application that helps users improve their typing speed and accuracy by providing real-time feedback and detailed statistics.

---

## Features

- **Case-insensitive word matching** - Type words in any case
- **Speed tracking** - Monitors your typing speed in WPM (words per minute)
- **Real-time word correctness checking** with color-coded feedback
- **Tracks mistyped words and mistake frequency**
- **Suggests corrections using Levenshtein Distance algorithm**
- **Interactive commands** - View stats during your session with `/stats`
- **Shows typing accuracy percentage**
- **Saves detailed session report** to `report.txt`
- **Visual formatting** with color highlighting for better readability

---

## How It Works

1. Loads valid English words from `words.txt` into a Trie data structure.
2. Prompts the user to type words in real time.
3. For each word:
   - If correct → shows ✓ Correct! in green
   - If incorrect → shows ✗ Mistake detected! in red and suggests up to 3 similar words.
   - If the same word is mistyped repeatedly → shows a warning.
4. User can type special commands:
   - `exit` - End the session and see summary
   - `/stats` - View current statistics (WPM, accuracy, etc.)
   - `/help` - Display available commands
5. When the user exits, a detailed session summary is displayed with:
   - Total words typed
   - Number of correct words
   - Mistake count
   - Words per minute (WPM)
   - Session duration
   - Top 5 most mistyped words
   - Overall accuracy

---

## Technologies Used

- C++
- Standard Template Library (STL)
  - `unordered_map`, `vector`, `string`
- Trie Data Structure
- Levenshtein Distance Algorithm
- File I/O

---

## How to Run

1. Make sure `g++` is installed on your system.
2. Clone or download this repository.
3. Make sure you have a file named `words.txt` containing dictionary words (one word per line).
4. Compile the program:

   **On Windows:**
   ```bash
   g++ -std=c++17 -O2 -Wall main.cpp trie.cpp -o TypingCoach.exe
   ```

   **On Linux/Mac:**
   ```bash
   g++ -std=c++17 -O2 -Wall main.cpp trie.cpp -o typingcoach
   ```

5. Run the application (from the directory with words.txt):

   **On Windows:**
   ```bash
   .\TypingCoach.exe
   ```

   **On Linux/Mac:**
   ```bash
   ./typingcoach
   ```

6. Type words as prompted, or use special commands:
   - Type `exit` to end the session
   - Type `/stats` to see your current stats
   - Type `/help` to see available commands
   - Type `/debug` to check if a word exists in the dictionary
