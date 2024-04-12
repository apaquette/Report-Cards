/******************************************************************************
Final Assessment - Report Cards
Alexandre Paquette
Date: December 10, 2020
Class: CIS-142
Program Description: 
This program is designed to allow a user to enter up to 5 student records for 5 
given classes. The program can display all student records, a specific student
record, a student's average grade, or an average class grade. All functions are 
designed to only to accept valid entries.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>   //allows the use of system function
#include <string.h>   //allows the use of strcmp
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#elif __APPLE__
    #define CLEAR_COMMAND "clear"
#elif __linux__
    #define CLEAR_COMMAND "clear"
#else
    #error "Unsupported operating system"
#endif

#define BUFFER 100   //set max array size as a constant

/*This is the student structure which includes their firs and last names, 
their grades, and a check variable. The check variable is used to determine
how many entries there are in an array.*/
typedef struct Student{
  char firstName[BUFFER],lastName[BUFFER];
  int check;
  float grade [2][5];
}stnt;

int checkFloatInput (char*string){//ensures input only contains a float to get around atof glitch of ignoring trailing characters.
  int check = 0, pointCount = 0;
  for (int i=0;i<strlen(string);i++){
    if (!isdigit(string[i]) && string[i] != '.')
      check = 1;
    else if(string[i] == '.')
      pointCount++;
    if (pointCount >1)
      check = 1;
  }
  if (check == 1)
    return -1;
  else
    return 0;
}

int checkIntegerInput (char*string){//ensures input only contains numbers to get around atoi glitch of ignoring trailing characters
  int check = 0;
  for (int i=0;i<strlen(string);i++){
    if (!isdigit(string[i]))
      check = 1;
  }
  if (check == 1) //not an integer
    return -1;
  else
    return atoi(string);
}

/******************************************************************************
Handles getting input from the user as well as input validation
string - variable where the user's input will be stored
message - message to be displayed to the user (ie main menu)
checkValue - will determin what value will be checked for
  1 - Check for integer
  2 - Check for float
  0 - RAW string input, no check needed
minMaxValue - Max value to be checked for (ie 5 is the highest menu option in main menu)
*******************************************************************************/
void getInput(char *string,char*message,int checkValue, int minMaxValue[2]){//fgets stores a newline which messes with my later functions. This will remove the newline from the string
  int invalidInput;
  do{
    if (invalidInput == 1)
      printf("\a!!!Invalid Input!!!\n");
      
    invalidInput = 0;

    printf("%s",message);
    fgets(string, BUFFER,stdin);
    system(CLEAR_COMMAND);
    string[strlen(string)-1] = 0;     //removes the newline stored by fgets()

    switch(checkValue){               //switch statement to check input validity
      case 1:   //check for an integer
      if (checkIntegerInput(string)==-1 || atoi(string)<minMaxValue[0] || atoi(string) >minMaxValue[1])
        invalidInput = 1;
      break;
      case 2:   //check float
      if (checkFloatInput(string)==-1 || atoi(string)<minMaxValue[0] || atoi(string) >minMaxValue[1])
        invalidInput = 1;
      break;
      default:  //RAW string input, used for first and last names
      break;
    }
  }while(invalidInput != 0);
}

//This array is used to check for the next available open position using the check value of the student structure
int checkPosition(stnt *SR){//check the current position in the array
  int x = 0;

  while (SR[x].check == 1)
    x++;
  
  return x;
}

/******************************************************************************
Print Class Average function:
This function will calculate and print a requested class average. The average 
is calculated using floats to allow for decimals.
******************************************************************************/
void printClassAverage(stnt *SR,int course){//Print selected course average
  int pos = checkPosition(SR);
  float averageGrade = 0.0;
  char *className[5] = {"History","Science","Math","Geography","English"};

  printf("%s-%d Class Average: ",className[course],(int)SR[0].grade[0][course]);

  for(int x=0; x<pos; x++)
    averageGrade += SR[x].grade[1][course];
  
  averageGrade = averageGrade/(pos);

  printf("%0.2f%%\n\n",averageGrade);
}

/******************************************************************************
Print Student Average function:
This function will calculate and print a requested student average.
******************************************************************************/
void printStudentAverage(stnt *SR){//Print selected student average
  float averageGrade = (SR->grade[1][0] + SR->grade[1][1] + SR->grade[1][2] + SR->grade[1][3] + SR->grade[1][4])/5;

  printf("%s %s Class Average: %0.2f%%\n\n",SR->firstName,SR->lastName, averageGrade);
}

/******************************************************************************
Print Data function:
This function will print a specific entry from the Student  Records for the
user to view. This function is used in the main menu and in the createSR
function to allow the user to validate their entry.
******************************************************************************/
void printData(stnt *SR){
  printf("\nFirst Name:\t\t %s\nLast Name:\t\t %s\nHistory-131:\t %0.2f%%\nScience-576:\t %0.2f%%\nMath-313:\t\t %0.2f%%\nGeography-322:\t %0.2f%%\nEnglish-123:\t %0.2f%%\n\n", SR->firstName, SR->lastName, SR->grade[1][0], SR->grade[1][1], SR->grade[1][2], SR->grade[1][3], SR->grade[1][4]);
}

