// P2_1211753_abdulrahmanshahen_3

#define _CRT_SECURE_NO_WARNINGS
// headers

#include <stdio.h>
#include <math.h>
#include <ctype.h>

// a stack that saves a char
typedef struct node* ptrToNode;
typedef ptrToNode stack;
//----------------structing node
struct node {
    char data;
    struct node* next;
};

// a stack that saves an int
typedef struct nodeNum* ptrToNodeNum;
typedef ptrToNodeNum stackNum;
//----------------structing node
struct nodeNum {
    double data;
    struct nodeNum* next;
};


//------------------ a method that creates a new linked list and allocate it in the memory
char pop(stack s);
void makeEmpty(stack s);
stack createStack() {
    stack s;
    s = (struct node*)malloc(sizeof(struct node));
    if (s == NULL) {
        printf("full memory\n");
        return NULL;
    }
    s->next = NULL;
    makeEmpty(s);
    return s;
}
//-------------------------------------------------------------------------------
//--------------------- a method to delete all the stacks nodes (data)
void makeEmpty(stack s) {
    if (s == NULL)
        printf("stack is empty ");
    else
        while (!isEmpty(s))
            pop(s);
}

//----------------------------------------------------
//------------ a method that removes the last pushed element in the stack and returns it
char pop(stack s) {
    ptrToNode firstCell;
    if (!isEmpty(s)) {
        firstCell = s->next;
        s->next = s->next->next;
        char poppedValue = firstCell->data;
        free(firstCell);
        return poppedValue;
    }
    else {
        printf("stack is empty");
        return 'e';
    }
}
//------------------------------------------------------------
//---------- a method that returns the last pushed element in the stack without removal
char top(stack s) {
    if (isEmpty(s))
        return NULL;
    return s->next->data;
}
//-------------------------------------------------------------------------
//---------------- a method that push (inserts) the given data in the stack
void push(char data, stack s) {
    ptrToNode temp;
    temp = (stack)malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("full memory");
        return NULL;
    }
    else {
        temp->data = data;
        temp->next = s->next;
        s->next = temp;
    }
}
//---------------------------------------------------------------------
//--------------------- a method to remove the whole stack from the memory 
void disposeStack(stack s) {
    makeEmpty(s);
    free(s);
}
//---------------------------------------------------


//--------------------- a method to check the list if its empty O(1)
int isEmpty(stack s) {
    return s->next == NULL;
}
//--------------------------------------------


//------------ a method to check a given node if its the last node in the list O(1)
int isLast(struct node* cell, struct node* list) {
    return cell->next == NULL;
}
//---------------------------------------------------


//------------------ a method that creates a new linked list and allocate it in the memory
double popNum(stackNum s);
void makeEmptyNum(stackNum s);
stackNum createStackNum() {
    stackNum s;
    s = (struct nodeNum*)malloc(sizeof(struct nodeNum));
    if (s == NULL) {
        printf("full memory\n");
        return NULL;
    }
    s->next = NULL;
    makeEmptyNum(s);
    return s;
}

//-------------------------------------------------------------------------------
//--------------------- a method to delete all the stacks nodes (data)
void makeEmptyNum(stackNum s) {
    if (s == NULL)
        printf("stackNum is empty ");
    else
        while (!isEmptyNum(s))
            popNum(s);
}

//----------------------------------------------------
//------------ a method that removes the last pushed element in the stack and returns it
double popNum(stackNum s) {
    ptrToNodeNum firstCell;
    if (!isEmptyNum(s)) {
        firstCell = s->next;
        s->next = s->next->next;
        double poppedValue = firstCell->data;
        free(firstCell);
        return poppedValue;
    }
    else {
        printf("stack is empty");
        return -1;
    }
}
//------------------------------------------------------------
//---------- a method that returns the last pushed element in the stack without removal
double topNum(stackNum s) {
    if (isEmptyNum(s))
        return 404;
    return s->next->data;
}
//-------------------------------------------------------------------------
//---------------- a method that push (inserts) the given data in the stack
void pushNum(double data, stackNum s) {
    ptrToNodeNum temp;
    temp = (stackNum)malloc(sizeof(struct nodeNum));
    if (temp == NULL) {
        printf("full memory");
        return NULL;
    }
    else {
        temp->data = data;
        temp->next = s->next;
        s->next = temp;
    }
}
//---------------------------------------------------------------------
//--------------------- a method to remove the whole stack from the memory 
void disposeStackNum(stackNum s) {
    makeEmptyNum(s);
    free(s);
}
//---------------------------------------------------


