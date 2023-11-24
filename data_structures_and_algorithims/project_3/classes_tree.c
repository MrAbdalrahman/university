// P3_1211753_abdulrahmanshahen_section3 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// struct for students that includes all of their students
struct student {
	struct student* left;
	struct student* right;
	int classId;
	int studentId;
	char* address;
	char* name;
	char* date;
};

int index = 0;


/// createNode method creates a new node and saves data as the data for the node

/// <param name="data"> the data that have to be saved in the new node</param>
/// <returns> a new allocated node in the memory </returns>
struct student* createNode(int classId, int studentId, char* address, char* name, char* date) {

	struct student* newNode;
	newNode = (struct student*)malloc(sizeof(struct student));

	if (newNode == NULL) {
		printf("memory is full\n");
		return NULL;
	}

	newNode->left = NULL;
	newNode->right = NULL;
	newNode->classId = classId;
	newNode->studentId = studentId;
	newNode->address = strdup(address);
	newNode->name = strdup( name);
	newNode->date = strdup(date) ;
	return newNode;

}


// a function that delete the whole tree and returns NULL after deleting it
 struct student*  disposeTree(struct student* tree)
{
	if (tree != NULL)
	{
		disposeTree(tree->left);
		disposeTree(tree->right);
		free(tree);
	}
	return NULL;
}

// a function to find the student  by id
struct student* find(struct student* tree, int studentId) {

	if (tree == NULL)
		return NULL;

	else if (studentId < tree->studentId)
		return find(tree->left, studentId);

	else if (studentId > tree->studentId)
		return find(tree->right, studentId);

	return tree;
}

 // a function that find the minimum node in the tree 
// (its used here just fin dleting node procces)
struct student* findMin(struct student* tree) {
	if (tree == NULL)
		return NULL;

	if (tree->left == NULL)
		return tree;
	else
		return findMin(tree->left);
}

// a function to insert a node into the BS tree
struct student* insert(struct student* tree, int classId, int studentId, char* address, char* name, char* date) {
	if (tree == NULL) {
		tree = createNode(classId, studentId, address, name, date);
		return tree;
	}
	else if (studentId < tree->studentId)
		tree->left = insert(tree->left, classId, studentId, address, name, date);
	else if (studentId > tree->studentId)
		tree->right = insert(tree->right, classId, studentId, address, name, date);
	return tree;

}

// a function to update student's data but not id
void update(struct student* tree, int classId, int studentId, char* address, char* name, char* date) {
	struct student* temp = find(tree, studentId);
	if (temp == NULL)
		return NULL;
	if (classId > 0)
		temp->classId = classId;
	if (strcmp(address,"n") != 0)
		temp->address = strdup(address);
	if (strcmp(name ,"n") != 0)
		temp->name = strdup(name);
	if (strcmp(date, "n") != 0)
		temp->date = strdup(date);
	 }

// a function to get the whole student's names
void getNames(struct student* tree,char** names) { // in
	if (tree != NULL) {
		getNames(tree->left,names);
		names[index++] = tree->name;
		getNames(tree->right,names);
	}
}

// a function to delete the student with the given id
struct student* delete (struct student* tree, int studentId) {
	if (tree == NULL)
		return NULL;
	else if (studentId < tree->studentId)
		tree->left = delete(tree->left, studentId);
	else if (studentId > tree->studentId)
		tree->right = delete(tree->right, studentId);

	else {
		if (tree->right && tree->left) {
			struct student* temp = tree;
			temp = findMin(tree->right);
			tree->studentId = temp->studentId;
			tree->right = delete(tree->right, studentId);
		}
		else {
			struct student* temp = tree;
			if (tree->left == NULL)
				tree = tree->right;
			else if (tree->right == NULL)
				tree = tree->left;
			free(temp);
		}
	}
	return tree;
}


// a function to print the main menu
void printMenu() {
	printf("please choose an operation to do.\n"
		"1- insert student\n"
		"2- find student.\n"
		"3- update studnet info.\n"
		"4- list student's names.\n"
		"5- list studnets of address.\n"
		"6- list students of class.\n"
		"7- delete student\n"
		"8- save on file.\n"
		"9- Exit.\n"
		"choice:"
	);
}

// the merge function used to merge to splited strings
// used in mergesort in this code
void merge(int left, int middle, int right, char** str) {
	int subi1 = middle - left + 1;
	int subi2 = right - middle;

	//temp strings
	char** tempRight = (char**)malloc(subi2 * sizeof(char*));

	char** tempLeft = (char**)malloc(subi1 * sizeof(char*));


	// temp strings data
	for (int j = 0; j < subi2; j++)
		tempRight[j] = str[middle + 1 + j];

	for (int i = 0; i < subi1; i++)
		tempLeft[i] = str[left + i];

	// connect temp strings 
	int  i1 = 0; // first index of substring 1
	int  i2 = 0; // first index of  substring 2
	int  i3 = left; // first index of connected substring

	while (i1 < subi1 && i2 < subi2) {
		if (strcmp(tempLeft[i1], tempRight[i2]) <= 0) {
			str[i3] = tempLeft[i1];
			i1++;
		}
		else {
			str[i3] = tempRight[i2];
			i2++;
		}
		i3++;
	}

	// cheking elements of the tempLeft
	while (i1 < subi1) {
		str[i3] = tempLeft[i1];
		i1++;
		i3++;
	}

	// cheking elements of the tempRight
	while (i2 < subi2) {
		str[i3] = tempRight[i2];
		i2++;
		i3++;
	}

	// clean memory
	free(tempLeft);
	free(tempRight);
}

