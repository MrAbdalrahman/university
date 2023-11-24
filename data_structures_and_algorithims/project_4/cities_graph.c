// p4_1211753_abdulrahmanshahen_3 

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include < limits.h>

// structing the list node
struct node {
	int dest;
	struct node* next;
	double weight;
};

//structing the list of nodes
struct list {
	struct node* head;
};

// strcucting a graph which is a list of nodes
struct Graph {
	 int order;
	struct list* array;
};

// a function to create a new  list node
struct node* createList(int dest)
{
	struct node* tempList = NULL;
	tempList = (struct node*)malloc(sizeof(struct node));
		if (tempList == NULL)
			return NULL;

		tempList->dest = dest;
		tempList->next = NULL;
		tempList->weight = 0;
	return tempList;
}

// a function that creates a graph of the given order
struct Graph* createGraph(int order){

	struct Graph* tempGraph = NULL;
	tempGraph = (struct Graph*)malloc(sizeof(struct Graph));
	if (tempGraph == NULL)
		return NULL;

	//sets the order of the graph
	tempGraph->order = order;

	//  ceating a (num of vertices or order) sized array of lists
	tempGraph->array = (struct list*)malloc(order * sizeof(struct list));
	if (tempGraph->array == NULL) {
		free(tempGraph);
		return NULL;
	}

	// setting each list's head to NULL  
	for (int i = 0; i < order; i++)
		tempGraph->array[i].head = NULL;

	return tempGraph;
}

// a function to Add an edge to the given graph
void addEdge(struct Graph* graph, int src, int dest, double weight)
{
	// Add an edge from src to dest. A new node is
	// added to the adjacency list of src. The node
	// is added at the beginning
	struct node* temp = NULL;
	struct node* newNode = createList(dest);
	newNode->weight = weight;
	/*
		if the source vertix head is NULL (has no edges) then
		then make the newNode's next points to that head
		and then make that src point to the newNode
	*/
	if (graph->array[src].head == NULL) {
		newNode->next = graph->array[src].head;
		graph->array[src].head = newNode;
	}
	/*
		but if the source vertix head is not NULL (has at leas 1 edge)
		then set temp list to that head and keep moving until the last
		node then add newNode to the list
	*/
	else {

		temp = graph->array[src].head;

		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}


}

// a function to print the graph
void printGraph(struct Graph* graph)
{
	for (int i = 0; i < graph->order; i++) {
		struct node* temp = graph->array[i].head;
		while (temp) {
			printf("%d--%0.1f--%d\n", i, temp->weight, temp->dest);
			temp = temp->next;
		}
		printf("\n");
	}
}

// a function that delete the whole graph
void destroyGraph(struct Graph* graph) {
	if (graph == NULL)
		return;

	for (int i = 0; i < graph->order; i++) {
		struct node* currentNode = graph->array[i].head;
		while (currentNode != NULL) {
			struct node* temp = currentNode;
			currentNode = currentNode->next;
			free(temp);
		}
	}

	free(graph->array);
	free(graph);
}


// a BFS algorithim emplementation for a graph represented via
// an adjecency list
int BFS(struct Graph* graph, int src, int des,int* path,double* totalWeight) {
	int* visited = (int*)malloc(sizeof(int) * graph->order);
	if (visited == NULL)
		return -1;
	int* parent = (int*)malloc(sizeof(int) * graph->order);
	if (parent == NULL) {
		free(visited);
		return -1;
	}

	// initialization of arrays
	for (int i = 0; i < graph->order; i++) {
		visited[i] = 0;
		parent[i] = -1;
	}

	struct node* temp = NULL;
	int queue[100]; //  a queue of maxsize of 100
	int front = 0, rear = 0; // queue elements

	// visiting source
	visited[src] = 1;  
	queue[rear++] = src;


	while (front != rear) { // while the queue is not empty

		src = queue[front++]; // dequeue and compare with dest
		if (src == des) // if its dest then break (we reached the goal)
			break;

		// else keep going 
		temp = graph->array[src].head; // a temp node to move in the list

		while (temp != NULL) { // checks until the last node with it

			// if that node was not visited
			if (!visited[temp->dest]) { // then mark it as visited 
				// and add its head to parent
				// and enqueue it 

				visited[temp->dest] = 1;
				parent[temp->dest] = src;
				queue[rear++] = temp->dest;

			}

			// move to the next node 
			temp = temp->next;
		}
	}

	// if the source didn't have the chance to meet destenation
	// that means they dont have path between them
	if (src != des) {

		printf("No path found!\n");
		free(visited);
		free(parent);
		return -4;

	}

	// reconstruct and print the shortest path

	// a path and its length
	int pathLength = 0;

	int	node = des;

	while (node != -1) { 

		path[pathLength++] = node;
		node = parent[node];

	}

	*totalWeight = 0;
	while (parent[des] != -1) {
		struct node* temp2 = graph->array[parent[des]].head;
		while (temp2 != NULL) {
			if (temp2->dest == des) {
				*totalWeight += temp2->weight;
				break;
			}
			temp2 = temp2->next;
		}
		des = parent[des];
	}
	free(visited);
	free(parent);

	return pathLength;
}