//--------------------- a method to check the list if its empty O(1)
int isEmptyNum(stackNum s) {
    return s->next == NULL;
}
//--------------------------------------------


//------------ a method to check a given node if its the last node in the list O(1)
int isLastNum(struct nodeNum* cell, struct nodeNum* list) {
    return cell->next == NULL;
}
//---------------------------------------------------

//-------- a method to print the main menu for the user
void printMenu() {
    printf("please select your operation by intering it's number\n1-read my equations\n2-check validity of my equations\n3-convert my equations from infix to prefix\n4-evaluate the prefix of my equations\n5-print the invalid equations\n6-print all info in a file\n7- Exit:\nchoice:  ");
}
//----------------------------------------------------------
//-------------- a method to check if a char of array ends with a specific suffix
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
//--------- a method to check the given char if its an operation
int isOperation(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/') {

        return 1;
    }
    else {
        return 0;
    }
}

int isClosing(char temp) {
    return (temp == '}' || temp == ')' || temp == '>' || temp == ']');
}

int isOpening(char temp) {
    return (temp == '{' || temp == '(' || temp == '<' || temp == '[');
}

//------- a method to check if the given equation is valid
int printValid(char equation[], int index) {
    int i = 0;
    stack symboles = NULL;
    symboles = createStack();
    int size = strlen(equation);
    for (i = 0;i < size;i++) {
        char temp = equation[i];

        if (isOperation(temp) && isOperation(equation[i - 1]) && isOperation(equation[i + 1])) {
            // in case the equation has more than two consequative operators  
            printf("equetion NO.%d is not valid since you cant place more than two consequative operators in sequence %c %c %c\n", index + 1, equation[i - 1], temp, equation[i + 1]);
            disposeStack(symboles);
            return 0;
        }

        if ((i == 0 && isOperation(temp)) || (isOperation(temp) && !isOperation(equation[i - 1]) && !isdigit(equation[i - 1]) && !isClosing(equation[i - 1]))) {
            printf("equetion NO.%d is not valid since there is no number before %c \n", index + 1, temp);
            disposeStack(symboles);
            return 0;
        }

        if (isOpening(temp)) {
            if (i > 0 && isdigit(equation[i - 1])) { // in case there is a number before an oppening 4(
                printf("equetion NO.%d is not valid since there is a number before %c but no operation\n", index + 1, temp);
                disposeStack(symboles);
                return 0;
            }

            else if (isOperation(equation[i + 1])) {//in case there is an operation after an oppening (+
                printf("equetion NO.%d is not valid since there is an operation after %c\n", index + 1, temp);
                disposeStack(symboles);
                return 0;
            }

            else if (isClosing(equation[i - 1])) {
                // in case there is a closing before an oppening )(
                printf("equetion NO.%d is not valid since there is a closing  bfore %c\n", index + 1, temp);
                disposeStack(symboles);
                return 0;
            }

            else
                push(temp, symboles);
        }

        else if (isClosing(temp)) {
            char scndElmnt;
            char tops;
            if (!isEmpty(symboles)) {
                tops = pop(symboles);
                scndElmnt = top(symboles);
                push(tops, symboles);
            }

            if (isEmpty(symboles)) {
                printf("equetion NO.%d is not valid since %c does not have an oppening\n", index + 1, temp);
                disposeStack(symboles);
                return 0;
            }

            else if (isdigit(equation[i + 1])) {  // in case there is a digit after a closing )4
                printf("equetion NO.%d is not valid since there is a digit after %c\n", index + 1, temp);
                disposeStack(symboles);
                return 0;
            }
            else if (isOperation(equation[i - 1])) { // in case there is an operation before closing +)
                printf("equetion NO.%d is not valid since there is an operation before %c\n", index + 1, temp);
                disposeStack(symboles);
                return 0;
            }
            //in  case that a closing does not match the oppening 
            else if (temp == '}') {
                if (tops != '{' && scndElmnt != '{') {
                    printf("equetion NO.%d is not valid since %c does not match %c \n", index + 1, tops, temp);
                    disposeStack(symboles);
                    return 0;
                }
            }

            else if (temp == ']') {
                if (tops != '[' && scndElmnt != '[') {
                    printf("equetion NO.%d is not valid since %c does not match %c \n", index + 1, tops, temp);
                    disposeStack(symboles);
                    return 0;
                }
            }

            else if (temp == ')') {
                if (tops != '(' && scndElmnt != '(') {
                    printf("equetion NO.%d is not valid since %c does not match %c \n", index + 1, tops, temp);
                    disposeStack(symboles);
                    return 0;
                }
            }

            else if (temp == '>') {
                if (tops != '<' && scndElmnt != '<') {
                    printf("equetion NO.%d is not valid since %c does not match %c \n", index + 1, tops, temp);
                    disposeStack(symboles);
                    return 0;
                }
            }

            if (tops == '<' && temp == '>')
                pop(symboles);
            else if (tops == '[' && temp == ']')
                pop(symboles);
            else if (tops == '{' && temp == '}')
                pop(symboles);
            else if (tops == '(' && temp == ')')
                pop(symboles);

        }// in case temp is not a valid exepretion a , ;
        else if (isalpha(temp) || temp == '!' || temp == '@' || temp == '#' || temp == '$' || temp == '%' ||
            temp == '^' || temp == '&' || temp == '?' || temp == '\\' ||
            temp == '~' || temp == '"' || temp == ':' || temp == '.' || temp == ',' ||
            temp == ';' || temp == '\'' || temp == '_' || temp == '=') {
            printf("equetion NO.%d is not valid since it contains a non allowed exepretion : %c\n", index + 1, temp);
            disposeStack(symboles);
            return 0;
        }
    }
    if (isEmpty(symboles)) {
        printf("operation NO.%d is valid\n", index + 1);
        disposeStack(symboles);
        return 1;
    }
    else { // in case since an oppening  has no closing
        printf("operation NO.%d is not valid since %c has no closing\n", index + 1, top(symboles));
        disposeStack(symboles);
        return 0;
    }

}

