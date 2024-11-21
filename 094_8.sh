#!/bin/bash

#NAME: BRAHMAJIT RAY
#ROLL NO.: 002211001094
#DATE: 13/11/2024
#OPERATING SYSTEMS LAB ASSIGNMENT 8: SHELL SCRIPT PROGRAMMING

# 1. Create a directory ‘All My Cprogs Files’ in your home directory.
echo "1. Creating directory 'All My Cprogs Files' in your home directory..."
mkdir -p "$HOME/All My Cprogs Files"
echo "Directory created."
echo

# 2. Move all the C files to the new directory.
echo "2. Moving all .c files to the 'All My Cprogs Files' directory..."
find . -type f -name "*.c" -exec cp --parents {} "$HOME/All My Cprogs Files/" \;
echo "Files moved."
echo

# 3. Show the list of files in your current directory.
echo "3. Listing files in the current directory..."
ls
echo "Files listed."
echo

# 4. Show current working directory.
echo "4. Showing the current working directory..."
pwd
echo "Current directory displayed."
echo

# 5. Display date in the dd:mm:yy format.
echo "5. Displaying the date in the format dd:mm:yy..."
date "+%d-%m-%y"
echo "Date displayed."
echo

# 6. Count the number of files in your home directory.
echo "6. Counting the number of files in your home directory..."
file_count=$(find "$HOME" -maxdepth 1 -type f | wc -l)
echo "Number of files in the home directory: $file_count"
echo

# 7. Create a file that lists all of the .C files in your directory.
echo "7. Creating a file that lists all .C files in your home directory..."
find . -type f -iname "*.C" > "C_files_list.txt"
echo "File 'C_files_list.txt' created."
echo "Listed C files :"
cat C_files_list.txt
echo

# 8. Script for addition of two numbers (real numbers).
echo "8. Performing addition of two real numbers..."
echo "Enter the first number:"
read num1
echo "Enter the second number:"
read num2
sum=$(echo "$num1 + $num2" | bc)
echo "The sum of $num1 and $num2 is $sum."
echo

# Script to convert a string from lowercase to uppercase.
echo "9. Converting a string from lowercase to uppercase"
echo "Enter a string to convert from lowercase to uppercase:"
read input_string

# Convert the string to uppercase
uppercase_string=$(echo "$input_string" | tr '[:lower:]' '[:upper:]')

echo "Converted string in uppercase: $uppercase_string"
echo

# 10. Read 'n' and generate a pattern.
echo "Generating a pattern..."
echo "Enter a number:"
read n
for ((i=1; i<=n; i++)); do
    for ((j=1; j<=i; j++)); do
        echo -n "$j "
    done
    echo
done

echo "Script completed."

