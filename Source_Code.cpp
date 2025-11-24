/*
Use Comments 
*/
# include<iostream>
using namespace std;

class Node {	// Double Linked List
public:
	int Priority;
	string Task;
	Node* Pervious;
	Node* Next;

	Node(string Task_user, int Priority_user) {
		Priority = Priority_user;
		Task = Task_user;
		Pervious = Next = NULL;
	}
};

class Linked_List {
public:
	Node* Head;
	Node* Tail;

	Linked_List() {
		Head = Tail = NULL;
	}

	bool Is_Empty() {
		return Head == NULL;
	}

	void Insert_At_Begining(string Task_user, int priority_user) {
		Node* new_Node = new Node(Task_user, priority_user);
		if (Is_Empty()) {
			Head = new_Node;
		}
		else {
			new_Node->Next = Head;
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

			while (Temp->Next != NULL) {	// To move To the Next Node

				if (New_Node->Priority >= 0) {

					if (Temp->Priority <= New_Node->Priority) {	//PT<=PN -> Move To Next Node
						Temp = Temp->Next;
					}
					else if (Temp->Priority > New_Node->Priority) {//PT>PN -> Insert At Begining


					}
					else if (New_Node->Priority == 0) {	// PN == 0 -> Insert At Begining 
						Insert_At_Begining(New_Node->Task, New_Node->Priority);
					}
				}

			}
		}
	}
};
int main() {

	return 0;
}