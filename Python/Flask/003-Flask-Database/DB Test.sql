USE flasklearning;

INSERT INTO user(user_name, email, pass) 
VALUES("Arup", "arup@gmail.com", "1234"), 
("Arpita", "arpita@gmail.com", "2345");

SELECT 
    *
FROM
    user;