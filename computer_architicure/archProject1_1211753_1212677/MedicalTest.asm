.data

# arrays
patientIDs: .space 400   # array for patient IDs
testNames: .space 2000   # array for test names
testDates: .space 800    # array for test dates
testResults: .space 400  # array for result

#some messages and buffers
latterE: .asciiz "e" # latter e for use
mainMenu: .asciiz "\n0- load data\n1- Add a new medical test\n2- Search for a test by patient ID\n3- Retrieve all patient tests\n4- Retrieve all normal tests\n5- Retrieve all patient tests in a given specific period\n6- Searching for unnormal tests\n7- Average test value\n8- Update an existing test result\n9- Delete a test\n10- Save to file\n11- Exit\n\nYour choice: "

floatString: .space 32  # string of float

floatTen: .float 10.0   # float constant
floatZero:  .float 0. #  constant

bufferIntToString: .space 8    # buffer of int 

# some constants
floatValue0: .float 13.8 #  constant
floatValue1: .float 17.2 #  constant
floatValue2: .float 70.0 #  constant
floatValue3: .float 99.0 #  constant
floatValue4: .float 100.0 #  constant
floatValue5: .float 120.0 #  constant
floatValue6: .float 80.0 #  constant
hgbLabel:   .asciiz "HGB"  #  constant
bgtLabel:   .asciiz "BGT" #  constant 
ldlLabel:   .asciiz "LDL" #  constant
bptsLabel:  .asciiz "BPTS" #  constant
bptdLabel:  .asciiz "BPTD" #  constant

# buffers for dates
date1: .space 11       # buffer for first date 
date2: .space 11       # buffer for second date

#more messages and errors
idError: .asciiz "patient ID not found\n"
askForInput: .asciiz "enter a choice (0-11): "
invalidIn: .asciiz "invalid input. Please enter a number\n"
invalidNum: .asciiz "invalid number. Please enter a valid number\n"
nl: .asciiz "\n"
askForId: .asciiz "enter patient ID (int only): "
askForTestName: .asciiz "enter test name: "
askForTestDate: .asciiz "enter test date (YYYY-MM): "
askForTestResult:.asciiz "enter test result (nums only eg. 15.3 15 15.0 ...): "
ls: .asciiz "data loaded successfully \n"
askForTestIndex: .asciiz "enter index of test to update: "
askForTestIndexDelete: .asciiz "enter the index of the test to delete: "
successMsg: .asciiz "successful\n"  
space: .asciiz " "
filename: .asciiz "C:/Users/mrabd/Desktop/Arch_project/tests.txt"
buffer: .space 1024 # a buffer to use in code
fileDescriptor: .word 0 # for file stuff
index: .word 0 # to save the last index of the arrays
bufferFloatToString: .space 32
avarageHGB: .asciiz "avarage HGB tests' results:"
avarageBGT: .asciiz "avarage BGT tests' results:"
avarageLDL: .asciiz "avarage LDL tests' results:"
avarageBPTS: .asciiz "avarage BPTS tests' results:"
avarageBPTD: .asciiz "avarage BPTD tests' results:"
averageResults: .space 400 #  store average results for 100 tests
numEntries: .word 0             # total number of test entries
buffer0: .space 20             # input buffer
ss: .asciiz "data saved Successfully\n"  
colonSpace: .asciiz ": "
commaSpace : .asciiz ", "

# dealing with errors
flag0: .word 1            # flag to prevent reading file more than one time
em20: .asciiz "cant load data more than one time\n"
em21: .asciiz "id is invalid the id should contain 7 digits and take this shape : 'axxxxxx' where a cant be '0'\n"
em22: .asciiz "invalid test name only :'HGB,BGT,LDL,BPTS,BPTD' (upper case) are available\n"
em23: .asciiz "invalid date format! right format: 'yyyy-mm'\n"


askStratDate: .asciiz "enter start date (YYYY-MM): "
askEndDate: .asciiz "enter end date (YYYY-MM): "

errorMsg:    .asciiz "no tests to update.\n"
errorMsg1:    .asciiz "no tests to delete.\n"
noTestsFound: .asciiz "no tests found in this period\n"
fileNotFound: .asciiz "file error please check the file tests.txt or change the filename path"
admin: .asciiz "0000Admin"

#/*-------------------------- MAIN --------------------------*/



.text
.globl main

main:

# initialize array indices
    la $t6, testNames       # testNames
    la $t7, testDates       # testDates
    la $t8, patientIDs      # patientIDs
    la $t9, testResults     # testResults

 

menuLoop:

   # print menu
    la $a0, mainMenu        # load menu string
    li $v0, 4               # print string
    syscall
    
    
   # getting user input
    la $a0, askForInput  
    li $v0, 4
    syscall

    la $a0, buffer0       # load address of buffer to store input
    li $a1, 20           # number of characters to read
    li $v0, 8            # read a string
    syscall

    # vsalidate and convert input
    la $a0, buffer0       # pointer to the input buffer
    jal convertInput       # jump to input converting function
    move $t0, $v0         # take the result
    beq $t0, -1, error    # if -1 --> invalid else --> valid

    # switch statemtn
    li $t1, 0
    beq $t0, $t1, loadData
    li $t1, 1
    beq $t0, $t1, addTest
    li $t1, 2
    beq $t0, $t1, searchTest
    li $t1, 3
    beq $t0, $t1, retrieveAllTests
    li $t1, 4
    beq $t0, $t1, retrieveNormalTests
    li $t1, 5
    beq $t0, $t1, retrieveTestsPeriod
    li $t1, 6
    beq $t0, $t1, retrieveUnnormalTests
    li $t1, 7
    beq $t0, $t1, averageTestValue
    li $t1, 8
    beq $t0, $t1, updateTest
    li $t1, 9
    beq $t0, $t1, deleteTest
    li $t1, 10
    beq $t0, $t1, saveData
    li $t1, 11
    beq $t0, $t1, exitProgram

# dealling invalid input
    la $a0, invalidNum
    li $v0, 4
    syscall
    j menuLoop       #  back to loop



#/*-------------------------- FUNCTIONS --------------------------*


# making sure not to read file twice
loadData:
   
    jal openFile
     lw $s0,flag0
    beqz $s0 error20
    addi,$s0,$s0,-1
    sw $s0,flag0
        jal readFile
        j storeData
        returnStoreData:
            li $v0,4
            la $a0,ls
            syscall
    j menuLoop

addTest:
    jal addANewTest
    j menuLoop

searchTest:
    jal searchTestById
    j menuLoop

retrieveAllTests:
    jal printAllTests
    j menuLoop

retrieveNormalTests:
    j retrieveAllNormalTests
    back:
    j menuLoop

retrieveTestsPeriod:
    j retrieveTestsBetweenDates
returnDates:
    j menuLoop

retrieveUnnormalTests:
    j retrieveAllUnnormalTests
    back2:
    j menuLoop

averageTestValue:
    jal calculateAverages
    j menuLoop

updateTest:
    jal updateATest
    j menuLoop

deleteTest:
    jal deleteATest 
    j menuLoop

saveData:
    j printAllTestsToFile
    
    backSaveFile:
    li $v0,4
    la $a0,ss
    syscall
    j menuLoop

exitProgram:
    li $v0, 10          
    syscall

#/*-------------------------- LOAD_DATA --------------------------*/   

  
error:
    la $a0, invalidIn
    li $v0, 4
    syscall
    j menuLoop
 
convertInput:
    li $t0, 0           # $t0 will hold the integer result
    li $t1, 0           # $t1 is the index for string converting
    li $t3, 1           # $t3 is the sign multiplier +1  default
    lb $t2, buffer0($t1) # Lload  first char in buffer

    # check for negative sign at the start
    li $t4, '-'         
    beq $t2, $t4, negateSign
    j checkDigit        # if +, go to digit checking

negateSign:
    li $t3, -1          # set tsign multiplier to -1 
    addi $t1, $t1, 1    # skip the - character
    lb $t2, buffer($t1) # next character after -

checkDigit :
    lb $t2, buffer0($t1) # load  current byte

    # check if character is nl and skip 
    li $t4, 10          # nl
    beq $t2, $t4, skipNewline

    # check if end of string 
    li $t4, 0           #  end of string = 0
    beq $t2, $t4, convertEnd  # if end of string convert

    # check if char is digit
    li $t4, '0'         # 0
    blt $t2, $t4, invalidInput
    li $t4, '9'         # 9
    bgt $t2, $t4, invalidInput

    # convert to integer
    sub $t2, $t2, '0'   # convert to integer
    li $t5, 10
    mul $t0, $t0, $t5   # shift left
    add $t0, $t0, $t2   # adding the new digit to result

    # next 
    addi $t1, $t1, 1
    j checkDigit 

skipNewline:
    # skip nl
    addi $t1, $t1, 1
    j checkDigit 

convertEnd:
    mul $t0, $t0, $t3   #  sign
    move $v0, $t0       #  result in $v0
    jr $ra              # return 

invalidInput:
    li $v0, -1          # return -1 invalid 
    jr $ra              # return 



openFile:
    la $a0, filename      # filename
    la $a1, 0      # mode read
    li $v0, 13            #open file
    syscall
    bltz $v0, FDNE
    sw $v0, fileDescriptor  # store file descriptor
    jr $ra
 
 
 
 
#/********************************************************************/ 
#/*-------------------------- 0) LOAD_DATA --------------------------*/


    
storeData:
    li $s4,1
    la $a0, buffer                  # start of buffer
    li $s2,0

    #  arrays 
    la $t6, testNames    
    la $t7, testDates   
    la $t8, patientIDs   
    la $t9, testResults

loopStoreData: 
    lb $t5, 0($a0)
    li $t3, 0
    beq $t5, $t3, endStoreData      # If 0  end 
    
    # convert and store the patient ID
    li $t2, 0                       # reset 

convertId:
    lb $t1, 0($a0)                  # load byte byte
   
    li $t4, ':'                     # load :
    beq $t1, $t4, finishId          # end of ID 
    sub $t1, $t1, '0'               # convert to integer
    
    li $t5, 10
    mul $t2, $t2, $t5               # shift left 
    add $t2, $t2, $t1               # add new digit
    addi $a0, $a0, 1                # next
    j convertId

finishId:
    sw $t2, 0($t8)                  # store the convertd ID
    addi $t8, $t8, 4                # increment the ID array index
    addi $a0, $a0, 2                # skip  ": "
    
    li $t0, 31                      # counter 


    # find the address to store test name
    lw $t1, index                   # koad thecurrent index
    sll $t1, $t1, 5                 # shift left by 5 bits
    add $t1, $t6, $t1               # Add offset to the base address testNames

    # initialize counter 
    li $t2, 31                      # set numOf chars for the name

