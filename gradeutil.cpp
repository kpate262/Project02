/*gradeutil.cpp*/

//
// <<Kisan Patel>>
// U. of Illinois, Chicago
// CS 341, Spring 2019
// Project #02: GradeUtil API
// 

#include <string>
#include <sstream>
#include <vector>
#include<iterator>
#include <regex>
#include <algorithm>

#include "gradeutil.h"

using namespace std;


//
// API:
//

// 
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S, U , W,Instructor
//   0      1     2       3    4 5 6 7 8 9 10 11 12  13 14
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior 
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
//
Course ParseCourse(string csvline)
{
    vector<string> parse(15);
    stringstream ss(csvline);
    
    for(string &s: parse){
        getline(ss, s, ',');
    }
  
    Course c(parse[0], parse[3], stoi(parse[1]), stoi(parse[2]), parse[14], stoi(parse[4]), 
            stoi(parse[5]), stoi(parse[6]), stoi(parse[7]), stoi(parse[8]), stoi(parse[9]),
             
            stoi(parse[11]), stoi(parse[12]), stoi(parse[13]), stoi(parse[10]));
    
  return c;
}


//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course, 
// department, or college.  For a course whose grading type 
// is defined as Course::Letter, the DFW rate is defined as
// 
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.  
//
// When computed for a dept or college, all courses of type 
// Course::Letter are considered in computing an overall DFW 
// rate for the dept or college.  The reference parameters 
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course& c, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
    
    if(c.getGradingType() != Course::Letter){
       return 0.0;
   }
  
  DFW = c.NumD +  c.NumF +  c.NumW;
  N =  c.NumA +  c.NumB +  c.NumC +  c.NumD +  c.NumF +  c.NumW;
   
  
  return (DFW*100.0/N);
}

double GetDFWRate(const Dept& dept, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  
  if(dept.Courses.empty() == true){
      DFW = N = 0;
      return 0.0;
  }
    
  for(const Course &c: dept.Courses){
      if(c.getGradingType() != Course::Letter){
          continue;
       }else{
          N += c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW;
          DFW += c.NumD + c.NumF + c.NumW;
      }
      
  }
  
  return (DFW*100.0/N);
}


double GetDFWRate(const College& college, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
   if(college.Depts.empty() == true){
        DFW = N = 0;
      return 0.0;
  }

    for( const Dept &d: college.Depts){
        for( const Course &c: d.Courses){
           if(c.getGradingType() != Course::Letter){
               continue;
           }else{
              N += c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW;
              DFW += c.NumD + c.NumF + c.NumW;
           }
        }
    }

  return (DFW*100.0/N);
}


//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B, 
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall 
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course& c)
{
    
    int n =  c.NumA +  c.NumB +  c.NumC +  c.NumD +  c.NumF;
   GradeStats g(n, c.NumA, c.NumB, c.NumC, c.NumD, c.NumF,
             ((c.NumA*100.0)/n), ((c.NumB*100.0)/n), ((c.NumC*100.0)/n), ((c.NumD*100.0)/n),
                ((c.NumF*100.0)/n) );
   if(c.getGradingType() != Course::Letter){
       return GradeStats();
    }
   
  return g;  
}

GradeStats GetGradeDistribution(const Dept& dept)
{
  if(dept.Courses.empty() == true){
      return GradeStats();
  }
  vector<int> percents(5);
  for(const int &i: percents)  {
      percents[i] = 0;
  }
    int n = 0;
    int index = 0;
  for(const Course &c: dept.Courses){
     if(c.getGradingType() != Course::Letter){
         index++;
         continue;
     }
        else{
             n +=  c.NumA +  c.NumB +  c.NumC +  c.NumD +  c.NumF;
             percents[0] += c.NumA;
             percents[1] += c.NumB;
             percents[2] += c.NumC;
             percents[3] += c.NumD;
             percents[4] += c.NumF;
        }
  }
    if(index+1 == dept.Courses.size()){
        return GradeStats();
    }
  
   GradeStats g(n, percents[0], percents[1], percents[2], percents[3], percents[4],
             ((percents[0]*100.0)/n), ((percents[1]*100.0)/n), ((percents[2]*100.0)/n), ((percents[3]*100.0)/n),
                ((percents[4]*100.0)/n) );
  return g;
}

