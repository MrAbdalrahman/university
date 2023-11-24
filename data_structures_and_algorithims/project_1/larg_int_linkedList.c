// p1_1211753_abdulrahmanShaheen_3

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

/*
The node structure has three members:
data of type int, which stores the actual data associated with the node.
next of type struct node*, which points to the next node in the doubly linked list.
previous of type struct node*, which points to the previous node in the doubly linked list.
*/


struct node {
    int data;
    struct node* next;
    struct node* previous;
    struct node* reminder;
};

// reminder  is added for the results that needs reminders

void deleteList(struct node* list);
/*
The function makeEmpty is intended to create an empty node by allocating memory for a new struct node and initializing its next and previous pointers to NULL.
The function assumes that there is a separate deleteList function defined elsewhere to delete the existing list if it is not NULL.
If the memory allocation for the new node fails, an error message is printed, and NULL is returned.
 O(n)*/

struct node* makeEmpty(struct node* list) {
    if (list != NULL)
        deleteList(list);
    list = (struct node*)malloc(sizeof(struct node));
    if (list == NULL) {
        printf("full memory\n");
        return NULL;
    }
    else {
        list->next = NULL;
        list->previous = NULL;
        list->reminder=NULL;
        return list;
    }
}

struct node* makeReminder(struct node* list) {
    list->reminder = makeEmpty(list);
}


//--------------------- a method to check the list if its empty O(1)
int isEmpty(struct node* list) {
    return list->next == NULL;
}

//------------ a method to check a given node if its the last node in the list O(1)
int isLast(struct node* cell, struct node* liost) {
    return cell->next == NULL;
}

/*
The function find is intended to search for a node in the given list that contains the specified data.
The function assumes the existence of an isEmpty function elsewhere to check if the list is empty.
It initializes a temporary pointer temp to the next node of the list parameter.
It then iterates through the list until it finds a node with the matching data or reaches the end of the list.
If the node is found, the function returns a pointer to that node; otherwise, it returns NULL.  
O(n)*/
struct node* find(int data, struct node* list) {
    if (isEmpty(list))
        return NULL;
    struct node* temp = list->next;
    while (list != NULL && temp->data != data)
        temp = temp->next;
    return temp;
}

/*
The function delete is intended to remove a node with the specified data from the given list.
It assumes the existence of two functions, find and isLast, elsewhere in the code.
The function first calls the find function to locate the node with the matching dataand assigns 
the result to the temp pointer.If the temp node is the last node in the list (as determined by 
the isLast function), it updates the previous node's next pointer to NULL and frees the memory of the temp node.
If the temp node is not the last node, it adjusts the next and previous pointers of the adjacent
nodes to bypass the temp node and then frees the memory of the temp node.
O(n)*/
void delete(int data, struct node* list) {  
    struct node* temp = NULL;
    temp = find(data, list);
    if (isLast(temp, list)) {
        temp->previous->next = NULL;
        free(temp);
    }
    else {
        temp->next->previous = temp->previous;
        temp->previous->next = temp->next;
        free(temp);
    }
}


/*
The function insert is intended to insert a new node with the specified data into the given list.
It assumes the existence of two functions, isEmpty and isLast, elsewhere in the code.
The function first allocates memory for a new node and assigns the address to the temp pointer.
If the memory allocation fails (when temp is NULL), an error message is printed, and the function returns.
If the list is empty (as determined by the isEmpty function), the temp node becomes the first node in the list.
The next pointer of the list node is updated to point to temp, and the previous, data, and next members of temp 
are set accordingly.If the list is not empty, the function traverses the list to find the last node by repeatedly
moving to the next node until the isLast function returns true. Then, it inserts the temp node after the last node by
updating the next pointer of the last node and setting the previous, data, and next members of temp.
O(1)*/
void insert(int data, struct node* list) { 
    struct node* temp = NULL;
    temp = (struct node*)malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("full memorry");
        return;
    }
    if (isEmpty(list))
    {
        list->next = temp;
        temp->previous = list;
        temp->data = data;
        temp->next = NULL;
    }
    else {
        struct node* last = list;
        while (!isLast(last, list))
            last = last->next;
        last->next = temp;
        temp->next = NULL;
        temp->data = data;
        temp->previous = last;
    }
}


