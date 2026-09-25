//
// Created by Gebruiker on 9/25/2026.
//

#pragma once
#include <string>


struct Student
{
private:
    std::string name;
    int studyID;
    int modulesLen;
    std::string* modules;
    void CopyStudent(const Student& student);
public:
    Student(std::string name, int studyID, int modulesLen);
    Student(const Student& student);
    ~Student();
    void UpdateName(std::string name);
    void AddModule(int moduleIndex, std::string moduleName);
    std::string GetName() const;
    int GetID() const;
    int GetModulesLen() const;
    std::string* GetModules() const;
    Student& operator= (const Student& student);
};
