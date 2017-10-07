# Group Member 1 : Akarsha Sehwag 2015010
# Group Member 2 : Kushagra Arora 2015049

import sys

data_filename = "data.txt"
metadata_filename = "meta.txt"
content = []

#reading data file
with open(data_filename) as dataFile:
	content = dataFile.readlines()

#removing the newline character from each line
content = [x.strip() for x in content]

#reading meta data file
with open(metadata_filename) as metaFile:
	meta = metaFile.readlines()

#removing the newline character from each line
meta = [x.strip() for x in meta]	

headings = content[0].split(",")
del content[0]

#checking if command line argument is present
if(len(sys.argv) == 2):
	sum_field = sys.argv[1]
else:
	sum_field = None

#parsing meta data
ordering = {}
dataType = {}
dataSize= {}

for index,x in enumerate(meta):
	elements = x.split(',')
	ordering[elements[0]] = index
	dataType[elements[0]] = elements[1]
	dataSize[elements[0]] = elements[2]

actualOrder = {}
for index, i in enumerate(headings):
	actualOrder[index] = ordering[i]

flag = 0
total=0
if(sum_field):
	if(dataType[sum_field] == 'I'):
		flag = 1
		total = 0
	elif(dataType[sum_field] == 'F'):
		flag = 2;
		total = 0.0

displayHeadings = [i for i in headings]
for index, i in enumerate(headings):
	displayHeadings[actualOrder[index]] = i

#displaying headings
for i in displayHeadings:
	print "%10s " % (i,),
print

for j, x in enumerate(content):
	elements = x.split(",")
	display = [i for i in elements]
	for index, i in enumerate(elements):
		display[actualOrder[index]] = i
	print
	for index, i in enumerate(display):
		print "%10s " %  (i,),
	#if sum_field is present and is integer
	if(flag == 1):
		total += int(display[ordering[sum_field]])
	#if sum_field is present and is float
	elif(flag == 2):
		total += float(display[ordering[sum_field]])

if(sum_field):
	if(flag):
		print "Total: ", total
	else:
		print "Error. This is not a numeric field."