/*
The function printList is intended to print the contents of the given list.
It assumes the existence of the isEmpty and isLast functions elsewhere in the code.
The function first checks if the list is empty using the isEmpty function. If the list
is not empty, it proceeds to print the contents.It initializes a temporary pointer 
temp to the list node.If the data of the list node is 1, it prints a hyphen (-) as a special case .
in that case the number is negative while in the other case the number is positive
The function then enters a do-while loop that iterates through the list by moving to the next node using 
temp = temp->next and printing the data of each node using printf("%d", temp->data).
The loop continues until the isLast function returns true, indicating that temp is the last node in the list.
Finally, a new line character is printed to complete the output.
*/
void printList(struct node* list) {
    if (list == NULL) {
        printf("not a list is given to printList");
        return;
    }
    if (!isEmpty(list)) {

        struct node* temp = list;
        if (temp->data == 1)
            printf("-");
        do {
            temp = temp->next;
            printf("%d", temp->data);
        } while (!isLast(temp, list));

        if (list->reminder != NULL) {
            printf(" r: ");
            printList(list->reminder);
        }
        printf("\n");
    }
    else
        printf("the list is empty\n");

}


void filePrintList(struct node* list,FILE* outputFile) {
    if (list == NULL) {
        printf("not a list is given to printList");
        return;
    }
    if (!isEmpty(list)) {

        struct node* temp = list;
        if (temp->data == 1)
            fprintf(outputFile,"-");
        do {
            temp = temp->next;
            fprintf(outputFile,"%d", temp->data);
        } while (!isLast(temp, list));

        if (list->reminder != NULL) {
            fprintf(outputFile, " r: ");
            filePrintList(list->reminder, outputFile);
        }
        fprintf(outputFile,"\n");
    }
    else
        printf("the list is empty\n");

}


//  the function deleteList delete all list's nodes ecxept header O(n)
void deleteList(struct node* list) {
    if (!isEmpty(list)) {
        struct node* temp = list->next;
        list->next = NULL;
        while (temp->next != NULL) {
            temp = temp->next;
            free(temp->previous);
        }
        free(temp);
    }
}


//the fuction szieOfList return the size of the given list O(n)
int sizeOfList(struct node* list) {
    int size = 0;
    if (!isEmpty(list)) {
        struct node* temp = list;
        while (!isLast(temp, list)) {
            temp = temp->next;
            size++;
        }
    }
    return size;
}

// the function insertFirst inserts the given data in the as the first cell in the given 
// linked list
void insertFirst(int data, struct node* list) { 
    struct node* temp = NULL;
    temp = (struct node*)malloc(sizeof(struct node));
    if (temp == NULL)
        printf("full memorry");
    if (isEmpty(list))
    {
        list->next = temp;
        temp->previous = list;
        temp->data = data;
        temp->next = NULL;
    }
    else {
        temp->data = data;
        temp->previous = list;
        temp->next = list->next;
        list->next->previous = temp;
        list->next = temp;
    }
}

//the function deleteNode deletes a node you give it to it and delete it from the given list
void deleteNode(struct node* target, struct node* list) {  
    if (isLast(target, list)) {
        target->previous->next = NULL;
        free(target);
    }
    else {
        target->next->previous = target->previous;
        target->previous->next = target->next;
        free(target);
    }
}




/*
The function comp is intended to modify the data of each node in the list by subtracting its current value from 9.
It assumes that the list is not empty and that the first node (list) has already been initialized.
The function initializes a temporary pointer temp to the list node.
It then moves temp to the next node using temp = temp->next.
The while loop iterates through the list until temp becomes NULL.
Inside the loop, it subtracts the current data value of temp from 9 and assigns the result back to temp->data.
Finally, it moves temp to the next node using temp = temp->next.
 O(n)*/
