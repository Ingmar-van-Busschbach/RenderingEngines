#include <iostream>
#include <vector>

#include "Student.h"

int main()
{
    auto* student = new Student("Ingmar", 588078, 3);
    student->AddModule(0, "Software Architecture");
    student->AddModule(1, "Rendering Engines");
    student->AddModule(2, "Game Systems");
    printf("%s \n", student->GetName().c_str());
    for (int i = 0; i < student->GetModulesLen(); i++)
    {
        printf("%s \n", student->GetModules()[i].c_str());
    }
    int studentCount = 5;
    Student* students[studentCount];
    for (int i = 0; i < studentCount; i++)
    {
        students[i] = new Student("Student " + std::to_string(i), 588070 + i, 3);
        students[i]->AddModule(0, "Software Architecture");
        students[i]->AddModule(1, "Rendering Engines");
        students[i]->AddModule(2, "Game Systems");
    }

    std::vector<Student*> studentVector;
    for(Student* studentEntry : students){
        studentVector.push_back(new Student(studentEntry));
    }
}
