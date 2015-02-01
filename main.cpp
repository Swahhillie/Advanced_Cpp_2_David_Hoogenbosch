#include <iostream>
#include <Student.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

Gender string_to_gender(const std::string& gender)
{
    if(gender == "m" || gender == "M")
    {
        return kMale;
    }
    else if(gender == "f" || gender == "F")
    {
        return kFemale;
    }
    else
    {
        return kOther;
    }

}
std::string gender_to_string(Gender gender)
{
    if(gender == kMale)return "M";
    else if(gender == kFemale)return "F";
    else return "";
}
std::ostream& operator<<(std::ostream& out, Gender g)
{
    out << gender_to_string(g);
    return out;
}

bool sort_students_by_name(const Student& a, const Student& b)
{
    if(a.last_name() != b.last_name())
        return a.last_name() < b.last_name();
    else// if(a.first_name() != b.first_name())
        return a.first_name() < b.first_name();
//    else if(a.city() != b.city())
//        return a.city() < b.city();
//    else if(a.gender() != b.gender())
//        return a.gender() < b.gender();
//    else if(a.class_name() != b.class_name())
//        return a.class_name() < b.class_name();
}
bool sort_students_by_gender(const Student& a, const Student& b)
{
    return a.gender() < b.gender();
}
bool sort_students_by_classname(const Student& a, const Student& b)
{
    return a.class_name() < b.class_name();
}
std::vector<Student> convert_to_students(std::istream& is)
{
    std::string line;
    std::string tokens [5];

    std::vector<Student> students;

    //read a whole line
    int linenum = 0;
    while(std::getline(is, line))
    {
        //a line must be in the following format: lastname,firstname,address,gender,classname
        std::istringstream iss(line);

        //if possible get five tokens from the line, tokens after the fifth are ignored.
        bool valid = true;
        for(int i = 0; i < 5 && valid; i++)
        {
            if(!std::getline(iss, tokens[i], ','))
            {
                //if the last token is missing that means that the class was a blank value
                //because there is no comma placed after a blank value if it is the last value of the line, we add a blank as we would do with normal missing values
                if(i == 4)tokens[4] = std::string();
                else
                {
                    std::cout << "bad line #" << linenum << " :" << line << " @token " << i << std::endl;
                    valid = false;
                }
            }

        }
        if(valid)
        {
            //TODO: Replace push_back with emplace_back when using a c++11 compiler for construction in place.
            students.push_back(Student(tokens[0], tokens[1], tokens[2], string_to_gender(tokens[3]), tokens[4]));
        }
        linenum++;
    }
    return students;
}

std::string convert_student_to_xml_string(const Student& student)
{
    std::stringstream format;
    format << "<Student last_name=\"" << student.last_name() <<
           "\" first_name=\"" << student.first_name() <<
           "\" city=\"" << student.city() <<
           "\" gender=\"" << student.gender() <<
           "\" class_name=\"" << student.class_name() << "\"/>";
    return format.str();

}
struct append_student
{
    append_student(std::ostream& a_out):
        out(a_out)
    {

    }
    void operator()(const Student& student)
    {
        out << "    " << convert_student_to_xml_string(student) << "\n";
    }
    std::ostream& out;
};
int main(int argc, const char** argv)
{
    if(argc != 3)
    {
        std::cout << "Invalid command line argument count\nUsage: \"<inputFilePath>\" \"<outputFilePath>\"" << std::endl;
        return 1;
    }

    std::ifstream student_input_csv(argv[1], std::ifstream::in);
    if(!student_input_csv)
    {
        std::cout << "failed to open input file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<Student> students = convert_to_students(student_input_csv);
    std::cout << "Converted "  << students.size() << " records to student objects" << std::endl;

    //TODO replace sort_student_by_name global function with a lambda when using a c++11 compiler.

    unsigned int students_before_remove_duplicates = students.size();
    std::sort(students.begin(), students.end(), sort_students_by_name);
    students.erase(std::unique(students.begin(), students.end()), students.end());

    std::cout << "Duplicates removed: " << students_before_remove_duplicates - students.size() << std::endl;

    //TODO replace sort_students_by_classname with lambda when using a c++11 compiler.
    std::stable_sort(students.begin(), students.end(), sort_students_by_classname);

    //write the students to the output file

    std::ofstream student_output_xml(argv[2], std::ofstream::out);

    if(!student_output_xml)
    {
        std::cout << "failed to open output file " <<argv[2] << std::endl;
        return 1;
    }
    {
        //mandatory use of for_each and function object.
        student_output_xml << "<Students>\n";
        for_each(students.begin(), students.end(), append_student(student_output_xml));
        student_output_xml << "</Students>\n";
    }

    //more reasonable code to do the same thing
//    student_output_xml << "<Students>\n";
//    for(unsigned int i = 0; i < students.size(); i++){
//        student_output_xml << "    " << convert_student_to_xml_string(students[i]) << "\n";
//    }
//    student_output_xml << "</Students>\n" << std::endl;

    //copy all male students to a different vector. for some reason
    std::vector<Student> female_students;
    //there is no copy_if in the old c++. only remove_copy_if (it was an accident)
    std::remove_copy_if(students.begin(), students.end(), std::back_inserter(female_students), Student::is_male);
    std::cout << "There are " << female_students.size() << " non-male students in the container" << std::endl;

    return 0;
}
