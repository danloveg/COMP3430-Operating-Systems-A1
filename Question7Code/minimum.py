#!/usr/bin/python

""" Verifier script that minelement returns the right number """

filelines = []
numbers = []

with open('./numbers.txt', 'r') as f:
    filelines = f.readlines()

for line in filelines:
    try:
        numbers.append(int(line.replace('\n', '')))
    except:
        print "Non-number found!"

print 'Numbers length: {0}'.format(len(numbers))
print 'Minimum value: {0}'.format(min(numbers))

