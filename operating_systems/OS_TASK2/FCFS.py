# FCFS 1211753 mr.abdelrahman Shahen Dr. Abdel Salam Sayad
import matplotlib.pyplot as plt
runTime = 201


class Process:  # a process class that holds process's data

    def __init__(self, processId, arrivalTime, burstTime, comesBackAfter, priority):
        self.processId = processId
        self.arrivalTime = arrivalTime
        self.burstTime = burstTime
        self.comesBackAfter = comesBackAfter
        self.priority = priority
        self.remainingTime = burstTime
        self.nextComesIn = arrivalTime
        self.executedBefore = False

    def __str__(self):
        return f"P{self.processId}: Arrival Time = {self.arrivalTime}, Burst Time = {self.burstTime}, Comes Back " \
               f"After = {self.comesBackAfter}, Priority = {self.priority},nextComesIn: {self.nextComesIn} "


def createProcesses(values):
    processes = []
    for i in range(1, 8):  # iterate 7 times 1 to 8
        arrivalTime = values[0][i - 1]
        burstTime = values[1][i - 1]
        comesBackAfter = values[2][i - 1]
        priority = values[3][i - 1]
        process = Process(i, arrivalTime, burstTime, comesBackAfter, priority)
        processes.append(process)
    return processes


def main():
    currentTime = 0
    values = [[0, 1, 3, 4, 6, 7, 8],  # the values for the processes status and times
              [10, 8, 14, 7, 5, 4, 6],
              [2, 4, 6, 8, 3, 6, 9],
              [3, 2, 3, 1, 0, 1, 2]]

    processes = createProcesses(values)  # process creations

    #  ---------------- neccesary values------------
    allArrivals = []
    readyQueue = []
    executions = []
    waitingQueue = []
    startTimes = []
    endTimes = []
    process = None
    # -------------------------------------------------

    # inserting all processes to the waiting Queue
    for process in processes:
        waitingQueue.append(process)

# time starts here
    while currentTime != runTime:
        if waitingQueue:  # waiting queue to ready queue handling
            for All in waitingQueue:
                if All.nextComesIn == currentTime:
                    readyQueue.append(waitingQueue.pop(0))
                    print(f"process{All.processId} inserted to the ready Queue at time:{currentTime + 1}")

        if readyQueue:  # ready queue processes scheduler
            front = readyQueue[0]
            if currentTime == 0:
                process = front
            else:
                if front.remainingTime == front.burstTime:
                    process = front
            if process.remainingTime == process.burstTime:
                startTimes.append(currentTime)
                print(f"process{process.processId} started execution at time:{currentTime}")
            process.remainingTime -= 1
            if process.remainingTime == 0:  # if process just finished
                print(f"process{process.processId} finished execution at time:{currentTime + 1}")
                process.remainingTime = process.burstTime
                if process.executedBefore:
                    allArrivals.append(process.nextComesIn + 1)
                else:
                    allArrivals.append(process.nextComesIn)
                process.executedBefore = True
                process.nextComesIn = currentTime + process.comesBackAfter
                executions.append(process)
                waitingQueue.append(readyQueue.pop(0))
                print(f"process{process.processId} inserted to the waiting Queue at time:{currentTime + 1}")
                endTimes.append(currentTime + 1)
        currentTime += 1

        # finished scheduling

    turnAroundTimes = []
    waitingTimes = []
    executionTimes = []

    i = 0
    for process in executions:
        turnAroundTimes.append(endTimes[i] - allArrivals[i]) # s
        executionTimes.append(endTimes[i] - startTimes[i])
        waitingTimes.append(turnAroundTimes[i] - executionTimes[i])
        i += 1

    i = 0
    summation = 0
    for i in turnAroundTimes:
        summation += i
    average1 = float(summation / len(turnAroundTimes))

    i = 0
    summation = 0
    for i in waitingTimes:
        summation += i
    average2 = float(summation / len(waitingTimes))

    print(f"average turn around times= {average1}    ", end="")
    print(f"average waiting times= {average2}    ")

    myProcesses = []
    counter = 0

    for i in executions:
        myProcesses \
            += [{f'process_name': f" p{i.processId}", 'start_time': startTimes[counter],
                 'end_time': endTimes[counter]}, ]
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
