
SELECT ID FROM student WHERE GPA > 3.0 AND ID < 4;
SELECT course.CourseID, enrollment.Semester FROM course INNER JOIN enrollment ON course.CourseID = enrollment.CourseID;
UPDATE enrollment SET Semester = 'Spring 2024', CourseID = 2 * (CourseID + 1) WHERE StudentID < 4;
UPDATE enrollment SET Semester = 'Spring 2023', CourseID = 2 * (CourseID + 1) WHERE StudentID = 3;
DELETE FROM enrollment WHERE StudentID = 4;