convertName:
    lb $t0, 0($a0)                  # aoad byte from name string
    beqz $t0, finishName            # if  null terminator end 
    beq $t0, ',', finishName        # if comma end
    beq $t2, $zero, finishName      # If > 31 chars end
    sb $t0, 0($t1)                  # store in testNames
    addi $t1, $t1, 1                #  testNames++
    addi $a0, $a0, 1                #  bufferAddress++
    addi $t2, $t2, -1               # charCount--
    j convertName                   # next

finishName:
    sb $zero, 0($t1)                # add (\0)
    addi $t1, $t1, 1                # skip  null terminator

    li $t3, 32                      # space for  name + (\0)
    sub $t3, $t3, $t2               # find how many bytes we need to skip to reach 32
    add $t1, $t1, $t3
    addi $a0, $a0, 2                # skip ", "



convertDate2:
    lb $t1, 0($a0)                  # lpad byte byte
    li $t4, ','
    beq $t1, $t4, finishDate        # end  
    sb $t1, 0($t7)
    addi $a0, $a0, 1
    addi $t7, $t7, 1
    j convertDate2

finishDate:
    sb $zero, 0($t7)                #   string+ \0
    addi $t7, $t7, 1                # newxt
    addi $a0, $a0, 2                # skip ", "


convertFloat:
    li $t1, 0                       # integer part 
    li $t2, 0                       # decimal part 
    li $t3, 0                       # decimal digits counter 

convertInteger:
    lb $t0, 0($a0)                  # load byte byte
    li $t5, '.'
    beq $t0, $t5, convertDecimalPart    # if '.' we go to decimal 
    li $t5, 0
    beq $t0, $t5, setupFloatingPoint    # if \0 go to floating point 
    sub $t0, $t0, '0'               # convert integer
    li $t5, 10
    mul $t1, $t1, $t5               # shift left
    add $t1, $t1, $t0               # add new digit
    addi $a0, $a0, 1                #  bufferPointer++
    j convertInteger

convertDecimalPart:
    addi $a0, $a0, 1                # skip '.'

convertDecimal:
    lb $t0, 0($a0)
    
    li $t5,'e'
    beq $t0, $t5, setupFloatingPoint # if \0 go to floating point 
    
    sub $t0, $t0, '0'               # convert integer
    
    mul $t2, $t2, 10                # sl
    add $t2, $t2, $t0               # add new digit
    
    addi $t3, $t3, 1                # decimal counter ++
    addi $a0, $a0, 1                # bufferPointer++
   
    j convertDecimal

setupFloatingPoint:
    mtc1 $t1, $f0                   # move integer part to float register
    cvt.s.w $f0, $f0                # convert integer  to float
    
    # check if there is a decimal part
    beqz $t2, fpe                   # if no decimal part, go to to store result

    #convert decimal part
    mtc1 $t2, $f1                   # move decimal part to float register
    cvt.s.w $f1, $f1                # convert decimal part to float


    li $t4, 10
    mtc1 $t4, $f2                   # load constant 10 into $f2
    cvt.s.w $f2, $f2                # convert to float

    
    # load the 10.0 value 1 into register $f2
    li $t0, 10          
    mtc1 $t0, $f2
    cvt.s.w $f2, $f2   
    


scaleDecimal:
    div.s $f1, $f1, $f2             # divide  by 10
    mul.s $f2, $f2, $f2             # scale divisor 
    addi $t3, $t3, -1               # decimal count --
    bgtz $t3, scaleDecimal          # continue scaling

fpe:

    add.s $f0, $f0, $f1             # add the integer and decimal parts

    # store the float result
    la $t0, index                   # load index
    lw $t1, 0($t0)                  # move it to $t1
    la $t2, testResults             # load array
    sll $t3, $t1, 2                 # mltiply the index by 4 
    add $t2, $t2, $t3               # add the shf9t
    swc1 $f0, 0($t2)                # store the float value

    #reset float registers
    sub.s $f0, $f0, $f0
    sub.s $f1, $f1, $f1
    sub.s $f2, $f2, $f2

    # update  index
    addi $t1, $t1, 1
    sw $t1, 0($t0)          

    addi $a0, $a0, 2                # skip e and new line
    
    # update  numEntites
    lw $s0, numEntries         
    addi $s0, $s0, 1           
    sw $s0, numEntries         
    

    j loopStoreData                 # next
    
endStoreData:
    j returnStoreData               # return
    
    #reacheddd

readFile:

    lw $a0, fileDescriptor          # Load file descriptor
    la $a1, buffer                  # Buffer to read data into
    li $a2, 1024                    # Number of bytes to read (assuming each line does not exceed this)
    li $v0, 14                      # Syscall for reading from file

    syscall                         # Perform the read syscall
    
    beqz $v0, endRead               # If no bytes read, reach end of file (v0 is the number of bytes read)
    move $t0, $v0                   # Save the number of bytes read
    sb $zero, buffer($t0)           # Null-terminate the string in buffer

endRead:
    # Close the file after reading all lines
    li $v0, 16                      # Syscall for closing a file
    lw $a0, fileDescriptor          # Load file descriptor
    syscall                         # Close the file

    jr $ra                          # Return from function
    
 
       
#/*-------------------------- 1) ADD_TEST --------------------------*/


addANewTest:

#sss
retakeID:
    la $t6, testNames               # estNames
    la $t7, testDates               # estDates
    la $t8, patientIDs              # patientIDs
    la $t9, testResults             # testResults
    
    
    #  ask for and store patient ID

    li $v0, 4
    la $a0, askForId
    syscall
    li $v0, 5                       # read integer
    syscall
    
    # check if the id is valid
    
    
    li $t2, 1000000                 # lower 
    li $t3, 9999999                 # upper 
    blt $v0, $t2, error21           #  number is less than 1000000 jump to error
    bgt $v0, $t3, error21           #  number is greater than 9999999 jump to error
    lw $t1, index                   #  current index
    sll $t1, $t1, 2
    add $t1, $t1, $t8
    sw $v0, 0($t1)                  # store  ID in the array

    # ask for and store test name

    retakeName:
    li $v0, 4
    la $a0, askForTestName
    syscall
    
    li $v0, 8                       # Syscall to read string
    la $a0, buffer                  # Use buffer to store input
    li $a1, 1024                    # Length of buffer
    syscall
    
    # Assume $a0 initially points to the start of the buffer where "RBC\n" is stored

    li $t2, 1024                    # Length of the buffer
    add $t3, $a0, $t2               # Calculate the end address of the buffer

findEnd:
    lb $t0, 0($a0)                  # Load a byte from the buffer
    beq $t0, $zero, checkNewLine    # If it is '\0' (end of string), check for newline before it
    addi $a0, $a0, 1                # Increment buffer address
    b findEnd                       # Loop until end of string is found

checkNewLine:
    addi $a0, $a0, -1               # Step back to last non-null character (likely '\n')
    lb $t0, 0($a0)                  # Load this character
    li $t1, 10                      # ASCII value of '\n'
    bne $t0, $t1, end               # If it's not a newline, no need to trim, go to end
    sb $zero, 0($a0)                # Replace '\n' with '\0' to trim the newline

end:
    #let's check validity
    move $s1,$a1
    move $s2,$a2
    
    la $a1, buffer                  # Load address of the buffer
    la $a2, hgbLabel                # Load address of the first string
    jal strcmp                      # Call strcmp function
    beq $v0, 0, valid20             # If return is 0, strings are equal

    la $a2, bgtLabel                # Check next string
    jal strcmp
    beq $v0, 0, valid20

    la $a2, ldlLabel
    jal strcmp
    beq $v0, 0, valid20

    la $a2, bptsLabel
    jal strcmp
    beq $v0, 0, valid20

    la $a2, bptdLabel
    jal strcmp
    beq $v0, 0, valid20

move $a1,$s1
move $a2,$s2

    j error22
    
valid20:   
    # Continue with your code
    lw $t1, index                   # Load index
    sll $t1, $t1, 5                 # Assume each name can take up to 32 bytes
    add $t1, $t1, $t6               # Calculate address in testNames
    la $t2, buffer
    li $t3, 32
    
copyNameLoop:
    lb $t0, 0($t2)
    beqz $t0, endCopyName
    sb $t0, 0($t1)
    addi $t1, $t1, 1
    addi $t2, $t2, 1
    addi $t3, $t3, -1
    bnez $t3, copyNameLoop
    
endCopyName:
    sb $zero, 0($t1)
    
    
    # 3) Ask for and store test date
    retakeDate:
    li $v0, 4
    la $a0, askForTestDate
    syscall
    li $v0, 8
    la $a0, buffer
    li $a1, 11                      # YYYY-MM plus null terminator
    syscall
    
    # Assume $a0 initially points to the start of the buffer where "RBC\n" is stored
    li $t2, 1024                    # Length of the buffer
    add $t3, $a0, $t2               # Calculate the end address of the buffer

findEnd2:
    lb $t0, 0($a0)                  # Load a byte from the buffer
    beq $t0, $zero, checkNewLine2   # If it is '\0' (end of string), check for newline before it
    addi $a0, $a0, 1                # Increment buffer address
    b findEnd2                      # Loop until end of string is found
                                            
checkNewLine2:
    addi $a0, $a0, -1               # Step back to last non-null character (likely '\n')
    lb $t0, 0($a0)                  # Load this character
    li $t1, 10                      # ASCII value of '\n'
    bne $t0, $t1, end2              # If it's not a newline, no need to trim, go to end
    sb $zero, 0($a0)                # Replace '\n' with '\0' to trim the newline

end2:
#validity testing
    la $a0, buffer                  # Address of the buffer
 
    move $s0,$t0
    move $s1,$t1
    move $s2,$t2
    move $s3,$t3
    move $s4,$t4
    move $s5,$t5
    jal checkDateFormat             # Jump to the date format checking function
    move $t0,$s0
    move $t1,$s1
    move $t2,$s2
    move $t3,$s3
    move $t4,$s4
    move $t5,$s5   
    
 
    beq $v0, 0, valid21             # If return value is 0, date is valid
    j error23                       # Otherwise, jump to error


valid21:
    # Continue with your code
    lw $t1, index
    sll $t1, $t1, 3                 # Each date can take up to 8 bytes
    add $t1, $t1, $t7               # Calculate address in testDates
    la $t2, buffer
    li $t3, 8
    
copyDateLoop:
    lb $t0, 0($t2)
    beqz $t0, endCopyDate
    sb $t0, 0($t1)
    addi $t1, $t1, 1
    addi $t2, $t2, 1
    addi $t3, $t3, -1
    bnez $t3, copyDateLoop
    
