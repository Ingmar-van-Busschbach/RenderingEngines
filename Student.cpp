//
// Created by Gebruiker on 9/25/2026.
//

#include "Student.h"

void Student::CopyStudent(const Student& student)
{
    this->name = student.GetName();
    this->studyID = student.GetID();
    this->modulesLen = student.GetModulesLen();
    this->modules = new std::string[modulesLen];
    for (int i = 0; i < modulesLen; i++)
    {
        this->modules[i] = student.GetModules()[i];
    }
}

Student::Student(std::string name, int studyID, int modulesLen)
{
    this->name = name;
    this->studyID = studyID;
    this->modulesLen = modulesLen;
    this->modules = new std::string[modulesLen];
}

Student::Student(const Student& student)
{
    this->studyID = 0;
    this->modulesLen = 0;
    this->modules = nullptr;
    Student::CopyStudent(student);
}

Student::~Student()
{
    delete[] modules;
}

void Student::UpdateName(std::string name)
{
    this->name = name;
}

void Student::AddModule(int moduleIndex, std::string moduleName)
{
    this->modules[moduleIndex] = moduleName;
}

std::string Student::GetName() const
{
    return this->name;
}

int Student::GetID() const
{
    return this->studyID;
}

int Student::GetModulesLen() const
{
    return this->modulesLen;
}

std::string* Student::GetModules() const
{
    return this->modules;
}

Student& Student::operator=(const Student& student)
{
    if (this != &student)
    {
        Student::CopyStudent(student);
    }
    return *this;
}
