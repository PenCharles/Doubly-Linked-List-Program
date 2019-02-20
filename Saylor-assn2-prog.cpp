//------------------------------------------------------------------------------
// CODE FILENAME: Saylor-assn2-prog.cpp
// DESCRIPTION: This program reads the integers from a file one at a time. Then 
//				the program determines if the integer is even or odd and places
//				the integer in the appropriate doubly linked list. Duplicates are 
//				ignored and they are displayed. After the entire file has been 
//				read into the program it will display the even and odd lists to
//				the user. Every integer inserted will be displayed along with a 
//				count of integers in each list. Then the program will open the
//				second file and read in those numbers to compare to the existing
//				even and odd double linked lists. The program will search the
//				lists to see if that particular integer being read from the second
//				file is in the even or odd lists. If the integer from the second
//				file is not found then the program will display a message to the 
//				user saying so. If the integer from the second file is found then 
//				a message will displayed to the user saying so and then program will 
//				list its position in the list.
// CLASS/TERM: CS372 Summer 8-Week 1;
// DESIGNER: James Saylor
// FUNCTIONS: InitializeList - creates memory for top pointer and intializes the elements
//			  EmptyList - returns true is list is empty
//			  NumInList - returns the node if the number is in the list
//			  OrderedListInsertion - inserts the integer into the right position in the list
//			  ListLength - returns the number of nodes inserted in the list
//			  PositionInList - returns the an integer that represents the node position in the list
//			  DisplayList - displays the nodes in each list
//			  DestroyList - destroys the links between all the nodes and deletes the node
//			  ErrorMessage - displays an error message to the user
//			  CheckCommandLine - returns true if the correct amount of files places in the line
//			  ReadAndInsert - reads the integers from the file and inserts them into nodes
//			  MemoryFull - returns true if memory is full
//			  SearchResults - displays the position of the node if in inserted into the list
//------------------------------------------------------------------------------  
#include <iostream>						// I/O purposes	
#include <iomanip>						// manipulating I/O 
#include <cstdlib>						// general utilities
#include <string>						// using string data types
#include <fstream>						// used to relate to data from files
#include <cstddef>						// defines the NULL function
using namespace std;

//global constants
const int DETERMINE_ODD_EVEN = 2; //this number is used to determine if the integer is odd or even

struct nodeRec
{
	int integer;		//holds the integer read in from the file
	nodeRec *next;		//points to the next node 
	nodeRec *previous;	//points to the previous node
};

struct listRec
{
	int count;		//holds the length of the list
	nodeRec *top;	//points to the top of the list
};

//function prototypes
listRec *InitializeList ();
bool EmptyList (listRec *list);
nodeRec *NumInList (listRec *list, int searchItem);
void OrderedListInsertion (bool& full, bool& insert, listRec *list, int insertItem);
int ListLength (listRec *list);
int PositionInList (listRec *list, int searchItem);
void DisplayList (listRec *list);
void DestroyList (listRec *list);
int ErrorMessage (string error);
bool CheckCommandLine (int argc);
void ReadAndInsert (ifstream &inFile1, bool &memoryFull, listRec *evens, listRec *odds);
void MemoryFull (bool memoryFull);	
void SearchResults (ifstream &inFile2, listRec *odds, listRec *evens);				

