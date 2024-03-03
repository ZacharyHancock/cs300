/*
Name: BST.cpp
Author: Zachary Hancock
Description: Creates data structure and menu system to be able to load and print courses
*/

#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;


//Structure definition for course
struct Course {
	string courseId;
	string courseName;
	vector <string> prerequisites;

};

// Structure for the internals of the BST
struct Node {
	Course course;
	Node* left;
	Node* right;

	//Default Constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// Constructor initialized with a Course object
	Node(Course newCourse) : Node(){
		course = newCourse;
	}
};


// BST class definition

class BST {

	//private members 
private:
	Node* root;

	void addNode(Node* node, Course course);

public:
	BST();
	void InOrder();
	void sortedOrder(Node* nodePtr);
	void insert(Course course);
	Course search(string courseId);

};

// Default constructor
BST::BST()
{
	root = nullptr;
}

// Private functions
void BST::addNode(Node* node, Course course)
{
	// Initialize a node pointer to track the current node equal to root
	Node* currentNode = root;

	// while loop iterates over the tree until the current node pointer equals nullptr
	while (currentNode != nullptr) {
		if (course.courseId < currentNode->course.courseId) {
			if (currentNode->left == nullptr) {
				currentNode->left = node;
				currentNode = nullptr;
			}
			else {
				currentNode = currentNode->left;
			}
		}
		else {
			if (currentNode->right == nullptr) {
				currentNode->right = node;
				currentNode = nullptr;
			}
			else {
				currentNode = currentNode->right;
			}
		}
	}
	node->left = nullptr;
	node->right = nullptr;
}


// public functions
void displayCourse(Course course) {
	cout << course.courseId << ", " << course.courseName << endl;
}

void BST::sortedOrder(Node* nodePtr)
{
	if (nodePtr != nullptr)
	{
		sortedOrder(nodePtr->left);
		displayCourse(nodePtr->course);
		sortedOrder(nodePtr->right);
	}
}

void BST::InOrder() {
	//calls sortedOrder with root pointer
	sortedOrder(root);
}

void BST::insert(Course course)
{
	Node* newNode = new Node(course); // new pointer node with course object to add to BST
	if (root == nullptr) // if root is empty the BST is empty so set root equal to newNode
	{

		root = newNode;
		newNode->left = nullptr;
		newNode->right = nullptr;

	}
	else   // Else adds node if root isnt empty and calls addNode that adds the node accordingly
		addNode(newNode, course); 
}

Course BST::search(string courseId)
{
	Node* currentNode = root;

	while (currentNode != nullptr) {        //traverse tree
		if (currentNode->course.courseId == courseId)
			return currentNode->course;
		else if (courseId < currentNode->course.courseId)  //traverse left if smaller
			currentNode = currentNode->left;
		else                                       //traverse right if bigger
			currentNode = currentNode->right;
	}
	Course course;
	return course;
}




void loadCourses(string filePath, BST* bst) {
	fstream newfile;				// new fstream object and opens inputted filePath
	newfile.open(filePath, ios::in);
	if (newfile.is_open()) {		// Checks to see if file is open
		string tempInput;	//Temporary string to hold each line
		while (getline(newfile, tempInput)) {
			//cout << tempInput; //test to see if line read in 
			vector<string> splitString;		// SplitString, s, and ss used to split each line using the delimiter of a comma
			string s;
			stringstream ss(tempInput);
			while (getline(ss, s, ',')) {	// splits the string and adds each token onto the end of splitString
				splitString.push_back(s);
				//cout << s << endl;				// Testing to see if line was split
			}
			if (splitString.size() > 2) {	// If-Else branch sees if there are prerequisites to branch if course has prerequisites to add to new course
				vector<string> prereqs(splitString.begin() + 2, splitString.end());  // Intializes prereq vector equalt to all the indices of splitString for the prerequisites that start at index 2
				Course course;
				course.courseId = splitString[0]; 
				course.courseName = splitString[1];
				course.prerequisites = prereqs;
				bst->insert(course);
			}
			else {
				Course course;
				course.courseId = splitString[0];
				course.courseName = splitString[1];
				bst->insert(course);
			}
		}
	}
}


int main(int argc, char* argv[]) {

	// Define and initialize binary search tree to hold all the courses
	BST* bst;
	bst = new BST();

	// Define a course object to be used for course search 
	// and a string input to be compated for the while loop and cases
	Course course;
	string input;

	int choice = 0;
	while (choice != 9) {
		cout << "Welcome to the course planner." << endl;		// prints out course menu
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl;
		cout << "What would you like to do? ";
		cin >> choice;

		switch (choice) {

		case 1:
			//Calls loadCourses to get the data from the txt file read into the data structure
			loadCourses("courseInformation.txt", bst);
			cout << endl;
			break;
		case 2:
			cout << endl;
			bst->InOrder();		// Calls InOrder to print out sorted order of courses
			cout << endl;
			break;
		case 3:
			cout << endl << "What course would you like to know about?";		// Prompt user for input
			cin >> input;
			transform(input.begin(), input.end(), input.begin(), toupper);		// Uses transform function to eliminate a Upper/Lower case specific issue for the users input
			course = bst->search(input);		

			// If/Else branch sees checks if searched course object is empty or not
			if (!course.courseId.empty()) {
				displayCourse(course);				// Calls displayCourse and if branch checks if its has prerequisites to print out
				if (!course.prerequisites.empty()) {
					cout << "Prerequisites: ";
					for (int i = 0; i < course.prerequisites.size(); i++) {		// For loop iterates over prerequisites
						if (course.prerequisites[i] != course.prerequisites.back())		// If/else branch sees if the prerequisite is the last one to avoid prinint a comman after the last entry
							cout << course.prerequisites[i] << ", ";
						else
							cout << course.prerequisites[i];
					}
					cout << endl;
				}
			}
			else {		// Else branch if course not found
				cout << "Course Id " << input << " not found." << endl;
			}
			cout << endl;
			break;
		}

		
	}
	cout << "Thank you for using the course planner." << endl;

	return 0;
}