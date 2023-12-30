# priority non-preemptive 1211753 mr.abdelrahman Shahen Dr. Abdel Salam Sayad

import matplotlib.pyplot as plt

runTime = 201


class Process:
    def __init__(self, process_id, arrival_time, burst_time, comes_back_after, priority):
        self.originalPriority = priority
        self.process_id = process_id
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.comes_back_after = comes_back_after
        self.priority = priority
        self.remainingTime = burst_time
        self.nextComesIn = arrival_time
        self.executed = False
        self.timeInReadyQueue = 0
        self.inReadyQueue = False
        self.executedBefore = False

    def __str__(self):
        return f"P{self.process_id}: Arrival Time = {self.arrival_time}, Burst Time = {self.burst_time}, Comes Back " \
               f"After = {self.comes_back_after}, Priority = {self.priority},nextComesIn: {self.nextComesIn} , remaining {self.remainingTime} "


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

def main():
    currentTime = 0
    allArrivals = []
    values = [[0, 1, 3, 4, 6, 7, 8],  # the values for the processes status and times
              [10, 8, 14, 7, 5, 4, 6],
              [2, 4, 6, 8, 3, 6, 9],
              [3, 2, 3, 1, 0, 1, 2]]

    processes = createProcesses(values)  # process creations
    readyQueue = []
    executions = []
    readyQueue.append(processes[0])
    print("process 1 is already in the ready queue")
    waitingQueue =[]
    for process in processes:
        if process.process_id == 1:
            continue
        waitingQueue.append(process)

    startTimes = []
    endTimes = []
    process = None
    previousProcess = None


    while currentTime != runTime:
        if waitingQueue:
            j = len(waitingQueue)
            while j > (0):
                All = waitingQueue[len(waitingQueue) - j]
                if All.nextComesIn == currentTime and currentTime != 0:
                    j = len(waitingQueue)
                    All.inReadyQueue = True
                    readyQueue.append(All)
                    waitingQueue.remove(All)
                    print(f"process {All.process_id} got into ready queue {currentTime }")
                readyQueue.sort(key=lambda p: (p.priority, p.nextComesIn))
                j -= 1

        readyQueue.sort(key=lambda p: (p.priority, p.nextComesIn))
        if currentTime == 1:
            process2 = waitingQueue.pop(0)
            readyQueue.append(process2)
            print("process 2 just got in to the ready queue")
            process2.inReadyQueue = True
        if readyQueue:
            print("rr")
            for l in readyQueue:
                print(l)
            front = readyQueue[0]
            print(f"f{front}")
            print(f"f2{process}")
            k = 1
            while front == process:
                front = readyQueue[k]
            k += 1
            if currentTime == 0:
                process = front
                previousProcess = process
                startTimes.append(currentTime)
                process.inReadyQueue = False
                print(f"started executing p{process.process_id} {currentTime}")
            else:
                if process.executed:
                    previousProcess = process
                    process.inReadyQueue = True
                    process = front
                    process.inReadyQueue = False
                    process.executed = False
                    print(f"started executing p{process.process_id} {currentTime}")
            if (process.remainingTime == process.burst_time or
                    previousProcess.process_id != process.process_id) and (currentTime != 0):
                previousProcess = process
                startTimes.append(currentTime)
            process.remainingTime -= 1
            if process.remainingTime == 0:  # if process just finished
                process.timeInReadyQueue = 0
                print(f"process {process.process_id} finished exe at {currentTime + 1 }")
                process.remainingTime = process.burst_time
                if process.executedBefore:
                    allArrivals.append(process.nextComesIn + 1)
                else:
                    allArrivals.append(process.nextComesIn)
                process.executedBefore = True
                process.executed = True
                process.inReadyQueue = True
                process.nextComesIn = currentTime + process.comes_back_after
                process.priority = process.originalPriority
                executions.append(process)
                waitingQueue.append(process)
                print(currentTime)
                readyQueue.remove(process)
                print(f"process {process.process_id} just got got into waiting queue {currentTime + 1}")
                for i in waitingQueue:
                    print(i)
                endTimes.append(currentTime + 1)
        for pr in readyQueue:
            if pr.timeInReadyQueue % 5 == 0 and pr.priority != 0 and pr.timeInReadyQueue != 0 and pr.inReadyQueue:
                pr.priority -= 1
            pr.timeInReadyQueue += 1

        currentTime += 1
        print(currentTime)
        print("ready Queue: ")
        for i in readyQueue:
            print(i)


    turnAroundTimes = []
    waitingTimes = []

    executionTimes = []
    i = 0
    for process in executions:
        turnAroundTimes.append(endTimes[i] - allArrivals[i])  # s
        executionTimes.append(endTimes[i] - startTimes[i])
        waitingTimes.append(turnAroundTimes[i] - executionTimes[i])
        i += 1

    sum = 0
    for i in turnAroundTimes:
        sum += i
    average1 = float(sum / len(turnAroundTimes))

    sum = 0
    for i in waitingTimes:
        sum += i
    average2 = float(sum / len(waitingTimes))


    for m in allArrivals:
        print(f"{m} ",end="")
    print("")

    for p in executions:
        print(f"{p.burst_time} " , end="")
    print("")

    for p in executions:
        print(f"{p.originalPriority} " , end="")
    print("")

    print(f"average turn around times= {average1}    ", end="")
    print(f"average waiting times= {average2}    ")

    myProcesses = []
    counter = 0

    for i in executions:
        myProcesses += [
        {f'process_name': f" p{i.process_id}", 'start_time': startTimes[counter], 'end_time': endTimes[counter]},
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














if __name__ == "__main__":
    main()