//------------------------------------------------------------------------------
// FUNCTION: Main
// DESCRIPTION: this function declares local variables, opens/closes files, calls 
//				other functions and displays messages to the user
// OUTPUT:
//		Returns: 0 when successful or 1 to indicate an error
// CALLS TO: CheckCommandLine, ErrorMessage, InitializeList, ReadAndInsert, 
//			 DisplayList, SearchResults, DestroyList
//------------------------------------------------------------------------------ 
int main (int argc, char *argv[])
{
	//local variables
	listRec *odds;				//pointer that points to odds doubly linked list
	listRec *evens;				//pointer that points to evens doubly linked list
	int integer;				//the number that is read in from the file
	bool memoryFull = false;	//if true then program displays message 
	ifstream inFile1 (argv[1]);	//stream variable associated with read in file
	ifstream inFile2 (argv[2]);	//stream variable associated with read in file
	
	if (CheckCommandLine (argc)) //check to see two filenames were given on the command line
	{
		return ErrorMessage("Not enough filenames were given on the command line!");
	}
	
	if (!inFile1.is_open() || !inFile2.is_open()) //check and see if both files exist and can be opened
	{
		return ErrorMessage("Could not open one of the files!");
	}
	
	odds = InitializeList (); //initialize both lists to be empty
	evens = InitializeList ();
	
	ReadAndInsert (inFile1, memoryFull, evens, odds); //read in integer and insert into list

	inFile1.close(); //close the read in file
	
	MemoryFull (memoryFull); //will display warning when memory runs out!
	
	cout << endl << endl << " Lists created using input file " << argv[1] << endl << endl;
	
	cout << "     " << odds->count << " integers inserted into ODD list:" << endl;
	DisplayList (odds);
	
	cout << "     " << evens->count << " integers inserted into EVEN list:" << endl;
	DisplayList (evens);
	
	cout << " Search results for input file " << argv[2] << endl << endl;
	
	SearchResults (inFile2, odds, evens); //display the search results from the second file
	
	DestroyList (odds); //destroy the lists
	DestroyList (evens);
	
 	cout << endl << endl;
 	return 0;
}
//function definitions
//------------------------------------------------------------------------------
// FUNCTION: SearchResults
// DESCRIPTION: this function reads in the integers from the second file and 
//				determines if they are odd or even. Then it checks to see if that
//				integer is in the applicable list. If the list is empty or the 
//				integer does not match any integer in the list than a message 
//				saying so is displayed to the user. If the integer is found in the
//				list then its position in the list is displayed to the user. Once
//				the last integer is read from the file then the program closes the
//				file
// INPUT: 
//		Parameters: inFile2 - this is the input stream variable that reads in from the file
//					odds - pointer to the top of the odds list
//					evens - pointer to the top of the evens list
// CALLS TO: NumInList, PositionInList, EmptyList
//------------------------------------------------------------------------------
void SearchResults (ifstream &inFile2, listRec *odds, listRec *evens)
{
	int oddOrEven; //holds 0 if even and >0 if odd
	int oddPos;	   //the position of the integer in the odd list
	int evenPos;   //the position of the integer in the even list
	int integer;   //the integer read in from the second file
	bool tempOdd; //if false then the odd list does not have the number in the list
	bool tempEven; //if false then the even list does not have the number in the list
	
	inFile2 >> integer; 
	while (inFile2) 
	{
		oddOrEven = integer % DETERMINE_ODD_EVEN; 
		if (oddOrEven > 0) 
		{
			tempOdd = NumInList (odds, integer); 
			if (!tempOdd)
			{
				if (EmptyList (odds))
				{
					cout << "     " << integer << " not found - odd list empty" << endl;
				}
				else
				{
					cout << "     " << integer << " not found in odd list." << endl;
				}
			}
			oddPos = PositionInList (odds, integer);
			if (tempOdd)
			{
				cout << "     " << integer <<" found " << oddPos << " numbers down in the odd list" << endl;
			}
		}
		else
		{
			tempEven = NumInList (evens, integer);
			if (!tempEven)
			{
				if (EmptyList (evens))
				{
					cout << "     " << integer << " not found - even list empty" << endl;
				}
				else
				{
					cout << "     " << integer << " not found in even list." << endl;
				}
			}
			evenPos = PositionInList (evens, integer);
			if (tempEven)
			{
				cout << "     " << integer <<" found " << evenPos << " numbers down in the even list" << endl;
			}
		}
		
		inFile2 >> integer;
	}
	inFile2.close();
}	
//------------------------------------------------------------------------------
// FUNCTION: MemoryFull
// DESCRIPTION: if the computers memory becomes full then the program displays the
//				message below
// INPUT: 
//		Parameters: memoryFull - if true then computer memory is full
//------------------------------------------------------------------------------
void MemoryFull (bool memoryFull)
{
	if (memoryFull)
	{
		cout << " Allocation of memory has failed. The program has stopped reading"
			 << " the file and inserting numbers into the list. You will proceed"
			 << " to the next part of the program." << endl << endl;
	}
}

