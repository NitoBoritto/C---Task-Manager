/*
Use Comments 
*/
# include <iostream>
# include <ctime>
using namespace std;

class Node {	// Double Linked List
public:
	int Priority;
	string Task;
	Node* Previous;
	Node* Next;
    string Status;

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

	void Insert_At_Begining(string Task_user, int priority_user) { // Standard Insert @ Ther Beginning
		Node* new_Node = new Node(Task_user, priority_user);
		if (Is_Empty()) {
			Head = Tail = new_Node;
		}
		else {
			new_Node->Next = Head;
			Head->Previous = new_Node;
			Head = new_Node;
		}
	}

	void Insert_Single_Task(string task, int priority_user) {
		Node* New_Node = new Node(task, priority_user);
		if (Is_Empty()) {
			Head = Tail = New_Node;
			return;
		}
		else {
			Node* Temp = Head;

			if (priority_user < Temp->Priority) {
				Insert_At_Begining(task, priority_user);
				return;
			}

			while (Temp->Next != NULL && Temp->Next->Priority <= priority_user) {
				Temp = Temp->Next;
			}

			if (Temp->Next == NULL) {
				Temp->Next = New_Node;
				New_Node->Previous = Temp;
				Tail = New_Node;
				return;
			}
			New_Node->Next = Temp->Next;
			New_Node->Previous = Temp;
			Temp->Next->Previous = New_Node;
			Temp->Next = New_Node;
		}
	}

	void Display() {
		if (Is_Empty()) {
			cout << "Your Linked List Is Empty !!\n";
			return;
		}
		else {
			Node* temp = Head;
			cout << "{";
			while (temp != NULL) {
				if (temp->Next != NULL) {
					cout << temp->Task << ",";
				}
				else {
					cout << temp->Task << "}";
				}
				temp = temp->Next;
			}
            cout << endl;
		}
	} 

    void Run_Task() {
        if(!Head) return;

        Head->Status = "Running";

        if((rand() % 100) < 20) {
            cout << "\nTask " << Head->Task << " Failed! ERROR 404 😢" << endl;
            Head->Status = "Failed;";
        } else {
            cout << "\nTask " << Head->Task << " Completed Succesfully 👏" << endl;
            Head->Status = "Completed";
        }
    }

};
int main() {
	srand(time(0));
	Linked_List L;
	L.Insert_Single_Task("Task(A)", 1);
	L.Insert_Single_Task("Task(B)", 2);
	L.Insert_Single_Task("Task(C)", 1);
	L.Display();
    L.Run_Task();
	cout << endl;
	return 0;
}



