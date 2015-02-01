#ifndef STUDENT_H
#define STUDENT_H

#include <string>

enum Gender{
    kMale,
    kFemale,
    kOther
};

class Student
{
    public:
        Student(std::string last_name, std::string first_name, std::string city, Gender gender, std::string class_name);
        virtual ~Student();

        const std::string& last_name()const{return last_name_;};
        const std::string& first_name()const{return first_name_;};
        Gender gender()const{return gender_;};
        const std::string& city()const{return city_;};
        const std::string& class_name()const{return class_name_;};

        //class_name of a student object will not count towards equality due to "problems with multiple registrations when a students switches class"
        bool operator==(const Student& other)const;
        bool operator!=(const Student& other)const;


    private:

        std::string last_name_;
        std::string first_name_;
        std::string city_;
        Gender gender_;
        std::string class_name_;
};

#endif // STUDENT_H