//------------------------------------------------------------------------------
// FUNCTION: ReadAndInsert
// DESCRIPTION: reads in integers from the file and inserts them into the applicable
//				stack
// INPUT:
//		Parameters: inFile1 - stream var associated with the file
//					integer - number read in from the file
//					memoryFull - if true then computer memory is full
//					evens - even doubly linked list
//					odds - odd double linked list
//		File: the data from the file is a list of integers each seperated by a space
//			  23 43 32 5 998 843
// OUTPUT:
//		Parameters: memoryFull - variable returns true when computer memory is full
// CALLS TO: OrderedListInsertion
//------------------------------------------------------------------------------
void ReadAndInsert (ifstream &inFile1, bool &memoryFull, listRec *evens, listRec *odds)
{
	bool nodeInserted = true; //assigned to false if integer is a duplicate
	int oddOrEven;			  //holds the number that determines if the read in integer is odd or even
	int integer;			  //number read in from the file
	
	inFile1 >> integer; //read one integer at a time from the 1st input file
	
	cout << endl << " Doubly Linked List Program" << endl << endl;
	cout << " Reading Input" << endl;
	while (inFile1 && !memoryFull)
	{
		oddOrEven = integer % DETERMINE_ODD_EVEN; //determine which list integer belongs to
		 
		if (oddOrEven == 0) //if even
		{
			OrderedListInsertion (memoryFull, nodeInserted, evens, integer);
		}
		else 
		{
			OrderedListInsertion (memoryFull, nodeInserted, odds, integer);
		}
		if (!nodeInserted) //if integer is a duplicate it will be ignored
		{
			cout << "     " << integer << " is a duplicate and will be ignored." << endl;
		}
		nodeInserted = true;
		inFile1 >> integer;
	}
}

//------------------------------------------------------------------------------
// FUNCTION: CheckCommandLine
// DESCRIPTION: This function checks to see if the right amount of file name were
//				provided to the program
// INPUT:
//		Parameters: argc - the amount of files or arguments inserted by the user
// OUTPUT:
//		Parameters: Return Val: true if the 3 arguments given or false
//------------------------------------------------------------------------------
bool CheckCommandLine (int argc)
{
	return (argc != 3);
}

//------------------------------------------------------------------------------
// FUNCTION: InitializeList
// DSCRIPTION: this function intitalizes the top pointer of the even or odd list
// OUTPUT:
//		Return Val: list - the pointer to the top of the even or odd list
//------------------------------------------------------------------------------ 
listRec *InitializeList ()
{
	listRec *list = new (nothrow) listRec(); //new memory for list top node
	if (list == NULL)
	{
		cout << " Error!!! Memory is full" << endl << endl; 
	}
	list->count = 0;  //inititalize the elements in the node
	list->top = NULL;
	
	return list;
}

//------------------------------------------------------------------------------
// FUNCTION: EmptyList
// DESCRIPTION: checks to see if the list is empty
// INPUT:
//		Parameters: list - the pointer to the node that points to the list
// OUTPUT:
//		Return Val: true if the list is empty and false if list has something in it
//------------------------------------------------------------------------------
bool EmptyList (listRec *list)
{
	return (list->top == NULL);
}

//------------------------------------------------------------------------------
// FUNCTION: NumInList
// DESCRIPTION: this function searches the list for a particular integer and 
//				returns the node with the found integer or if not found a NULL node
// INPUT:
//		Parameters: list - pointer to the top of the odd or even list
//					serachItem - the integer that we are looking for in the list
// OUTPUT:
//		Return Val: current - the node that holds the found integer or NULL when not found
//------------------------------------------------------------------------------
nodeRec *NumInList (listRec *list, int searchItem)
{
	bool found = false;				//true when the integer is found
	nodeRec *current = list->top;	//assigns list top to a temp pointer
	
	while (current != NULL && !found) //loops while integer not found and current != NULL
	{
		if (current->integer == searchItem)
		{
			found = true;
		}
		else
		{
			current = current->next;	
		}
	}
	return current;
}