struct node* comp(struct node* list) {
    struct node* temp = list;
    temp = temp->next;
    while (temp != NULL) {
        temp->data = 9 - temp->data;
            temp = temp->next;
    }
   }


// the function printMenu  prints the user menu
void printMenu() {
    printf("pelase select an operation\n");
    printf("1- load my numbers\n");
    printf("2- select numbers\n");
    printf("3- add my numberes\n");
    printf("4- subtract my numbers\n");
    printf("5- mulyiply my numbers\n");
    printf("6- divide my numbers \n");
    printf("7- print my results on console\n");
    printf("8- print on file \n");
    printf("9- Eixt \n");
}


/*
this bigSum method receives 2 lists. if any list's header's data is 1
then the number in the list is negative else if it's 0 then the 
number in the list is positive. this method can be used for both 
summation and subtraction. it returns the result of the two numbers
summation or subtraction and the operation is determined in the main.
this function have the time complexity of O(n) in it's worst case, where
n is the longest linked list.
*/

struct node* bigSum(struct node* num1, struct node* num2) {
   //---- if one of the lists is empty then there is no need to do any operation
    if (isEmpty(num1)) {
        struct node* temp = NULL;
        struct node* tempPtr =num2->next;
        temp = makeEmpty(temp);
        for (int i = 0;i < sizeOfList(num2);i++) {
            insert(tempPtr->data, temp);
            tempPtr = tempPtr->next;
        }
        temp->data = num2->data;
return temp;
    }  
    if (isEmpty(num2)) {
        struct node* temp = NULL;
        struct node* tempPtr = num1->next;
        temp = makeEmpty(temp);
        for (int i = 0;i < sizeOfList(num1);i++) {
            insert(tempPtr->data, temp);
            tempPtr = tempPtr->next;
        }
        temp->data = num1->data;
        return temp;
    }
    //---------------

    //--- some defenitions that going to be used 
    /*
    *sum and carry to hold some temp values
    * result to save the result of the operation in it
    * ptr1,ptr2 to point at the last node in each num1 and num2
    * so the two while loops are to set ptr1 and ptr2 to the last node of each of them
    */
    int sum = 0, carry = 0;
    struct node* result = NULL;
    result = makeEmpty(result);
    struct node* ptr1 = NULL;
    struct node* ptr2 = NULL;
    ptr1 = num1;
    while (!isLast(ptr1,num1)) 
        ptr1 = ptr1->next;
    ptr2 = num2;
    while (!isLast(ptr2, num2))
        ptr2 = ptr2->next;
    //----------------

  // now each case has it's operations

    // in case num1 and num2 holds the same sign +,+ or -,-
    if (num1->data == num2->data ){
        result->data = num1->data;   // setting the sign of the result 

        // this loop walks through every digit in both num1 and num2 and sum them and consider carry
        // until one of the ptrs reach the header of one of the nums
        while (ptr1->previous != NULL && ptr2->previous != NULL) { 
            sum = ptr1->data + ptr2->data + carry;
            if (sum >= 10) { // if carry is nedded
                carry = 1;
                insertFirst((sum % 10), result);
            }
            else { // if carry is not nedded
                insertFirst(sum, result);
                carry = 0;
            }
            ptr1 = ptr1->previous;
            ptr2 = ptr2->previous;
            // if one of the ptrs reached the headr and there is still carry it adds 1 to the left
            if (ptr1 == num1 && ptr2 == num2 && carry == 1) 
                insertFirst(1, result);
        }

        // now thse two if cases cheks if there is any digits in the other ptr 
        // since the previous loop ends if one of the ptrs reaches the header
        // so the other pointer may not be the header 

        // this one checks for ptr1
        if (ptr1->previous != NULL) {
            while (ptr1->previous != NULL) {
                if (ptr1->data + carry >= 10) {
                    insertFirst((ptr1->data + carry) % 10, result);
                    carry = 1;
                }                  
                else{
                    insertFirst((ptr1->data + carry), result);
                    carry = 0;
            }
                ptr1 = ptr1->previous;
                if (ptr1 == num1 && carry == 1)
                    insertFirst(1, result);
            }
        }

        // and this one checks for ptr2
        else  if (ptr2->previous != NULL) {
            while (ptr2->previous != NULL) {
                if (ptr2->data + carry >= 10) {
                    insertFirst((ptr2->data + carry) % 10, result);
                    carry = 1;
                }
                else {
                    insertFirst((ptr2->data + carry), result);
                    carry = 0;
                }
                ptr2 = ptr2->previous;
                if (ptr2 == num2 && carry == 1)
                    insertFirst(1, result);
            }
        }
    }  
    //--------------------

