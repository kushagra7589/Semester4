# Group Member 1 : Akarsha Sehwag 2015010
# Group Member 2 : Kushagra Arora 2015049

import sys

filename = "data.txt"
content=[]

#reading file
with open(filename) as file:
	content = file.readlines()

#removing the newline character from each line
content = [x.strip() for x in content]

headings = content[0].split(",")
del content[0]

#checking if command line argument is present
if(len(sys.argv) == 2):
	sum_field = sys.argv[1]
else:
	sum_field = None

sum_index = -1;
if(sum_field):
	for index, i in enumerate(headings):
		if(i == sum_field) :
			sum_index = index

total = 0
flag = 0

for i in headings:
	print "%10s " % (i,),
print
for j, x in enumerate(content):
	elements = x.split(",")
	if(sum_field):
		try :
			#trying whether given element is an integer field
			total += int(elements[sum_index])
		except:
			try:
				#trying whether given element is float
				total += float(elements[sum_index])
			except:
				#when element is is a non-numeric field
				flag = 1
	print
	for index, i in enumerate(elements) :
		print "%10s " % (i,),

if(sum_field):
	if(flag):
		print "Error. This is not a numeric field."
	else:
		print "Total: ", total