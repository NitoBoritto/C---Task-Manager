/*
Use Comments
*/
# include <iostream>
# include <ctime>
# include <thread>
# include <chrono>
# include <fstream>
# include <limits>
# include <string>
# include <sstream>
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
				if (Temp->Next != NULL) cout << "{" << Temp->Task << " | "  // {Task(A) | Pending | 0} <-- {Task(B) | Pending | 2}
					<< Temp->Status << " | "
					<< Temp->Priority << "}" << " <-- ";
				else cout << "{" << Temp->Task << " | "
					<< Temp->Status << " | "
					<< Temp->Priority << "}" << ".";
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

		if (Write_Header) file << "Task,Priority,Status" << endl;

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
		if (Is_Empty()) {
			cout << "Task Manager is empty.\nPlease add tasks before running tasks manager!" << endl;
			return;
		};

		Head->Status = "Running";
		cout << "\n" << Head->Status << "...." << endl;
		this_thread::sleep_for(3s); // 3 second wait-time for task execution

		if ((rand() % 100) < 20) { // Initalizing randomized values for 20% task failure
			cout << "Task " << Head->Task << " Failed! ERROR 404 😢" << endl;
			Head->Status = "Failed";
		}
		else {
			cout << "Task " << Head->Task << " Completed Succesfully! 👏" << endl;
			Head->Status = "Completed";
		}

		Log_Task(Head);
		cout << "Saved to log! ✅" << endl;

		Remove_Task();
		cout << "Task removed from queue! ✅" << endl;
	}

	// GUI Starts from here



};

// Append loaded task preserving file order
void Append_Loaded_Task(Linked_List& L, const string& task, int priority, const string& status){
	Node* n = new Node(task, priority);
	n->Status = status;
	if(L.Is_Empty()) { 
		L.Head = L.Tail = n; 
	} else { 
		L.Tail->Next = n; n->Previous = L.Tail; L.Tail = n; 
	}
}

// Persistent state I/O (use same absolute path as other program)
static const string State_Path = "C:\\Users\\medo7\\OneDrive - Egyptian Russian University\\Planning For Future DA\\Projects\\C++ Task Manager\\TasksState.csv";

void SaveState(Linked_List& L){
	ofstream f(State_Path, ios::trunc);
	if(!f.is_open()) return;
	f << "Task,Priority,Status\n";
	Node* t = L.Head;
	while(t){
		f << t->Task << "," << t->Priority << "," << t->Status << "\n";
		t = t->Next;
	}
	f.close();
}

static void LoadState(Linked_List& L){
	ifstream f(State_Path);
	if(!f.is_open()) return; // no state yet
	string line;
	// optional header
	if(getline(f, line)){
		if(line.rfind("Task,Priority,Status", 0) != 0){
			stringstream ss(line);
			string task, prio, status;
			if(getline(ss, task, ',') && getline(ss, prio, ',') && getline(ss, status)){
				int p = 0; try{ p = stoi(prio);}catch(...){ p = 0; }
				Append_Loaded_Task(L, task, p, status);
			}
		}
	}
	while(getline(f, line)){
		stringstream ss(line);
		string task, prio, status;
		if(getline(ss, task, ',') && getline(ss, prio, ',') && getline(ss, status)){
			int p = 0; try{ p = stoi(prio);}catch(...){ p = 0; }
			Append_Loaded_Task(L, task, p, status);
		}
	}
	f.close();
}

int main() {
	srand(time(0)); // Setting up randomized values for failure simulation
	Linked_List L;

	// Load existing persisted tasks so terminal-only program sees current queue
	LoadState(L);

	// Main integreation starts here
	int choice;
	string task;
	int priority;

	cout << "+-------------------------------------------+\n";
	cout << "|       Welcome to Task Manager Pro!        |\n";
	cout << "| Simple system to track & run your tasks.  |\n";
	cout << "+-------------------------------------------+\n\n";

	while (true) {
		cout << "\n============== MAIN MENU ==============\n";
		cout << "1. Add New Task\n";
		cout << "2. Display Task Queue\n";
		cout << "3. Run Next Task\n";
		cout << "4. Open file log\n";
		cout << "5. Exit Program\n";
		cout << "=======================================\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if (!cin.good()) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input! Enter numbers only.\n";
			continue;
		}

		switch (choice) {

		case 1:
			cout << "\nEnter task name (one sentence): ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, task);

			cout << "Enter priority (0 = highest -> 5 = lowest): ";
			cin >> priority;

			if (priority < 0 || priority > 5) {
				cout << "Invalid priority!\n";
				break;
			}

			L.Insert_Single_Task(task, priority);
			cout << "Task added successfully! \n";
			SaveState(L); // persist after adding
			break;

		case 2:
			cout << "\nCurrent Task Queue:\n";
			L.Display();
			break;

		case 3:
			cout << "\nExecuting next task...\n";
			L.Run_Task();
			SaveState(L); // persist after running/removing
			break;

		case 4:
			system("start \"\" \"C:\\Users\\medo7\\OneDrive - Egyptian Russian University\\Planning For Future DA\\Projects\\C++ Task Manager\\Log.csv\"");
			break;

		case 5:
			cout << "\nGoodbye! \n";
			SaveState(L); // final persistence
			return 0;

		default:
			cout << "Invalid choice! Please select 1 - 4.\n";
		}
	}

	return 0;
}