    // this case applies when num1 and num2 have different signs +,- or -,+
    // this function uses 9's complement to subtract numbers
    else if (num1->data != num2->data) {

        // some nedded defenetions
        struct node* comparePtr1 = num1;
        struct node* comparePtr2 = num2;
        struct node* temp = NULL;
        //------------------------

        //---------------------------------------------------------------
       // there are two cases for this case,
        // in this case num1 is negative and num2 is positive 
        if (num1->data > num2->data) {

            // --------------finding the sign of the result--------
            int num1TempSign = num1->data;
            int num2TempSign = num2->data;
            num1->data = 0;
            num2->data = 0;

            if (compareLists(num1, num2) > 0)
                result->data = 1;
            else
                result->data = 0;

            num1->data = num1TempSign;
            num2->data = num2TempSign;

            //-------------------------------------------

            // getting the 9's complement 

            if (sizeOfList(num1) < sizeOfList(num2))
            while (sizeOfList(num2) != sizeOfList(num1))
                insertFirst( 0 , num1 );
             comp(num1);      
        }
        //---------------------------------------------------------------
        // 
                // in this case num2 is negative and num1 is positive 
        else if (num2->data > num1->data){

            // --------------finding the sign of the result--------

            int num1TempSign = num1->data;
            int num2TempSign = num2->data;
            num1->data = 0;
            num2->data = 0;

            if (compareLists(num2,num1 ) > 0)
                result->data = 1;
            else
                result->data = 0;         

            num1->data = num1TempSign;
            num2->data = num2TempSign;

            // getting the 9's complement 

            if (sizeOfList(num1) > sizeOfList(num2))
                while (sizeOfList(num1) != sizeOfList(num2))
                    insertFirst(0, num2);
             comp(num2);           
        }
        //---------------------------------------------------        
        //-------------------------------------------------------------

          // now using ordinary summation to sum positive num with the comp of the negative one 
         while (ptr1->previous != NULL && ptr2->previous != NULL) {
            sum = ptr1->data + ptr2->data + carry;
            if (sum >= 10) {
                carry = 1;
                insertFirst((sum % 10), result);
            }
            else {
                insertFirst(sum, result);
                carry = 0;
            }
            ptr1 = ptr1->previous;
            ptr2 = ptr2->previous;
            if (ptr1 == num1 && ptr2 == num2 && carry == 1)
                insertFirst(1, result);
        }
        if (ptr1->previous != NULL) {
            while (ptr1->previous != NULL) {
                if (ptr1->data + carry >= 10) {
                    insertFirst((ptr1->data + carry) % 10, result);
                    carry = 1;
                }
                else {
                    insertFirst((ptr1->data + carry), result);
                    carry = 0;
                }
                ptr1 = ptr1->previous;
                if (ptr1 == num1 && carry == 1)
                    insertFirst(1, result);
            }
        }
        else  if (ptr2->previous != NULL) {
            while (ptr2->previous != NULL) {
                if (ptr2->data + carry >= 10) {
                    insertFirst((ptr2->data + carry) % 10, result);
                    carry = 1;
                }
                else {
                    insertFirst((ptr2->data + carry), result);
                    carry = 0;
                }
                ptr2 = ptr2->previous;
                if (ptr2 == num2 && carry == 1)
                    insertFirst(1, result);
            }
        }
        //------------------------------------------------------------------


        // after obtaining result there are two cases case 1:
        // getting carry.
        // in this case just remove carry and add 1 to the result and 
        // the result is prefectly cocked
        if (sizeOfList(num1) == sizeOfList(num2) && sizeOfList(result) > sizeOfList(num1)) {
            result->data = 0;
            struct node* tempOne = NULL;
            tempOne = result->next;
            tempOne->next->previous = result;
            result->next = tempOne->next;
            free(tempOne);
            struct node* one = NULL;
                one = makeEmpty(one);
            one->data = 0;
            insert(1,one);
            result=bigSum(one, result);
            deleteList(one);
            free(one);
        }
        // case 2 , if there is nocarry take the complement of the result
        // and its ready 
        else if (sizeOfList(result) == sizeOfList(num1) || sizeOfList(result) == sizeOfList(num2)) {
          comp(result);
        }
    }
    //-----------------
    // now just before reternung result 
    // i want to make sure that the complemented lists would get 
    // complemented again
    if (num1->data == 1 && num2->data == 0)
        comp(num1);
    else if (num2->data == 1 && num1->data == 0)
        comp(num2);

