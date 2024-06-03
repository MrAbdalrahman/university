CREATE DATABASE clinic;
USE clinic;

-- Patients table
CREATE TABLE Patients (
    Patient_ID INT PRIMARY KEY,
    Name VARCHAR(255),
    DOB DATE,
    Address VARCHAR(255),
    Email VARCHAR(255)
);

-- Appointments table
CREATE TABLE Appointments (
    Appointment_ID INT PRIMARY KEY,
    Patient_ID INT,
    Date DATE,
    Time TIME,
    Purpose VARCHAR(255),
    FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) on delete cascade
);

-- Visits table
CREATE TABLE Visits (
	patient_ID INT,
    Visit_ID INT PRIMARY KEY,
    Date DATE,
    Time TIME,
	FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) on delete cascade
);



-- Messages table
CREATE TABLE Messages (
    Message_ID INT PRIMARY KEY,
    Appointment_ID INT,
    Patient_ID INT,
    visit_ID INT,
    Date DATE,
    Type VARCHAR(100),
    Content TEXT,
    FOREIGN KEY (Appointment_ID) REFERENCES Appointments(Appointment_ID) on delete cascade ,
    FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) on delete cascade ,
    FOREIGN KEY (visit_ID) REFERENCES visits(visit_ID) on delete cascade 
);




-- Services table
CREATE TABLE Services (
    Service_ID INT PRIMARY KEY,
    Name VARCHAR(255),
    Description TEXT
);


-- Treatment records table
CREATE TABLE Treatment_Records (
	Visit_ID INT,
    Record_ID INT PRIMARY KEY,
    Description TEXT,
    FOREIGN KEY (Visit_ID) REFERENCES Visits(Visit_ID) on delete cascade 
);


-- Contact Numbers table
CREATE TABLE Contact_Numbers (
    Contact_ID INT AUTO_INCREMENT PRIMARY KEY,
    Patient_ID INT,
    Contact_number VARCHAR(50),
    FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) on delete cascade
);


create table service_Patient(
	patient_ID INT,
    service_ID INT,
    primary key(patient_ID,service_ID),
	FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) on delete cascade,
	FOREIGN KEY (service_ID) REFERENCES services(service_ID) on delete cascade 
);

create table service_visit(
	visit_ID INT,
    service_ID INT,
    primary key(visit_ID,service_ID),
	FOREIGN KEY (visit_ID) REFERENCES visits(visit_ID) on delete cascade,
	FOREIGN KEY (service_ID) REFERENCES services(service_ID) on delete cascade
);

create table service_message(
	message_ID INT,
    service_ID INT,
    primary key(message_ID,service_ID),
	FOREIGN KEY (message_ID) REFERENCES messages(message_ID) on delete cascade,
	FOREIGN KEY (service_ID) REFERENCES services(service_ID) on delete cascade
);

create table service_Treatment_record(
	Record_ID INT,
    service_ID INT,
    primary key(Record_ID,service_ID),
	FOREIGN KEY (Record_ID) REFERENCES Treatment_records(Record_ID) on delete cascade,
	FOREIGN KEY (service_ID) REFERENCES services(service_ID) on delete cascade
);