endCopyDate:
    sb $zero, 0($t1)
    
    
    # 4) Ask for and store test result (assuming it is a float number)
    li $v0, 4
    la $a0, askForTestResult
    syscall
    li $v0, 6                       # Syscall to read float
    syscall
    lw $t1, index
    sll $t1, $t1, 2                 # Each result is a float, 4 bytes
    add $t1, $t1, $t9               # Calculate address in testResults
    swc1 $f0, 0($t1)                # Store the float result

    # Increment the index for the next entry
    lw $t0, index
    addi $t0, $t0, 1
    sw $t0, index
    
    lw $t0, numEntries              # Load the current number of entries
    addi $t0, $t0, 1                # Increment the count
    sw $t0, numEntries              # Store the updated count back to memory
    
    # Return to the main menu or caller
    j returnStoreData

# eee



#/*-------------------------- 2) SEARCH_TEST --------------------------*/



# Function to search for a test by patient ID
searchTestById:
    # Ask for and read the patient ID
    li $v0, 4
    la $a0, askForId
    syscall

    li $v0, 5                       # Syscall to read integer
    syscall
    move $t2, $v0                   # Move entered patient ID to $t2

    # Prepare to loop through patient IDs
    lw $t0, numEntries              # Load number of entries
    li $t1, 0                       # Initialize index to 0
    la $t3, patientIDs              # Base address of patientIDs

loopSearch:
    # Check if we've checked all entries
    beq $t1, $t0, endSearch2        # If index == numEntries, ID not found

    # Load the current patient ID from array
    lw $a0, 0($t3)
    beq $a0, $t2, foundMatch        # If current ID matches entered ID, go to found_match

    # Increment index and address for the next ID
    addi $t1, $t1, 1                # Increment index
    addi $t3, $t3, 4                # Move to the next patient ID
    j loopSearch                    # Repeat the loop

foundMatch:
    # Print the test details using the current index $t1
    li $v0, 4
    la $a0, nl
    syscall
    
    # Print the Patient ID
    li $v0, 1                       # syscall to print integer
    move $a0, $t2                   # $t2 contains the patient ID entered by user which matches
    syscall

    li $v0,4
    la $a0,space
    syscall
    
    

    # Print Test Name
    li $v0, 4
    sll $t4, $t1, 5                 # Calculate offset for test name (assuming 32 bytes per name)
    la $t5, testNames
    add $t5, $t5, $t4
    la $a0, 0($t5)                  # Address of the test name
    syscall
    
    li $v0,4
    la $a0,space
    syscall

    # Print Test Date
    li $v0, 4
    sll $t4, $t1, 3                 # Calculate offset for test date (assuming 8 bytes per date)
    la $t5, testDates
    add $t5, $t5, $t4
    la $a0, 0($t5)                  # Address of the test date
    syscall
    
    li $v0,4
    la $a0,space
    syscall

    # Print Test Result
    li $v0, 2
    sll $t4, $t1, 2                 # Calculate offset for test result (assuming 4 bytes per result)
    la $t5, testResults
    add $t5, $t5, $t4
    lwc1  $f12, 0($t5)              # Load test result
    syscall
    
    li $v0, 4
    la $a0, nl
    syscall
    

    j endSearch

endSearch:
    jr $ra                          # Return from the subroutine



endSearch2:
    # Print a message indicating that the ID was not found
    li $v0, 4                 # syscall to print string
    la $a0, idError  # Load the address of the "ID not found" message
    syscall

    jr $ra                    # Return from the subroutine
    
  
    

#/*-------------------------- 3) RETRIEVE_ALL_TESTS --------------------------*/


printAllTests:
    lw $t0, numEntries              # Load number of entries
    li $t1, 0                       # Initialize index to 0

    la $t2, patientIDs              # Base address of patientIDs
    la $t3, testNames               # Base address of testNames
    la $t4, testDates               # Base address of testDates
    la $t5, testResults             # Base address of testResults

loopPrint:
    # Check if we've printed all entries
    beq $t1, $t0, endPrint          # If index == numEntries, we are done

    # Load and print the patient ID
    lw $a0, 0($t2)                  # Load patient ID
    li $v0, 1                       # syscall to print integer
    syscall

    li $v0,4
    la $a0,space
    syscall
    
    # Print the test name
    la $a0, 0($t3)                  # Address of the test name
    li $v0, 4                       # syscall to print string
    syscall
    
    
    li $v0,4
    la $a0,space
    syscall

    # Print the test date
    la $a0, 0($t4)                  # Address of the test date
    syscall
    
    la $a0,space
    syscall

    # Load and print the test result (assuming it's a float for illustration)
    lwc1  $f12, 0($t5)              # Load test result
    li $v0, 2                       # syscall to print float
    syscall
    
    li $v0,4
    la $a0, nl
    syscall

    # Increment index and addresses for the next entry
    addi $t1, $t1, 1                # Increment index
    addi $t2, $t2, 4                # Move to the next patient ID
    addi $t3, $t3, 32               # Move to the next test name
    addi $t4, $t4, 8                # Move to the next test date
    addi $t5, $t5, 4                # Move to the next test result

    j loopPrint                     # Repeat the loop
	
endPrint:
    jr $ra                          # Return from the subroutine
 
        
             
#/*-------------------------- 4) NORMAL_TEST --------------------------*/ 



# Function to retrieve and display normal tests
retrieveAllNormalTests:
    lw $t0, numEntries              # Load the number of test entries
    li $t1, 0                       # Initialize index to 0
    la $t2, testNames               # Base address of testNames
    la $t3, testDates               # Base address of testDates
    la $t4, patientIDs              # Base address of patientIDs
    la $t5, testResults             # Base address of testResults

    sll $t6, $t1, 5                 # Each name takes up 32 bytes; shift left by 5 is equivalent to t1 * 32
    add $a0, $t2, $t6               # Add this offset to the base address of testNames


loopRetrieveNormal:

    beq  $t1, $t0, endRetrieveNormal    # If index  numEntries, we are done
    
    #meh
    








 la $a1, admin                # Address of "BGT" label for comparison
     move $s3,$a0              
    move $a0,$a1
    li $v0 , 4
   # syscall
    move $a0,$s3 

        move $s0,$t0
    move $s1,$t1
    jal compareStrings
     move $t0,$s0
    move $t1,$s1
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBGT        # If matched with "BGT", check BGT range




















    la $a1, bgtLabel                # Address of "BGT" label for comparison
     move $s3,$a0              
    move $a0,$a1
    li $v0 , 4
   # syscall
    move $a0,$s3 

        move $s0,$t0
    move $s1,$t1
    jal compareStrings
     move $t0,$s0
    move $t1,$s1
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBGT        # If matched with "BGT", check BGT range









  la $a1, hgbLabel   # Address of "Hgb" label for comparison
    move $s3,$a0              
    move $a0,$t2
    li $v0 , 4
   # syscall
    move $a0,$s3 
    
    move $s0,$t0
    move $s1,$t1
    jal compareStrings              # Jump to string comparison function
      move $t0,$s0
     move $t1,$s1
    add $a0, $t2, $t6               # Add this offset to the base address of testNames    
    beq $v0, $zero, checkHGB        # If matched with "Hgb", check Hgb range




























    la $a1, ldlLabel                # Address of "LDL" label for comparison
    
     move $s3,$a0              
    move $a0,$a1
    li $v0 , 4
  # syscall
    move $a0,$s3 
    
    
        move $s0,$t0
    move $s1,$t1
    jal compareStrings
     move $t0,$s0
    move $t1,$s1
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkLDL        # If matched with "LDL", check LDL range

    
    la $a1, bptsLabel               # Address of "BPTS" label for comparison
    
     move $s3,$a0              
    move $a0,$a1
    li $v0 , 4
   # syscall
    move $a0,$s3 
    
    
    
        move $s0,$t0
    move $s1,$t1
    jal compareStrings
     move $t0,$s0
    move $t1,$s1
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBPTS       # If matched with "BPTS", check BPTS range

   
    la $a1, bptdLabel               # Address of "BPTD" label for comparison
    
     move $s3,$a0              
    move $a0,$a1
    li $v0 , 4
   # syscall
    move $a0,$s3 
    
        move $s0,$t0
    move $s1,$t1
    jal compareStrings
     move $t0,$s0
    move $t1,$s1
    
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBPTD       # If matched with "BPTD", check BPTD range
    j skipPrint                     # Skip printing if no conditions met

checkBGT:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue2
    l.s $f2, floatValue3
    c.le.s $f1, $f0
    bc1f skipPrint
    c.lt.s $f0, $f2
    bc1f skipPrint
    j printDetails


checkHGB:
    lwc1 $f0, 0($t5)                # Load the test result (float)
    l.s $f1, floatValue0            # Lower bound for Hgb
    l.s $f2, floatValue1            # Upper bound for Hgb
    c.le.s $f1, $f0                 # Check if result < 13.8
    bc1f skipPrint              # If true, print details because it's less than normal
    c.lt.s $f0, $f2                 # Check if result > 17.2
    bc1f skipPrint              # If true, print details because it's greater than normal
    j printDetails                   # Jump to print if in range
    
    

    
checkLDL:
    lwc1 $f0, 0($t5)                # Load word from memory into $f0
    l.s $f1, floatValue4            # Load float constant into $f1
    c.lt.s $f0, $f1                 # Compare $f0 < $f1
    bc1t printDetails               # If $f0 < $f1 is true, skip the next instruction
    j skipPrint                     # Jump to skip_print if $f0 >= $f1  

checkBPTS:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue5
    c.lt.s $f0, $f1
    bc1f skipPrint
    j printDetails

checkBPTD:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue6
    c.lt.s $f0, $f1
    bc1f skipPrint
    j printDetails


printDetails:
    # Load and print the patient ID
    lw $a0, 0($t4)                  # Load patient ID
    li $v0, 1                       # syscall to print integer
    syscall

    li $v0,4
    la $a0,space
    syscall
    
    # Print Test Name
    move $a0, $t2                   # Move the address of the Test Name string into $a0
    li $v0, 4                       # Load syscall code for printing string
    syscall                         # Execute syscall to print the string
    
    
    li $v0,4
    la $a0,space
    syscall

    # Print the test DATE
    la $a0, 0($t3)                  # Address of the test DATE
    li $v0, 4                       # syscall to print string
    syscall
    
    la $a0,space
    syscall

    # Load and print the test result (assuming it's a float for illustration)
    lwc1  $f12, 0($t5)              # Load test result
    li $v0, 2                       # syscall to print float
    syscall
    
    li $v0,4
    la $a0, nl
    syscall
    

skipPrint:
    # Increment pointers and index for next loop iteration
    addi $t1, $t1, 1                # Increment index
    addi $t2, $t2, 32               # Move to the next test name
    addi $t3, $t3, 8                # Move to the next test date (Should be 8 bytes, not 10?)
    addi $t4, $t4, 4                # Move to the next patient ID
    addi $t5, $t5, 4                # Move to the next test result
    
    
    
    j loopRetrieveNormal            # Jump back to start of loop


