# priority2 1211753 mr.abdelrahman Shahen Dr. Abdel Salam Sayad
#s

import multiprocessing
from time import sleep
import numpy as np

import matplotlib.pyplot as plt

speed = 20
runTime = 54  # (Reminder: 200)
quantum = 5  # (Reminder: 5)
decrement = 1  # (Reminder: 1)
timeToStay = 5  # (Reminder: 5)
v = 0


class Process:
    def __init__(self, process_id, arrival_time, burst_time, comes_back_after, priority):
        self.process_id = process_id
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.comes_back_after = comes_back_after
        self.priority = priority

    def __str__(self):
        return f"P{self.process_id}: Arrival Time = {self.arrival_time}, Burst Time = {self.burst_time}, Comes Back " \
               f"After = {self.comes_back_after}, Priority = {self.priority}"


def createProcesses(values):
    processes = []
    for i in range(1, 8):  # iterate 7 times 1 to 8
        arrival_time = values[0][i - 1]
        burst_time = values[1][i - 1]
        comes_back_after = values[2][i - 1]
        priority = values[3][i - 1]

        process = Process(i, arrival_time, burst_time, comes_back_after, priority)
        processes.append(process)

    return processes


def executeProcess(process, currentTime, removed, finishTimes, remainingTimes, stopFlag,
                   readyQueue, currentPriority, startTimes, numOfExe, waitMore):
    global v

    print(f"Executing process{process.process_id} at time {currentTime.value}")



    startTimes[numOfExe.value] = currentTime.value
    for i in range(process.burst_time):
        sleep(1/speed)
        sleep(waitMore.value * 0.37/speed)

    removed[process.process_id - 1] = 0
    print(f"P{process.process_id} has { remainingTimes[process.process_id -1 ]} left  at time {currentTime.value}")
    finishTimes[numOfExe.value] = currentTime.value


def readyQueueFiller(readyQueue, processes, currentTime, stopFlag, currentPriority, waitMore):
    sleepTimes = [1, 2, 1, 2, 1, 1]
    index = 0
    for process in processes:
        if process.process_id == 1:
            continue
        for i in range((sleepTimes[index])):
            sleep( 1/ speed)
            sleep(waitMore.value * 0.9/speed)
        readyQueue.append(process)
        index += 1
        print(f" process.priority{process.priority} currentPriority.value{currentPriority.value}")

        print(f"process {process.process_id} just got into the ready queue {currentTime.value}")


def readyQueueFiller2(readyQueue, waitingProcesses, currentTime, timesToWait, removed):
    while currentTime.value < runTime:
        for process in waitingProcesses:
            if removed[process.process_id - 1] == 1:
                continue
            if timesToWait[process.process_id - 1] == 0:
                readyQueue.append(process)
                removed[process.process_id - 1] = 1
                timesToWait[process.process_id - 1] = process.comes_back_after
    print("readyQueue filler stopped")


def schedule(readyQueue, waitingProcesses, completedProcesses, currentTime, removed, finishTimes,
             remainingTimes, stopFlag, currentPriority, startTimes,
             numOfExe, waitMore, executedTimes, priorities,readyTimes,currentPriorityId):
    while currentTime.value < runTime:

        while readyQueue:
            index = 0
            realIndex = 0
            min = priorities[0]
            for i in priorities:
                if i<min:
                    min = i
            while(1):
                waitMore.value +=1
                if min == priorities[index]:
                    break
                index += 1

                j = 0
                for i in readyQueue:
                    if i.process_id -1 == index:
                        realIndex = j
                    j += 1

            process = readyQueue.pop(realIndex)
            print(f"fa3eer {process.process_id} {priorities[process.process_id -1 ]}")
            currentPriority.value = priorities[process.process_id - 1]
            currentPriorityId.value = process.process_id
            print(f"currentPriority.value changed to {currentPriority.value} which is processes {process.process_id}")
            executeProcess(process, currentTime, removed, finishTimes, remainingTimes,
                            stopFlag, readyQueue, currentPriority, startTimes, numOfExe, waitMore)

            executedTimes[process.process_id - 1] += 1
            completedProcesses.append(process)
            numOfExe.value += 1
            waitingProcesses.append(process)
            print(f"added process{process.process_id} to the waiting processes ")
    print("scheduler stopped")


def timeStart(waitingProcesses, currentTime, timesToWait,
              removed, waitMore, readyQueue, currentPriority,
              stopFlag, priorities, readyTimes,currentPriorityId):

    while currentTime.value < runTime:  # keep counting till runTime
        sleep(1 /speed  )  # sleep for one unit of time
        currentTime.value += 1  # then increase time
        print(f"current time: {currentTime.value}")  # then print that current time
        for process in readyQueue:
            readyTimes[process.process_id - 1] += 1
        waitMore.value = 1
        for process in readyQueue:
            if readyTimes[process.process_id -1] % 5 == 0:
                if currentPriorityId.value == process.process_id:
                   if currentPriority.value != 0:
                       currentPriority.value -= 1
                if priorities[process.process_id - 1] != 0:
                    priorities[process.process_id - 1] -= 1
                    print(f"p{process.process_id} is now {priorities[process.process_id -1]}")
        waitMore.value = 0
        for process in waitingProcesses:
            if removed[process.process_id - 1] == 1:
                continue
            if timesToWait[process.process_id - 1] != 0:
                timesToWait[process.process_id - 1] -= 1
    print("timer stopped")


