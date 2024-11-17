CREATE DATABASE db_university;
USE DATABASE db_university;
CREATE TABLE student (
ID INTEGER,
Name TEXT,
GPA FLOAT
);
INSERT INTO student VALUES (1000, 'Jay Chou', 3.0);
INSERT INTO student VALUES (1001, 'Taylor Swift', 3.2);
INSERT INTO student VALUES (1002, 'Adele', 2.9);
SELECT ID,Name FROM student WHERE Name = 'Jay Chou';
SELECT ID,Name FROM student WHERE Name = 'Taylor Swift';
SELECT ID,Name FROM student WHERE Name = 'Adele';
SELECT ID,Name FROM student WHERE GPA > 2.8;