endRetrieveNormal:
    j back                          # Return from the subroutine

compareStrings:


    compareLoop:                    # Start of the loop to compare characters
        lb $t0, 0($a0)              # Load a byte from the first string
        lb $t1, 0($a1)              # Load a byte from the second string
        addiu $a0, $a0, 1           # Increment the address of the first string
        addiu $a1, $a1, 1           # Increment the address of the second string
        bne $t0, $t1, stringsNotEqual  # If bytes are not equal, jump to strings_not_equal
        beqz $t0, stringsEqual      # If end of string (null-byte), strings are equal
        j compareLoop               # Continue comparing next characters

    stringsEqual:                   # Label for strings are equal
        li $v0, 0                   # Set return value to 0 (strings are equal)
        j restoreAndReturn          # Jump to restore registers and return

    stringsNotEqual:                # Label for strings are not equal
        li $v0, 1                   # Set return value to 1 (strings are not equal)

    restoreAndReturn:               


        jr $ra                      # Return from the function
        # Return from the function




#/*-------------------------- 5) UNNORMAL_TEST --------------------------*/ 



# Function to retrieve and display unnormal tests
retrieveAllUnnormalTests:
    lw $t0, numEntries              # Load the number of test entries
    li $t1, 0                       # Initialize index to 0
    la $t2, testNames               # Base address of testNames
    la $t3, testDates               # Base address of testDates
    la $t4, patientIDs              # Base address of patientIDs
    la $t5, testResults             # Base address of testResults

    sll $t6, $t1, 5                 # Each name takes up 32 bytes; shift left by 5 is equivalent to t1 * 32
    add $a0, $t2, $t6               # Add this offset to the base address of testNames


loopRetrieveUnnormal:
    beq  $t1, $t0, endRetrieveUnnormal   # If index  numEntries, we are done
   
   
    la $a1, admin                # Address of "Hgb" label for comparison
    jal compareStrings2             # Jump to string comparison function
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkHGB2       # If matched with "Hgb", check Hgb range
   
   
   
    # Calculate the address of the current test name based on index
    la $a1, hgbLabel                # Address of "Hgb" label for comparison
    jal compareStrings2             # Jump to string comparison function
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkHGB2       # If matched with "Hgb", check Hgb range


    la $a1, bgtLabel                # Address of "BGT" label for comparison
    jal compareStrings2
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBGT2       # If matched with "BGT", check BGT range

    
    la $a1, ldlLabel                # Address of "LDL" label for comparison
    jal compareStrings2
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkLDL2       # If matched with "LDL", check LDL range


    la $a1, bptsLabel               # Address of "BPTS" label for comparison
    jal compareStrings2
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBPTS2      # If matched with "BPTS", check BPTS range


    la $a1, bptdLabel               # Address of "BPTD" label for comparison
    jal compareStrings2
    add $a0, $t2, $t6               # Add this offset to the base address of testNames
    beq $v0, $zero, checkBPTD2      # If matched with "BPTD", check BPTD range

    j skipPrint2                    # Skip printing if no conditions met


   checkHGB2:
    lwc1 $f0, 0($t5)                # Load the test result (float)
    l.s $f1, floatValue0            # Lower bound for Hgb
    l.s $f2, floatValue1            # Upper bound for Hgb
    c.lt.s $f0, $f1                 # Check if result < 13.8
    bc1t printDetails2              # If true, print details because it's less than normal
    c.le.s $f2, $f0                 # Check if result > 17.2
    bc1f skipPrint2                 # If true, print details because it's greater than normal
    
    j printDetails2                    # Jump to print if in range
    
    
    
    
 

checkBGT2:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue2
    l.s $f2, floatValue3
    c.le.s $f1, $f0
    bc1f printDetails2
    c.lt.s $f0, $f2
    bc1f printDetails2
    j skipPrint2
  
    
checkLDL2:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue4
    c.le.s $f1, $f0
    bc1f skipPrint2
    j printDetails2

checkBPTS2:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue5
    c.le.s $f1, $f0
    bc1f skipPrint2
    j printDetails2

checkBPTD2:
    lwc1 $f0, 0($t5)
    l.s $f1, floatValue6
    c.le.s $f1 $f0
    bc1f skipPrint2
    j printDetails2

printDetails2:
    # Load and print the patient ID
    lw $a0, 0($t4)                  # Load patient ID
    li $v0, 1                       # syscall to print integer
    syscall

    li $v0,4
    la $a0,space
    syscall
    
    # Print Test Name
    move $a0, $t2                   # Move the address of the Test Name string into $a0
    li $v0, 4                       # Load syscall code for printing string
    syscall                         # Execute syscall to print the string
    
    li $v0,4
    la $a0,space
    syscall

    # Print the test DATE
    la $a0, 0($t3)                  # Address of the test DATE
    li $v0, 4                       # syscall to print string
    syscall
    
    la $a0,space
    syscall

    # Load and print the test result (assuming it's a float for illustration)
    lwc1  $f12, 0($t5)              # Load test result
    li $v0, 2                       # syscall to print float
    syscall
    
    li $v0,4
    la $a0, nl
    syscall
    

skipPrint2:
    # Increment pointers and index for next loop iteration
    addi $t1, $t1, 1                # Increment index
    
    addi $t2, $t2, 32               # Move to the next test name
    addi $t3, $t3, 8                # Move to the next test date (Should be 8 bytes, not 10?)
    addi $t4, $t4, 4                # Move to the next patient ID
    addi $t5, $t5, 4                # Move to the next test result
    
	j loopRetrieveUnnormal          # Jump back to start of loop


endRetrieveUnnormal:
    j back2   


compareStrings2:
    pushRegisters2:                 # Save registers that will be modified
        addi $sp, $sp, -12          # Allocate space on the stack for three registers
        sw $ra, 8($sp)              # Save the return address at the highest address
        sw $t0, 4($sp)              # Save $t0
        sw $t1, 0($sp)              # Save $t1, which is crucial as it's modified here

    compareLoop2:                   # Start of the loop to compare characters
        lb $t0, 0($a0)              # Load a byte from the first string
        lb $t1, 0($a1)              # Load a byte from the second string
        addiu $a0, $a0, 1           # Increment the address of the first string
        addiu $a1, $a1, 1           # Increment the address of the second string
        bne $t0, $t1, stringsNotEqual2  # If bytes are not equal, jump to strings_not_equal
        beqz $t0, stringsEqual2     # If end of string (null-byte), strings are equal
        j compareLoop2              # Continue comparing next characters

    stringsEqual2:                  # Label for strings are equal
        li $v0, 0                   # Set return value to 0 (strings are equal)
        j restoreAndReturn2         # Jump to restore registers and return

    stringsNotEqual2:               # Label for strings are not equal
        li $v0, 1                   # Set return value to 1 (strings are not equal)

    restoreAndReturn2:              # Restore registers and return from function
        lw $ra, 8($sp)              # Restore the return address
        lw $t0, 4($sp)              # Restore $t0
        lw $t1, 0($sp)              # Restore $t1
        addi $sp, $sp, 12           # Deallocate space on the stack
        jr $ra                      # Return from the function
    # Return from the function




#/*-------------------------- 6) PERIOD_TEST --------------------------*/ 




retrieveTestsBetweenDates:
   
    reAskDate1: 
        li $t9,0
        li $v0, 4
        la $a0, askStratDate
        syscall


        # Read the first date
        li $v0, 8
        la $a0, buffer              # Temporary buffer for input
        li $a1, 1024                # Buffer size
        syscall
        
        li $a1, 7
        jal checkDateFormat1
        beq $v0, $zero, afd 

    # Copy first date to a safer location
    la $t0, buffer                  # Address of buffer
    la $t1, date1                   # Address to store first date safely
    li $t2, 7                       # Copy 7 characters for YYYY-MM
    move $t3, $zero                 # Counter for loop

copyDate1:
    lb $t4, 0($t0)                  # Load byte from buffer
    sb $t4, 0($t1)                  # Store byte in date1
    addi $t0, $t0, 1                # Increment source pointer
    addi $t1, $t1, 1                # Increment destination pointer
    addi $t3, $t3, 1                # Increment counter
    blt $t3, $t2, copyDate1         # Loop if not done
    sb $zero, 0($t1)                # Set null terminator right after the last copied character



    # Prompt and read the second date

reAskDate2:
    li $v0, 4
    la $a0, askEndDate
    syscall


    # Read the second date
    li $v0, 8
    la $a0, buffer                  # Reuse buffer for second input        
    li $a1, 1024                    # Buffer size
    syscall

    # Validate the second date format YYYY-MM
    la $a0, buffer                  # Address of buffer
    li $a1, 10                      # Correct length for "YYYY-MM"        
    jal checkDateFormat1            # Jump to the date validation function 
    beq $v0, $zero, asd             # If validation fails, prompt again

    # Copy second date to a safer location
    la $t0, buffer                  # Address of buffer
    la $t1, date2                   # Address to store second date safely
    li $t2, 7                       # Copy 7 characters for YYYY-MM
    move $t3, $zero                 # Counter for loop
    #sb $zero, 0($t1)               # After each copy loop for date1 and date2

copyDate2:
    lb $t4, 0($t0)                  # Load byte from buffer
    sb $t4, 0($t1)                  # Store byte in date2
    addi $t0, $t0, 1                # Increment source pointer
    addi $t1, $t1, 1                # Increment destination pointer
    addi $t3, $t3, 1                # Increment counter
    blt $t3, $t2, copyDate2         # Loop if not done
    sb $zero, 0($t1)                # Set null terminator right after the last copied character

   # Now, iterate over stored tests and compare dates
    lw $t0, numEntries              # Load number of entries
    li $t1, 0                       # Initialize index to 0
    la $t2, testDates               # Base address of testDates
 
         
loopDates:
    beq $t1, $t0, endDateLoop       # If index == numEntries, we are done
    la $a0, 0($t2)                  # Load address of the current date in the array
    la $a1, date1                   # Address of the first date
    la $a2, date2                   # Address of the second date
    
    move $s1, $t1                   # Save $t1 to $s1 before the call
    move $s4,$t0
    move $s3,$t2
    j compareDates                  # Call function to compare dates

    
returnCompareDates:
    move $t2,$s3
    move $t0,$s4
    move $t1, $s1                   # Restore $t1 from $s1 after the call
    
    beqz $v0, skipPrintDate         # If the date is not within range, skip printing

    # If within range, print details
    move $a0, $t1                   # Pass the index to print_details_period
      
    move $s3 , $t2
    jal printDetailsPeriod 
    addi $t9,$t9,1
    move $t2 , $s3
	

    
skipPrintDate:
    addi $t1, $t1, 1                # Increment index
    addi $t2, $t2, 8                # Move to the next date in the array
    j loopDates                     # Loop back