def main():
    manager = multiprocessing.Manager()  # Creating a manager object
    readyQueue = manager.list()  # creating ready queue
    numOfExe = multiprocessing.Value('i', 0)
    waitingProcesses = manager.list()  # creating waiting list
    startTimes = manager.Array('i', [0] * 100)  # creating a shared start times array
    finishTimes = manager.Array('i', [0] * 100)   # creating a shared finish times array
    turnAroundTimes = manager.Array('i', [0] * 7)  # creating a shared turn around times array
    waitingTimes = manager.Array('i', [0] * 7)  # creating a shared waiting times array
    remainingTimes = manager.Array('i',[10, 8, 14, 7 , 5, 4, 6])
    priorities = manager.Array('i', [3, 2, 3, 1, 0, 1, 2] )

    stopFlag = manager.Value('i', 0)
    currentPriority = manager.Value('i', 0)
    currentPriorityId = multiprocessing.Value('i',0)
    completedProcesses = manager.list()  # creating a list for completed processes
    currentTime = multiprocessing.Value('i', 0)  # shared timer
    timesToWait = multiprocessing.Array('i', [2, 4, 6, 8, 3, 6, 9])  # shared array saves the current
    waitMore = multiprocessing.Value('f', 0)
    # time to wait for each process
    removed = multiprocessing.Array('i', [0] * 7)  # shared array to save each removed
    executedTimes = multiprocessing.Array('i',[0] * 7)  # shared array to save how many times the process went into cpu
    readyTimes = multiprocessing.Array('i', [0] * 7)
    # process because I couldn't find a way to remove it without having issues
    values = [[0, 1, 3, 4, 6, 7, 8],  # the values for the processes status and times
              [10, 8, 14, 7, 5, 4, 6],
              [2, 4, 6, 8, 3, 6, 9],
              [3, 2, 3, 1, 0, 1, 2]]

    processes = createProcesses(values)  # process creations

    readyQueue.append(processes[0])
    schedulingProcess = multiprocessing.Process(target=schedule, args=(
                                                                       readyQueue, waitingProcesses,
                                                                       completedProcesses, currentTime,
                                                                       removed, finishTimes,
                                                                       remainingTimes, stopFlag, currentPriority,
                                                                       startTimes, numOfExe,
                                                                       waitMore, executedTimes, priorities,readyTimes,currentPriorityId))
    # creating the process that will

    # schedule the 7 processes and blot the gantt chart
    readyQueueProcess = multiprocessing.Process(target=readyQueueFiller, args=(readyQueue, processes,
                                                                               currentTime, stopFlag
                                                                               , currentPriority, waitMore))
    # ready queue filling process

    #readyQueueProcess2 = multiprocessing.Process(target=readyQueueFiller2, args=(readyQueue,
    #                                                                          waitingProcesses,
    #                                                                             currentTime, timesToWait, removed))
    # ready queue filling process2

    timerProcess = multiprocessing.Process(target=timeStart, args=(waitingProcesses,
                                                                   currentTime, timesToWait,
                                                                   removed, waitMore,
                                                                   readyQueue, currentPriority,
                                                                   stopFlag, priorities, readyTimes,currentPriorityId))
    # creating a timer process that will start the timer
   # readyQueueProcess2.start()  # starting filling the ready queue with the waiting processes
    timerProcess.start()  # starting timer
    schedulingProcess.start()  # starting scheduler
    readyQueueProcess.start()  # starting filling the ready queue

    sleep(runTime/speed + 2)

    occurrences = [0, 0, 0, 0, 0, 0, 0]
    realFinishTimes = [0, 0, 0, 0, 0, 0, 0]
    indx = 0

    for i in finishTimes:
        if i == 0:
            break
        occurrences[completedProcesses[indx].process_id - 1] += 1
        if executedTimes[completedProcesses[indx].process_id - 1] == occurrences[completedProcesses[indx].process_id - 1]:
            realFinishTimes[completedProcesses[indx].process_id - 1] = i
        indx += 1

    for process in processes:
        turnAroundTimes[process.process_id-1] = realFinishTimes[process.process_id-1] - process.arrival_time
        waitingTimes[process.process_id-1] = turnAroundTimes[process.process_id-1] - process.burst_time

    sum = 0
    for i in range(7):
        sum += turnAroundTimes[i-1]
    average1 = float(sum / 7)

    sum = 0
    for i in range(7):
        sum += waitingTimes[i]
    average2 = float(sum / 7)


    print(f"average turn around times= {average1}    ", end="")
    print(f"average waiting times= {average2}    ")

    myProcesses = []
    counter = 0

    for i in completedProcesses:
        myProcesses += [
        {f'process_name': f" p{i.process_id}", 'start_time': startTimes[counter], 'end_time': finishTimes[counter]},
                       ]
        counter += 1

    fig, ax = plt.subplots(figsize=(20, 12))
    for i, process in enumerate(myProcesses):
        start_time, end_time = process['start_time'], process['end_time']
        duration = end_time - start_time

        ax.barh(i, duration, left=start_time, height=0.5, label=process['process_name'])

    ax.set_yticks(range(len(myProcesses)))
    ax.set_yticklabels([process['process_name'] for process in myProcesses])

    ax.set_xticks(range(min([process['start_time'] for process in myProcesses]),
                        max([process['end_time'] for process in myProcesses]) + 1, 1))

    ax.grid(True, which='both', linestyle='-', linewidth=0.5)

    ax.set_xlabel('Timeline')
    ax.set_ylabel('Process')
    ax.legend()

    plt.show()

  # readyQueueProcess2.join()  # joining new process to the other processes
    timerProcess.join()  # joining new process to the other processes
    readyQueueProcess.join()  # joining new process to the other processes
    schedulingProcess.join()  # joining the process to the other processes

if __name__ == "__main__":
    main()
