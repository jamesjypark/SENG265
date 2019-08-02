#!/usr/bin/env python3 

# File: sengfmt.py 
# Student Name  : James (Juyoung) Park 
# SENG 265 - Assignment 2
import argparse
import sys
import math 

tempOutput = []
margin = 0
newMargin = 0
maxwidth = 0
maxwidth_declare = False
currFormat= False
cap = False
string_carry = False
	
# Changes global margin value
def changeMargin( mrgn ):
	global margin
	global currFormat
	global newMargin
	newMargin = margin
	currFormat= True
	if ( mrgn[0] == "+"):
		newMargin += int(mrgn[1:])
	elif ( mrgn[0] == "-"):
		newMargin -= int(mrgn[1:])
	else:
		newMargin = int(mrgn[0:])
	if (newMargin < 0):
		newMargin = 0
	if (maxwidth_declare and newMargin > maxwidth - 20):
		newMargin = maxwidth - 20
	if not maxwidth_declare or len(tempOutput) is 0:
		margin = newMargin
	

# Changes global currFormatboolean value
def changeFormat( fmt ):
	global currFormat
	if (fmt == "on"):
		currFormat= True
	else:
		currFormat= False

# Changes global maxwidth value
def changeMaxwidth( mwidth ):
	global maxwidth
	global maxwidth_declare
	global currFormat
	currFormat = True
	maxwidth_declare = True
	if ( mwidth[0] == "+"):
		maxwidth += int(mwidth[1:])
	elif ( mwidth[0] == "-"):
		maxwidth -= int(mwidth[1:])
	else:
		maxwidth = int(mwidth[0:])

# Changes glocal cap boolean value
def changeCap( cp ):
	global cap
	if (cp == "on"):
		cap = True
	else:
		cap = False
# Prints in a justified format
def printJustified( words,numChar ):
	global margin
	global newMargin
	# If there is one word, print it without no justification
	if (len(words) is 1):
		casePrint(words[0])
		print("")
		margin = newMargin
		return
	# Total number of spaces
	numSpaces = maxwidth - margin - numChar
	# Total number of breaks in the sentence
	numBreaks = len(words) - 1
	# Total number of remaining spaces that needs to be distributed
	remainder = numSpaces % numBreaks
	# Number of base spaces distributed to each break 
	division = math.floor(numSpaces / numBreaks)
	spaces = [] 
	# Create a spaces[] list that contains the number of spaces for each line
	for i in range( 0,numBreaks ):
		spaces.append(division) 
	for j in range ( 0,remainder):
		spaces[j] += 1
	
	for k in range ( 0,len(words)-1):
		casePrint(words[k])
		for a in range(0, spaces[k]):
			print(" ",end="")
	casePrint(words[len(words)-1])
	print("")
	margin = newMargin

# Formats the line 
def formatLine( line ):
	global tempOutput
	global string_carry
	if (not currFormat):
		print(line,end="")
	elif (not maxwidth_declare):
		for x in range(0, margin):
			print(" ",end="")
		casePrint(line)
	else:
		words = line.split()
		for word in words:
			outputLength = sum(len(x) for x in tempOutput)
			if (len(word) + len(tempOutput) + outputLength < maxwidth - margin + 1):
				tempOutput.append(word)
				string_carry = True
			else:
				for x in range(0, margin):
					print(" ",end="")
				printJustified(tempOutput,outputLength)
				tempOutput = []
				tempOutput.append(word)
				string_carry = True
				
	


# Converts string to appropriate upper case or lower case
def casePrint(line):
	if (cap):
		print(line.upper(),end="")
	else:
		print(line,end="")



# Iterates through the input line by line
# CODE STARTS HERE
if len(sys.argv)>=2:
    contents = open(sys.argv[1]).readlines()
else:
    contents = sys.stdin.readlines()
for line in contents:
	words = line.split()
	# If the line is empty, print the pending line
	if(len(words) == 0):
		if (currFormat):
			outputLength = sum(len(x) for x in tempOutput)
			# If line is not empty
			if string_carry:
				for i in range(0,margin):
					print(" ",end="")
			if maxwidth_declare and string_carry:
				printJustified(tempOutput,outputLength)
				string_carry = False
				print("")
			else:
				casePrint(line)
			tempOutput = []
		else:
			print(line,end="")
	# Checks if the line is a formatting condition
	elif (words[0] == "?mrgn"):
		changeMargin(words[1])
	elif (words[0] == "?fmt"):
		changeFormat(words[1])
	elif (words[0] == "?maxwidth"):
		changeMaxwidth(words[1])
	elif (words[0] == "?cap"):
		changeCap(words[1])
	else:
		formatLine( line )

if (currFormat and maxwidth_declare and len(tempOutput)>=1):
	outputLength = sum(len(x) for x in tempOutput)
	for i in range(0,margin):
		print(" ",end="")
	printJustified(tempOutput,outputLength)