GradeStats GetGradeDistribution(const College& college)
{
   if(college.Depts.empty() == true){
      return GradeStats();
  }
  vector<int> percents(5);
  for(const int &i: percents)  {
      percents[i] = 0;
  }
    int n = 0;
    int index = 0;
  for( const Dept &d: college.Depts){
      for(const Course &c: d.Courses){
         if(c.getGradingType() != Course::Letter){
             index++;
             continue;
         }
        else{
             n +=  c.NumA +  c.NumB +  c.NumC +  c.NumD +  c.NumF;
             percents[0] += c.NumA;
             percents[1] += c.NumB;
             percents[2] += c.NumC;
             percents[3] += c.NumD;
             percents[4] += c.NumF;
        }
      }
  }
    if(index+1 == college.Depts.size()){
        return GradeStats();
    }
    GradeStats g(n, percents[0], percents[1], percents[2], percents[3], percents[4],
             ((percents[0]*100.0)/n), ((percents[1]*100.0)/n), ((percents[2]*100.0)/n), ((percents[3]*100.0)/n),
                ((percents[4]*100.0)/n) );
  
  return g;
}


//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, and returned in
// ascending order by section number.
//
vector<Course> FindCourses(const Dept& dept, int courseNumber)
{
  vector<Course> courses;
  int index = 0;
  for(Course c: dept.Courses){
      if(c.Number == courseNumber){
          courses.push_back(c);
      }
  }
  
  if(courses.empty() == true){
      return courses;
  }
  else{
      if(courses.size() >= 1){
          sort(courses.begin(), courses.end(), 
               [](Course sectionA, Course sectionB){
                   if(sectionA.Section < sectionB.Section){
                       return true;
                   }
                   else{
                       return false;
                   }
               }

              );
      }
      
  }
    
  return courses;
}


//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If 
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in 
// ascending order by course number.  If two courses have the 
// same course number, they are given in ascending order by 
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept& dept, string instructorPrefix)
{
  vector<Course> courses;
  int index = 0;
  for(Course c: dept.Courses){
    string g = "(" + instructorPrefix + ")(.*)";
    regex r(g);
    
    if(regex_match(c.Instructor, r)){
        courses.push_back(c);
    }
   
  }
  
  if(courses.empty() == true){
      return courses;
  }
  else{
      if(courses.size() >= 1){
          sort(courses.begin(), courses.end(),
               [](Course numberA, Course numberB){
                   if(numberA.Number < numberB.Number){
                       return true;
                   }
                   
                   else if(numberA.Number == numberB.Number){
                       //sort(courses.begin(), courses.end(), //ascending order by section number
                          // [](Course sectionA, Course sectionB){
                               if(numberA.Section < numberB.Section){
                                   return true;
                               }
                               else{
                                   return false;
                               }
                           //
                        ///);//inner sort function
                   }
                    else{
                        return false;
                    }

               
              }
            );//outter sort function
      }
  }
    
  return courses;
}


//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, int courseNumber)
{
  vector<Course>  courses;
  
  for(const Dept &d: college.Depts){
      for(const Course &c: d.Courses){
          if(c.Number == courseNumber){
              courses.push_back(c);
          }
      }
  }
    
  if(courses.empty() == true){
      return courses;
  }
  else{
      if(courses.size() >= 1){
          sort(courses.begin(), courses.end(),
              [](Course s, Course h){
                  if(s.Dept < h.Dept){
                      return true;
                  }
                  else if(s.Dept == h.Dept){
                      if(s.Number > s.Number){
                          return false;
                      }
                      else{
                          return true;
                      }
                  }
                  else{
                      return false;
                  }
              }
              
              
              
              );
          
      }
  }  
  
    
  return courses;
}


//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by instructor, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, string instructorPrefix)
{
  vector<Course>  courses;
  
  for(const Dept &d: college.Depts){
      for(const Course &c: d.Courses){
            string g = "(" + instructorPrefix + ")(.*)";
            regex r(g);

            if(regex_match(c.Instructor, r)){
                courses.push_back(c);
            }
      }
  }
    
  if(courses.empty() == true){
      return courses;
  }
  else{
      if(courses.size() >= 1){
          sort(courses.begin(), courses.end(),
              [](Course s, Course h){
                  if(s.Instructor < h.Instructor){
                      return true;
                  }
                  else if(s.Instructor > h.Instructor){
                      return false;
                  }
                  else if(s.Instructor == h.Instructor){
                      if(s.Number < h.Number){
                          return true;
                      }
                      else if(s.Number == h.Number){
                          if(s.Section < h.Section){
                               return true;
                           }
                           else{
                               return false;
                           }
                      }
                      else{
                          return false;
                      }
                  }
                  else{
                      return false;
                  }
              }
                 
              );
          
      }
  }  
  
  return courses;
}