// the merge sort function for strings
void mergeSort(int left, int right, char** arr) {
	if (left < right) {
		int middle = ((right - left) / 2) + left;

		// sort parts
		mergeSort(left, middle, arr);
		mergeSort(middle + 1, right, arr);

		// merge the sorted parts
		merge(left, middle, right, arr);
	}
}

// a function that saves all the student who are from the given address
void findStudentsFromAddress(struct student* tree, char* address, char** names) {
	if (tree != NULL) {
		findStudentsFromAddress(tree->left, address, names);
		if (strcmp(tree->address, address) == 0)
			names[index++] = tree->name;
		findStudentsFromAddress(tree->right, address, names);
	}
}

// a function to print the studnts and their info in a file
void finorder(struct student* tree,FILE* students) { 
	if (tree != NULL) {
		finorder(tree->left, students);
		fprintf(students, "%d %d %s %s %s", tree->classId, tree->studentId, tree->address, tree->name, tree->date);
		finorder(tree->right, students);
	}
}

// a function to print the studnts and their info in a file
void printOnFile(struct student* tree) {
	FILE* students;
	students = fopen("students.data", "w");
	if (tree == NULL)
		fprintf(students, "");
	else
		finorder(tree, students);
	fclose(students);
}

// both functions above work togother and print all students info

