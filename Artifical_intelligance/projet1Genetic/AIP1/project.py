import random
import matplotlib.pyplot as plt
import matplotlib.patches as patches


# operation is like assiging the current part of the job to a specefic machine like for example 
# assigning j1 to M1 for 20 time unites M1[20]
class Operation:  # Gene
    def __init__(self, jobID, machineID, processingTime):
        self.jobID = jobID
        self.machineID = machineID
        self.processingTime = processingTime
    


# job is the set of operations to operate in the machines for example j1 :  M1[10] -> M2[5] -> M4[12]
class Job:
    def __init__(self, jobID, operations):
        self.jobID = jobID
        self.operations = operations
    


# machine is wehere we assign operations of jobs e.g. : M1
class Machine:
    def __init__(self, machineID):
        self.machineID = machineID
        self.schedule = []
   


# this function is made to create the requered jobs and machines 
def decodeInput(inputJobs):
    jobs = []  # list of jobs to save
    jobID = 0 # the job id to track the current job
    uniqueMachines = set() # set to prevent repetetion of machines
    for job in inputJobs: # M1[10] -> M2[5] -> M4[12]
        
        operations = []
        for operation in job.split('->'): #  M1[10] , M2[5] , M4[12]
            machine, time = operation.strip().split('[')  # machine = M1 , time = 10]
            machineID = int(machine[1:]) - 1  # machine_id for [M,1] = 1 - 1 = 0  
            processingTime = int(time[:-1]) # processing_time for [ 1 , 0 , ] ] from the first char to the last without the last = 10 
            operations.append(Operation(jobID, machineID, processingTime))  # create an operation with the exeplored data 
            uniqueMachines.add(machineID) # add the exeplored machine's id to the machines_set

        jobs.append(Job(jobID, operations)) # create a job with the exeplored data 
        jobID += 1 # increment jobs

    machines = []
    temp = []
    for i in uniqueMachines:
        temp.append(i)

    maxMachineID = max(temp)

    i = 0
    for i in range(maxMachineID + 1):
        machines.append(Machine(i))



    return jobs, machines


def generateInitialPopulation(jobs, numOfSolutions):
    population = []
    for _ in range(numOfSolutions):

        solution = []

        for job in jobs:
            solution.extend(job.operations)

        random.shuffle(solution)

        population.append(solution)

    return population

def getFitness(solution, machines):


# Child1: (M1[10], M1[8], M3[15], M4[12], M2[7])

    # clear machines schedules
    for machine in machines:
        machine.schedule = []


    jobsEndTimes = [0] * len(jobs)  # track the end time of the last operation for each job
    machinesAvailableTimes = [0] *  len(machines)  # track when each machine becomes available
    CurrentOperationIndicesOfJobs = [0] * len(jobs)  # track the current operation index for each job


    currentOperations = list(solution)  # Start with all operations pending

    while currentOperations:
        
        isScheduled = False
        for operation in currentOperations:
            jobID = operation.jobID
            machineID = operation.machineID
            # check if this operation is the next in sequence for the job
            if CurrentOperationIndicesOfJobs[jobID] < len(jobs[jobID].operations) and \
               jobs[jobID].operations[CurrentOperationIndicesOfJobs[jobID]] == operation:
                
                # get start time 
                startTime = max(jobsEndTimes[jobID], machinesAvailableTimes[machineID])
                endTime = startTime + operation.processingTime
                if startTime == machinesAvailableTimes[machineID]:
                    # update schedules and available times
                    machines[machineID].schedule.append((operation, startTime, endTime))
                    jobsEndTimes[jobID] = endTime
                    machinesAvailableTimes[machineID] = endTime
                    # move to the next operation for this job
                    CurrentOperationIndicesOfJobs[jobID] += 1
                    # remove the operation from pending list and make flag true
                    currentOperations.remove(operation)
                    isScheduled = True
                    break
        # If no operation could be scheduled, increment the time of the earliest available machine
        if not isScheduled:
            nextAvailableTime = min(machinesAvailableTimes) 
            for i in range(len(machinesAvailableTimes)):
                if machinesAvailableTimes[i] == nextAvailableTime:
                    machinesAvailableTimes[i] += 1



    makespan = max(machinesAvailableTimes)
    return makespan

def selection(population, fitnesses): 
    sumFitness = sum(fitnesses)
    probabilities = [1 - (f / sumFitness) for f in fitnesses]  
    selected = random.choices(population, probabilities, k=2)
    return selected

