import multiprocessing
from time import sleep
from datetime import datetime


# 1 unit of time : 1 second

class Process:
    def __init__(self, process_id, arrival_time, burst_time, comes_back_after, priority):
        self.process_id = process_id
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.comes_back_after = comes_back_after
        self.priority = priority

    def __str__(self):
        return f"P{self.process_id}: Arrival Time = {self.arrival_time}, Burst Time = {self.burst_time}, Comes Back After = {self.comes_back_after}, Priority = {self.priority}"


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


def executeProcess(process):
    current_time = datetime.now().strftime("%H:%M:%S")
    print(f"Executing {process} at time {current_time}")

    # Simulate the process execution
    sleep(process.burst_time)

    finish_time = datetime.now().strftime("%H:%M:%S")
    print(
        f"P{process.process_id} finishes at time {finish_time}, comes back after {process.comes_back_after} units of time")


def main():
    values = [[0, 1, 3, 4, 6, 7, 8],
              [10, 8, 14, 7, 5, 4, 6],
              [2, 4, 6, 8, 3, 6, 9],
              [3, 2, 3, 1, 0, 1, 2]]

    processes = createProcesses(values)

    processPool = multiprocessing.Pool()

    for process in processes:
        processPool.apply_async(executeProcess, args=(process,))

    processPool.close()
    processPool.join()


if __name__ == "__main__":
    main()
