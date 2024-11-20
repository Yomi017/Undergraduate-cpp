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
SELECT ID,Name FROM student WHERE GPA > 2.8 AND GPA < 3.2;
SELECT ID,Name FROM student WHERE ID = 1000;
SELECT ID,Name FROM student WHERE GPA > 3.0 OR GPA < 3.0;