/*
Use Comments 
*/
# include <iostream>
# include <ctime>
# include <thread>
# include <chrono>
# include <fstream>
using namespace std;

class Node {	// Double Linked List
public:
	int Priority;
	string Task;
	Node* Previous;
	Node* Next;
    string Status; // Pending, Running, Completed, Failed

	Node(string Task_user, int Priority_user) {
		Priority = Priority_user;
		Task = Task_user;
		Previous = Next = NULL;
        Status = "Pending";
	}
};

class Linked_List { // Standard Linked List Initialization
public:
	Node* Head;
	Node* Tail;

	Linked_List() {
		Head = Tail = NULL;
	}

	bool Is_Empty() {
		return Head == NULL;
	}

	void Insert_At_Begining(string Task_user, int Priority_user) { // Standard Insert @ The Beginning
		Node* new_Node = new Node(Task_user, Priority_user);
		if (Is_Empty()) {
			Head = Tail = new_Node;
		}
		else {
			new_Node->Next = Head;
			Head->Previous = new_Node; // Due to being a doubly linked list. Next connects to the node after, Previous connects to the node before
			Head = new_Node;
		}
	}

	void Insert_Single_Task(string Task, int Priority_user) { // Inserting a task node & automatically sorting depending on the priority number from 0 - 5
		Node* New_Node = new Node(Task, Priority_user);
		if (Is_Empty()) {
			Head = Tail = New_Node;
			return;
		}
		else {
			Node* Temp = Head;

			if (Priority_user < Temp->Priority) { // Inserting task node at the beginning if it has the highest priority
				Insert_At_Begining(Task, Priority_user);
				return;
			}

			while (Temp->Next != NULL && Temp->Next->Priority <= Priority_user) { // Traversing as long as long as the task priority is less than the next node
				Temp = Temp->Next;
			}

			if (Temp->Next == NULL) { //  While traversing if Temp reached the end then add a task node at the end
				Temp->Next = New_Node;
				New_Node->Previous = Temp;
				Tail = New_Node;
				return;
			}
			New_Node->Next = Temp->Next; // If neither then add in the middle depending on its priority
			New_Node->Previous = Temp;
			Temp->Next->Previous = New_Node;
			Temp->Next = New_Node;
		}
	}

	void Display() { // Display the content of the task tracker
		if (Is_Empty()) {
			cout << "Your Linked List Is Empty !!\n";
			return;
		}
		else {
			Node* Temp = Head;
			while (Temp != NULL) {
				if (Temp->Next != NULL) cout << "{" << Temp->Task << ", "  // {Task(A), Pending, 0} <-- {Task(B), Pending, 2}
											<< Temp->Status << ", " 
											<< Temp->Priority << "} " << "<-- ";
				else cout << Temp->Task << Temp->Status << ", " << Temp->Priority << "} " << ".";
				Temp = Temp->Next;
			}
            cout << endl;
		}
	} 

    void Log_Task(Node* Task) { // Logging executed tasks
		const string Path = "C:\\Users\\medo7\\OneDrive - Egyptian Russian University\\Planning For Future DA\\Projects\\C++ Task Manager\\Log.csv";

		ifstream check(Path);
		bool Write_Header = !check.good() || check.peek() == EOF;
		check.close();

		ofstream file(Path, ios::app);

		if(Write_Header) file << "Task,Priority,Status" << endl;

        file << Task->Task << "," 
				<< Task->Priority << "," 
				<< Task->Status << endl;

        file.close();
    }

    void Remove_Task() { // Deleting tasks after executing & logging them
        Node* Temp = Head;
        Head = Head->Next;
        delete Temp;
    }

    void Run_Task() { // Executing tasks
        if(Is_Empty()) {
            cout << "Task Manager is empty.\nPlease add tasks before running tasks manager!" << endl;
            return;
        };

        Head->Status = "Running";
        cout << "\n" << Head->Status << "...." << endl;
        this_thread::sleep_for(3s); // 3 second wait-time for task execution

        if((rand() % 100) < 20) { // Initalizing randomized values for 20% task failure
            cout << "Task " << Head->Task << " Failed! ERROR 404 😢" << endl;
            Head->Status = "Failed";
        } else {
            cout << "Task " << Head->Task << " Completed Succesfully! 👏" << endl;
            Head->Status = "Completed";
        }

        Log_Task(Head);

        Remove_Task();
    }

	// GUI Starts from here

};

int main() {
	srand(time(0)); // Setting up randomized values for failure simulation
	Linked_List L;


	// GUI Starts from here either

	
	return 0;
}