//----- this method checks if char1 > char2 in priority
int isHigherPriority(char char1, char char2) {
    if (char1 == char2)
        return 0;
    else if (char1 == '+' && char2 == '-')
        return 0;
    else if (char1 == '-' && char2 == '+')
        return 0;
    else if (char1 == '*' && char2 == '/')
        return 0;
    else if (char1 == '/' && char2 == '*')
        return 0;
    else if (char2 == '*' || char2 == '/')
        return 0;
    else if (char2 == '+' || char2 == '-')
        return 1;

}
//---------------

//------------- a method to convert the equation from infix to prefix
/*the algorithim is as follows:
* 1->reversing the equation
* 2->converting the reversed equation to prefix
* 3->reversing the equation again
* the returned value is the prefix vertion of the given equation
*/
char* inToPre(char equation[]) {
    //--- some defenetions and initalizations
    stack s = NULL;
    s = createStack();
    int index = 0;
    int size = strlen(equation);
    char temp;
    char* temp1;
    char* temp2;
    char topS;
    temp1 = (char*)malloc(((size + 1) * 2) * sizeof(char));
    temp2 = (char*)malloc(((size + 1) * 2) * sizeof(char));
    int i;
    //-----

    //-- reversing equation and inserting it into temp1
    for (i = 0; i < size - 1; i++)
        temp1[i] = equation[size - i - 2];
    temp1[size - 1] = '\0';
    //--

    temp1[size] = '\0'; // adding the null terminator to the end of temp1
    //-- changing every opening to closing and vice versa
    for (i = 0;i < size - 1;i++) {
        temp = temp1[i];
        if (temp == '{' || temp == '<' || temp == '[')
            temp += 2;
        else if (temp == '}' || temp == '>' || temp == ']')
            temp -= 2;
        else if (temp1[i] == '(')
            temp++;
        else if (temp == ')')
            temp--;

        temp1[i] = temp;
    }
    //-----
        //---- converting the equation from reversed infix into prefix and placing it in temp2
    for (i = 0;i < size - 1;i++) { // the loop to scan every char from the left
        if (i > 0)
            topS = top(s);
        temp = temp1[i];



        //-------if the char is a digit
        if (isdigit(temp)) {
            while (isdigit(temp1[i])) {
                temp2[index++] = temp1[i++];
            }
            i--;
            temp2[index++] = ' ';
        }
        //------

        //----- if the char is an operation (/,*,+,-)       
        else if (isOperation(temp)) {

            if (temp == '+' && temp1[i + 1] == '+')
                continue;

            else if (temp == '+' && temp1[i - 1] == '-')
                continue;

            else if (temp == '+' && temp1[i + 1] == '-')
                continue;

            else if (temp == '-' && temp1[i + 1] == '-')
                continue;

            else if (temp == '-' && temp1[i - 1] == '-')

                temp = '+';

            if (!isOperation(top(s))) {
                push(temp, s);
                topS = top(s);
            }
            // if the stack's top's priority is not higher than temp, temp get pushed 
            else if (!isHigherPriority(top(s), temp)) {
                push(temp, s);
                topS = top(s);
            }
            /* this case represents the opposite of the previos case
             so in this case the elemnts in the stack keep pushed into temp2 until
            we reach the case above then we push the operator into tjhe stack */
            else {
                while (isOperation(top(s)) && isHigherPriority(top(s), temp) && !isEmpty(s))
                    temp2[index++] = pop(s);
                push(temp, s);
                topS = top(s);
                temp2[index++] = ' ';
            }
        }
        //--------

        //--- if the char is a closing 
        /*
        the algorithim is as follows:
        assum temp is '}' then all elements keep poped until
        the top is the closing of the found oppening in this case '{'
        then the oppening gets poped
        */
        else if (temp == '}') {
            while (topS != '{') {
                temp2[index++] = pop(s);
                topS = top(s);
                temp2[index++] = ' ';
            }
            pop(s);
        }
        else if (temp == '>') {
            while (topS != '<') {
                temp2[index++] = pop(s);
                topS = top(s);
                temp2[index++] = ' ';
            }
            pop(s);
        }
        else if (temp == ')') {
            while (topS != '(') {
                temp2[index++] = pop(s);
                topS = top(s);
                temp2[index++] = ' ';
            }
            pop(s);
        }
        else if (temp == ']') {
            while (topS != '[') {
                temp2[index++] = pop(s);
                topS = top(s);
                temp2[index++] = ' ';
            }
            pop(s);
        }
        // if temp is an oppening
        else if (isOpening(temp)) {
            push(temp, s);
            topS = top(s);
        }
    }
    //----   
    //--------

    // inserting the rest of stacked operations in temp2    
    while (!isEmpty(s)) {
        temp2[index++] = pop(s);
        temp2[index++] = ' ';
    }
    temp2[index] = '\0';
    //-----
    //--- reversing the postfix of the reversed equation
    for (i = 0;i < index;i++)
        temp1[i] = temp2[index - i - 2];
    temp1[index - 1] = '\0';
    //----
    free(temp2);
    disposeStack(s);
    return temp1;
}