/******************************************************************************
Create Student Record function: 
This function will prompt the user for the 
students First and last names and ask for grades for each of the five 
classes (History, Science, Math, Geography, and English). For the grades, the
function ensures the user is only able to enter integers between 0-100. Data
is stored using the given address of the original Student Record array.
*******************************************************************************/
void createSR (stnt *SR){
  int minMaxValue[2];
  char select[BUFFER], *className[5] = {"History-","Science-","Math-","Geography-","English-"},*classCode[5] = {"131","576","313","322","123"},message[1000];
  char ptrEntry[BUFFER];

  do{//Enter student record main loop
    minMaxValue[0] = 0;
    minMaxValue[1] = 100;
    getInput(SR->firstName, "\nEnter the student's first name: ",0,0);
    getInput(SR->lastName, "\nEnter the student's last name: ", 0,0);
    
    for(int x=0; x<5; x++){//enter student Grades loop. Each grade corresponds to array positions 0-4.
      strcpy(message,"\nEnter the student's grades (Enter a value between 0-100):\n\nEnter a grade for ");
      strcat(message,className[x]);
      strcat(message,classCode[x]);
      strcat(message,": ");
      getInput(select, message, 2,minMaxValue);
      SR->grade[0][x] = atoi(classCode[x]);
      SR->grade[1][x] = atof(select);
    }
    printData(SR);
    strcpy(message,"Does the above look ok?\n1 - Yes\n2 - No\n\nSelect: ");
    minMaxValue[0] = 1;
    minMaxValue[1] = 2;
    getInput(select,message,1,minMaxValue);
  }while(atoi(select) != 1);    //end the loop if user says inputted data is ok (by selecting 1)

  SR->check = 1;
}

/******************************************************************************
Create Class Average function:
This function will list all classes and prompt the user to select a class to
list the average or return to the main menu. If a class is selected, Student
Records and the selection will be sent to the printClassAverage function.
*******************************************************************************/
void classAverage(stnt *SR){//Class Average Menu
  char select[BUFFER], *message = "Display a class average:\n1 - History-131\n2 - Science-576\n3 - Math-313\n4 - Geography-322\n5 - English-123\n6 - Return\n\nSelect: ";
  int minMaxValue[2] = {1,6};
  
  do{
      getInput(select,message,1,minMaxValue);
      if(atoi(select)!=6)
        printClassAverage(SR,(atoi(select)-1));
  }while(atoi(select) != 6);//End the loop if exit is selected
}

/******************************************************************************
Create Student Average function:
This function will list all available students from the Student Records array
as options to list their averages. The menu is dynamically created based on how
many students there are in the array at any given time. The user has the option
to either select a student to list their average or return to the main menu. If
a student is selected, their data is sent to the printStudentAverage function.
*******************************************************************************/
void studentAverage(stnt *SR){//Student Average Menu
  char select[BUFFER], message[BUFFER],num[10];
  int minMaxValue[2];
  int x=0;
  do{
    strcpy(message,"Display a student average:");
    for (x=0; SR[x].check == 1; x++){//this loop dynamically creates a menu from the Student Records entries
      sprintf(num, "\n%d", x+1);    //convert integer to string
      strcat(message,num);
      strcat(message," - ");
      strcat(message, SR[x].firstName);
      strcat(message," ");
      strcat(message, SR[x].lastName);
    }
    sprintf(num, "\n%d", x+1);    //convert integer to string
    strcat(message,num);
    strcat(message," - Return\n\nSelect: ");

    minMaxValue[0] = 1;
    minMaxValue[1] = x+1;

    getInput(select,message,1,minMaxValue);

    if(atoi(select)!=x+1)
      printStudentAverage(&SR[atoi(select)-1]);

  }while(atoi(select) != x+1);
}

/******************************************************************************
Main function: 
Includes the main menu with options for creating a student record, displaying 
all student records, displaying a class's average, and displaying a student's 
average. There is also an exit option to end the program.
*******************************************************************************/
int main(void) {
  char *select, *mainMenu = "Select one of the following options:\n1 - Create a student record\n2 - Display all student records\n3 - Display a class's average\n4 - Display a student's average\n5 - Exit\n\nSelect: ";
  int maxStudents = 5,pos, minMaxINT[2] = {1,5};
  stnt studentRecords[maxStudents];

  do{//main menu loop
    getInput(select, mainMenu, 1, minMaxINT);    //get and validate user input

    pos = checkPosition(studentRecords);  //check to see which free slot is available (if any)

    switch (atoi(select)){
      case 1:           //Create a student Report
      if (pos < maxStudents)
        createSR(&studentRecords[pos]);   //send the next available array slot to the Create Student Records function
      
      else
        printf("\n\a!!!The records are full!!!\n\n");
      break;

      case 2:           //Display all student records
      if (pos == 0)     //if no student records exit, and no records can be displayed.
        printf("\a\n!!!There are no entries!!!\n\n\r");
      
      else{
        printf("Current Entries: ");
        for (int x = 0; x < pos; ++x)
          printData(&studentRecords[x]);        //send each filled student record slot to the printData function
      }
      break;

      case 3:           //Display a class's average
      if (pos == 0)     //if no student records exist, no class average can be calculated
        printf("\a\n!!!There are no entries!!!\n\n\r");
      
      else
        classAverage(studentRecords);
      break;

      case 4:         //Display a student's average
      if (pos == 0)     //if no student records exist, no student average can be calculated
        printf("\a\n!!!There are no entries!!!\n\n\r");
      
      else
        studentAverage(studentRecords);         //send all student records to the studentAverage function
      break;

      case 5:                   //Exit program
      break;
    }
  }while(atoi(select) != 5);    //end the program if the user wants to exit (5)

  printf("\nExited Program");

  return 0;
}