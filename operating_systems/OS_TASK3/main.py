import csv  # to get the code to read .csv

N = 10  # number of processes
M = 5  # number of types of resources

isValid = [False, False, False]  # to verify dimensions are consistent

available = []  # available vector
request = []  # request vector
allocation = []  # allocation vector

# 1- Read the input files, and verify that the dimensions are consistent.

# ----------------------------available--------------------------------------

with open("Available.csv", 'r') as file:  # open file
    reader = csv.reader(file)  # create a reader
    next(reader)  # skip 1st line
    secondLine = next(reader)  # take 2nd line
    available = [int(value) for value in secondLine]
    # take all integers in secondLine and append them to available

    if len(available) == M:
        isValid[0] = True  # valid

print("available:", available)

# ----------------------------------------------------------------------------


# ----------------------------request-----------------------------------------

with open("Request.csv", 'r') as file:
    reader = csv.reader(file)  # create a reader
    next(reader)  # skip first line

    for row in reader:  # as long as there are lines:
        process_id = row[0]  # get the process id
        process_data = [int(value) for value in row[1:]]  # convert resource values to int
        request.append((process_id, process_data))  # add them as a tuple

    if len(request) == N:
        for instance in request:
            if len(instance[1]) == M:
                isValid[1] = True

print("request:", request)

# ----------------------------------------------------------------------------


# ----------------------------allocation--------------------------------------

with open("allocation.csv", 'r') as file:
    reader = csv.reader(file)  # create a reader
    next(reader)  # skip first line

    for row in reader:  # as long as there are lines:
        process_id = row[0]  # get the process id
        process_data = [int(value) for value in row[1:]]  # convert resource values to int
        allocation.append((process_id, process_data))  # add them as a tuple

    if len(request) == N:
        for instance in request:
            if len(instance[1]) == M:
                isValid[2] = True
print("allocation:", allocation)
# ----------------------------------------------------------------------------

filesNames = ["available.csv", "request.csv", "allocation.csv"]
counter = 0
for v in isValid:
    if v:
        print(f" {filesNames[counter]} is valid")
    else:
        print(f" {filesNames[counter]} is invalid")
    counter += 1

# 2- Detect whether or not there is a deadlock condition.
print("executing...")
# 1-

# a)
work = available  # let Work be vector of length M

# b)
finish = [False] * N  # let Finish be vector of length N
finished = True  # to check each resource allocated for process i
for i in range(N):  # for all processes

    # check all resources allocated for process i
    for j in allocation[i][1]:
        if j != 0:
            finished = False
            break

    # if process has no resources allocated then it has finished
    if finished:
        finish[i] = True
    else:
        finish[i] = False

    finished = True  # reset finished

# 2-
counter = 0
canGrant = True
i = 0
executions = []
while i < N:
    print(f"{counter})")
    print(f"process: {i}")
    print(f"finish:{finish}")
    if not finish[i]:

        print(f"request: {request[i][1]}")
        print(f"work: {work}")
        for j in range(M):
            if request[i][1][j] > work[j]:
                canGrant = False
                break
    if canGrant:
        print("granted")
    else:
        print("not granted")

    if canGrant:
        # 3-
        for j in range(M):
            work[j] = work[j] + allocation[i][1][j]
        finish[i] = True
        executions.append(allocation[i][0])

        t = 0
        for t in range(N):
            if not finish[t]:
                break
        i = t - 1

    canGrant = True
    i += 1
    print()
    counter += 1
    if all(finish):
        print("finished execution")
        break
print("--------------------------------------------------------------------------------")
print()

print("results:")
print("---------------------------------------------------")
# 4-
deadLock = False
deadLocked = []

for i in range(N):
    if not finish[i]:
        deadLock = True
        deadLocked.append(request[i][0])


#  3- If the system is deadlocked, list the processes that are deadlocked.
if deadLock:
    print("ops! deadlock detected!! ")
    print("deadlocked processes:", end="")
    print(deadLocked)

#  4- If not, show a series of process executions that are possible without deadlock.
else:
    print("nice! no deadlock detected!!")
    print("series of executions:", end="")
    print(executions)
print("---------------------------------------------------")
print()
print("Process finished with exit code 0")
