#include <iostream>
#include <fstream>

class dijktra{
	public:
	int numNodes;
	int sourceNode;
	int minNode;
	int currentNode;
	int newCost;
	
	int** costMatrix;
	int* fatherArray;
	int* toDoArray;
	int* bestArray;
	
	dijktra(int arraySize){
		this->numNodes = arraySize;
		this->sourceNode = 0;
		this->minNode = 0;
		this->currentNode = 0;
		this->newCost = 0;
		
		this->costMatrix = new int*[this->numNodes + 1];
		for(int i=1;i<this->numNodes+1;i++){
			this->costMatrix[i] = new int[this->numNodes + 1];
		}
		for(int i=1;i<this->numNodes+1;i++){
			for(int j=1;j<this->numNodes+1;j++){
				if(i == j){
					this->costMatrix[i][j] = 0;
				}
				else{
					this->costMatrix[i][j] = 9999;
				}
			}
		}
		
		this->fatherArray = new int[this->numNodes + 1];
		
		this->toDoArray = new int[this->numNodes + 1];
		for(int i=1;i<this->numNodes+1;i++){
			this->toDoArray[i] = 1;
		}
		
		this->bestArray = new int[this->numNodes + 1];
		for(int i=1;i<this->numNodes+1;i++){
			this->bestArray[i] = 9999;
		}
	}
	
	void loadCostMatrix(std::ifstream& input){
		int val1;
		int val2;
		int cost;
		//clear first value
		while(input >> val1){
			input >> val2;
			input >> cost;
			this->costMatrix[val1][val2] = cost;
		}
	}
	void setBestArray(int sourceNode){
		for(int i=1;i<this->numNodes+1;i++){
			this->bestArray[i] = this->costMatrix[sourceNode][i];
		}
	}
	void setFatherArray(int sourceNode){
		for(int i=1;i<this->numNodes+1;i++){
			this->fatherArray[i] = sourceNode;
		}
	}
	void setToDoArray(int sourceNode){
		for(int i=1;i<this->numNodes+1;i++){
			this->toDoArray[i] = 1;
		}
		this->toDoArray[sourceNode] = 0;
	}
	int findMinNode(){
		int minCost = 9999;
		this->minNode = 0;
		int index = 1;
		for(index;index<=numNodes;index++){
			if(this->toDoArray[index] == 1 && this->bestArray[index]<minCost){
				minCost = this->bestArray[index];
				this->minNode = index;
			}
		}
		return minNode;
	}
	int computeCost(int minNode, int Node){
		int bestCost = 0;
		bestCost = this->bestArray[minNode] + this->costMatrix[minNode][Node];
		return bestCost;
	}
	bool checkToDoArray(){
		for(int i=1;i<=this->numNodes;i++){
			if(this->toDoArray[i] == 1){
				return false;
			}
		}
		return true;
	}
	void debugPrint(std::ofstream& output){
		output<<"Source Node:"<<this->sourceNode<<std::endl;
		
		output<<"Father Array\n";
		for(int i=1;i<=this->numNodes;i++){
			output<<this->fatherArray[i]<<" ,";
		}
		output<<std::endl;
		
		output<<"To Do Array\n";
		for(int i=1;i<=this->numNodes;i++){
			output<<this->toDoArray[i]<<" ,";
		}
		output<<std::endl;
		
		output<<"Best Array\n";
		for(int i=1;i<=this->numNodes;i++){
			output<<this->bestArray[i]<<" ,";
		}
		output<<std::endl;
	}
	void printShortestPath(int current, int source, std::ofstream& output){
		int cost = this->bestArray[current];
		output<<"Path from "<<source<<" to "<<current<<": ";
		while(current != this->fatherArray[current]){
			output<<current<<"<-";
			current = this->fatherArray[current];
		}
		output<<current<<"<-"<<source<<": cost = "<<cost<<std::endl;
	}
};

int main(int argc, char* argv[]){
	if(argc != 4){
		std::cout<<"You must have 3 inputs! inFile, SSSFile, DebugFile\n";
		return -1;
	}
	//step 0
	std::ifstream input(argv[1]);
	if(!input.good()){
		std::cout<<"Your input file is invalid!\n";
		return -1;
	}
	std::ofstream output(argv[2]);
	std::ofstream debug(argv[3]);
	
	int nodeCount;
	input >> nodeCount;
	dijktra* path = new dijktra(nodeCount);
	//step 1
	path->loadCostMatrix(input);
	
	path->sourceNode = 1;
	//step 14
	while(path->sourceNode <= path->numNodes){
		//step 2
		path->setBestArray(path->sourceNode);
		path->setFatherArray(path->sourceNode);
		path->setToDoArray(path->sourceNode);
		//step 8
		while(!path->checkToDoArray()){
			//step 3
			path->minNode = path->findMinNode();
			path->toDoArray[path->minNode] = 0;
			path->debugPrint(debug);
			//step 4
			int childNode = 1;
			//step 7 loop
			while(childNode <= path->numNodes){
				//step 5
				if(path->toDoArray[childNode] == 1){
					path->newCost = path->computeCost(path->minNode, childNode);
					if(path->newCost < path->bestArray[childNode]){
						path->bestArray[childNode] = path->newCost;
						path->fatherArray[childNode] = path->minNode;
						path->debugPrint(debug);
					}
				}
				//step 6
				childNode++;
			}
		}
		//step 9
		path->currentNode = 1;
		//step 12
		while(path->currentNode <= path->numNodes){
			//step 10
			path->printShortestPath(path->currentNode, path->sourceNode, output);
			//step 11
			path->currentNode++;
		}
		//step 13
		path->sourceNode++;
	}
	//step 15
	input.close();
	output.close();
	debug.close();
	return 0;
}