    // lets free result before reterning it to save some space in memory
    struct node* tempResult = NULL;
    tempResult = result;
    result = NULL;
    free(result);
    // now lets get of the zeros we add during 9's complement convertion for each num1,num2,tempResult
    struct node* tempZero=NULL;
    tempZero = num1->next;
    while (tempZero->data == 0 && !isLast(tempZero,num1)) {
        tempZero = tempZero->next;
        deleteNode(tempZero->previous, num1);
    }


    tempZero = num2->next;
    while (tempZero->data == 0 && !isLast(tempZero,num2)) {
        tempZero = tempZero->next;
        deleteNode(tempZero->previous, num2);
    }


    tempZero = tempResult->next;
    while (tempZero->data == 0 && !isLast(tempZero,tempResult)) {
        tempZero = tempZero->next;
        deleteNode(tempZero->previous, tempResult);
    }
    //-------------------------------------------------

    // result is ready to be out of the furnace  :
    return tempResult;
}
//---------------------------------------------------------------------------------------------------


//----a function that multiplies two numbers 
struct node* bigMult(struct node* num1, struct node* num2) {

    // defenetions
    int temp=0;
    struct node* mult = NULL;
    mult = makeEmpty(mult);
    mult -> data = 0;
    struct node* result = NULL;
    result = makeEmpty(result);
    struct node* ptr1 = NULL;
    struct node* ptr2 = NULL;
    ptr1 = num1;
    result->data = 0;
    //----------

    //-------making ptrs points to last node of each num----------------
    while (!isLast(ptr1, num1))
        ptr1 = ptr1->next;
    ptr2 = num2;
    while (!isLast(ptr2, num2))
        ptr2 = ptr2->next;
    //---------------------------

    //------------ multiplication operation-------
    for (int i = 0;i < sizeOfList(num2);i++) {

        for (int j = 0;j < sizeOfList(num1);j++) { 
            deleteList(mult);
                temp = ptr2->data * ptr1->data;
                while (temp != 0) {
                    insertFirst((temp % 10), mult);
                    temp /= 10;
                }

                for (int k = 0;k < j+i;k++)
                    insert(0, mult);

                result = bigSum(result, mult);
            ptr1 = ptr1->previous;
        }
        while (!isLast(ptr1, num1))
            ptr1 = ptr1->next;
        ptr2 = ptr2->previous;

    }
    //------------------------------

      //------- deciding the sign of result
    if (num1->data != num2->data)
        result->data = 1;
    else
        result->data = 0;
    //--------------------------------

