# SRTF 1211753 mr.abdelrahman Shahen Dr. Abdel Salam Sayad

import matplotlib.pyplot as plt

runTime = 200


class Process:
    def __init__(self, process_id, arrival_time, burst_time, comes_back_after, priority):
        self.process_id = process_id
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.comes_back_after = comes_back_after
        self.priority = priority
        self.remainingTime = burst_time
        self.nextComesIn = arrival_time
        self.executed = False
        self.executedBefore = False
        self.executingFor = 0

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

    values = [[0, 1, 3, 4, 6, 7, 8],  # the values for the processes status and times
              [10, 8, 14, 7, 5, 4, 6],
              [2, 4, 6, 8, 3, 6, 9],
              [3, 2, 3, 1, 0, 1, 2]]
    allArrivals = []
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


    finishedProcesses = []
    actualStarts = []
    actualEnds = []
    acutalExecutionTimes = []

    while currentTime != runTime:
        if currentTime == 0:
            readyQueue.append(waitingQueue.pop(0))
            print("process 2 just got in to the ready queue")
        if readyQueue:
            front = readyQueue[0]
            if currentTime == 0:
                process = front
                previousProcess = process
                startTimes.append(currentTime)
                actualStarts.append(currentTime)
                print(f"started executing p{process.process_id} {currentTime}")
            else:
                if front.remainingTime < process.remainingTime or process.executed:
                    previousProcess = process
                    if not process.executed:
                        executions.append(process)
                        endTimes.append(currentTime)
                        print(f"process {process.process_id} stoped  at {currentTime + 1}")
                    process = front
                    process.executed = False
                    print(f"started executing p{process.process_id} {currentTime}")
            if (process.remainingTime == process.burst_time or
                    previousProcess.process_id != process.process_id) and (currentTime != 0):
                previousProcess = process
                startTimes.append(currentTime)
                if process.remainingTime == process.burst_time:
                    actualStarts.append(currentTime)

            process.remainingTime -= 1
            if process.remainingTime == 0:  # if process just finished
                print(f"process {process.process_id} finished exe at {currentTime + 1 }")
                process.remainingTime = process.burst_time
                if process.executedBefore:
                    allArrivals.append(process.nextComesIn + 1)
                else:
                    allArrivals.append(process.nextComesIn)
                process.executedBefore = True
                acutalExecutionTimes.append(process.executingFor)
                process.executed = True
                process.nextComesIn = currentTime + process.comes_back_after
                finishedProcesses.append(process)
                executions.append(process)
                waitingQueue.append(process)
                print(currentTime)
                readyQueue.remove(process)
                print(f"process {process.process_id} just got got into waiting queue {currentTime + 1}")
                endTimes.append(currentTime + 1)
                actualEnds.append(currentTime + 1)
        if waitingQueue:
            for All in waitingQueue:
                if All.nextComesIn == currentTime and currentTime != 0:
                    readyQueue.append(All)
                    waitingQueue.remove(All)
                    print(f"process {All.process_id} got into ready queue {currentTime }")
            readyQueue.sort(key=lambda p: p.remainingTime)

        currentTime += 1
        print(currentTime)
        print("ready Queue: ")
        for i in readyQueue:
            print(i)


    turnAroundTimes = []
    waitingTimes = []
    executionTimes = []




    i = 0
    for process in finishedProcesses:
        turnAroundTimes.append(actualEnds[i] - allArrivals[i])
        waitingTimes.append(turnAroundTimes[i] - process.burst_time)

        i += 1

    sum = 0
    for i in turnAroundTimes:
        sum += i
    average1 = float(sum / len(turnAroundTimes))

    sum = 0
    for i in waitingTimes:
        sum += i
    average2 = float(sum / len(waitingTimes))

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