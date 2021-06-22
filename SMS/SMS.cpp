// Header Files
#include "pch.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <string.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <dos.h>

//Using Standard Namespace 
using namespace std;

// Global Variable that stores the keys
char retry_key;
int menu_select;
short int login_fail = 1;

// Structure Decleration 
struct registeration_variables
{
	string fullname, address;
	char phone_no[10], age[10];

	char teacher_faculty[10], teacher_type[20];
	float teacher_salary;
	int teacher_class;

	char student_faculty[10];
	int student_class, student_roll;
};

struct idPassword
{
	string username, password;
};

// Use to set the Pointer of printing line
void setPointer(short int xpnt, short int ypnt)
{
	COORD position = { xpnt, ypnt };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// Use to print top part
void top_printing()
{
	system("CLS");
	setPointer(60, 12);
	cout << "SCHOOL MANAGEMENT SYSTEM" << endl;
}

// Use to open the program in full screen 
void fullscreen()
{
	keybd_event(VK_MENU, 0x38, 0, 0); //Press down Alt
	keybd_event(VK_RETURN, 0x1c, 0, 0); //Press down Enter

	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0); //Release Alternate
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);//Release Enter
}

// Input the username and password when there is a new registeration 
class UidPWD
{
	struct idPassword ip;
public:
	int username_and_password()
	{
		top_printing();
		setPointer(60, 17);
		cout << "Choose a Username and Password ";
		setPointer(60, 20);
		cout << "Username : ";
		setPointer(60, 22);
		cout << "Password : ";
		setPointer(40, 40);
		cout << "NOTE: Username Should letters and numbers. No Special Symbols are allowed.";
		setPointer(40, 41);
		cout << "      Password should be less than 20 charactes. It should contain letters,";
		setPointer(40, 42);
		cout << "      numbersand symbols.";
		setPointer(71, 20);
		cin >> ip.username;
		setPointer(71, 22);
		cin >> ip.password;

		return 0;
	}
};

// Class Made to connnect to SQL SERVER 
class Sql
{
public:
	//define handles and variables
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

public:
	void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
	{
		SQLCHAR SQLState[1024];
		SQLCHAR message[1024];

		if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		{
			// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
			setPointer(15, 40);
			cout << "SQL Error Message " << message;
			setPointer(15, 41);
			cout << "SQL State: " << SQLState << "." << endl;
		}
	}

public:
	void sqlconnection()
	{
		do {
			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
				// Allocates the environment
				break;

			if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
				// Sets attributes that govern aspects of environments
				break;

			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
				// Allocates the connection
				break;

			if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
				// Sets attributes that govern aspects of connections
				break;

			SQLCHAR retConString[1024]; // Conection string
			switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)"DRIVER={SQL Server}; SERVER=localhost, 14808; DATABASE=sms;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
				// Establishes connections to a driver and a data source
			case SQL_SUCCESS:
				setPointer(60, 28);
				cout << "Connected to Database";
				setPointer(60, 29);
				cout << "Proceeding to main Program ........";
				Sleep(4000);
				break;
			case SQL_SUCCESS_WITH_INFO:
				setPointer(60, 28);
				cout << "Connected to Database";
				setPointer(60, 29);
				cout << "Proceeding to main Program ........";
				Sleep(4000);
				break;
			case SQL_NO_DATA_FOUND:
				setPointer(60, 28);
				cout << "Not Connected to Database";
				showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
				setPointer(60, 29);
				cout << "Exiting the Program ........";
				Sleep(4000);
				sql_close();
				exit(0);
				retCode = -1;
				break;
			case SQL_INVALID_HANDLE:
				setPointer(60, 28);
				cout << "Not Connected to Database";
				showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
				setPointer(60, 29);
				cout << "Exiting the Program ........";
				Sleep(4000);
				sql_close();
				exit(0);
				retCode = -1;
				break;
			case SQL_ERROR:
				setPointer(60, 28);
				cout << "Not Connected to Database";
				showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
				setPointer(60, 29);
				cout << "Exiting the Program ........";
				Sleep(4000);
				sql_close();
				exit(0);
				retCode = -1;
				break;
			default:
				break;
			}

			if (retCode == -1)
				break;

			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
				// Allocates the statement
				break;
		} while (FALSE);
	}

public:
	void sql_query_executer(char sqlQuery[])
	{
		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)sqlQuery, SQL_NTS))
		{
			// Executes a preparable statement
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		}
		else
		{
			cout << "\n\n Inserted";
		}
	}

	int sql_query_searcher(char sqlQuery[])
	{
		int id = 0;
		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)sqlQuery, SQL_NTS))
		{
			// Executes a preparable statement
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		}
		else
		{
			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
			{
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
				// Retrieves data for a single column in the result set
			}
		}
		return id;
	}