endDateLoop:
    beqz $t9,printEmpty

ff:
    j returnDates                   # Return from the function
    
printEmpty:
    la $a0,noTestsFound
    li $v0,4
    syscall

    j ff


#/*---compare---*/
# Function: compare_dates
# Description: Checks if a date is within the specified range.
# Arguments:
#   $a0 - address of the date to compare
#   $a1 - address of the start date
#   $a2 - address of the end date
# Returns:
#   $v0 - 1 if the date is within range, 0 otherwise

compareDates:

    # 1) current date
    #Parse the current date
    jal parseDate                   # Call parse_date for current date
    move $t0, $v0                   # Store parsed current date in $t0  
     
    # 2) start date     
    #parse the start date
    move $a0, $a1                   #  Move start date to $a0   
    jal parseDate                   # Call parse_date for start date
    move $t1, $v0                   # Store parsed start date in $t1
 
    move $s0, $t1                   # Save $t1 to $s0 before the call
    
    # 3) End date 
    # Parse the end date
    move $a0, $a2                   # Move end date to $a0
    jal parseDate                   # Call parse_date for end date
    
    move $t1, $s0                   # Restore $t1 from $s0 after the call
    move $t2, $v0                   # Store parsed end date in $t2


    # Compare parsed dates
    
    slt $t3, $t1, $t0               # Check if start date < current date
    slt $t4, $t0, $t2               # Check if current date < end date
    and $v0, $t3, $t4               # Both conditions must be true
    j returnCompareDates            # Return
    
   

# Function: parse_date
# Description: Parses a date string "YYYY-MM" into an integer YYYYMM
# Arguments:
#   $a0 - address of the date string
# Returns:
#   $v0 - parsed integer

parseDate:
    li $v0, 0                       # Clear $v0 for result
    li $t1, 10000                   # Multiplier for year part
    lb $t2, 0($a0)                  # Load first digit of year
    lb $t3, 1($a0)                  # Load second digit of year
    lb $t4, 2($a0)                  # Load third digit of year
    lb $t5, 3($a0)                  # Load fourth digit of year
    lb $t6, 5($a0)                  # Load first digit of month
    lb $t7, 6($a0)                  # Load second digit of month


    # Convert characters to integers and form the integer date
    subi $t2, $t2, '0'
    subi $t3, $t3, '0'
    subi $t4, $t4, '0'
    subi $t5, $t5, '0'
    subi $t6, $t6, '0'
    subi $t7, $t7, '0'
    

    # Calculate year
    mul $t2, $t2, 1000
    mul $t3, $t3, 100
    mul $t4, $t4, 10
    add $t2, $t2, $t3
    add $t2, $t2, $t4
    add $t2, $t2, $t5
    mul $t2, $t2, 100               # Shift year left for MM placement
    
    
   
    # Calculate month
    mul $t6, $t6, 10
    add $t6, $t6, $t7

    # Form final date
    add $v0, $t2, $t6               # YYYYMM

    jr $ra                          # Return



#/*---PRINT---*/


printDetailsPeriod:

    # Assume $a0 holds the index $t1
    la $t2, patientIDs              # Base address of patientIDs
    la $t3, testNames               # Base address of testNames
    la $t4, testDates               # Base address of testDates
    la $t5, testResults             # Base address of testResults

    sll $t6, $a0, 2                 # Calculate offset for patient ID (4 bytes per ID)
    add $t2, $t2, $t6               # Address of the current patient ID
    
    sll $t6, $a0, 5                 # Calculate offset for test name (32 bytes per name)
    add $t3, $t3, $t6               # Address of the current test name
    
    sll $t6, $a0, 3                 # Calculate offset for test date (8 bytes per date)
    add $t4, $t4, $t6               # Address of the current test date
    
    sll $t6, $a0, 2                 # Calculate offset for test result (4 bytes per result)
    add $t5, $t5, $t6               # Address of the current test result

    # Print the patient ID
    lw $a0, 0($t2)
    li $v0, 1
    syscall

    # Print a space after ID
    la $a0, space
    li $v0, 4
    syscall

    # Print the test name
    la $a0, 0($t3)
    li $v0, 4
    syscall
    
    
    li $v0,4
    la $a0,space
    syscall
    

    # Print the test date
    la $a0, 0($t4)
    syscall
    
    li $v0,4
    la $a0,space
    syscall

    # Print the test result (assuming it's a float)
    lwc1 $f12, 0($t5)
    li $v0, 2
    syscall

    # Print a new line
    li $v0,4
    la $a0, nl
    syscall

    jr $ra
     
               
#/*-------------------------- 7) UPDATE_TEST --------------------------*/ 



# Assuming all required data is correctly initialized and relevant system calls are set up

# Start of the updateATest routine
updateATest:
    li $v0, 4                       # syscall to print string
    la $a0, nl                      # Print a newline for better formatting
    syscall

    lw $t0, numEntries              # Load the total number of test entries
    beq $t0, $zero, errorToMenu     # If numEntries is 0, jump to error handling
    li $t1, 0                       # Initialize index counter to 0
    
    la $s2, patientIDs              # Base address of patientIDs
    la $s3, testNames               # Base address of testNames
    la $s4, testDates               # Base address of testDates
    la $s5, testResults             # Base address of testResults

# Print existing tests with an index number
loopPrintTests:
    beq $t1, $t0, endPrintTests     # Exit loop if all tests are printed

    # Printing index
    li $v0, 1                  
    move $a0, $t1
    syscall
    la $a0, space              
    li $v0, 4
    syscall

    # Printing patient ID
    sll $t6, $t1, 2                 # Offset calculation for patient ID
    add $t6, $s2, $t6               # Address of the current patient ID
    lw $a0, 0($t6)                  # Load patient ID
    li $v0, 1                       # syscall to print integer
    syscall
    la $a0, space                   # Space after the ID
    li $v0, 4
    syscall

    # Printing Test Name
    sll $t6, $t1, 5                 # Offset for test name
    add $t6, $s3, $t6               # Address of the current test name
    la $a0, 0($t6)                  # Load address of test name
    li $v0, 4                       # syscall to print string
    syscall
    la $a0, space                   # Space after the name
    li $v0, 4
    syscall

    # Printing Test Date
    sll $t6, $t1, 3                 # Offset for test date
    add $t6, $s4, $t6               # Address of the current test date
    la $a0, 0($t6)                  # Load address of test date
    li $v0, 4
    syscall
    la $a0, space                   # Space after the date
    li $v0, 4
    syscall

    # Printing Test Result
    sll $t6, $t1, 2                 # Offset for test result
    add $t6, $s5, $t6               # Address of the current test result
    lwc1 $f12, 0($t6)               # Load test result (assuming it's a float)
    li $v0, 2                       # syscall to print float
    syscall
    la $a0, nl                      # Newline after each entry
    li $v0, 4
    syscall

    addi $t1, $t1, 1                # Increment index
    j loopPrintTests                # Jump back to start of loop

endPrintTests:
    # Ask user to select a test by index
    li $v0, 4
    la $a0, askForTestIndex         # Load askFor for user input
    syscall
    li $v0, 5                       # Syscall to read integer
    syscall
    move $t2, $v0                   # Store selected index in $t2

    # Check if the selected index is within range
    lw $t3, numEntries
    blt $t2, $t3, updateDetails     # If index is less than numEntries, proceed
    li $v0, 4
    la $a0, invalidIn               # Print "Invalid index."
    syscall
    j updateATest                   # Restart the update process



updateDetails:

retakeNameAgain:
    # Assuming $t2 holds the index of the test to be updated
    # Load base addresses
    la $t6, testNames               # Pointer to start of testNames
    la $t7, testDates               # Pointer to start of testDates
    la $t8, patientIDs              # Pointer to start of patientIDs
    la $t9, testResults             # Pointer to start of testResults

    # Update Test Name
    li $v0, 4

    la $a0, askForTestName          # "Enter new test name: "
    syscall
    li $v0, 8                       # Syscall to read string
    la $a0, buffer                  # Use buffer to store input
    li $a1, 1024                    # Length of buffer
    syscall

    # Remove newline from input string
    la $a1, buffer                  # Pointer to buffer
    li $t0, 0                       # Index counter



removeNewlineName:
    lb $t1, 0($a1)                  # Load byte from buffer
    beqz $t1, endRemoveNewlineName  # End of string
    li $t3, 10                      # ASCII for newline
    beq $t1, $t3, replaceNewlineName # Newline found
    addi $a1, $a1, 1                # Move to next character
    j removeNewlineName

replaceNewlineName:
    sb $zero, 0($a1)                # Replace newline with null terminator

endRemoveNewlineName:
    # Calculate offset for test name based on index $t2
    sll $t3, $t2, 5                 # Each name can take up to 32 bytes
    add $t3, $t6, $t3               # Calculate address in testNames
    la $t4, buffer                  # Address of buffer
    li $t5, 32                      # Length to copy

    #let's check validity
 
    la $a1,buffer

    move $s0,$a1
    move $s1,$a2
    move $s6,$t1
    move $s7,$t2
    
    
    
    la $a1, buffer                  # Load address of the buffer
    la $a2, hgbLabel                # Load address of the first string
    jal strcmp2                     # Call strcmp function
    beq $v0, 0, valid22             # If return is 0, strings are equal

    la $a2, bgtLabel                # Check next string
    jal strcmp2
    beq $v0, 0, valid22

    la $a2, ldlLabel
    jal strcmp2
    beq $v0, 0, valid22

    la $a2, bptsLabel
    jal strcmp2
    beq $v0, 0, valid22

    la $a2, bptdLabel
    jal strcmp2
    beq $v0, 0, valid22

    move $a1,$s0
    move $a2,$s1
    move $t1,$s6
    move $t2,$s7

    j error27
    valid22:
    
    
    move $a1,$s0
    move $a2,$s1
    move $t1,$s6
    move $t2,$s7


    jal stringCopy                  # Copy the string from buffer to testNames



retakeDateAgain:
    # Update Test Date
    li $v0, 4
    la $a0, askForTestDate          # "Enter new test date (YYYY-MM): "
    syscall
    li $v0, 8                       # Syscall to read string
    la $a0, buffer                  # Use buffer to store input
    li $a1, 11                      # Length of buffer for date
    syscall

    # Remove newline from date string
    la $a1, buffer                  # Pointer to buffer
    li $t0, 0                       # Index counter



removeNewlineDate:
    lb $t1, 0($a1)                  # Load byte from buffer
    beqz $t1, endRemoveNewlineDate  # End of string
    li $t3, 10                      # ASCII for newline
    beq $t1, $t3, replaceNewlineDate # Newline found
    addi $a1, $a1, 1                # Move to next character
    j removeNewlineDate