int minDistance(int* dist, int* sptSet, int numVertices) {
	int min = INT_MAX, minIndex = 0;
	for (int v = 0; v < numVertices; v++) {
		if (!sptSet[v] && dist[v] < min) {
			min = dist[v];
			minIndex = v;
		}
	}
	return minIndex;
}

// a function to print the shortest path to dest
double printShortestPath(struct Graph* graph, int* parent, int dest, char** cities) {
	double totalWeight = 0.0;

	if (parent[dest] == -1) {
		printf("%s", cities[dest]);
		return totalWeight;
	}

	totalWeight += printShortestPath(graph, parent, parent[dest], cities);

	struct node* temp = graph->array[parent[dest]].head;
	while (temp != NULL) {
		if (temp->dest == dest) {
			printf(" -%0.1f-> %s", temp->weight, cities[dest]);
			totalWeight += temp->weight;
			break;
		}
		temp = temp->next;
	}

	return totalWeight;
}


void fprintShortestPath(struct Graph* graph, int* parent, int dest, char** cities,FILE* shortest_distance) {
	if (parent[dest] == -1) {
		fprintf(shortest_distance,"%s", cities[dest]);
		return;
	}

	 fprintShortestPath(graph, parent, parent[dest], cities, shortest_distance);

	struct node* temp = graph->array[parent[dest]].head;
	while (temp != NULL) {
		if (temp->dest == dest) {
			fprintf(shortest_distance," -%0.1f-> %s", temp->weight, cities[dest]);
			break;
		}
		temp = temp->next;
	}
}
	/*
		a function that implements Dijkstra's single source
		 shortest path algorithm for a graph represented using
		 adjList
	 */
void dijkstra(struct Graph* graph, int src, int dest,int* distance,int* parent) {

	// setting the order instead of keep accessing it
	int numVertices = graph->order;

	//  temperary array of integers allocated by the size of order

	int* procced = (int*)malloc(sizeof(int) * numVertices);
	if (procced == NULL) {
		printf("full memory");
		return;
	}

	// initializing the temperary arrays
	for (int i = 0; i < numVertices; i++) {

		distance[i] = INT_MAX; // infinity
		parent[i] = -1; 
		procced[i] = 0;
	}

	distance[src] = 0; // setting the source distance to 0
	//
	for (int i = 0; i < numVertices; i++) { // a for loop reapeatd order times

		int u = minDistance(distance, procced, numVertices); // temp verix u 
		procced[u] = 1; // adding the vertix u to the procced array

		struct node* currentNode = graph->array[u].head; // a temp node points to the reached node

		while (currentNode != NULL) {

			int v = currentNode->dest; // temp vertix v

			double weight = currentNode->weight; // temp weight to stop accessing it

			/*
				if v is not procceded and u is not infinity and
				the new cost is less than the old cost
				then update the distance and assign u as a parent 
			*/
			if (!procced[v] && distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
				distance[v] = distance[u] + weight;
				parent[v] = u;
			}

			// move to the next node in the adj list
			currentNode = currentNode->next;
		}
	}
	// wipe wipe wipe...
	free(procced); 
}