public:
	void sql_close()
	{
		SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
		SQLDisconnect(SQLConnectionHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
		// Frees the resources and disconnects
	}
};

// Class for Registeration 
class Register
{
	char position = 'A';
	
	struct registeration_variables reg;

public:
	void registeration(int position_automatic)
	{
		int phone_test = 0;
		int converted_age = 0;

	re_enter_register:
		top_printing();

		setPointer(60, 17);
		cout << "Fill the Details!";
		setPointer(60, 20);
		cout << "Full Name         : ";
		setPointer(60, 22);
		cout << "Age               : ";
		setPointer(60, 24);
		cout << "Address           : ";
		setPointer(60, 26);
		cout << "Phone No.         : ";
		setPointer(60, 28);

		// Automatically Decides the user is a teacher/admin or anyone else
		if (position_automatic == 1)
		{
			cout << "Poistion (T/S/O)  : ";
		}
		else if (position_automatic == 2)
		{
			cout << "Poistion (T/S/O)  : S";
		}

		setPointer(80, 20);
		cin.ignore();
		getline(cin, reg.fullname);

		setPointer(80, 22);
		cin >> reg.age;
		converted_age = atoi(reg.age);

		// Check the age 
		if (!(converted_age > 0 && converted_age < 100))
		{
			setPointer(60, 30);
			cout << "Wrong Age!, Re-Enter Age.....";
			Sleep(1500);
			goto re_enter_register;
		}

		setPointer(80, 24);
		cin.ignore();
		getline(cin, reg.address);

		setPointer(80, 26);
		cin >> reg.phone_no;
		phone_test = strlen(reg.phone_no);

		//Checks the phone
		if (phone_test != 10)
		{
			setPointer(60, 30);
			cout << "Wrong Phone No.!, Re-Enter Roll No......";
			Sleep(1800);
			goto re_enter_register;
		}

		// Position Determiner part 2 if the position is of admin then give him the chance to choose the position or use the variable to determine
		if (position_automatic == 1)
		{
			setPointer(80, 28);
			cin >> position;
			fflush(stdin);
		}
		else if (position_automatic == 2)
		{
			position = 's';
		}

		// Checks for the postion
		if (position == 't' || position == 'T')
		{
			teacher_register();
		}
		else if (position == 's' || position == 'S')
		{
			student_register();
		}
		else if (position == 'o' || position == 'O')
		{
			staff_register();
		}
		else
		{
			setPointer(60, 30);
			cout << "No such Position found !";
			setPointer(60, 31);
			cout << "Hit Enter to retry and Esc to exit ...";
			retry_key = _getch();

			if (retry_key == 27)
			{
				exit(0);
			}
			else
			{
				goto re_enter_register;
			}
		}
	}

	void teacher_register()
	{
		setPointer(60, 30);
		cout << "Class             : ";
		setPointer(60, 32);
		cout << "Faculty           : ";
		setPointer(60, 34);
		cout << "Type              : ";
		setPointer(60, 36);
		cout << "Salary            : ";
		setPointer(80, 30);
		cin >> reg.teacher_class;
		setPointer(80, 32);
		cin >> reg.teacher_faculty;
		setPointer(80, 34);
		cin >> reg.teacher_type;
		setPointer(80, 36);
		cin >> reg.teacher_salary;

		//Input Username and Password
		UidPWD teacherUidPwd;
		teacherUidPwd.username_and_password();
	}

	void student_register()
	{
		setPointer(60, 30);
		cout << "Class             : ";
		setPointer(60, 32);
		cout << "Faculty           : ";
		setPointer(60, 34);
		cout << "Roll No.          : ";
		setPointer(80, 30);
		cin >> reg.student_class;
		setPointer(80, 32);
		cin >> reg.student_faculty;
		setPointer(80, 34);
		cin >> reg.student_roll;

		//Input Username and Password
		UidPWD studentUidPwd;
		studentUidPwd.username_and_password();

	}
	void staff_register()
	{
		char staff_type[20];
		float staff_salary;

		setPointer(60, 30);
		cout << "Type              : ";
		setPointer(60, 32);
		cout << "Salary            : ";
		setPointer(80, 30);
		cin >> staff_type;
		setPointer(80, 32);
		cin >> staff_salary;

		//Input Username and Password
		UidPWD staffUidPwd;
		staffUidPwd.username_and_password();
	}

