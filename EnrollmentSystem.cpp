#pragma once

#include "EnrollmentSystem.h"
#include "Utilities.h"
#include "University.h"
#include "College.h"
#include "Course.h"
#include "CourseUnit.h"
#include "Date.h"
#include "Student.h"
#include "CourseUnitClass.h"
#include "Tutor.h"


EnrollmentSystem::EnrollmentSystem(unsigned int mc) : MAXIMUM_CREDITS(mc)
{
	//LOAD DEFAULT SORTING OPTIONS
	courseSortOption = &compareCourseByName;
	courseUnitSortOption = &compareCourseUnitByTime;
	courseUnitClassSortOption = &compareCourseUnitClassByNumber;
	studentsSortOption = &compareStudentByID;
	professorsSortOption = &compareProfessorByID;
	universitySortOption = &compareUniversityByName;
	collegeSortOption = &compareCollegeByName;
}

void EnrollmentSystem::loadUniversities() {
	string line;
	ifstream file;

	file.open(unifile);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			string name, acr, country;
			char delimiter;
			istringstream iss(line);

			getline(iss, name, delimiter);
			getline(iss, acr, delimiter);
			getline(iss, country, '\n');

			addUniversity(new University(name, acr, country));
		}
		file.close();
	}
}

void EnrollmentSystem::loadColleges(){
	ifstream file;
	string line, uni, name, acr;
	char ch;

	file.open(collegefile);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			istringstream iss(line);
			getline(iss, uni, ch);
			bool exists = false;
			vector<University*>::iterator it;
			for (it = universitiesVector.begin(); it != universitiesVector.end(); it++) {
				if ((*it)->getAcronym() == uni)
					exists = true;
			}
			if (exists) {
				getline(iss, name, ch);
				getline(iss, acr, '\n');
				new College(name, acr, (*it));
			}
		}
		file.close();
	}
}

void EnrollmentSystem::loadCourses() {
	ifstream file;
	string line, uni, col, name, acr;
	char ch;
	University *univ;

	file.open(collegefile);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			istringstream iss(line);
			getline(iss, uni, ch);
			bool existsuni = false;
			vector<University*>::iterator ituni;
			for (ituni = universitiesVector.begin(); ituni != universitiesVector.end(); ituni++) {
				if ((*ituni)->getAcronym() == uni)
					existsuni = true;
			}
			if (existsuni) {
				getline(iss, col, ch);
				bool existscol = false;
				vector<College*> collegesvec = (*ituni)->getColleges();
				vector<College*>::iterator itcol;
				for (itcol = collegesvec.begin(); itcol != collegesvec.end(); itcol++) {
					if ((*itcol)->getAcronym() == col)
						existscol = true;
				}
				if (existscol) {
					getline(iss, name, ch);
					getline(iss, acr, '\n');
					new Course(name, acr, (*itcol));
				}
			}
		}
		file.close();
	}
}

University* getUniversity(EnrollmentSystem &s)
{
	if (s.universitiesVector.size() == 0)
		throw NotFound<University*, EnrollmentSystem>(s);

	int answer = -1;

	while (answer <= 0 || answer > s.universitiesVector.size()) {
		system("CLS");
		for (size_t i = 0; i < s.universitiesVector.size(); i++) //SHOW AS MENU
		{
			cout << (i + 1) << ". ";
			cout << s.universitiesVector[i]->getName();
		}
		answer = enterInput<int>(); //READ SELECTION
	}

	answer--;

	return s.universitiesVector[answer];
}

College* getCollege(EnrollmentSystem &s)
{
	University* university = getUniversity(s);

	int answer_2 = -1;

	if (university->getColleges().size() == 0)
		throw NotFound<College*, University*>(university);

	while (answer_2 <= 0 || answer_2 > university->getColleges().size()) {
		system("CLS");
		for (size_t i = 0; i < university->getColleges().size(); i++) //SHOW AS MENU
		{
			cout << (i + 1) << ". ";
			cout << university->getColleges()[i]->getName();
		}
		answer_2 = enterInput<int>(); //READ SELECTION
	}

	answer_2--;

	return university->getColleges()[answer_2];
}

