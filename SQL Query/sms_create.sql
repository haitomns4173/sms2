create database sms

use sms

create table faculty
(
	ID int primary key identity(1,1),
	FACULTY_NAME varchar(20) not null unique,
	FACULTY_LETTER char not null unique
);

create table classes 
(
	CLASS_CODE varchar(4) primary key,
	CLASS varchar(10),
	ROOM_NO int not null unique,
	FACULTY_NAME int references faculty(ID)
);

create table position_code 
(
	POSITION_CODE varchar(4) primary key,
	POSITION_NAME varchar(100) not null unique
);

create table credentials_login
(
	ID int primary key identity(1,1),
	username varchar(20),
	password varchar(50),
	POSITION_CODE varchar(4) not null
);

create table teacher_type
(
	ID int primary key identity(1,1),
	TYPE_NAME_IS varchar(20) not null unique
);

create table student_table 
(
	ID  int primary key identity(1001,1),
	FULL_NAME varchar(100) not null,
	AGE smallint,
	GENDER char,
	ADDRESS varchar(100),
	PHONE_NO varchar(10),
	CLASS smallint not  null,
	ROLL int not null,
	FACULTY varchar(10)
);

create table teacher_table 
(
	ID  int primary key identity(1001,1),
	FULL_NAME varchar(100) not null,
	AGE smallint,
	ADDRESS varchar(100),
	PHONE_NO varchar(10),
	CLASS smallint,
	SALARY decimal not  null,
	TEACHER_TYPE int
);

drop table teacher_table

insert into teacher_table (FULL_NAME, AGE, GENDER, ADDRESS, PHONE_NO, CLASS, SALARY, TEACHER_TYPE) values('"++"');