	void registeration_query()
	{
		
		char reg_qurey[800];
		char uname[30] = "alok", pname[30] = "ghhgh";

		if (position == 't' || position == 'T')
		{
			strcpy(reg_query,"inser into uid (Username, Password) values('"uname "','"pname"');");
			//strcpy(reg_qurey,"INSERT INTO teacher_table (FULL_NAME,AGE,ADDRESS,PHONE_NO,CLASS,SALARY,TEACHER_TYPE) VALUES('" + reg.fullname + "','" + reg.age + "','" + reg.address + "','" + reg.phone_no + "',1,500,'" + reg.teacher_type + "');");
		}
		else if (position == 's' || position == 'S')
		{
			student_register();
		}
		else if (position == 'o' || position == 'O')
		{
			staff_register();
		}
	}


	//Function to show the Registertion Number
	void display_registeration_no()
	{
		int query_result;
		char sql_search_query[254] = "select id from teacher_table where name = 'alok'";

		Sql displayRegNo;
		query_result = displayRegNo.sql_query_searcher(sql_search_query);

		if (query_result == 0)
		{
			char sql_search_query_2[254] = "select id from teacher_table where name = 'alok'";
			query_result = displayRegNo.sql_query_searcher(sql_search_query_2);
			if (query_result == 0)
			{
				char sql_search_query_3[254] = "select id from teacher_table where name = 'alok'";
				query_result = displayRegNo.sql_query_searcher(sql_search_query_3);
			}
			else
			{
				cout << " Your Registeration ID : NHSS" << query_result;
			}
		}
		else
		{
			cout << " Your Registeration ID : NHSS" << query_result;
		}
	}
};

// Class for Editing the Record
class Edit
{
public:
	void edit_main(int position_automatic)
	{
		short int id_edit_search;
		top_printing();

		setPointer(60, 17);
		cout << "Fill the Details!";
		setPointer(60, 20);
		cout << "ID         : ";
		setPointer(80, 20);
		cin >> id_edit_search;
	}
};

// Admin Panel
class Admin : public Register, public Edit
{
public:
	void  admin_panel()
	{
	re_try_teacher:

		top_printing();
		setPointer(60, 17);
		cout << "Admin Panel";
		setPointer(60, 19);
		cout << "1. Registeration";
		setPointer(60, 21);
		cout << "2. Edit";
		setPointer(60, 23);
		cout << "3. Delete";
		setPointer(60, 25);
		cout << "4. View";
		setPointer(60, 27);
		cout << "5. Accountancy";
		setPointer(60, 29);
		cout << "6. Push Notification";
		setPointer(60, 32);
		cout << "Press the number .... ";
		cin >> menu_select;
		fflush(stdin);
		switch (menu_select)
		{
		case 1:
			registeration(1);
			break;
		case 2:
			edit_main(1);
			break;
		case 3:
			delete_by_admin();
			break;
		case 4:
			view_record_by_admin();
			break;
		case 5:
			accountancy_admin();
			break;
		case 6:
			push_notification();
			break;
		default:
			setPointer(60, 35);
			cout << "Wrong Input!";
			setPointer(60, 37);
			cout << "Hit Enter to re-enter and esc to exit ....";
			retry_key = _getch();

			if (retry_key == 27)
			{
				exit(0);
			}
			else
			{
				goto re_try_teacher;
			}
		}
	}

public:
	void edit_by_admin()
	{
		cout << "\n\n\n Edit By ADMIN";
	}

	void delete_by_admin()
	{
		cout << "\n\n\n Edit By delete";
	}

	void view_record_by_admin()
	{
		cout << "\n\n\n view By ADMIN";
	}

	void accountancy_admin()
	{
		cout << "\n\n\n accountantBy ADMIN";
	}

	void push_notification()
	{
		cout << "\n\n\n push By ADMIN";
	}
};

// Teachers Panel
class Teacher : public Register, public Edit
{
public:
	void  teacher_panel()
	{
	re_try_teacher:

		top_printing();
		setPointer(60, 17);
		cout << "Teacher Panel";
		setPointer(60, 19);
		cout << "1. Student Registeration";
		setPointer(60, 21);
		cout << "2. Edit";
		setPointer(60, 23);
		cout << "3. Delete";
		setPointer(60, 25);
		cout << "4. View";
		setPointer(60, 27);
		cout << "5. Attendance ";
		setPointer(60, 29);
		cout << "6. Marksheet";
		setPointer(60, 31);
		cout << "7. Push Notification";
		setPointer(60, 34);
		cout << "Press the number .... ";
		cin >> menu_select;
		fflush(stdin);
		switch (menu_select)
		{
		case 1:
			registeration(2);
			break;
		case 2:
			edit_main(2);
			break;
		case 3:
			delete_by_admin();
			break;
		case 4:
			view_record_by_admin();
			break;
		case 5:
			accountancy_admin();
			break;
		case 6:
			push_notification();
			break;
		default:
			setPointer(60, 36);
			cout << "Wrong Input!";
			setPointer(60, 38);
			cout << "Hit Enter to re-enter and esc to exit ....";
			retry_key = _getch();

			if (retry_key == 27)
			{
				exit(0);
			}
			else
			{
				goto  re_try_teacher;
			}
		}
	}

public:
	void edit_by_admin()
	{
	}

