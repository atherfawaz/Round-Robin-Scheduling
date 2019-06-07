//Scheduler with Singly Cirular Linked List

#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <string>
using namespace std;

template<class T>
class List;

template<class T>
class Node {
	T data;
	Node<T> * next;
public:
	Node() { next = nullptr; }
	Node(T d, Node* n = nullptr) : data(d), next(n) {}
	friend class List<T>;
};

template <class T>
class List {
private:
	Node<T> tail; // dummy tail
	class listiterator {
		Node<T> * curr;
	public:
		friend class List;
		listiterator(Node<T>* n = nullptr) {
			curr = n;
		}
		listiterator& operator++() {		//pre inc
			if (curr != nullptr)
				curr = curr->next;
			return *this;
		}
		listiterator operator++(int) {		//post inc
			listiterator temp = *this;
			curr = curr->next;
			return temp;
		}
		bool operator!=(const listiterator &it) const {
			return (curr != it.curr);
		}
		T& operator*() {
			return curr->data;
		}
	};
public:
	typedef listiterator literator;
	List() {
		tail.next = nullptr;
	}
	void insert(T d) {
		if (tail.next == nullptr) {
			Node<T> * temp = new Node<T>(d);
			tail.next = temp;
			temp->next = temp;
		}
		else {
			Node<T> * temp = new Node<T>(d);
			Node<T> * temp2 = tail.next->next;		//save tail
			tail.next->next = temp;
			temp->next = temp2;
			tail.next = tail.next->next;
		}
	}
	listiterator remove(listiterator& it) {		//remove iterator element
		if (it.curr->next == it.curr->next->next) {
			Node<T> *toDelete = it.curr;
			it.curr = nullptr;
			tail.next = nullptr;
			delete toDelete;
			return it;
		}
		else {
			Node<T> *temp = it.curr->next;
			while (temp->next != it.curr) {
				temp = temp->next;
			}
			Node<T> *toDelete = it.curr;
			it.curr = it.curr->next;		//move iterator forward
			temp->next = it.curr;
			delete toDelete;
			return it;
		}
	}
	bool isEmpty() {
		return (tail == nullptr);
	}
	listiterator begin() {
		return listiterator(tail.next->next);		//from first element
	}
	listiterator end() {
		return listiterator(nullptr);
	}
};
bool checkInsertion(bool& nomoretasks, const int tasks, bool*& arr) {
	for (int y = 0; y < tasks; y++) {
		if (arr[y] == false) {
			return nomoretasks;
		}
	}
	return !nomoretasks;
}
bool tasksleft(int* &duration, int tasks) {
	for (int i = 0; i < tasks; i++) {
		if (duration[i] > 0)
			return true;
	}
	return false;
}
void fileReading(int& tasks, int*& ID, int*& Duration, int*& Arrival_time) {
	ifstream fin("file.txt");
	int i = 0;
	fin >> tasks;
	ID = new int[tasks];
	Duration = new int[tasks];
	Arrival_time = new int[tasks];
	string line;
	getline(fin, line);
	getline(fin, line);
	if (fin.is_open()) {
		while (!fin.eof()) {
			fin >> ID[i];
			fin >> Duration[i];
			fin >> Arrival_time[i];
			i++;
		}
	}
	else {
		cerr << "File does not exist. Check directory!";
	}
	cout << "------Given Processes------" << endl;
	cout << "Process  " << " Duration Time  " << "  Running Time  " << endl;
	for (int i = 0; i < tasks; i++) {
		cout << "P" << ID[i] << " " << Duration[i] << " " << Arrival_time[i] << endl;
	}
	cout << "SCHEDULER";
}
void calculations(int*& completion, int*& turn, int*& wait, const int tasks, int*& duration, int*& arrivaltime, double& mean) {
	cout << endl << "Completion Time: " << endl;
	for (int i = 0; i < tasks; i++) {
		cout << "P" << i + 1 << " " << completion[i] << endl;
	}
	cout<< "Turnaround Time " << endl;
	for (int i = 0; i < tasks; i++) {
		turn[i] = completion[i] - arrivaltime[i];
		cout << "P" << i + 1 << " " << turn[i] << endl;
	}
	cout << "Waiting Time " << endl;
	for (int i = 0; i < tasks; i++) {
		wait[i] = turn[i] - duration[i];
		cout << "P" << i + 1 << " " << turn[i] << endl;
	}
	cout << "Mean Time " << endl;
	int result = 0;
	for (int i = 0; i < tasks; i++) {
		result += wait[i];
	}
	mean = result / tasks;
	cout << "Mean = " << mean << endl;
}

int main() {
	int tasks = 0;
	List <int> Scheduler;
	int* ID, *duration, *arrivaltime;
	ID = duration = arrivaltime = nullptr;
	fileReading(tasks, ID, duration, arrivaltime);		//sets up the data
	bool* inserted = new bool[tasks];
	int* completion_time = new int[tasks];
	for (int i = 0; i < tasks; i++)
		inserted[i] = false;
	List<int>::literator iterator;
	cout << endl;
	int current = 0;
	for (int i = 0; i < tasks; i++) {		//insert first processes
		if (arrivaltime[i] == 0) {
			Scheduler.insert(ID[i]);
			inserted[i] = true;
		}
	}
	iterator = Scheduler.begin();		//initialize scheduling after process insertion
	while (tasksleft(duration, tasks)) {
		cout << "P" << *iterator << " ";
		duration[*iterator - 1]--;
		if (duration[*iterator - 1] == 0) {		//check if any process has reached its completion time
			completion_time[*iterator - 1] = current;
			Scheduler.remove(iterator);
		}
		for (int i = 0; i < tasks; i++) {		//insert for later processes
			if ((arrivaltime[i] <= current) && (!inserted[i])) {
				Scheduler.insert(ID[i]);
				inserted[i] = true;
			}
		}
		++iterator;		//element pointed by iterator
		current++;		//cycles used
		Sleep(100);		//pause for human comprehension
	}
	int *turnaround_time = new int[tasks];
	int *waiting_time = new int[tasks];
	double mean_time;
	cout << endl << "No tasks left to be scheduled.";
	calculations(completion_time, turnaround_time, waiting_time, tasks, duration, arrivaltime, mean_time);
	_getch();
}
