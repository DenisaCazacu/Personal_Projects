CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS messages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sender_username TEXT NOT NULL,
    receiver_username TEXT NOT NULL,
    content TEXT NOT NULL,
    is_read BOOLEAN DEFAULT 0,
    FOREIGN KEY (sender_username) REFERENCES users (username),
    FOREIGN KEY (receiver_username) REFERENCES users (username)
);