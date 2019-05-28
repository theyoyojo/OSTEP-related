#!/bin/python2
data = open("numbers")

counter = 0

ticketcount = 0

total = 0

tickets = []

# first line is number of jobs
# tickets per each job follow
# then the random numbers

for line in data:
    item = int(line.split()[0])
    if counter == 0:
        ticketcount = item
    elif counter <= ticketcount:
        tickets.append(item)
        total = total + item
    else:
        winner = item % total
    counter = counter + 1