replaceNewlineDate:
    sb $zero, 0($a1)                # Replace newline with null terminator

endRemoveNewlineDate:
    # Calculate offset for test date based on index $t2
    sll $t3, $t2, 3                 # Each date can take up to 8 bytes
    add $t3, $t7, $t3               # Calculate address in testDates
    la $t4, buffer                  # Address of buffer
    li $t5, 8                       # Length to copy
    
    la $a0, buffer                  # Address of the buffer
 
    move $s0,$t0
    move $s1,$t1
    move $s2,$t2
    move $s3,$t3
    move $s4,$t4
    move $s5,$t5
    jal checkDateFormat           # Jump to the date format checking function
    move $t0,$s0
    move $t1,$s1
    move $t2,$s2
    move $t3,$s3
    move $t4,$s4
    move $t5,$s5   
    
 
    beq $v0, 0, valid23             # If return value is 0, date is valid
    j error26                       # Otherwise, jump to error
    
    valid23:
    jal stringCopy                  # Copy the string from buffer to testDates


    # Update Test Result
    li $v0, 4
    la $a0, askForTestResult        # "Enter new test result: "
    syscall
    li $v0, 6                       # Syscall to read float
    syscall

    # Calculate offset for test result based on index $t2
    sll $t3, $t2, 2                 # Each result is a float, 4 bytes
    add $t3, $t9, $t3               # Calculate address in testResults
    swc1 $f0, 0($t3)                # Store the float result

    li $v0, 4
    la $a0, ls                      # "Data updated successfully \n"
    syscall

    j menuLoop                      # Return to main menu loop



# String copy subroutine: copies up to $t5 bytes from $t4 (src) to $t3 (dest), and ensures not to overwrite.
stringCopy:
    li $t0, 0                       # Index for copying

loopCopy:
    lb $t1, 0($t4)                  # Load byte from source
    sb $t1, 0($t3)                  # Store byte to destination
    beqz $t1, stringCopyDone        # If zero (null-terminator read), finish copying
    addiu $t4, $t4, 1               # Increment source address
    addiu $t3, $t3, 1               # Increment destination address
    addiu $t0, $t0, 1               # Increment index
    blt $t0, $t5, loopCopy          # Continue if index is less than max length

stringCopyDone:
    sb $zero, 0($t3)                # Ensure null termination in destination
    jr $ra                          # Return from subroutine




#/*-------------------------- 8) DELETE_TEST --------------------------*/ 





# DeleteATest function
deleteATest:
    # Display all tests with index
    lw $t0, numEntries              # Load the total number of test entries
    beq $t0, $zero, errorToMenu1    # If numEntries is 0, jump to error handling
    li $t1, 0                       # Initialize index counter to 0

    la $s2, patientIDs              # Base address of patientIDs
    la $s3, testNames               # Base address of testNames
    la $s4, testDates               # Base address of testDates
    la $s5, testResults             # Base address of testResults

loopPrintTestsForDeletion:
    beq $t1, $t0, endPrintTestsForDeletion  # Exit loop if all tests are printed

    # Print index
    li $v0, 1                       # syscall to print integer
    move $a0, $t1
    syscall
    la $a0, space                   # Print space
    li $v0, 4
    syscall

    # Print patient ID
    sll $t6, $t1, 2                 # Calculate offset for patient ID
    add $t6, $s2, $t6               # Address of the current patient ID
    lw $a0, 0($t6)
    li $v0, 1                       # syscall to print integer
    syscall
    la $a0, space
    li $v0, 4
    syscall

    # Print Test Name
    sll $t6, $t1, 5                 # Offset for test name
    add $t6, $s3, $t6
    la $a0, 0($t6)
    li $v0, 4
    syscall
    la $a0, space
    li $v0, 4
    syscall

    # Print Test Date
    sll $t6, $t1, 3                 # Offset for test date
    add $t6, $s4, $t6
    la $a0, 0($t6)
    li $v0, 4
    syscall
    la $a0, space
    li $v0, 4
    syscall

    # Print Test Result
    sll $t6, $t1, 2                 # Offset for test result
    add $t6, $s5, $t6
    lwc1 $f12, 0($t6)
    li $v0, 2
    syscall
    la $a0, nl
    li $v0, 4
    syscall

    addi $t1, $t1, 1
    j loopPrintTestsForDeletion

endPrintTestsForDeletion:
    # Prompt user to select an index to delete
    li $v0, 4
    la $a0, askForTestIndexDelete
    syscall
    li $v0, 5
    syscall
    move $t2, $v0

    # Validate index
    lw $t3, numEntries
    blt $t2, $t3, validIndex
    li $v0, 4
    la $a0, invalidIn
    syscall
    j deleteATest


validIndex:
    # Shift elements in arrays to delete the selected index
    li $t1, 0
    la $s2, patientIDs
    la $s3, testNames
    la $s4, testDates
    la $s5, testResults

shiftLoop:
    # Calculate the address offset for each type of data based on index
    sll $t3, $t2, 2     #t3=4       # Calculate word offset for patientIDs and testResults
    sll $t4, $t2, 5     #t4=32      # Calculate offset for testNames (32 bytes per entry)
    sll $t5, $t2, 3     #t5=8       # Calculate offset for testDates (8 bytes per entry)
   

    # Calculate actual addresses for each item
    add $a0, $s2, $t3               # Address for current patientID
    add $a1, $s3, $t4               # Address for current testName
    add $a2, $s4, $t5               # Address for current testDate
    add $a3, $s5, $t3               # Address for current testResult

    # Check if current index is within the range
    blt $t2, $t0, updateItems # If within range, update items 
    

# After deletion, we must shift each subsequent entry to fill the gap.
updateItems:
    # Update the patient ID and testResult which are straightforward
    lw $t6, 4($a0)                  # Load patientID from the next entry
    lw $t7, 4($a3)                  # Load testResult from the next entry
    sw $t6, 0($a0)                  # Store patientID one slot back
    sw $t7, 0($a3)                  # Store testResult one slot back

    # Update the testName
    add $t8, $a1, 32                # Address of the next testName
    li $t5, 31                      # Set loop counter for 32 bytes (maximum name length minus null terminator)

deleteCopyNameLoop:
    lb $t6, 0($t8)                  # Load a byte from next testName
    sb $t6, 0($a1)                  # Store the byte in current testName
    addiu $t8, $t8, 1               # Increment source address
    addiu $a1, $a1, 1               # Increment destination address
    addi $t5, $t5, -1               # Decrement loop counter
    bnez $t5, deleteCopyNameLoop    # If there are characters left, continue loop
    sb $zero, 0($a1)                # Null terminate the string

    # Update the testDate
    add $t9, $a2, 8                 # Address of the next testDate
    li $t5, 7                       # Set loop counter for 8 bytes (maximum date length minus null terminator)

deleteCopyDateLoop:
    lb $t7, 0($t9)                  # Load a byte from next testDate
    sb $t7, 0($a2)                  # Store the byte in current testDate
    addiu $t9, $t9, 1               # Increment source address
    addiu $a2, $a2, 1               # Increment destination address
    addi $t5, $t5, -1               # Decrement loop counter
    bnez $t5, deleteCopyDateLoop    # If there are characters left, continue loop
    sb $zero, 0($a2)                # Null terminate the string

    # Prepare for next iteration
    addiu $t2, $t2, 1               # Increment index for next item
    lw $t0, numEntries              # Load the total number of entries
    blt $t2, $t0, shiftLoop         # If there are more items, keep shifting
    j finishDeletion                # All items have been shifted, we can finish

finishDeletion:
    addi $t0, $t0, -1
    sw $t0, numEntries              # Store new total back to memory
    lw $s7 ,index
    addi,$s7,$s7,-1
    sw $s7,index

    # Print success message
    la $a0, ls                      # Load the address of the success message
    li $v0, 4                       # Load syscall number for print string
    syscall                         # Make the syscall to print the message

    j finish                        # Jump to the finish label (if you have cleanup to do)

finish:
    # Optional: handle any clean-up or exit routines here
    jr $ra                          # Return from subroutine

 
 
 
 
#/*-------------------------- 9) AVERAGE_TEST --------------------------*/ 
 



calculateAverages:
    # Stack frame setup
    addi $sp, $sp, -52              # Allocate stack space for registers and data
    sw $ra, 48($sp)                 # Save return address
    sw $s0, 44($sp)                 # Save $s0, base address for testNames
    sw $s1, 40($sp)                 # Save $s1, base address for testResults
    sw $s2, 36($sp)                 # Save $s2, loop counter
    sw $s3, 32($sp)                 # Save $s3, number of entries
    sw $s4, 28($sp)                 # Save $s4, index for results

    # Load base addresses and initialize counter
    la $s0, testNames
    la $s1, testResults
    lw $s3, numEntries              # Load total number of entries
    li $s2, 0                       # Initialize index counter to 0

    # Initialize floating point accumulators
    lwc1 $f2, floatZero             # Set $f2 to 0.0 (Sum for HGB)
    lwc1 $f3, floatZero             # Set $f3 to 0.0 (Sum for BGT)
    lwc1 $f4, floatZero             # Set $f4 to 0.0 (Sum for LDL)
    lwc1 $f5, floatZero             # Set $f5 to 0.0 (Sum for BPTS)
    lwc1 $f6, floatZero             # Set $f6 to 0.0 (Sum for BPTD)

    # Initialize counters for each test
    li $t0, 0                       # Counter for HGB
    li $t1, 0                       # Counter for BGT
    li $t2, 0                       # Counter for LDL
    li $t3, 0                       # Counter for BPTS
    li $t4, 0                       # Counter for BPTD

loopTests:
    beq $s2, $s3, calculateAveragesDone  # Exit loop if all entries are processed

    # Compute the offset for the current test name and result
    sll $s4, $s2, 5                 # Assume each entry is 32 bytes
    add $a0, $s0, $s4               # Address of current test name
    sll $s4, $s2, 2                 # Assume each entry is 4 bytes
    add $a1, $s1, $s4               # Address of current test result


    lwc1 $f1, 0($a1)                # Load the current test result

    # Compare current test name with known labels and update counters and sums
    la $a2, hgbLabel
    
    move $s5,$t0
    move $s6,$t1
    move $s7,$t2
    move $t7,$t3
    jal stringCompareAvg
    move $t0,$s5
    move $t1,$s6
    move $t2,$s7
    move $t3,$t7  
       
    beq $v0, 0, isHGB


    la $a2, bgtLabel
    move $s5,$t0
    move $s6,$t1
    move $s7,$t2
    move $t7,$t3
    jal stringCompareAvg
    move $t0,$s5
    move $t1,$s6
    move $t2,$s7
    move $t3,$t7
    
    beq $v0, 0, isBGT


    la $a2, ldlLabel
    move $s5,$t0
    move $s6,$t1
    move $s7,$t2
    move $t7,$t3
    jal stringCompareAvg
    move $t0,$s5
    move $t1,$s6
    move $t2,$s7
    move $t3,$t7
    
    beq $v0, 0, isLDL

    la $a2, bptsLabel
    move $s5,$t0
    move $s6,$t1
    move $s7,$t2
    move $t7,$t3
    jal stringCompareAvg
    
    move $t0,$s5
    move $t1,$s6
    move $t2,$s7
    move $t3,$t7
    
    beq $v0, 0, isBPTS

    la $a2, bptdLabel
    move $s5,$t0
    move $s6,$t1
    move $s7,$t2
    move $t7,$t3
    jal stringCompareAvg
    
    move $t0,$s5
    move $t1,$s6
    move $t2,$s7
    move $t3,$t7
    
    beq $v0, 0, isBPTD

    j updateIndex