	void delete_by_admin()
	{
	}

	void view_record_by_admin()
	{
	}

	void accountancy_admin()
	{
	}

	void push_notification()
	{
	}
};

// Students Panel
class Student : public Register
{
public:
	void  student_panel()
	{
	re_try_student:

		top_printing();
		setPointer(60, 17);
		cout << "Student Panel";
		setPointer(60, 19);
		cout << "1. Update INFO";
		setPointer(60, 21);
		cout << "2. View Details";
		setPointer(60, 23);
		cout << "3. Bill";
		setPointer(60, 25);
		cout << "4. Marksheet";
		setPointer(60, 27);
		cout << "5. Attendance";
		setPointer(60, 29);
		cout << "6. Exam Schedule";
		setPointer(60, 31);
		cout << "7. Notice";
		setPointer(60, 33);
		cout << "8. Push Notification";
		setPointer(60, 35);
		cout << "Press the number .... ";
		cin >> menu_select;
		fflush(stdin);
		switch (menu_select)
		{
		case 1:
			registeration(2);
			break;
		case 2:
			edit_by_admin();
			break;
		case 3:
			delete_by_admin();
			break;
		case 4:
			view_record_by_admin();
			break;
		case 5:
			accountancy_admin();
			break;
		case 6:
			push_notification();
			break;
		default:
			setPointer(60, 38);
			cout << "Wrong Input!";
			setPointer(60, 40);
			cout << "Hit Enter to re-enter and esc to exit ....";
			retry_key = _getch();

			if (retry_key == 27)
			{
				exit(0);
			}
			else
			{
				goto  re_try_student;
			}
		}
	}

public:
	void edit_by_admin()
	{
	}

	void delete_by_admin()
	{
	}

	void view_record_by_admin()
	{
	}

	void accountancy_admin()
	{
	}

	void push_notification()
	{
	}
};

// Class for login 
class Login
{
	char username[20], password[10];
	char temp_username[20] = "alok";
	char temp_password[10] = "123";
	char temp_username2[20] = "ashish";
	char temp_password2[10] = "123";
	char temp_username3[20] = "haitomns";
	char temp_password3[10] = "123";

public:
	void login_input()
	{
	re_try_login:

		top_printing();

		setPointer(60, 17);
		cout << "LOGIN";
		setPointer(60, 20);
		cout << "Username : " << endl;
		setPointer(60, 22);
		cout << "Password : ";
		setPointer(71, 20);
		cin >> username;
		setPointer(71, 22);
		cin >> password;

		// Check if the user is valid or not 
		if (strcmp(temp_username, username) == 0 && strcmp(temp_password, password) == 0)
		{
			setPointer(60, 25);
			cout << "Success!";
			setPointer(60, 28);
			cout << "Proceeding to the panel ......";
			Sleep(1500);
			Admin adminUser;
			adminUser.admin_panel();
		}
		else if (strcmp(temp_username2, username) == 0 && strcmp(temp_password2, password) == 0)
		{
			setPointer(60, 25);
			cout << "Success!";
			setPointer(60, 28);
			cout << "Proceeding to the panel ......";
			Sleep(1500);
			Teacher teacherUser;
			teacherUser.teacher_panel();
		}
		else if (strcmp(temp_username3, username) == 0 && strcmp(temp_password3, password) == 0)
		{
			setPointer(60, 25);
			cout << "Success!";
			setPointer(60, 28);
			cout << "Proceeding to the panel ......";
			Sleep(1500);
			Student studentUser;
			studentUser.student_panel();
		}
		else
		{
			setPointer(60, 25);
			cout << "Fail!";
			if (login_fail <= 10)
			{
				setPointer(60, 28);
				cout << "Hit Esc to exit and Enter to retry ....";
				retry_key = _getch();
				if (retry_key == 27)
				{
					exit(0);
				}
				else
				{
					login_fail++;
					goto re_try_login;
				}
			}
			else
			{
				setPointer(60, 28);
				cout << "You are not a valid user !!!";
				system("PAUSE");
				exit(0);
			}
		}
	}
};

// Mani Function 
int main()
{
	fullscreen();
	system("COLOR 97");
	top_printing();

	setPointer(60, 20);
	cout << "WELCOME TO THE PROGRAM";
	setPointer(60, 21);
	cout << "Coded by : Haitomns";

	// Connection Between the program and the datab
	Sql mainSql;
	mainSql.sqlconnection();

	Login user;
	user.login_input();

	return 0;
	mainSql.sql_close();
}