def crossover(parent1, parent2):
    numOfOperations = len(parent1)
    firstCrossoverPoint, secondCrossoverPoint = sorted(random.sample(range(numOfOperations), 2))
    child1 = [None]*numOfOperations
    child2 = [None]*numOfOperations
    
    child1[firstCrossoverPoint:secondCrossoverPoint] = parent1[firstCrossoverPoint:secondCrossoverPoint]
    child2[firstCrossoverPoint:secondCrossoverPoint] = parent2[firstCrossoverPoint:secondCrossoverPoint]
    
    track1, track2 = secondCrossoverPoint, secondCrossoverPoint
    for i in range(numOfOperations):
        if parent2[(i + secondCrossoverPoint) % numOfOperations] not in child1:
            child1[track1 % numOfOperations] = parent2[(i + secondCrossoverPoint) % numOfOperations]
            track1 += 1
        if parent1[(i + secondCrossoverPoint) % numOfOperations] not in child2:
            child2[track2 % numOfOperations] = parent1[(i + secondCrossoverPoint) % numOfOperations]
            track2 += 1
            
    return child1, child2

def mutation(solution, mutationRate):
    if random.random() < mutationRate:
        i, j = random.sample(range(len(solution)), 2)
        solution[i], solution[j] = solution[j], solution[i]
    return solution

def GENETICALGORITHM(jobs, machines, numOfGenerations, numOfSolutions, mutationRate):
    population = generateInitialPopulation(jobs, numOfSolutions)
    
    bestSolution = None
    bestFitnessValue = float('inf')

    saveSize = max(1, numOfSolutions // 10)  # number of solutions to save

    
    for _ in range(numOfGenerations):

        fitnessesValues = [getFitness(indicies, machines) for indicies in population]
        print(fitnessesValues[0])
        

        # keep the best old solutions
        indiciesToSave = sorted(range(len(fitnessesValues)), key=lambda i: fitnessesValues[i])[:saveSize]
        solutionsToSave = [population[i] for i in indiciesToSave]
        
        newPopulation = []
        for _ in range(numOfSolutions // 2):
            parent1, parent2 = selection(population, fitnessesValues) # 
            child1, child2 = crossover(parent1, parent2)
            newPopulation.append(mutation(child1, mutationRate))
            newPopulation.append(mutation(child2, mutationRate))
        
         
        newPopulation.extend(solutionsToSave)

        population = newPopulation
        
        bestFitnessValueOfCurrentPopulation = min(fitnessesValues)
        if bestFitnessValueOfCurrentPopulation < bestFitnessValue:
            bestFitnessValue = bestFitnessValueOfCurrentPopulation
            bestSolution = population[fitnessesValues.index(bestFitnessValue)]

            

    return bestSolution, bestFitnessValue


# this function is used to plot the results in a way that any human can understand 
def plotResults(machines):
    _, ax = plt.subplots(figsize=(40, 40))  
    barHeight = 0.4  
    colors = plt.cm.tab20.colors
    maxX = 0
    for machine in machines:
        for task in machine.schedule:
            operation, startTime, endTime = task
            color = colors[operation.jobID % len(colors)]
            ax.add_patch(patches.Rectangle((startTime, machine.machineID - barHeight/2), endTime - startTime, barHeight, edgecolor='black', facecolor=color))
            ax.text(startTime + (endTime - startTime) / 2, machine.machineID, f'J{operation.jobID+1}', ha='center', va='center', color='white')
            maxX = max(maxX, endTime)

    ax.set_xlim(-5, maxX + 10)  # set x axis limit to max end time plus some padding
    ax.set_ylim(-5, len(machines) + 5)  
    ax.set_xticks(range(0, maxX + 11, 1))  # set x axis ticks to increase 1 by 1  
    ax.set_yticks([i for i in range(len(machines))])
    ax.set_yticklabels([f'M{i+1}' for i in range(len(machines))])
    ax.set_xlabel('time')
    ax.set_ylabel('machines')
    ax.grid(True)   
    plt.show()


# read file
filePath = "input.txt"
inputJobs = []

with open(filePath, 'r') as file:
    for line in file:
        inputJobs.append(line.strip('\n'))


# bring ingrediants 
jobs, machines = decodeInput(inputJobs)

# let it cook
bestSchedule, bestFitnessesValues = GENETICALGORITHM(jobs, machines, numOfGenerations=200, numOfSolutions=50, mutationRate=0.1)

# introduce the meal
plotResults(machines)

""" 

dOnE bY: 
Abdelrahman shaheen 1211753
Mahmoud Awad 1212677

"""