isHGB:
    addi $t0, $t0, 1 
    add.s $f2, $f2, $f1
    j updateIndex

isBGT:
    addi $t1, $t1, 1
    add.s $f3, $f3, $f1
    j updateIndex

isLDL:
    addi $t2, $t2, 1
    add.s $f4, $f4, $f1
    j updateIndex

isBPTS:
    addi $t3, $t3, 1
    add.s $f5, $f5, $f1
    j updateIndex

isBPTD:
    addi $t4, $t4, 1
    add.s $f6, $f6, $f1

updateIndex:
    addi $s2, $s2, 1                # Increment the index
    j loopTests


calculateAveragesDone:
    # Check if counters are non-zero and calculate averages
    # Assume syscalls are used to print the averages for demonstration purposes

    # HGB Average Calculation
    bnez $t0, calculateHGBaverage
    j skipHGBprint                  # Skip print if count is zero

calculateHGBaverage:
    li $v0, 4 
    la $a0, avarageHGB
    syscall
    
    la $a0,space
    syscall
    
    li $v0, 2                       # Load syscall for float division
    mtc1 $t0, $f7                   # Move integer counter to float register
    cvt.s.w $f7, $f7                # Convert integer to float
    div.s $f2, $f2, $f7             # Divide sum by count
    li $v0, 2                       # Load syscall to print float
    mov.s $f12, $f2                 # Move average to $f12
    syscall                         # Print the result
    
    li $v0, 4 
    la $a0,nl
    syscall
    
skipHGBprint:
    # Repeat for other tests: BGT, LDL, BPTS, BPTD
    # BGT Average Calculation
    bnez $t1, calculateBGTaverage
    j skipBGTprint



calculateBGTaverage:
    li $v0, 4 
    la $a0, avarageBGT
    syscall
    
    la $a0,space
    syscall
    
    li $v0, 2                       # Load syscall for float division
    mtc1 $t1, $f7                   # Move integer counter to float register
    cvt.s.w $f7, $f7                # Convert integer to float
    div.s $f3, $f3, $f7             # Divide sum by count
    li $v0, 2                       # Load syscall to print float
    mov.s $f12, $f3                 # Move average to $f12
    syscall                         # Print the result
    
    li $v0, 4 
    la $a0,nl
    syscall
       
skipBGTprint:
    # LDL Average Calculation
    bnez $t2, calculateLDLaverage
    j skipLDLprint



calculateLDLaverage:
    li $v0, 4 
    la $a0, avarageLDL
    syscall
    
    la $a0,space
    syscall
    
    li $v0, 2                       # Load syscall for float division
    mtc1 $t2, $f7                   # Move integer counter to float register
    cvt.s.w $f7, $f7                # Convert integer to float
    div.s $f4, $f4, $f7             # Divide sum by count
    li $v0, 2                       # Load syscall to print float
    mov.s $f12, $f4                 # Move average to $f12
    syscall                         # Print the result
    
    li $v0, 4 
    la $a0,nl
    syscall
      
skipLDLprint:
    # BPTS Average Calculation
    bnez $t3, calculateBPTSaverage
    j skipBPTSprint



calculateBPTSaverage:
    li $v0, 4 
    la $a0, avarageBPTS
    syscall
    
    la $a0,space
    syscall
    
    li $v0, 2                       # Load syscall for float division
    mtc1 $t3, $f7                   # Move integer counter to float register
    cvt.s.w $f7, $f7                # Convert integer to float
    div.s $f5, $f5, $f7             # Divide sum by count
    li $v0, 2                       # Load syscall to print float
    mov.s $f12, $f5                 # Move average to $f12
    syscall                         # Print the result
    
    li $v0, 4 
    la $a0,nl
    syscall
      
skipBPTSprint:
    # BPTD Average Calculation
    bnez $t4, calculateBPTDaverage
    j skipBPTDprint


calculateBPTDaverage:
    li $v0, 4 
    la $a0, avarageBPTD
    syscall
    
    la $a0,space
    syscall
    
    li $v0, 2                       # Load syscall for float division
    mtc1 $t4, $f7                   # Move integer counter to float register
    cvt.s.w $f7, $f7                # Convert integer to float
    div.s $f6, $f6, $f7             # Divide sum by count
    li $v0, 2                       # Load syscall to print float
    mov.s $f12, $f6                 # Move average to $f12
    syscall                         # Print the result
    
    li $v0, 4 
    la $a0,nl
    syscall
    
skipBPTDprint:
    # Restore stack and registers
    lw $ra, 48($sp)                 # Restore return address
    lw $s0, 44($sp)                 # Restore $s0
    lw $s1, 40($sp)                 # Restore $s1
    lw $s2, 36($sp)                 # Restore $s2
    lw $s3, 32($sp)                 # Restore $s3
    lw $s4, 28($sp)                 # Restore $s4
    addi $sp, $sp, 52               # Deallocate stack space
    jr $ra                          # Return


# Function to compare two strings
# $a0 = pointer to first string, $a2 = pointer to second string
# $v0 = result (0 if equal, 1 otherwise)
stringCompareAvg:
    # Initialize pointers for the two strings
    move $t0, $a0  # $t0 will point to the current character of the first string
    move $t1, $a2  # $t1 will point to the current character of the second string



compareLoopAvg:
    lb $t2, 0($t0)                  # Load byte from first string
    lb $t3, 0($t1)                  # Load byte from second string
    
 
    # Check if we've reached the end of the strings
    beqz $t2, stringsEqualAvg

    # Check if characters are different
    bne $t2, $t3, stringsNotEqualAvg

    # Move to the next characters in each string
    addi $t0, $t0, 1
    addi $t1, $t1, 1

    # Loop back to compare the next characters
    j compareLoopAvg

stringsEqualAvg:
    li $v0, 0                       # Set return value to 0 (strings are equal)
    jr $ra                          # Return from the function

stringsNotEqualAvg:
    li $v0, 1                       # Set return value to 1 (strings are not equal)
    jr $ra                          # Return from the function
    
    





#/*-------------------------- 10) SAVE_FILE --------------------------*/  




printAllTestsToFile:
    # Open file for writing
    la $a0, filename                # Load the address of the filename
    la $a1, 1                       # Load the flags (write)
    li $a2, 0                       # Mode (not used in write mode)
    li $v0, 13                      # System call for open file
    syscall
    move $s6, $v0                   # Store the file descriptor

    # Setup loop variables
    lw $t0, numEntries              # Load number of entries
    li $t1, 0                       # Initialize index to 0

    la $t2, patientIDs              # Base address of patientIDs
    la $t3, testNames               # Base address of testNames
    la $t4, testDates               # Base address of testDates
    la $t5, testResults             # Base address of testResults


loopPrintToFile:
    beq $t1, $t0, closeFile         # Exit loop if all entries are processed

    # Write patient ID
    move $a0,$s6  # file descriptor
    lw $t6,0($t2) 
    move $s0,$t0
    move $s7,$t1
    jal intToStringFixed
    move $t0,$s0
    move $t1,$s7
 
    la $a2,7    		            # num of chars for id is 7
    li $v0, 15		                # Prepare to print to file
    syscall                         # Execute print
    
    # write ": "
    li $v0 , 15
    la $a1,colonSpace
    la $a2,2
    syscall
    
    # write test name
    move $s0,$t0
    move $s7,$t1    
    j strlen                        # Call strlen to count the number of chars in the string  
    strlenReturn:
    move $t0,$s0
    move $t1,$s7  
     
    move $a0,$s6  # file descriptor
    move $a1, $t3                   # Copy the string pointer to $a2
    move $a2,$v0
    li $v0, 15		                # Prepare to print to file
    syscall
    
    
    # write ", "
    move $a0,$s6                    # file descriptor
    li $v0 , 15
    la $a1,commaSpace
    li $a2,2
    syscall
    
    

    #write test dates
	move $a0,$s6  # file descriptor
    move $a1, $t4                   # Copy the string pointer to $a2
    la $a2,7
    li $v0, 15		                # Prepare to print to file
    syscall
	   
	
	# write ", "
    move $a0,$s6  # file descriptor
    li $v0 , 15
    la $a1,commaSpace
    li $a2,2
    syscall
    
    
    #write the result
    lwc1 $f12, 0($t5)               # Load the current test result into $f12  
    move $s0,$t0
    move $s7,$t1
    move $s1,$t2
    move $s3,$t3
    move $s4,$t4
    move $s5,$t5
	j floatToString


floatToStringReturn:  
    move $t0,$s0
    move $t1,$s7 
    move $t2,$s1 
    move $t3,$s3
    move $t4,$s4 
    move $t5,$s5 
      
      
    move $s0,$t0
    move $s7,$t1    
    j strlen2                       # Call strlen to count the number of chars in the string
    strlenReturn2:
    move $t0,$s0
    move $t1,$s7  
        
   
    move $a0,$s6  # file descriptor
    la $a1,bufferFloatToString      # Copy the string pointer to $a2
    move $a2,$v0
    li $v0, 15		                # Prepare to print to file
    syscall
  
	# write "e"
    move $a0,$s6  # file descriptor
    li $v0 , 15
    la $a1,latterE
    li $a2,1
    syscall
	
	
	#write a new line
	move $a0,$s6  # file descriptor 
	li $v0 , 15
    la $a1,nl
    li $a2,1
    syscall
	
	
    # Increment for next entry
    addi $t1, $t1, 1                # Increment index
    addi $t2, $t2,4                 # Next patient ID
    addi $t3, $t3, 32               # Next test name
    addi $t4, $t4, 8                # Next test date
    addi $t5, $t5, 4                # Next test result
    j loopPrintToFile               # Loop back


closeFile:
    move $a0, $s6                   # Move file descriptor into $a0
    li $v0, 16                      # System call for close file
    syscall

    j backSaveFile                  # Return from subroutine