void printMenu() {
	printf(" please choose an operation: \n"
		"1- load cities\n"
		"2- enter source\n"
		"3- enter destenation\n"
		"4- Exit\n"
		);
}

int vertixOfCity(char* city, char** cities, int numOfCities) {
	for (int i = 0; i < numOfCities;i++)
		if (strcmp(cities[i], city) == 0)
			return i;
	return -1;
}

int main() {
	char** cities = (char**)malloc(sizeof(char*) * 40);
	char src[40];
	char dest[40];
	int numOfCities = 0;
	struct Graph* citiesGraph = NULL;
	int choice;
	int scanned = 0;


		FILE* citiesFile = fopen("cities.txt", "r");
		if (citiesFile == NULL)
			printf("file couldn't be oppened\n");

		// some temporary vars
		char* temp = (char*)malloc(sizeof(char) * 40);
		int  exists = 0;


		// scaning the file to get the order nedded for the graph and save sities(vertices)
		while (fgets(temp, 40, citiesFile) != NULL) {

			char* token1 = strtok(temp, "\t");
			char* token2 = strtok(NULL, "\t");
			char* token3 = strtok(NULL, "\t");

			char* tempInfo[3];

			tempInfo[0] = strdup(token1);
			tempInfo[1] = strdup(token2);
			tempInfo[2] = strdup(token3);

			{
				for (int i = 0;i < numOfCities;i++)
					if (strcmp(cities[i], tempInfo[0]) == 0)
						exists = 1;
				if (!exists)
					cities[numOfCities++] = tempInfo[0];
				exists = 0;
			}


			{
				for (int i = 0;i < numOfCities;i++)
					if (strcmp(cities[i], tempInfo[1]) == 0)
						exists = 1;
				if (!exists)
					cities[numOfCities++] = tempInfo[1];
				exists = 0;
			}

		}

		fclose(citiesFile);

		citiesGraph = createGraph(numOfCities); // graph creation

		int* distance = (int*)malloc(sizeof(int) * numOfCities);
		if (distance == NULL)
			printf("full memory");
		int* parent = (int*)malloc(sizeof(int) * numOfCities);
		if (parent == NULL) {
			free(distance);
			printf("full memory");
		}

		//initailization of distance and parent
		for (int i = 0; i < numOfCities; i++) {

			distance[i] = INT_MAX; // infinity
			parent[i] = -1;
		}



		FILE* citiesFile2 = fopen("cities.txt", "r");
		if (citiesFile2 == NULL)
			printf("file couldn't be oppened\n");

		int numOfInputs = 0;
		char*  srcs [100];
		char* dests[100];
		double totalWeightsBFS[100];
		double totalWeights[100];
		int totalEdges[100];
		int* pathes[100];
		int* parents[100];
		int* distances[100];


	do {
		printMenu();
		scanf("%d", &choice);

		switch (choice) {
		case 1: 	// dealing with the file :
			printf("loading data... \n");

			while (fgets(temp, 40, citiesFile2) != NULL) {

				char* token1 = strtok(temp, "\t");
				char* token2 = strtok(NULL, "\t");
				char* token3 = strtok(NULL, "\t");

				char* tempInfo[3];

				tempInfo[0] = strdup(token1);
				tempInfo[1] = strdup(token2);
				tempInfo[2] = strdup(token3);
				addEdge(citiesGraph, vertixOfCity(tempInfo[0], cities, numOfCities), vertixOfCity(tempInfo[1], cities, numOfCities), (double)strtod(tempInfo[2], NULL));

			}

			// graph is loaded and is ready to be used

			fclose(citiesFile);

			printf("data loadded\n");

			break;
		case 2:{
		int valid = 0;


			do{
				printf("source city : ");
				scanf("%s", src);
				for (int i = 0;i < numOfCities;i++)
					if (strcmp(cities[i], src) == 0) {
						valid = 1;
						break;
					}
				if (!valid)
					printf("invalid city\n");
		}while (!valid);
			printf("loaded\n");
			scanned++;
		}
			break;
		case 3: 
			printf("");
			int valid = 0;
			do {
				printf("destenation city : ");
				scanf("%s", dest);
				for (int i = 0;i < numOfCities;i++)
					if (strcmp(cities[i], dest) == 0) {
						valid = 1;
						break;
					}
				if (!valid)

					printf("invalid city\n");

			} while (!valid);


			scanned++;

			double totalWeight;
			int pathLength = 0;
			int path[30];
			int totalDistance = -1;
				printf("loaded\n");
				double totalWeightBFS = 0;
				double* totalWeightBFSPtr = &totalWeightBFS;

				pathLength = BFS(citiesGraph, vertixOfCity(src, cities, numOfCities), vertixOfCity(dest, cities, numOfCities), path, totalWeightBFSPtr);
				{
					printf("Shortest path from %s to %s using BFS: ",src,dest);
					for (int i = pathLength - 1; i >= 0; i--, totalDistance++)

						printf("%s ", cities[path[i]]);


					printf(" totla distance: %d %s ", totalDistance, (totalDistance == 1) ? "edge" : "edges");
					printf(" total weight :%0.1f  ", totalWeightBFS);

					printf("\n");
				}


				dijkstra(citiesGraph, vertixOfCity(src, cities, numOfCities), vertixOfCity(dest, cities, numOfCities), distance, parent);

				// if the distance from src to dest is enfinity then there is no path
				if (distance[vertixOfCity(dest, cities, numOfCities)] == INT_MAX)
					printf("there is no path from %d to %d\n", srcs, dest);

				// else print the path
				else {
					printf("Shortest path from %s to %s using dijkstra: ", cities[vertixOfCity(src, cities, numOfCities)], cities[vertixOfCity(dest, cities, numOfCities)]);
					totalWeight = printShortestPath(citiesGraph, parent, vertixOfCity(dest, cities, numOfCities), cities);
					printf(". total weight = %0.1f", totalWeight);
					printf("\n");
				}

				srcs[numOfInputs] = strdup(src);
				dests[numOfInputs] = strdup(dest);
				 totalWeightsBFS[numOfInputs] = totalWeightBFS;
				 totalWeights[numOfInputs] = totalWeight;
				 totalEdges[numOfInputs] = totalDistance;
				 pathes[numOfInputs] = path;
				 parents[numOfInputs] = parent ;
				 distances[numOfInputs] = distance;
				numOfInputs++;
			  break;
		case 4:

			if (scanned >= 2) {
				printf("%s", "thanks for using my program\n");
				FILE* shortest_distance;

				shortest_distance = fopen("shortest_distance.txt", "w");
				fprintf(shortest_distance, "");
				fclose(shortest_distance);

				shortest_distance = fopen("shortest_distance.txt", "a");

				for (int j = 0;j < numOfInputs;j++) {

					fprintf(shortest_distance, "Shortest path from %s to %s using BFS: ", srcs[j], dests[j]);
					for (int i = pathLength - 1; i >= 0; i--)

						fprintf(shortest_distance, "%s ", cities[pathes[j][i]]);


					fprintf(shortest_distance, " totla distance: %d %s ", totalEdges[j], (totalEdges[j] == 1) ? "edge" : "edges");
					fprintf(shortest_distance, " total weight :%0.1f  ", totalWeightsBFS[j]);
					fprintf(shortest_distance, "\n");


					if (distance[vertixOfCity(dests[j], cities, numOfCities)] == INT_MAX)
						fprintf(shortest_distance, "there is no path from %d to %d\n", srcs[j], dests[j]);


					else {
						fprintf(shortest_distance, "Shortest path from %s to %s using dijkstra: ", cities[vertixOfCity(srcs[j], cities, numOfCities)], cities[vertixOfCity(dests[j], cities, numOfCities)]);
						fprintShortestPath(citiesGraph, parents[j], vertixOfCity(dests[j], cities, numOfCities), cities, shortest_distance);
						fprintf(shortest_distance, ". total weight : %0.1f", totalWeights[j]);
						fprintf(shortest_distance, "\n\n");
					}
				}
				fclose(shortest_distance);
			}
			else
				printf("you have to enter cities\n");


			break;
		default: printf("invalid choice\n");
		}
		} while (choice != 4);

		destroyGraph(citiesGraph);
		free(parent);
		free(distance);
		free(cities);

	}