// this method applies the given operator on the given two numbers
double operate(double n1, char operation, double n2)
{
    switch (operation)
    {
    case '+': return  (n1 + n2);
    case '-': return  (n1 - n2);
    case '*': return  (n1 * n2);
    case '/': return  (n1 / n2);
    }
}

// this method calculates the result of the prefix of the equations 
// using stack algorithim
double calculatePrefix(char prefix[])
{
    double first;
    double seconde;
    char temp;
    double tempInt;
    stackNum s = NULL;
    int digits = 0;
    s = createStackNum();

    for (int i = strlen(prefix) - 1;i >= 0;i--) {

        temp = prefix[i];

        if (isdigit(temp)) {

            digits = 1;
            tempInt = 0;

            while (isdigit(prefix[i])) {

                tempInt += ((prefix[i]) - '0') * digits;
                i--;
                digits *= 10;

            }
            pushNum(tempInt, s);

        }
        else if (isOperation(temp)) {
            first = popNum(s);
            seconde = popNum(s);

            if (temp == '/' && seconde == 0)
            {
                return 0.0001;
            }

            pushNum(operate(first, temp, seconde), s);

        }

    }

    double tempResult = topNum(s);
    disposeStackNum(s);
    return tempResult;
}

int main() {


    // to keep the sequence of the menu tasks
    int scanned = 0;
    int checked = 0;
    int converted = 0;
    int solved = 0;


    int validityEquationsIndexes[100]; // to save each equations'validity
    int choice = 0; // user's choice 
    char* prefix[100];
    int numOfEquations = 0;
    char equation[100][500];
    double results[100];
    int i = 0;
    do {

        printMenu();
        scanf("%d", &choice);
        switch (choice) {

        case 1:
            if (scanned == 0) {
                scanned++;
                printf("you have to consider adding a newLine when finishing each equation even the last one\n");
                printf("noting that you cant enter more than 100 hundred equations. each equation includes the max of 500 character\n");
                printf("please enter your file name and end it with \".txt\"\n");
                char inputFileName[30];



                scanf("%s", inputFileName);
                while (!endsWith(inputFileName, ".txt")) {
                    printf("your file should incloude \".txt\" at the end of it\n");
                    scanf("%s", inputFileName);
                }
                FILE* inputFile;
                inputFile = fopen(inputFileName, "r");
                if (inputFile == NULL)
                    printf("sorry, couldnt open %s", inputFileName);
                else {
                    while (fgets(equation[i], 500, inputFile) != NULL) {
                        i++;
                        if (i > 100) {
                            printf("its mentioned that the programm holds the maximum of 100 equation\n");
                            printf("tnaks for shutting down the program\n");
                            printf("reduce the number of equations or edit the number of equation allowed\n");
                            exit(0);
                        }
                    }
                }
            }
            else
                printf("you have already scanned the file to rescan it please restart the program\n");

            break;


        case 2:
            if (scanned > 0) {
                checked++;
                numOfEquations = i;
                i = 0;
                do {
                    validityEquationsIndexes[i] = printValid(equation[i], i);
                    i++;
                } while (i != numOfEquations);
            }

            else printf("make sure that the file is scanned\n");
            break;


        case 3:
            if (checked > 0) {
                converted++;
                for (i = 0;i < numOfEquations;i++) {
                    if (validityEquationsIndexes[i] == 1) {
                        prefix[i] = inToPre(equation[i]);
                        printf(" the prefix of equation NO.%d is :%s\n", i + 1, prefix[i]);
                    }
                }
            }
            else {
                printf("make sure that you have ");
                if (checked < 1)
                    printf("checked for validity ");
                if (scanned < 1)
                    printf("scanned the file ");
                printf("\n");
            }
            break;


        case 4:
            if (converted > 0) {
                solved++;
                for (i = 0;i < numOfEquations;i++) {
                    if (validityEquationsIndexes[i] == 1) {
                        results[i] = calculatePrefix(prefix[i]);
                        if (results[i] == 0.0001)
                            printf("in equation NO.%d you cant divide by zeeero :)\n",i+1);
                        else 
                        printf(" the result of infix of the equation NO.%d is :%.1f\n", i + 1, results[i]);
                    }
                }
            }
            else {
                printf("make sure that you have ");
                if (converted < 1)
                    printf("converted to prefix ");
                if (checked < 1)
                    printf("checked for validity ");
                if (scanned < 1)
                    printf("scanned the file ");
                printf("\n");
            }
            break;
        case 5:
            if (checked > 0) {
                for (i = 0;i < numOfEquations;i++) {
                    if (validityEquationsIndexes[i] != 1) {
                        printf("equation NO.%d: %s\n", i + 1, equation[i]);
                    }
                }
            }
            else
                printf("make sure check the validity of your equations\n");
            break;


        case 6:
            if (solved > 0) {

                char outputFileName[100];

                printf("please enter your file name and end it with \".txt\"\n");
                scanf("%s", outputFileName);

                while (!endsWith(outputFileName, ".txt")) {
                    printf("your file should incloude \".txt\" at the end of it\n");
                    scanf("%s", outputFileName);
                }

                FILE* outputFile = NULL;
                outputFile = fopen(outputFileName, "w");

                if (outputFile == NULL)
                    printf("sorry, couldnt open %s\n", outputFileName);
                else
                    fprintf(outputFile, "");
                fclose(outputFile);

                outputFile = fopen(outputFileName, "a");
                if (outputFile == NULL)
                    printf("sorry, couldnt open %s\n", outputFileName);
                else {

                    for (int counter = 0;counter < numOfEquations;counter++) {

                        equation[counter][strlen(equation[counter]) - 1] = '\0';
                        fprintf(outputFile, "equation NO.%d: %s,", counter + 1, equation[counter]);
                        equation[counter][strlen(equation[counter])] = '\n';

                        if (validityEquationsIndexes[counter] == 1) {
                            fprintf(outputFile, " validity: valid, ");
                        }

                        else
                            fprintf(outputFile, " validity: invalid, ");

                        if (validityEquationsIndexes[counter] == 1) {
                            fprintf(outputFile, " prefix: ( %s ), ", prefix[counter]);
                        }
                        else
                            fprintf(outputFile, "no prefix since invalid, ");

                        if (validityEquationsIndexes[counter] == 1) {
                            fprintf(outputFile, "result: %.1f.", results[counter]);
                        }
                        else
                            fprintf(outputFile, "no result since invalid. ");

                        fprintf(outputFile, "\n");
                    }

                    printf("information are printed succesfully \n");
                }
                fclose(outputFile);
            }

            else {

                printf("make sure that you have ");
                if (solved < 1)
                    printf("solved  ");
                if (converted < 1)
                    printf("converted to prefix ");
                if (checked < 1)
                    printf("checked for validity ");
                if (scanned < 1)
                    printf("scanned the file ");
                printf("\n");
            }
            break;


        case 7:
            printf("logging out... \n");
            break;


        default:
            printf("%d is an invalid task\n", choice);
            break;

        }
    } while (choice != 7);

    for (int i = 0;i < numOfEquations;i++) {
        if (validityEquationsIndexes == 1) {
            free(prefix[i]);
        }
    }
    printf("thanks for using equetion's master");

}