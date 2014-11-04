/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

void attendance();
void classroster();
void about();
void viewbydate();

int main()
{
  //local declarations
  char menu;

  //menu
  cout << endl
       << "               _            _    _ _    _ " << endl
       << "              (_)          | |  | | |  | |" << endl
       << "  _____      ___ _ __   ___| |  | | |__| |" << endl
       << " / __\\ \\ /\\ / / | '_ \\ / _ \\ |  | |  __  |" << endl
       << " \\__ \\\\ V  V /| | |_) |  __/ |__| | |  | |" << endl
       << " |___/ \\_/\\_/ |_| .__/ \\___|\\____/|_|  |_|" << endl
       << "----------------| |-----------------------" << endl
       << "                |_|                       " << endl << endl
       << "1) Begin Taking Attendance" << endl
       << "2) Input Class Roster" << endl
       << "3) View Roll By Date" << endl
       << "9) About" << endl
       << "0) Quit" << endl << endl
       << "Choose an option." << endl
       << ">";
  cin >> menu;

  switch (menu)
    {
    case '1':
      cout << endl << endl
	   << "Attendance started." << endl
	   << "To return to menu, enter 'm'." << endl;
      attendance();
      break;
    case '2':
      cout << endl << endl
	   << "Starting class roster input." << endl << endl;
      classroster();
      break;
    case '3':
      cout << endl << endl
	   << "Starting roll viewer." << endl << endl;
      viewbydate();
      break;
    case '9':
      about();
      break;
    case '0':
      break;
    default:
      cout << "Invalid input. Try again." << endl;
    }

  //program exit is buggy
  return 0;
}

void attendance()
{
  //local declarations
  char swipe[30];
  char id[7];
  char ch3[7];
  int x;
  int ver=0;
  int limit=0;

  //waiting for ID card swipe
  cout << endl << "Swipe ID Card." << endl;
  cin >> swipe;

  //if return to menu
  switch (swipe[0])
    {
    case 'm':
      main();
    case 'M':
      main();
    }

  //detect type of ID and extract ID# from card data
  //need to test that this works
  if (swipe[11] == ';') //myUH card type. Two leading zeroes on #.
    for(x=3; x<11; x++)
      id[x-3] = swipe[x];
  else if (swipe[9] == ';') //PeopleSoft card type. No leading zeroes on #.
    for(x=1; x<9; x++)
      id[x-1] = swipe[x];
  else
    {
      cout << "INVALID SWIPE. Is this a UH ID card?" << endl;
      attendance();
    }

  //get class roster file
  string line;
  ifstream classfile ("roster.txt");

  //search roster for matching ID#, import student name
  while(ver==0)
    {
      ++limit;
      getline(classfile,line); //grabs line by line
      for(x=0; x<7; x++) //checks for matching ID#
	{
	  if(id[x]!=line[x+1])
	    break;
	}
      if(x==7) //if preceding for loop completes, match found
	ver=1;
      if(limit>500) //fixes infinite loop if match not found. roster =< 500
	break;
    }

  //create string for roll file that includes date
  time_t rawtime;
  struct tm * timeinfo;
  char filename[19];
  time(&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (filename,20,"roll-%F.txt",timeinfo);

  //create or open roll file
  ofstream roll;
  roll.open (filename, ios::app); //opens at end of file
  
  //get local time
  time_t now = time(0);
  string timestamp = ctime(&now);

  //runs if match found from roster. Appends ID# and name to roll.
  if(ver==1 && limit<=500)
    {
      roll << line;
      if (line.length() < 40)
	for (x = 40 - line.length(); x>0; x--)
	  roll << ' ';
      //NOTE: name will break formatting if over 40 characters. No big deal.
      roll << timestamp; //return not needed

      cout << endl << " FOUND STUDENT:" << endl << line << endl << ' ' << timestamp;
    }
  
  //runs if no match found from roster. Appends only ID# to roll.
  //effectively limits roster size to 500 students. Can increase this.
  //if increasing max roster size, also change 'if' statement in 'while' loop above
 if(limit>500)
   {
     cout << endl << " STUDENT NOT FOUND:" << endl << ' ';
     roll << ' ';
     for(x=0; x<7; x++)
       {
	 roll << id[x];
	 cout << id[x];
       }
     roll << " UNKNOWN STUDENT";
     for(x=0; x<16; x++)
       roll << ' ';
     roll << timestamp; //return not needed
     cout << endl << ' ' << timestamp;
   }

  //close files
  classfile.close();
  roll.close();

  //repeat function
  attendance();
}

void classroster()
{
  //local declarations
  char id[7];
  string lastname;
  string firstname;
  int x;

  //input ID#, last name, and first name
  cout << "To return to menu, enter 'm'." << endl
       << "Enter PeopleSoft #:" << endl
       << ">";
  cin >> id;
  //if return to menu
  switch (id[0])
    {
    case 'm':
      main();
    case 'M':
      main();
    }
  cout << "Enter last name:" << endl
       << ">";
  cin >> lastname;
  cout << "Enter first name:" << endl
       << ">";
  cin >> firstname;
 
  //open roster file
  ofstream roster;
  roster.open("roster.txt", ios::app);
 
  //add line to roster file
  roster << ' ';
  for(x=0; x<7; x++)
    roster << id[x];
  roster << ' ' << lastname << ", " << firstname << '\n';

  //close roster file
  roster.close();

  cout << endl << "Student added to roster!" << endl;

  classroster();
}

void about() //optional function to list authors. might delete this.
{
  cout << endl << endl
       << "The swipeUH team:" << endl
       << "Cantu, Noe" << endl
       << "Chancellor, Christopher" << endl
       << "Dao, Tam" << endl
       << "Garcia, Jessica" << endl
       << "West, Alan" << endl;

  main();
}

void viewbydate() //need to add comments
{
  //local declarations
  string year;
  string month;
  string day;
  string filename;
  string line;
  char menu;

  //enter year, month, day
  cout << "Enter year (YYYY):" << endl << ">";
  cin >> year;
  cout << "Enter month (MM):" << endl << ">";
  cin >> month;
  cout << "Enter day (DD):" << endl << ">";
  cin >> day;

  //creat filename from previously entered data
  filename = "roll-" + year + "-" + month + "-" + day + ".txt";

  //open file
  ifstream file;
  file.open(filename.c_str());

  //error message if roll doesn't exist for that date
  if (file.fail())
    {
    cout << endl
	 <<"A roll file does not exist for that date. Please try again." << endl << endl;
    file.close();
    viewbydate();
    }

  //display roll
  while (file.good())
    {
      getline(file, line);
      cout << endl << line;
    }

  file.close();

  //menu to continue or return to main()
  cout << endl << "View roll from another date? (y/n)" << endl
       << ">";
  cin >> menu;
  switch (menu)
    {
    case 'y':
      viewbydate();
    case 'Y':
      viewbydate();
    case 'n':
      main();
    case 'N':
      main();
    default:
      cout << endl << "Invalid choice. Returning to menu." << endl;
      main();
    }
}