Course* getCourse(EnrollmentSystem &s)
{
	College* college = getCollege(s);

	int answer_3 = -1;

	if (college->getCourses().size() == 0)
		throw NotFound<Course*, College*>(college);

	while (answer_3 <= 0 || answer_3 > college->getCourses().size()) {
		system("CLS");
		for (size_t i = 0; i < college->getCourses().size(); i++) //SHOW AS MENU
		{
			cout << (i + 1) << ". ";
			cout << college->getCourses()[i]->getName();
		}
		answer_3 = enterInput<int>(); //READ SELECTION
	}

	answer_3--;

	return college->getCourses()[answer_3];
}

CourseUnit* getCourseUnit(EnrollmentSystem &s)
{
	Course* course = getCourse(s);

	int answer_4 = -1;

	if (course->getCourseUnits().size() == 0)
		throw NotFound<CourseUnit*, Course*>(course);

	while (answer_4 <= 0 || answer_4 > course->getCourseUnits().size()) {
		system("CLS");
		for (size_t i = 0; i < course->getCourseUnits().size(); i++) //SHOW AS MENU
		{
			cout << (i + 1) << ". ";
			cout << course->getCourseUnits()[i]->getName();
		}
		answer_4 = enterInput<int>(); //READ SELECTION
	}

	answer_4--;

	return course->getCourseUnits()[answer_4];
}

Student* EnrollmentSystem::getStudent(unsigned long long int &ID)
{
	vector<University*>::iterator unIt;
	for (unIt = universitiesVector.begin();
		unIt != universitiesVector.end();
		unIt++
		)
	{
		vector<College*>::iterator colIt;
		for (colIt = (*unIt)->getColleges().begin();
			colIt != (*unIt)->getColleges().end();
			colIt++
			)
		{
			vector<Course*>::iterator courseIt;
			for (courseIt = (*colIt)->getCourses().begin();
				courseIt != (*colIt)->getCourses().end();
				courseIt++
				)
			{
				vector<Student*>::iterator sIt; 
				for (sIt = (*courseIt)->getStudents().begin();
					sIt != (*courseIt)->getStudents().end();
					sIt++
					)
				{
					if ((*sIt)->getID() == ID)
						return (*sIt);

				}
			}
		}
	}
	throw NotFound<Student*, unsigned long long int>(ID);
}

void EnrollmentSystem::removeUniversity(University* u)
{
	vector<University*>::iterator it;
	it = find(universitiesVector.begin(), universitiesVector.end(), u);
	if (it != universitiesVector.end()) {
		universitiesVector.erase(it);
		return;
	}
	else
		throw NotFound<University*, string>(u->getName());

}

bool addStudentHandler(EnrollmentSystem& s)
{
	Student* student;
	Date* dateOfBirth;
	Course* course;
	string studentName;

	try
	{
		studentName = enterString("\nAdd Student\n\n", "Enter the name of the student [CTRL+Z to cancel] : ");
		while (true)
		{
			dateOfBirth = new Date(enterString("\nAdd Student\n\n", "Enter the date of birth (DD-MM-YYYY) [CTRL+Z to cancel] : "));
			if (dateOfBirth->getValid())
				break;
			cout << "\nInvalid Date!\n";
			system("PAUSE");
		}
	}
	catch (EndOfFile &eof)
	{
		cout << "\nAddition canceled!\n";
		system("pause");
		return false;
	}
	
	try {
		course = getCourse(s); //GET THE COURSE FROM THE USER
	}
	catch (EndOfFile &eof)
	{
		cout << "\nAddition canceled!\n";
		return false;
	}
	catch (NotFound<University*, EnrollmentSystem> &nfu)
	{
		cout << "There are no universities in the system\n";
		return false;
	}
	catch (NotFound<College*, University*> &nfc)
	{
		cout << "There are no colleges in: " << nfc.getMember()->getName() << endl;
		return false;
	}
	catch (NotFound<Course*, College*> &nfco)
	{
		cout << "There are no courses in: " << nfco.getMember()->getName() << endl;
		return false;
	}
	
	student = new Student(studentName, *dateOfBirth, course);
	
	course->addStudent(student);

	return true;
}