intToStringFixed:
    # Input: $t6 = integer to convert (must be exactly 7 digits and positive)
    # Output: $a1 = address of the string buffer containing the integer
    la $a1, bufferIntToString       # Load the address of the buffer
    li $t0, 10                      # Constant 10 for division

    # Start converting each digit from the least significant
    addi $s2, $a1, 6                # Point $s2 to the last position for digits in buffer
    


convertLoop:
    div $t6, $t0                    # Divide $t6 by 10
    mfhi $t1                        # Upper half result (remainder)
    addi $t1, $t1, '0'              # Convert the remainder to ASCII
    sb $t1, 0($s2)                  # Store the ASCII character
    addi $s2, $s2, -1               # Move backwards in the buffer
    mflo $t6                        # Lower half result (new $a0 for next iteration)
    bnez $t6, convertLoop           # Continue if there are more digits

    # Null-terminate the string
    addi $s2, $s2, 8                # Point to the end of the 7 characters
    sb $zero, 0($s2)                # Store zero byte to null-terminate the string
    
    # Return the address of the buffer containing the string
    move $v0, $a1                   # Move start address of string to return register
    jr $ra                          # Return from subroutine


strlen:
    move $t0, $t3                   # Move the base address of the string to $t0
    li $v0, 0                       # Initialize length counter in $v0

strlenLoop:
    lb $t1, 0($t0)                  # Load a byte from the string
    beqz $t1, strlenEnd             # If the byte is zero (null terminator), end loop
    addiu $v0, $v0, 1               # Increment the length counter
    addiu $t0, $t0, 1               # Move to the next byte
    j strlenLoop                    # Jump back to the start of the loop

strlenEnd:
  j strlenReturn                    # Return from the function with length in $v0





strlen2:
    la $t0,bufferFloatToString      # Move the base address of the string to $t0
    li $v0, 0                       # Initialize length counter in $v0

strlenLoop2:
    lb $t1, 0($t0)                  # Load a byte from the string
    beqz $t1, strlenEnd2            # If the byte is zero (null terminator), end loop
    addiu $v0, $v0, 1               # Increment the length counter
    addiu $t0, $t0, 1               # Move to the next byte
    j strlenLoop2                   # Jump back to the start of the loop

strlenEnd2:
    j strlenReturn2                 # Return from the function with length in $v0


floatToString:
    # Input: $f12 (floating point number)
    # Output: string at address in bufferFloatToString
    li $t0, 10                      # Load constant for multiplication/division
    cvt.w.s $f0, $f12               # Convert float to integer (truncate)
    mfc1 $t1, $f0                   # Move converted integer to a general register


    # Convert integer part to string
    la $t3, bufferFloatToString     # Address of the buffer to store integer part



    # Convert integer to string in reverse
integerToStrLoop:
    div $t1, $t0                    # Divide $t1 by 10
    mflo $t4                        # Quotient to $t4
    mfhi $t5                        # Remainder to $t5
    addi $t5, $t5, '0'              # Convert remainder to ASCII
    sb $t5, 0($t3)                  # Store the ASCII character
    move $t1, $t4                   # Update $t1 with the quotient
    addi $t3, $t3, 1                # Move the buffer pointer
    bnez $t4, integerToStrLoop

    # Handling the decimal part 
    lwc1 $f10, floatTen
    mul.s $f0, $f12, $f10           # Multiply float by 10
    cvt.w.s $f0, $f0                # Convert to integer
    mfc1 $t1, $f0                   # Move to integer register
    div $t1, $t0                    # Divide by 10 to isolate the first decimal digit
    mfhi $t1                        # Remainder is the decimal digit
    addi $t1, $t1, '0'              # Convert to ASC
    
    
    # Append the decimal part
    li $t4, '.'
    sb $t4, 0($t3)                  # Store the decimal point
    addi $t3, $t3, 1
    sb $t1, 0($t3)                  # Store the decimal digit
    addi $t3, $t3, 1

 
    # Null-terminate the string
    sb $zero, 0($t3)                # Null-terminate the string
    #add,$t3,$t3,$t9
    

    la $a0,bufferFloatToString
    move $t0, $a0

    
    findDecimal:
        lb $t1, 0($a0)              # Load a byte from string
        beq $t1, '.', reverse       # If byte is '.', start reversing
        beq $t1, '\0', endReverse   # If byte is '\0', end of string
        addiu $a0, $a0, 1           # Increment pointer
        j findDecimal               # Loop
    
    
    # Reverse the string before decimal
    reverse:
        subu $a0, $a0, 1            # Move back to the last digit before '.'

    reverseLoop:
        # Pointers: $t0 = start, $a0 = end
        lb $t2, 0($t0)              # Load byte from start
        lb $t3, 0($a0)              # Load byte from end
        sb $t3, 0($t0)              # Store end byte at start
        sb $t2, 0($a0)              # Store start byte at end
        addiu $t0, $t0, 1           # Increment start pointer
        subu $a0, $a0, 1            # Decrement end pointer
        bge $t0, $a0, endReverse    # If pointers cross, end
        j reverseLoop               # Continue loop
        
    
    endReverse:
        j floatToStringReturn       # Return to caller





#/*-------------------------- ERRORS_FUNCTIONS --------------------------*/  



error20:
    la $a0,em20
    li $v0,4
    syscall
    j menuLoop


error21:
    la $a0,em21
    li $v0,4
    syscall
    j retakeID


error22:
    la $a0,em22
    li $v0,4
    syscall
    j retakeName


strcmp:
    lb $t1, 0($a1)                  # Load byte from first string
    lb $t2, 0($a2)                  # Load byte from second string

strcmpLoop:
    beq $t1, $zero, strcmpEnd       # If end of string1, check string2
    beq $t2, $zero, strcmpEnd       # If end of string2, check string1
    bne $t1, $t2, strcmpFalse       # If chars not equal, strings not equal
    addi $a1, $a1, 1                # Increment address of string1
    addi $a2, $a2, 1                # Increment address of string2
    lb $t1, 0($a1)                  # Load next char from string1
    lb $t2, 0($a2)                  # Load next char from string2
    j strcmpLoop                    # Repeat comparison

strcmpFalse:
    li $v0, 1                       # Set result to 1 (not equal)
    jr $ra                          # Return from function

strcmpEnd:
    beq $t1, $t2, strcmpTrue        # Check for both strings ended together
    li $v0, 1                       # Strings not equal
    jr $ra                          # Return from function

strcmpTrue:
    li $v0, 0                       # Strings are equal
    jr $ra                          # Return from function



checkDateFormat:
    li $t0, 0                       # Counter for index in the string
    li $t1, 7                       # Expected length of the date string

loopCheckDateFormat:
    beq $t0, $t1, endCheck          # If reached expected length, end check
    lb $t2, 0($a0)                  # Load current character

    # Check if character at position 4 is '-'
    li $t3, 4
    beq $t0, $t3, checkDash

    # Check if other positions are digits
    li $t4, 48                      # ASCII value of '0'
    li $t5, 57                      # ASCII value of '9'
    blt $t2, $t4, invalid           # Less than '0'
    bgt $t2, $t5, invalid           # Greater than '9'
    j nextChar

checkDash:
    li $t6, 45                      # ASCII value of '-'
    bne $t2, $t6, invalid           # If not '-', it's invalid
    j nextChar

nextChar:
    addi $a0, $a0, 1                # Move to the next character
    addi $t0, $t0, 1                # Increment index
    j loopCheckDateFormat

endCheck:
    li $v0, 0                       # Return 0 for valid format
    jr $ra

invalid:
    li $v0, 1                       # Return 1 for invalid format
    jr $ra


error23:
la $a0,em23
li $v0,4
syscall
j retakeDate


errorToMenu:
    li $v0, 4                       # syscall to print string
    la $a0, errorMsg                # Load address of the error message
    syscall

    j menuLoop                      # Jump back to the menu loop
    
errorToMenu1:
    li $v0, 4                       # syscall to print string
    la $a0, errorMsg1               # Load address of the error message
    syscall

    j menuLoop                      # Jump back to the menu loop
    


afd: 
    li $v0, 4                       # syscall to print string
    la $a0, em23                    # Load address of the error message
    syscall
    j reAskDate1

asd:
    li $v0, 4                       # syscall to print string
    la $a0, em23                    # Load address of the error message
    syscall
    j reAskDate2


          
checkDateFormat1:
    li $t0, 0                       # Counter for index in the string
    li $t1, 7                       # Expected length of the date string

loopCheckDateFormat1:
    beq $t0, $t1, endCheck1        # If reached expected length, end check
    lb $t2, 0($a0)                  # Load current character

    # Check if character at position 4 is '-'
    li $t3, 4
    beq $t0, $t3, checkDash1

    # Check if other positions are digits
    li $t4, 48                      # ASCII value of '0'
    li $t5, 57                      # ASCII value of '9'
    blt $t2, $t4, invalid1          # Less than '0'
    bgt $t2, $t5, invalid1          # Greater than '9'
    j nextChar1

checkDash1:
    li $t6, 45                      # ASCII value of '-'
    bne $t2, $t6, invalid1          # If not '-', it's invalid
    j nextChar1

nextChar1:
    addi $a0, $a0, 1                # Move to the next character
    addi $t0, $t0, 1                # Increment index
    j loopCheckDateFormat1

endCheck1:
    li $v0, 1                       # Return 1 for valid format
    jr $ra

invalid1:
    li $v0, 0                       # Return 0 for invalid format
    jr $ra


#/*-------------------------*/    

error27:
    la $a0,em22
    li $v0,4
    syscall
    j retakeNameAgain

error26:
    la $a0,em23
    li $v0,4
    syscall
    j retakeDateAgain


strcmp2:
    lb $t1, 0($a1)                  # Load byte from first string
    lb $t2, 0($a2)                  # Load byte from second string

strcmpLoop2:
    beq $t1, $zero, strcmpEnd2      # If end of string1, check string2
    beq $t2, $zero, strcmpEnd2      # If end of string2, check string1
    bne $t1, $t2, strcmpFalse2      # If chars not equal, strings not equal
    addi $a1, $a1, 1                # Increment address of string1
    addi $a2, $a2, 1                # Increment address of string2
    lb $t1, 0($a1)                  # Load next char from string1
    lb $t2, 0($a2)                  # Load next char from string2
    j strcmpLoop2                   # Repeat comparison

strcmpFalse2:
    li $v0, 1                       # Set result to 1 (not equal)
    jr $ra                          # Return from function

strcmpEnd2:
    beq $t1, $t2, strcmpTrue2       # Check for both strings ended together
    li $v0, 1                       # Strings not equal
    jr $ra                          # Return from function

strcmpTrue2:
    li $v0, 0                       # Strings are equal
    jr $ra                          # Return from function
    
    
FDNE:


    # error message
    la $a0, fileNotFound        # load message string
    li $v0, 4                        # print string
    syscall
    j menuLoop                       # return



    