    return result;
}
// compares two lists
int compareLists(struct node* num1, struct node* num2) {
    struct node* comparePtr1 = num1;
    struct node* comparePtr2 = num2;
    struct node* temp = NULL;

    // at first lets see if we can decide which is bigger by sign checking 
    if (num1->data > num2->data)
        return -1;
    else if (num1->data < num2->data)
        return 1;

    // if they have the same sign then lets check size
    if (sizeOfList(num1) < sizeOfList(num2))
        return -1;
    else if (sizeOfList(num1) > sizeOfList(num2))
        return 1;

    // if they have the same size there is last thing to check lets loop through lists
    // we have nothing else to do 
    else if (sizeOfList(num1) == sizeOfList(num2)) {
        comparePtr1 = comparePtr1->next;
        comparePtr2 = comparePtr2->next;
        while (comparePtr1 != NULL) {
            if (comparePtr1->data < comparePtr2->data) {
                return -1;
            }
            else if (comparePtr1->data > comparePtr2->data) {
                return 1;
            }
            comparePtr1 = comparePtr1->next;
            comparePtr2 = comparePtr2->next;
        }
        comparePtr1 = NULL;
        comparePtr2 = NULL;
        free(comparePtr1);
        free(comparePtr2);
        return 0;
    }
}void copyLists(struct node* source, struct node* destenation) {
    struct node* temp = source->next;
    while (temp != NULL) {
        insert(temp->data,destenation);
        temp = temp->next;
    }
}

int isZero(struct node* num) {
    int sum = 0;
    struct node* temp = num->next;
    while (temp != NULL) {
        sum += temp->data;
        temp = temp->next;
    }

    return sum == 0;
}

//-------------------------------------------------------------
// divition using repeated subtraction
struct node* subDivition(struct node* dividend, struct node* divisor) {
    struct node* temp = NULL;
    temp = makeEmpty(temp);
    struct node* ptr = dividend->next;
    int result = 0;
    int tempSign;
    while (ptr != NULL) {
        insert(ptr->data, temp);
        ptr = ptr->next;
    }

    temp->data = 0;

    ptr = NULL;
    free(ptr);

    while (compareLists(temp, divisor) > -1  ) {
        result++;
        tempSign = divisor->data;
        divisor->data = 1;
        temp = bigSum(temp, divisor);
        divisor->data = tempSign;
    }

    struct node* resultList = NULL;
    resultList = makeEmpty(resultList);
    insert(result, resultList);

    return resultList;
}
// list to long long

// long divition 
struct node* bigDivide(struct node* dividend, struct node* divisor)
{

    if (isZero(divisor) ) {
        return NULL;
    }

    int tempDividendData = dividend->data;
    int tempDivisorData = divisor->data;

    dividend->data = 0;
    divisor->data = 0;

    int tempSign;
    struct node* quiotiont = NULL;
    quiotiont= makeEmpty(quiotiont);
    if (compareLists(dividend, divisor) > -1 ) {
        struct node* temp = NULL;
        temp = makeEmpty(temp);
        temp->data = 0;

        struct node* temp2 = NULL;
        temp2 = makeEmpty(temp2);
        temp2->data = 1;
        insert(1, temp2);

        struct node* tempResult = NULL;
        tempResult = makeEmpty(tempResult);

        struct node* ptrDividend = dividend->next;
        while (ptrDividend != NULL) {
            insert(ptrDividend->data, temp);
            tempResult = subDivition(temp, divisor);
            insert(tempResult->next->data, quiotiont);
            temp2 = bigMult(divisor, tempResult);
            temp = bigSum(temp, temp2);
            ptrDividend = ptrDividend->next;
            if (temp->next->data == 0)
                deleteList(temp);

            // setting reminder 
            //-------------------
            quiotiont->reminder = makeEmpty(quiotiont->reminder);
            copyLists(temp, quiotiont->reminder);
        }

        // removing leading zeros
        struct node* tempZero = NULL;
        tempZero = quiotiont->next;
        while (tempZero->data == 0 && !isLast(tempZero, quiotiont)) {
            tempZero = tempZero->next;
            deleteNode(tempZero->previous, quiotiont);
        }
        //--------------------

        //rearranging signs 
        dividend->data = tempDividendData;
        divisor->data = tempDivisorData;

        // setting sign of the quiotiont
        quiotiont->data = dividend->data != divisor->data;
    }
    else { // in case that 
        insert(0, quiotiont);
        quiotiont->reminder = makeEmpty(quiotiont->reminder);
        copyLists(dividend, quiotiont->reminder);
    }