bool removeStudentHandler(EnrollmentSystem& s)
{
	Student* student;
	unsigned long long int ID;

	try
	{
		ID = enterInput<unsigned long long int>("\nRemove Student\n\n", "Enter the ID of the student [CTRL+Z to cancel] : ");
		student = s.getStudent(ID);
	}
	catch (EndOfFile &eof)
	{
		cout << "\nAddition canceled!\n";
		return false;
	}
	catch (NotFound<Student*, unsigned long long int> &nfs)
	{
		cout << "Student " << nfs.getMember() << " not found!\n";
		return false;
	}
	
	student->getCourse()->removeStudent(student);

	return true;
}

bool removeUniversityHandler(EnrollmentSystem& s) {
	University* uni;
	string name;

	try
	{
		cout << "\nRemove University\n\n";
		uni = getUniversity(s);
		s.removeUniversity(uni);
	}
	catch (EndOfFile &eof)
	{
		cout << "\Remove canceled!\n";
		return false;
	}
	catch (NotFound<University*, string> &nfu)
	{
		cout << "University " << nfu.getMember() << " not found!\n";
		return false;
	}

	cout << "University Erased";

	return true;
}

bool enrollmentHandler(EnrollmentSystem& s)
{
	Student* student;
	unsigned long long int ID;

	try
	{
		ID = enterInput<unsigned long long int>("\nEnrollment System\n\n", "Enter the ID of the student [CTRL+Z to cancel] : ");
		student = s.getStudent(ID);
	}
	catch (EndOfFile &eof)
	{
		cout << "\nAddition canceled!\n";
		return false;
	}
	catch (NotFound<Student*, unsigned long long int> &nfs)
	{
		cout << "Student " << nfs.getMember() << " not found!\n";
		return false;
	}
	
		vector<CourseUnit*> courseUnitsToShow = student->getCourse()->getCourseUnitsNotCompleted(student, student->getYear());
		unsigned short int answer = 0;
		try 
		{
			while (answer != (courseUnitsToShow.size() + 1) || courseUnitsToShow.size() == 0) //EXIT
			{
				system("CLS");
				for (size_t i = 0; i < courseUnitsToShow.size(); i++) //SHOW AS MENU
				{
					cout << (i + 1) << ". ";
					courseUnitsToShow[i]->show();
				}
				answer = enterInput<unsigned short int>(); //READ SELECTION
				if (answer >= 1 && answer <= courseUnitsToShow.size()) 
				{
					if (student->getCredits() + courseUnitsToShow[answer - 1]->getCredits() <= s.getMaxCredits())
					{
						student->enrollCourseUnit(courseUnitsToShow[answer - 1]);
						courseUnitsToShow = student->getCourse()->getCourseUnitsNotCompleted(student, student->getYear()); //REFRESH MENU
						student->setCredits(student->getCredits() + courseUnitsToShow[answer - 1]->getCredits());
					}
					else cout << "Student cannot enroll this course unit. Maximum credits have been exceeded";
				}
			}
		}
		catch (EndOfFile &eof)
		{
			cout << "Enrollment Canceled!\n";
			return false;
		}
	
	return courseUnitsToShow.size() == 0;
}

