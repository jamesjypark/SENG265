#!/opt/local/bin/python

import sys
import math
import re
import calendar

class Formatter:
    """This is the definition for the class"""
    margin = 0
    def __init__(self, filename=None, inputlines=None):
        self.inputlines = inputlines
        self.output = []
        self.filename = filename
        self.margin = 0
        self.currFormat = False
        self.maxwidth = 0
        self.maxwidth_declare = False
        self.cap = False
        self.tempOutput = []
        self.tempString = ""
        self.string_carry = False
        self.monthabbr = False
        if (self.filename is not None):
            readFile = open(filename,"r")
            self.inputlines = readFile.readlines()

    # Return the formatted lines in as an array of each line.
    def get_lines(self):

        # Run through the input and if there is ?replace command, change
        # all matching words ahead of time. 
        # If month abbreviation format appears and monthabbr is on
        # change to appropriate date format.

        for i in range(0,len(self.inputlines)):
            words = self.inputlines[i].split()
            # List of commands
            conditionList = ["?replace","?monthabbr","?cap","?mrgn","?maxwidth","?fmt"]
            if (len(words) is not 0):
                if (words[0] == "?replace"):
                    self.replaceWord(words[1],words[2])
                if (words[0] == "?monthabbr"):
                    self.changeMonthabbr(words[1])
                if (words[0] == "?cap"):
                    self.changeCap(words[1])
            # If capitalization should be applied on the line
            if (self.cap and words[0] not in conditionList):
                self.inputlines[i] = self.inputlines[i].upper()
            # if monthabbr is on, find and change to approrpriate date format
            if (self.monthabbr):
                tempMatch = re.findall(r'\d\d[\/.-]\d\d[\/.-]\d\d\d\d', self.inputlines[i])
                if len(tempMatch) > 0:
                    newMatch = self.replaceDate(tempMatch[0])
                    self.inputlines[i] = re.sub(r'\d\d[\/.-]\d\d[\/.-]\d\d\d\d', newMatch, self.inputlines[i])

        # Runs through the input and formats the lines accordingly. 

        for line in self.inputlines:
            words = line.split()
            # If line is empty, print the existing line
            if ( len(words)==0 ):
                if (self.currFormat):
                    outputLength = sum(len(x) for x in self.tempOutput)
                    # If existing line is not empty
                    if self.string_carry:
                        for i in range(0,self.margin):
                            self.tempString += " "
                    if self.maxwidth_declare and self.string_carry:
                        self.printJustified(self.tempOutput, outputLength)
                        self.output.append(self.tempString)
                        self.output.append("")
                        self.tempString = ""
                        self.tempOutput = []
                        self.string_carry = False
                    else:
                        self.tempString += line.rstrip('\n')
                        self.output.append(self.tempString)
                    self.tempOutput = []
                else:
                    self.output.append("")

            # If line is a command

            elif (words[0] == "?replace"):
                self.replaceWord(words[1],words[2])
                # ERROR IF ONLY ONE WORD
            elif (words[0] == "?mrgn"):
                self.changeMargin(words[1])
            elif (words[0] == "?fmt"):
                self.changeFormat(words[1])
            elif (words[0] == "?maxwidth"):
                self.changeMaxwidth(words[1])
            elif (words[0] == "?cap"):
                self.changeCap(words[1])
            elif (words[0] == "?monthabbr"):
                self.changeMonthabbr(words[1])
            
            # If line is not a command, format the line
            else:
                self.formatLine(line)

        # If end of input list is reached and we have an existing temp line, 
        # print the line accordingly. 

        if (self.currFormat and self.maxwidth_declare and len(self.tempOutput) >= 1):
            outputLength = sum(len(x) for x in self.tempOutput)
            for i in range(0,self.margin):
                self.tempString += " "
            self.printJustified(self.tempOutput, outputLength)
            self.output.append(self.tempString)
        return self.output

    # Change the margin value of the instance
    def changeMargin(self, mrgn):
        self.currFormat = True
        if ( mrgn[0] == "+" ):
            self.margin += int(mrgn[1:])
        elif ( mrgn[0] == "-" ):
            self.margin -= int(mrgn[1:])
        else:
            self.margin = int(mrgn[0:])
        if (self.margin < 0):
            self.margin = 0
        # Implement maxwdith declare margin
        # Catch error if no number is given for margin value

    # Change the format value of the instance
    def changeFormat(self, fmt):
        if (fmt == "on"):
            self.currFormat = True
        else:
            self.currFormat = False
        # Catch error for no parameter or given weird on/off value 
    
    # Change the maxwidth of the instance
    def changeMaxwidth(self, mwidth):
        self.currFormat = True
        self.maxwidth_declare = True
        if ( mwidth[0] == "+"):
            self.maxwidth += int(mwidth[1:])
        elif ( mwidth[0] == "-"):
            self.maxwidth -= int(mwidth[1:])
        else:
            self.maxwidth = int(mwidth[0:]) 
        if (self.maxwidth < 0):
            self.maxwidth = 0
        # Catch error for sketchy maxwidth happening

    # Change the cap value of the instance
    def changeCap(self, cap):
        if (cap == "on"):
            self.cap = True
        else:
            self.cap = False
        # Catch error when cap is neither on or off

    # Change the monthabbr value of the instance
    def changeMonthabbr(self, val):
        if (val == "on"):
            self.monthabbr = True
        else:
            self.monthabbr = False
        # Catch error when cap is neither on or off

    # Justify words from parameter in and append
    # the justified line to self.tempString 
    def printJustified(self, words, numChar ):
        if (len(words) is 1):
            self.tempString += words[0]
            return
        # Total number of spaces
        numSpaces = self.maxwidth - self.margin - numChar
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
            self.tempString += words[k]
            for a in range(0, spaces[k]):
                self.tempString += " "
        self.tempString += words[len(words)-1]

    # Format parameter line and append the formatted line to self.output
    def formatLine(self, line):
        if (not self.currFormat):
            self.output.append(line)
        elif (not self.maxwidth_declare):
            for x in range (0, self.margin):
                self.tempString += " "
            self.tempString += line
            self.output.append(self.tempString)
            self.tempString = ""
        else:
            words = line.split()
            for word in words:
                outputLength = sum(len(x) for x in self.tempOutput)
                if (len(word) + len(self.tempOutput) + outputLength < self.maxwidth - self.margin + 1):
                    self.tempOutput.append(word)
                    self.string_carry = True
                else:
                    for x in range(0, self.margin):
                        self.tempString += " "
                    #IMPLEMENT JUSTIFICATION
                    # Attaches the justified string on self.tempString
                    self.printJustified(self.tempOutput, outputLength)
                    self.output.append(self.tempString)
                    self.tempString = ""
                    self.tempOutput = []
                    self.tempOutput.append(word)
                    self.string_carry = True

    # Iterate through the entire input file and change all matching
    # oldWord regex expression to newWord. 
    def replaceWord(self, oldWord, newWord):
        for i in range(0,len(self.inputlines)):
            regexCommand = r"" + oldWord + r""
            self.inputlines[i] = re.sub(regexCommand, newWord, self.inputlines[i])
            
    # Change the format of date in oldDate to desired format of date
    def replaceDate(self, oldDate):
        newDate = re.split('-|\.|/|\n',oldDate)
        newMonth = newDate[0]
        newDay = newDate[1]
        newYear = newDate[2]
        newAlphaMonth = calendar.month_abbr[int(newMonth)]
        newDate = newAlphaMonth + ". " + newDay + ", " + newYear
        return newDate
        
