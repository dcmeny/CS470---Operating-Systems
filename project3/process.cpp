//Dalton Meny
//CS 470
//Project 3

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

void create(int n, int b);
void destroy(int n);
void tick(int n); //interrupt
void wait(int n);
void event(int n);

//stuct to hold all the info of a process
struct process {
  int pid;
  int parent;
  vector<int> children;
  int rbt; //remaining burst time
  int rqt; //remaining quantum time
  int event = 0;
};

//output file
ofstream out;

process zero; //PID 0
process current; //current process
int q; //time quantume
deque<process> rq; //ready queue
deque<process> wq; //wait queue

//Hold data for X command
process last_current;
deque<process> last_rq;
deque<process> last_wq;


int main(int argc, char *argv[]) {
  if (argc != 4) {
    cout << "Enter q input.txt output.txt" << endl;
    exit(0);
  }

  q = atoi(argv[1]);

  //Checking input file
  ifstream in(argv[2]);
  if (!in) {
    cout << "Error input opening file" << endl;
    exit(0);
  }

  //Opening output file
  out.open(argv[3]);
  if (!out) {
    cout << "Error output opening file" << endl;
    exit(0);
  }

  //initializing PID 0
  zero.pid = 0;
  zero.rbt = -1;
  zero.rqt = -1;
  current = zero;

  //Initial tick
  tick(0);

  string input;
  int n, b;
  string command;

  //Read in command while input or command X
  while(getline(in, input) || command != "X") {
    out << input << endl;
    command = input.substr(0,1);
    
    if (command == "C") {
      n = stoi(input.substr(2,input.find(" ")));
      b = stoi(input.substr(4));
      create(n, b);
    }

    if (command == "D") {
      n = stoi(input.substr(2));
      //tick(0);
      if(current.pid == n || current.parent == n)
	destroy(n);
      tick(0);
    }

    if (command == "I") {
      tick(0);
    }

    if (command == "W") {
      n = stoi(input.substr(2));
      wait(n);
    }

    if (command == "E") {
      n = stoi(input.substr(2));
      tick(n);
    }

    if(command == "X") {
      out << "Current state of simulation:" << endl;

      //Print the running process
      if (last_current.pid !=0) 
	out << "PID " << last_current.pid << " " << last_current.rbt
	    << " running with " << last_current.rqt << " left" << endl;
      else
	out << "PID 0 running" << endl; //if PID 0

      //Print Ready Queue
      out << "Ready Queue: " << flush;
      for (size_t i=0; i<last_rq.size(); i++) {
	out << "PID " << last_rq[i].pid << " " << last_rq[i].rbt << " "
	    << flush;
      }
      out << endl;

      //Pring Wait Queue
      out << "Wait Queue: " << flush;
      for (size_t i=0; i<last_wq.size(); i++) {
	out << "PID " << last_wq[i].pid << " " << last_wq[i].rbt << " "
	    << last_wq[i].event << " " << flush;
      }
      out << endl;
    }
  }

  //Closing files
  in.close();
  out.close();
  return 0;
}

void create (int n, int b) {
  process newp;
  newp.pid = n;
  newp.parent = current.pid;
  current.children.push_back(newp.pid);
  newp.rbt = b;
  newp.rqt = q;

  out << "PID " << n << " " << b << " placed on Ready Queue" << endl;

  if (current.pid != 0) 
    rq.push_back(newp);
  else 
    current = newp;
  
  tick(0);
}

void destroy(int n) {
  if (current.pid == n) {
    out << "PID " << n << " " << current.rbt << " terminated" << endl;
    if (!current.children.empty()) {

      //destroy all children
      for(size_t i=0; i<current.children.size(); i++)
	destroy(current.children[i]);
    }
    
    //Changing current which serves as a destroy
    if(!rq.empty()) {
      current = rq.front();
      rq.pop_front();
    }
    else
      current = zero;
  }
  else {
    for (size_t i=0; i<rq.size(); i++)
      if (rq[i].pid == n) {
	out << "PID " << n << " " << rq[i].rbt << " terminated" << endl;

	if(!rq[i].children.empty()) {
	  for(size_t j=0; j<rq[i].children.size(); j++)
	    destroy(rq[i].children[j]);
	}
	//if it becomes the current process after deleting a child
	if(current.pid == n)
	  current = zero;
	else
	  rq.erase(rq.begin() + i);
      }

    for (size_t i=0; i<wq.size(); i++)
      if (wq[i].pid == n) {
	out << "PID " << n << " " << wq[i].rbt << " terminated" << endl;
	
	if(!wq[i].children.empty()) {
	  for(size_t j=0; j<wq[i].children.size(); j++)
	    destroy(wq[i].children[j]);
	}
	
	wq.erase(wq.begin() + i);
      }	     
  }
}

void tick(int n) {
  
  if (current.rbt == 0 && current.pid !=0)
    destroy(current.pid); 


  //Check if the remaining q time is zero and change current process if ness
  if (current.rqt == 0 && current.pid !=0) {
    out << "PID " << current.pid << " " << current.rbt
	<< " placed on Ready Queue" << endl;
    current.rqt = q;
    rq.push_back(current);
    current = rq.front();
    rq.pop_front();
    }

  if(n !=0)
    event(n);

  //Saving info for X command
  last_current = current;
  last_rq = rq;
  last_wq = wq;

  //Print the running process
  if (current.pid !=0) 
    out << "PID " << current.pid << " " << current.rbt << " running with "
	<< current.rqt << " left" << endl;
  else
    out << "PID 0 running" << endl; //if PID 0

  //Decrement remaining burst and quantum times
  current.rbt--;
  current.rqt--;
  
  
  //Print Ready Queue
  out << "Ready Queue: " << flush;
  for (size_t i=0; i<rq.size(); i++) {
    out << "PID " << rq[i].pid << " " << rq[i].rbt << " " << flush;
  }
  out << endl;

  //Pring Wait Queue
  out << "Wait Queue: " << flush;
  for (size_t i=0; i<wq.size(); i++) {
    out << "PID " << wq[i].pid << " " << wq[i].rbt << " "
	<< wq[i].event << " " << flush;
  }
  out << endl;
}

void wait(int n){
  out << "PID " << current.pid << " " << current.rbt << " placed on Wait Queue"
      << endl;
  current.event = n;
  current.rqt = q;
  wq.push_back(current);
  if(!rq.empty()) {
  current = rq.front();
  rq.pop_front();
  }
  else
    current = zero;
  tick(0);
}

void event(int n) {
  for (size_t i=0; i<wq.size(); i++) {
    if (wq[i].event == n) {
      rq.push_back(wq[i]);
      out << "PID " << wq[i].pid << " " << wq[i].rbt
	  << " placed on Ready Queue" << endl;
      wq.erase(wq.begin() + i);
    }
  }
}