bool studentFinishedCourseUnitHandler(EnrollmentSystem& s) 
{
	Student* student;
	unsigned long long int ID;
	CourseUnit* courseUnit;
	unsigned short int grade;

	try
	{
		ID = enterInput<unsigned long long int>("\nFinish Course Unit\n\n", "Enter the ID of the student [CTRL+Z to cancel] : ");
		student = s.getStudent(ID);
 		grade = enterInput<unsigned short int>("\nFinish Course Unit\n\n", "Enter the grade of the student to this course unit [CTRL+Z to cancel] : ");
		student = s.getStudent(ID);
		courseUnit = getCourseUnit(s);
	}
	catch (EndOfFile &eof)
	{
		cout << "\nAddition canceled!\n";
		return false;
	}
	catch (NotFound<Student*, unsigned long long int> &nfs)
	{
		cout << "Student " << nfs.getMember() << " not found!\n";
		return false;
	}
	catch (NotFound<University*, EnrollmentSystem> &nfu)
	{
		cout << "There are no universities in the system\n";
		return false;
	}
	catch (NotFound<College*, University*> &nfc)
	{
		cout << "There are no colleges in: " << nfc.getMember()->getName() << endl;
		return false;
	}
	catch (NotFound<Course*, College*> &nfco)
	{
		cout << "There are no courses in: " << nfco.getMember()->getName() << endl;
		return false;
	}
	catch (NotFound<CourseUnit*, Course*> &nfcu)
	{
		cout << "There are no course units in: " << nfcu.getMember()->getName() << endl;
		return false;
	}
	

	student->completedClass(courseUnit, grade); 
	CourseUnitClass* courseUnitClass = student->getClassesCurrentlyAtending().at(courseUnit);
	courseUnitClass->removeStudent(student); 
	courseUnit->removeStudent(student);
	
	//CHECK IF STUDENT HAS COMPLETED ALL THE COURSE UNITS OF THE CURRENT YEAR
	if (student->completedAllCourseUnits(student->getYear()))
	{
		//GET READY FOR NEXT YEAR
		student->setCredits(0);
		student->setYear(student->getYear() + 1);
	}

	return true;
}

bool EnrollmentSystem::changeCourseUnitSortOption(unsigned short int &option)
{
	vector<bool(*)(CourseUnit*, CourseUnit*)> options = {
		compareCourseUnitByName,
		compareCourseUnitByNumberStudents,
		compareCourseUnitByTime
	};

	size_t minimum = 1, maximum = options.size() + 1;

	if (option >= minimum && option <= (maximum - 1)) {
		courseUnitSortOption = options[option - 1]; //CALL THE CORRESPONDENT FUNCTION
		return true;
	}
	else if (option == maximum) //EXIT
		return true;
	else return false; //CALL THE FUNCTION AGAIN
}

bool EnrollmentSystem::changeCourseUnitClassSortOption(unsigned short int &option)
{
	vector<bool(*)(CourseUnitClass*, CourseUnitClass*)> options = {
		compareCourseUnitClassByNumber,
		compareCourseUnitClassByNumberStudents
	};

	size_t minimum = 1, maximum = options.size() + 1;

	if (option >= minimum && option <= (maximum - 1)) {
		courseUnitClassSortOption = options[option - 1]; //CALL THE CORRESPONDENT FUNCTION
		return true;
	}
	else if (option == maximum) //EXIT
		return true;
	else return false; //CALL THE FUNCTION AGAIN
}

bool EnrollmentSystem::changeStudentsSortOption(unsigned short int &option)
{
	vector<bool(*)(Student*, Student*)> options = {
		compareStudentByID,
		compareStudentByName,
		compareStudentByBirth,
		compareStudentByCouseYear,
	};

	size_t minimum = 1, maximum = options.size() + 1;

	if (option >= minimum && option <= (maximum - 1)) {
		studentsSortOption = options[option - 1]; //CALL THE CORRESPONDENT FUNCTION
		return true;
	}
	else if (option == maximum) //EXIT
		return true;
	else return false; //CALL THE FUNCTION AGAIN
}

bool EnrollmentSystem::changeProfessorsSortOption(unsigned short int &option)
{
	vector<bool(*)(Tutor*, Tutor*)> options = {
		compareProfessorByID,
		compareProfessorByName,
		compareProfessorByBirth
	};

	size_t minimum = 1, maximum = options.size() + 1;

	if (option >= minimum && option <= (maximum - 1)) {
		professorsSortOption = options[option - 1]; //CALL THE CORRESPONDENT FUNCTION
		return true;
	}
	else if (option == maximum) //EXIT
		return true;
	else return false; //CALL THE FUNCTION AGAIN
}
