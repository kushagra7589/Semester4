from random import randint

[n, k] = list(map(int, input().split(" ")))

MAP = {}
countLabel = [0, 0]
countWords = [0, 0]
setWords1 = []
setWords2 = []
for i in range(n) : 
	testData = list(map(str, input().split(" ")))
	label = int(testData[0])
	countLabel[label - 1] += 1;
	testData = list(set(testData[1: ]))
	countWords[label - 1] += len(testData)
	if(label == 1):
		setWords1.extend(testData)
	else:
		setWords2.extend(testData)
	for j in testData[1:] :
		if(j not in MAP):
			MAP[j] = [0, 0]
		MAP[j][label - 1] += 1

# print(MAP)

setWords1 = list(set(setWords1))
setWords2 = list(set(setWords2))

for i in range(k) :
	ans1 = (float(countLabel[0]))
	ans2 = (float(countLabel[1]))
	training = list(map(str, input().split()))
	# print(training)
	# print(ans1, ans2)
	for j in training:
		if j in MAP :
			# print(MAP[j][0])
			# print(countWords[0])
			# print(MAP[j][1])
			# print(countWords[1])
			ans1 *= (float(MAP[j][0]) + 1) / len(setWords1)
			ans2 *= (float(MAP[j][1]) + 1) / len(setWords2)
	# 		print(ans1, ans2)
	# print(ans1, ans2)
	if(ans1 >= ans2) : 
		print(1)
	else:
		print(2)