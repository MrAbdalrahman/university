CREATE TABLE users (
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL,
    PRIMARY KEY (username)
);

use clinic;
select * from users;

INSERT INTO Patients (Patient_ID, Name, DOB, Address, Email) VALUES
(10patients02, 'John Doe', '1980-01-01', '1234 Elm Street', 'johndoe@example.com');

select * from patients users;

use users;
select * from users;
delete from users where username = 'abood';

drop database clinic_user_abood;

use clinic;

select * from contact_numbers;

SELECT contact_id, patient_id, contact_number FROM contact_numbers;

select * from visits;