    return  quiotiont ;
}

//----------------------checks if word ends with subWord------------------------------------------------------
int endsWith(char word[], char subWord[]) {
    if (subWord == "")
        return 0;
    int wordSize = strlen(word), subWordSize = strlen(subWord);
    if (wordSize < subWordSize)
        return 0;
    int i, j;
    for (i = 0, j = wordSize - subWordSize;j < wordSize;i++, j++)
        if (word[j] != subWord[i])
            return 0;
    return 1;
}
//-------------------------------------------------------------------
//------------------------------------------MAIN--------------------------------------------------
int main() {

    //----getting 50 lists ready-----defention creation initialization------
 struct node* lists[50];
 for (int i = 0;i < 50;i++) {
     lists[i] = NULL;
    lists[i] = makeEmpty(lists[i]);
     lists[i] -> data = 0;
 }

 //----getting 100 lists ready-----defention creation initialization------
 struct node* results[100];
 for (int i = 0;i < 100;i++) {
     results[i] = NULL;
     results[i] = makeEmpty(results[i]);
     results[i]->data = 0;
 }

 struct node* chosenList1 = NULL;
 struct node* chosenList2 = NULL;

 // i used a 2D array to recive the numbers this program can hold 10 equations 
 // which each equation holds 200 digits and you can edit these numbers
    int choice = 1;
    char nums[10][200]; // if you want to scan more digits add to 200
    int limit = 0; // to make sure that the file get scanned once
    int i = 0;
    int chosenlist;
    int n = 0; //number of results
    int isSelected = 0;
    //---------

    do { // a do while loop to print the menu and do the wanted tasks until the user enters 5

        printMenu();

        scanf("%d",&choice);

        switch (choice)
        {

        case 1:
            if (limit == 0) { // if the file is not scanned yet

                // file handling :
                printf("please enter your file name and end it with \".txt\"\n");
                printf("please consider adding new line after the last number\n ");
                char inputFileName[30];
                scanf("%s", inputFileName);
                while (!endsWith(inputFileName, ".txt")) {
                    printf("your file should incloude \".txt\" at the end of it\n");
                    scanf("%s", inputFileName);
                }
                printf("scaning file...\n");
                //--------------scanning data from the file----------------
                FILE* inputFile;
                inputFile = fopen(inputFileName, "r");
                if (inputFile == NULL)
                    printf("sorry, couldnt open %s", inputFileName);
                else {
                    while (fgets(nums[i], 1000, inputFile) != NULL) // if you want to scan more digits increase 1000
                        i++;
                    //------------------------------------------------------------

                         //--------------inserting data into the lists--------
                }
                    for (int j = 0, k = 0;j < i;j++) {
                    while (nums[j][k] != '\n') {
                        if (nums[j][k] == '-') {
                            lists[j]->data = 1;
                            k++;
                            continue;
                        }
                        insert((nums[j][k++] - '0'), lists[j]); // here '0' is to change the char number to int 
                    }
                    k = 0;

                }
                limit++; // an indicator the file is scanned
                printf("file scanned succesfully\n");
            }
            else // if the file is scanned
                printf("you have already scnaed the file. please restart the program for more numbers\n");
            //--------------------------------------
            break;


        case 2:

            if (limit == 1) { // if the file is scanned i let user to choose 2 numbers
                printf("please choose what numbers to perform operations on: (note that you cant chose the same number )\n");
                for (int counter = 0;counter < i;counter++) {
                    printf("%d- ", counter + 1);
                    printList(lists[counter]);
                }
                printf("num1:\n");
                scanf("%d", &chosenlist);
                chosenList1 = lists[chosenlist - 1];

                printf("num2:\n");
                scanf("%d", &chosenlist);
                chosenList2 = lists[chosenlist - 1];
                isSelected = 1;
            }
            else
                printf("please scan file at first\n");

            break;

        case 3:

            if (limit == 0) {
                printf("please scan file at first\n");
                break;
            }

            if (isSelected == 0) {
                printf("please select numbers at first\n");
                break;
            }

            //------case1 where all data passed to bigSum and returned to result                      
            results[n] = bigSum(chosenList1, chosenList2);
            n++;
            break;


            //------case2 where all data passed to bigSum too and returned to result
            // but the differance is from the seconde equation and above each equation
            // will be given the opposite of its sign. thats what makes bigSum works for
            //both summation and subtraction
        case 4:

            if (limit == 0) {
                printf("please scan file at first\n");
                break;
            }

            if (isSelected == 0) {
                printf("please select numbers at first\n");
                break;
            }

            // change sign of temp2 
            int tempSign = chosenList2->data;

            if (chosenList2->data == 1)
                chosenList2->data = 0;
            else if (chosenList2->data == 0)
                chosenList2->data = 1;
            results[n] = bigSum(chosenList1, chosenList2);
            chosenList2->data = tempSign;
            n++;

            break;

            //-------multiplication----------
        case 5:

            if (limit == 0) {
                printf("please scan file at first\n");
                break;
            }

            if (isSelected == 0) {
                printf("please select numbers at first\n");
                break;
            }

            results[n] = bigMult(chosenList1, chosenList2);
            n++;
            break;

        case 6: //-----------devition-----------------    

            if (limit == 0) {
                printf("please scan file at first\n");
                break;
            }

            if (isSelected == 0) {
                printf("please select numbers at first\n");
                break;
            }

            struct node* temp = bigDivide(chosenList1, chosenList2);
            if ( temp == NULL) {
                printf("you cant divide a number by zero \n");
                    break;
            }
            results[n] = bigDivide(chosenList1, chosenList2);
            n++;

            break;
            //----------------------------------------
        case 7:

            if (limit == 0) {
                printf("please scan file at first\n");
                break;
            }

            if (isSelected == 0) {
                printf("please select numbers at first\n");
                break;
            }


            for (int i = 0;i < n;i++)
                printList(results[i]);
            break;

        case 8 :
            if (limit == 0) {
                printf("please scan file at first\n");
                break;
            }

            if (isSelected == 0) {
                printf("please select numbers at first\n");
                break;
            }


            FILE* outputFile;
            char outputFileName[30];
            printf("please enter your file name and end it with \".txt\"\n");             
            scanf("%s", outputFileName);
            while (!endsWith(outputFileName, ".txt")) {
                printf("your file should incloude \".txt\" at the end of it\n");
                scanf("%s", outputFileName);
            }
            outputFile = fopen(outputFileName, "w");
            fprintf(outputFile,"");
            fclose(outputFile);
            outputFile = fopen(outputFileName, "a");
            if (outputFile == NULL)
                printf("sorry, couldnt open %s", outputFileName);
            else {
                for (int i = 0;i < n;i++)                  
                    filePrintList(results[i],outputFile);
          printf(" numbers printed succesfully\n");
          fclose(outputFile);
            }

            break;
        case 9:
            printf("i hope i was usefull.\n");
            break;
        default: printf("invalid operation");
            break;
        }
    } while (choice != 9);
    for (int i = 0;i < 50;i++) {
        deleteList(lists[i]);
        free(lists[i]);
    }

    for (int i = 0;i < 50;i++) {
        deleteList(lists[i]);
        free(lists[i]);
    }
    for (int i = 0;i < 100;i++) {
        deleteList(results[i]);
        free(results[i]);
    }
}