// a function that saves all the students who have the given classId
void getClassStundets(struct student* tree, int* classId, char** names) {
	if (tree != NULL) {
		getClassStundets(tree->left, classId, names);
		if (tree->classId == classId)
			names[index++] = tree->name;
		getClassStundets(tree->right, classId, names);
	}
}
// main function
int main() {

	// some important variables
	int exists = 0;
	int numOfClasses = 0;
	int** classes = (int**) malloc(sizeof(int*)*60);
	int choice = 0;
	struct student* studentsTree = NULL;
	char temp[200];

	//reading file it may be empty no problem
	FILE* students;
	students = fopen("students.data", "r");
	if (students == NULL)
		printf("file is not loaded");
	printf("loading file...\n");


	while (fgets(temp, 200, students) != NULL) {
		char* token1 = strtok(temp, " ");
		char* token2 = strtok(NULL, " ");
		char* token3 = strtok(NULL, " ");
		char* token4 = strtok(NULL, " ");
		char* token5 = strtok(NULL, " ");
		char* tempInfo[5];
		tempInfo[0] = strdup(token1);
		tempInfo[1] = strdup(token2);
		tempInfo[2] = strdup(token3);
		tempInfo[3] = strdup(token4);
		tempInfo[4] = strdup(token5);
		for (int i = 0;i < numOfClasses;i++)
			if (classes[i] == atoi(tempInfo[0]))
				exists = 1;
		if (!exists)	
			classes[numOfClasses++] = atoi(tempInfo[0]);
		exists = 0;
		if (studentsTree == NULL)
			studentsTree = createNode(atoi(tempInfo[0]), atoi(tempInfo[1]), tempInfo[2], tempInfo[3], tempInfo[4]);
		else
			studentsTree = insert(studentsTree, atoi(tempInfo[0]), atoi(tempInfo[1]), tempInfo[2], tempInfo[3], tempInfo[4]);
	}
	fclose(students);
	printf("file loaded successfully\n");

	do {


		// menu stuff
		if (studentsTree == NULL)
			printf(" be aware that there are no students in this system\n");

		printMenu();

		scanf("%d", &choice);



		switch (choice) {
		case 1:

			// add student
			int studentId, classId, flag = 0;
			char address[30];
			char* name = (char*)malloc(sizeof(char) * 20);
			char* date = (char*)malloc(sizeof(char) * 20);

			// checking if student exists
			do {
				if (flag > 0)
					printf("student does already exixts\n");
				printf("studnet id: ");
				scanf("%d", &studentId);
				flag++;
			} while (find(studentsTree, studentId) != NULL);

			// taking class id for some future work
			printf("calss id: ");
			scanf("%d", &classId);
			for (int i = 0;i < numOfClasses;i++)
				if (classes[i] == classId)
					exists = 1;
			if (!exists) 
				classes[numOfClasses++] = classId;
			exists = 0;
			printf("name: ");
			scanf("%s", name);
			printf("address: ");
			scanf("%s", address);
			printf("date of enrollment: ");
			scanf("%s", date);
			printf("saving student...\n");

			// assume date = 400 line 1- date="400" line 2- date="400\n"
			date[strlen(date)+1] = '\0'; // adding null terminator
			date[strlen(date)] = '\n'; // adding newline

			// adding the student
			if (studentsTree == NULL)
				studentsTree = createNode(classId, studentId, address, name, date);
			else
				studentsTree = insert(studentsTree, classId, studentId, address, name, date);
			printf("student has been added succesfully.\n");

			// free memory
			free(name);
			free(date);
			break;
		case 2:
			// find student
			struct student* tempStudent;
			printf("student ID: ");
			scanf("%d", &studentId);
			printf("looking for student...\n");
			tempStudent = find(studentsTree, studentId);
			if (tempStudent != NULL)
				printf("student found his/her name is %s \n", tempStudent->name);
			else
				printf("stundet does not exist\n");
			break;
		case 3:
			// check the exestance of the student then update what the user needs
			struct student* temp = NULL;
			int c = 0, tempInt;
			char* tempStr = (char*)malloc(sizeof(char)*20);
			printf("student ID: ");
			scanf("%d", &studentId);
			printf("looking for student...\n");
			temp = find(studentsTree, studentId);
			if (temp == NULL)
				printf("stundet does not exist\n");
			else {
				printf("studnet found\n");
				do {
					printf("please choose what to update: \n");
					printf(
						"1- classID\n"
						"2- address\n"
						"3- name\n"
						"4- date of nerrolment\n"
						"5- done\n"
					);
					scanf("%d", &c);
					switch (c) {
					case 1:
						printf("new classID: ");
						scanf("%d", &tempInt);
						update(temp, tempInt, studentId, "n", "n","n");
						break;
					case 2:						
						printf("new address: ");
						scanf("%s", tempStr);
						update(temp, -1, studentId, tempStr, "n", "n");
						break;
					case 3:
						printf("new name: ");
						scanf("%s", tempStr);
						update(temp, -1, studentId, "n", tempStr, "n");
						break;
					case 4:
						printf("new date: ");
						scanf("%s", tempStr);
						tempStr[strlen(tempStr) + 1] = '\0'; // adding null terminator
						tempStr[strlen(tempStr)] = '\n'; // adding newline
						update(temp, -1, studentId,"n", "n", tempStr);
						break;
					case 5: printf("changes done successfully\n");
						break;
					default: printf("ivalid cohice\n");
					}
				} while (c != 5);
			}
			free(tempStr);
			break;
		case 4:
			char* names1[2000];
			printf("loading...\n");
			if (studentsTree == NULL)
				printf("there are no students in this system\n");
			else {
				getNames(studentsTree, names1);
				int size = index - 1;
				index = 0;
				mergeSort(0, size, names1);
				for (int i = 0;i <= size;i++)
					printf("%s ", names1[i]);
				printf("\n");
			}
			break;
		case 5:
			char* names2[2000];
			char* tempAddress = (char*)malloc(sizeof(char) * 30);
			printf("address: ");
			scanf("%s", tempAddress);
			findStudentsFromAddress(studentsTree, tempAddress, names2);
			if (index == 0)
				printf("the address is not found\n");
			else {
				int size = index - 1;
				index = 0;
				mergeSort(0, size, names2);
				for (int i = 0;i <= size;i++)
					printf("%s ", names2[i]);
				printf("\n");
				free(tempAddress);
			}
			break;
		case 6:
			if (studentsTree == NULL)
				printf("there are no students in this system\n");
			else {
				printf("loading...\n");
				for (int i = 0;i < numOfClasses;i++)
				{
					char* names3[2000];
					names3[0] = "empty";
					getClassStundets(studentsTree, classes[i], names3);
					if (strcmp(names3[0],"empty") != 0) {
						printf("class ID %d: ", classes[i]);
						int size = index - 1;
						mergeSort(0, size, names3);
						for (int j = 0;j <= size;j++)
						{
							printf("%s ", names3[j]);
						}
						printf("\n");
						index = 0;
					}
				}
			}
			break;
		case 7:
			int tempStudentId;
			printf("student ID: ");
			scanf("%d", &tempStudentId);
			printf("looking for student...\n");
			if (find(studentsTree, tempStudentId) != NULL) {
			studentsTree = delete(studentsTree, tempStudentId);
				printf("studnet deleted successfully\n");
		}
			else
				printf("stundent does not exist\n");
			break;
		case 8:
			printf("saving...\n");
			printOnFile(studentsTree);
			printf("data saved successfully\n");
			break;
		case 9:
			printf("thank you for using my services\n");
			//deleteT
			// feedback will be usefull
			free(classes);
			studentsTree = disposeTree(studentsTree);
			break;
		default: printf("invalid choice\n");
		}
	} while (choice != 9);
	return 0;
}

// done 