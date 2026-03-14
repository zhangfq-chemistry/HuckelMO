#!/usr/bin/python


import sys
import os
import string


fh = open("1.txt")
for line in fh:
	line=line.strip();	
	if (";" in line):
		continue;
	line.replace("\\", "\:\\")
	print (line)

fh.close()

