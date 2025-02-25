DROP TABLE IF EXISTS questions;
CREATE TABLE IF NOT EXISTS questions (
    id INTEGER PRIMARY KEY,
    question TEXT NOT NULL,
    option_a TEXT NOT NULL,
    option_b TEXT NOT NULL,
    option_c TEXT NOT NULL,
    correct_option TEXT NOT NULL 
);
INSERT INTO questions VALUES (
    '1',
    'What is acrophobia a fear of?',
    'Spiders',
    'Snakes',
    'Heights',
    'c'
);
INSERT INTO questions VALUES (
    '2',
    'Who was the Ancient Greek God of the Sun? ',
    'Zeus',
    'Apollo',
    'Ares',
    'b'
);
INSERT INTO questions VALUES (
    '3',
    'Which day of the week does the Jewish Sabbath begin? ',
    'Monday',
    'Friday',
    'Sunday',
    'b'
);
INSERT INTO questions VALUES (
    '4',
    'Where would you be if you were standing on the Spanish Steps? ',
    'Rome',
    'Barcelona',
    'Madrid',
    'a'
);
INSERT INTO questions VALUES (
    '5',
    'Where is Angel Falls, the world’s largest waterfall, located? ',
    'Brazil',
    'Argentina',
    'Venezuela',
    'c'
);
INSERT INTO questions VALUES (
    '6',
    'Which planet has the most moons? ',
    'Saturn',
    'Mercury',
    'Mars',
    'a'
);
INSERT INTO questions VALUES (
    '7',
    'Who was the first woman to win a Nobel Prize? ',
    'Mother Teresa',
    'Marie Curie',
    'Jane Addams',
    'b'
);
INSERT INTO questions VALUES (
    '8',
    'What artist has the most streams on Spotify?',
    'Drake',
    'Ariana Grande',
    'Rihanna',
    'a'
);
INSERT INTO questions VALUES (
    '9',
    'Which country has won the most World Cups?',
    'USA',
    'France',
    'Brazil',
    'c'
);
INSERT INTO questions VALUES (
    '10',
    'How many hearts does an octopus have?',
    'Three',
    'One',
    'Four',
    'a'
);