//------------------------------------------------------------------------------
// FUNCTION: OrderedListInsertion
// DESCRIPTION: this fucntion takes memory out for a new node and checks to see
//				and then assigns the new integer to the new node. Then it searches
//				through the list and inserts the node in the correct order of 
//				ascending integers. Once the list is inserted it increments the 
//				list count
// INPUT:
//		Parameters: full - if computer memory is full than it evaluates to true
//					insert - evaluates to true if integer is not a duplicate
//					list - pointer to the top of the list
//					insertItem - integer to be inserted into the node
// OUTPUT:
//		Paramters: full - if computer memory is full than it evaluates to true
//				   insert - evaluates to true if integer is not a duplicate
//------------------------------------------------------------------------------
void OrderedListInsertion (bool& full, bool& insert, listRec *list, int insertItem)
{
	nodeRec *current;		//points the current node in the list when traversing
	nodeRec *trailCurrent;	//points to the previous node in the list when traversing
	nodeRec *newNode;		//new node to be assigned and inserted
	bool found = false;		//true if duplicate integer found
	
	newNode = new (nothrow) nodeRec;
	if (newNode == NULL)
	{
		cout << " Heap error - could not allocate memory." << endl;
		full = true;
	}
	
	newNode->integer = insertItem;
	newNode->next = NULL;
	newNode->previous = NULL;
	
	if (list->top == NULL)//if list is empty
	{
		list->top = newNode;
		list->count++;
		insert = true;
	}
	else
	{
		current = list->top;
		while (current != NULL && insert)//check for duplicate number in list
		{
			//compare current integer wit insertInteger
			if(current->integer == insertItem)
			{
				insert = false;
			}
			else 
			{
				current = current->next;
			}
			if (!insert)
			{//make sure to delete this new node if we find a duplicate
				delete newNode;
			}
		}
		if (insert)//newNode has not been deleted
		{
			current = list->top;
			while (current != NULL && !found)//place number in correct position
			{
				if (current->integer >= insertItem)
				{
					found = true;
				}
				else
				{
					trailCurrent = current;
					current = current->next;
				}
			}
			if (current == list->top)//insert newNode before first
			{
				list->top->previous = newNode;
				newNode->next = list->top;
				list->top = newNode;
				list->count++;
			}
			else
			{
				if (current != NULL)//place node inbetween trailCurrent and current
				{
					trailCurrent->next= newNode;
					newNode->previous = trailCurrent;
					newNode->next = current;
					current->previous = newNode;
				}
				else//place node at end of list
				{
					newNode->next = NULL;
					newNode->previous = trailCurrent;
					trailCurrent->next = newNode;
				}
				list->count++;
			}
			
		}
	}
}

//------------------------------------------------------------------------------
// FUNCTION: ListLength
// DESCRIPTION: this function gets the length of the list from the top node
//				returns it to main
// INPUT:
//		Parameters: list - pointer to the top of the even or odd list
// OUTPUT:
//		Return Val: listCount - the length of the list
//------------------------------------------------------------------------------
int ListLength (listRec *list)
{
	int listCount; //the length of the list

	listCount = list->count;
	
	return listCount;
}

//------------------------------------------------------------------------------
// FUNCTION: PositionInList
// DESCRIPTION: calls NumInList to see if the integer is in the list and then
//				traverses the list to find the node and determine its position in
//				the list
// INPUT:
//		Parameters: list - pointer to the top of even or odd list
//					searchItem - integer that will be searched for in the list
// OUTPUT:
//		Return Val: count - the position of the integer in the list
// CALLS TO: nodeInList
//------------------------------------------------------------------------------
int PositionInList (listRec *list, int searchItem)
{
	nodeRec *nodeInList; //the 
	int count = 0;
	
	nodeInList = NumInList (list, searchItem);
	//if node in list
	if (nodeInList != NULL)
	{
		// if node is the first in the list then count needs to be assigned one
		count = 1;
		while (nodeInList->previous != NULL)
		{
			//use backward pointers to determine the position of node in list
			nodeInList = nodeInList->previous;
			count++;
		}
	}
	//when found return an integer that represents the position in the list or 0 if not found
	return count;
}

//------------------------------------------------------------------------------
// FUNCTION: DisplayList
// DESCRIPTION: displays the list to the user
// INPUT:
//		Parameters: list - the pointer to the top of the list
//------------------------------------------------------------------------------
void DisplayList (listRec *list)
{
	nodeRec *current = list->top; //assings the list top pointer to the temp pointer
	
	if (current == NULL) //if list is empty
	{
		cout << "          List is empty" << endl << endl;
	}
	cout << "          ";
	while (current != NULL) //while current has not reached the end of the list
	{
		for (int i = 1; i <= 10; i++) //for loop that displays 10 integers on a line
		{
			cout << current->integer << " ";
			current = current->next;
			if (current == NULL)
			{
				i = 11;
			}
		}
		cout << endl;
		cout << "          ";
	}
	cout << endl << endl;
}

//------------------------------------------------------------------------------
// FUNCTION: DestroyList
// DESCRIPTION: traverses the list and deletes each node
// INPUT:
//		Parameters: list - ponts to the top of the list
//------------------------------------------------------------------------------
void DestroyList (listRec *list)
{
	nodeRec* temp; //holds the node to be deleted
	
	while (list->top != NULL) //while list top still has nodes to be deleted
	{
		temp = list->top;
		list->top = list->top->next;
		delete temp;
	}

}

//------------------------------------------------------------------------------
// FUNCTION: ErrorMessage
// DESCRIPTION: displays error message 
// INPUT:
//		Parameters: error - message that designer wants to display to user
// OUTPUT:
//		Return val: 1 when error occurs
int ErrorMessage (string error)
{
	cout << " Error!! " << error;
	return 1;
}
