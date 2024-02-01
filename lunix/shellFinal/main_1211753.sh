#!/bin/bash


#sorry no time to explain more on checking matches :(

# check rows 
checkRows() {
 rowMatch="no match across rows"
while IFS='|' read -r a b c
do
if [[ $a == "$b" && $b == "$c" ]] 
then
rowMatch="symbols in at least one row are the same"
break
fi
done < "$in"
echo "$rowMatch"
echo "$rowMatch" >> "$outF"
}

# check columns
checkColumns() {
columnMatch="no match across columns"
local -a column
line=0
while IFS='|' read -r a b c 
do
column[0+line]="$a"
column[1+line]="$b"
column[2+line]="$c"
((line+=3))
done < "$in"
for ((i=0; i<3; i++)) 
do
if [[ ${column[i]} == "${column[i+3]}" && ${column[i+3]} == "${column[i+6]}" ]] 
then
columnMatch="symbols in at least one column are the same"
break
fi
done
echo "$columnMatch"
echo "$columnMatch" >> "$outF"
}

# check diagonals
checkDiagonals() {
diagonalMatch="no match across diagonals"
local -a diagonal
index=0
while IFS='|' read -r a b c 
do
diagonal[index]="$a"
diagonal[index+1]="$b"
diagonal[index+2]="$c"
((index+=3))
done < "$in"
if [[ ${diagonal[0]} == "${diagonal[4]}" && ${diagonal[4]} == "${diagonal[8]}" ]] || 
[[ ${diagonal[2]} == "${diagonal[4]}" && ${diagonal[4]} == "${diagonal[6]}" ]] 
then
diagonalMatch="a match across at least one diagonal"
fi
echo "$diagonalMatch"
echo "$diagonalMatch" >> "$outF"
}

id=1211753 # my id number 

# make output folder and file name from user input
read -p "enter your first name: " name #read my name abood
out="${name} ${id} final ENCS3130-Shell" # name of the new dir 
mkdir -p "$out" # create a new dir to save outp
outF="${out}/output.txt" #  the path of the output

# take name of a text file as input 
read -p "enter the name of the text file: " in
if [[ ! -f $in ]] 
then
echo "file does not exist."
exit 1
fi

echo "Player name: $name" #print the name on console
echo "Player name: $name" > "$outF" #print the name on out file
echo "text file:" 
cat "$in" # print content of input to console
echo "text file:" >> "$outF"
cat "$in" >> "$outF"  # print content of input to out fike
t=$(date '+%H:%M:%S') # using date formatt tp so get hours minutes and sconds
echo "time: $t" #print it to console
echo "time: $t" >> "$outF" #print it to file
checkRows
checkColumns
checkDiagonals
