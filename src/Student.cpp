#include "Student.hpp"

Student::Student(std::string last_name, std::string first_name, std::string city, Gender gender, std::string class_name):
    last_name_(last_name),
    first_name_(first_name),
    city_(city),
    gender_(gender),
    class_name_(class_name)
{

}

Student::~Student()
{
    //dtor
}
bool Student::operator==(const Student& other) const
{
    return last_name_ == other.last_name_ &&
           first_name_ == other.first_name_ &&
           city_ == other.city_ &&
           gender_ == other.gender_;

}
bool Student::operator!=(const Student& other)const
{
    return !(*this == other);
}
