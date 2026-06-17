# Trivia - Multiplayer Trivia Game

A client-server multiplayer trivia game built in C++ (server) and C# WPF (client).

## Demo

[Google Drive - Project Demonstration](https://drive.google.com/drive/folders/1BQA4PytYZTDhh1xX5uezaS70WIWyMXLS?usp=sharing)

---

## How It Works

Players connect to a central server, log in or sign up, and can create or join trivia rooms.
Once the room admin starts the game, all players answer the same questions simultaneously with a countdown timer.
Results are shown at the end with a leaderboard.

---

## Architecture

- **Server** - C++, TCP socket server, SQLite database, multithreaded (one thread per client)
- **Client** - C# WPF desktop app
- **Protocol** - Custom binary protocol: "[1 byte code] [4 bytes length] [JSON body]"

### Server Components

| Component | Role |
|---|---|
| Communicator | Accepts clients, spawns a thread per connection |
| Request Handlers | State machine - each client has an active handler (Login / Menu / Room / Game) |
| LoginManager | Signup, login, logout with REGEX validation |
| RoomManager | Create, join, close, and track rooms |
| GameManager | Manages active games and player state |
| StatisticsManager | Reads and writes player statistics |
| SqliteDatabase | Persistent storage for users, questions, and stats |

---

## Features

- User registration and login
- Create and join game rooms
- Real time waiting room with player list (polling every 3 seconds)
- Timed trivia questions with answer highlighting
- End game leaderboard
- Personal statistics and high score table

---

## Bonuses Implemented

### ✅ Bonus 1.0.4 - Extended User Info with REGEX Validation

At signup, users must provide additional fields that are validated server-side using regular expressions:

| Field | Rule |
|---|---|
| **Password** | Minimum 8 characters, must include uppercase, lowercase, digit, and a special character from "! @ # $ % ^ & *" |
| **Email** | Format: "letters/digits @ domain.tld" - supports multi-part domains like ".co.il" |
| **Address** | Format: "Street, Apt, City" - street and city letters only, apt numbers only |
| **Phone** | Format: "prefix-number" - prefix starts with "0", 2 digits for landline (e.g. "03") or 3 for mobile (e.g. "052"), followed by 7 digits |
| **Birthdate** | Format: "DD/MM/YYYY" or "DD.MM.YYYY" |

All fields are stored in the database and the statistics row is initialized to zero on signup.

### ✅ Bonus 1.0.4 — Singleton Design Pattern

All major server-side managers and services are implemented as Singletons, ensuring a single shared instance throughout the program's lifetime: "SqliteDatabase", "LoginManager", "RoomManager", "GameManager", "StatisticsManager", "RequestHandlerFactory", "Communicator", and "Server".

### ✅ Bonus 4.0.1 - Add Custom Trivia Questions

A dedicated GUI screen allows users to add new trivia questions to the created game.

Features:

- Enter a question
- Enter the correct answer
- Enter three incorrect answers

Questions added through this screen become part of the current trivia game questions.

### ✅ Bonus 4.0.1 - Head-to-Head (1v1) Matchmaking

A special "Head-to-Head" game mode was implemented.

Features:

- Dedicated GUI button and screen
- Two layers enter a matchmaking queue
- The server waits until exactly two players are available
- Both players receive a 5 second countdown notification
- The trivia game automatically starts for both participants simultaneously

This mode provides a fast competitive experience without requiring players to create or join rooms manually.

### Bonus 5.0.0 - One-Time Pad (OTP) Encryption
All client-server communication (requests and responses) is encrypted using a One-Time Pad (OTP) encryption.

- Every message is encrypted
- Keys are unique and are generated in the signup proccess.
- Ensures unbreakable communication.

This applies to the entire protocol layer, not just specific messages.

### Bonus - Email Verification System
During signup, users must verify their account using a 6-digit one-time verification code sent via email.

- Accounts will not be created until verification is completed
- Prevents fake or automated account creation
- Ensures valid email ownership before login access

### Bonus - Thread Pooling
The server uses a thread pool (size 20) instead of spawning a thread per client.

Benefits:
- predictable resource usage
- improved scalability under load

### Bonus - Custom Trivia Questions System
Room admins can add new trivia questions through a dedicated GUI screen.

Each question supports:
- question text  
- correct answer  
- three incorrect answers  

New questions are immediately integrated into the active question.

### Bonus - Questions With Pictures
When a room admin adds a new question he is presented with the option of adding an image.

- The file must be an image.
- The question can be about the image, making the game more interactive.

### GUI Improvements - Animated WPF Interface
The client features an enhanced animated GUI with improved transitions making the gameplay experience much more fun!

#### Room Configuration Control
Room admins can configure the number of questions per game session, allowing customizable game length.


---

## Running the Project

1. Start the server ("Trivia.exe") - listens on port 12334
2. Launch one or more clients
3. Sign up or log in, then create or join a room
4. Admin starts the game when ready
5. Have lots of fun!
