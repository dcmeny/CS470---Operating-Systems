//Dalton Meny
//CS 470
//Project 4

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

struct mem {
  int pid;
  int page;
};

bool mem_compare(mem m1, mem m2);
//function to compare 2 pids and pages
int fifo(int frames, vector<mem> input);
int lru(int frames, vector<mem> input);
int opt(int frames, vector<mem> input);


int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Error: enter input.dat num_frames" << endl;
    exit(0);
  }

  int frames = atoi(argv[2]);

  //Checking input file
  ifstream in(argv[1]);
  if (!in) {
    cout << "Error input opening file" << endl;
    exit(0);
  }

  string input;
  vector<mem> inputv;

  //Reading in input into vector
  while(getline(in,input)) {
    mem temp;
    temp.pid = stoi(input.substr(0,input.find(" ")));
    temp.page = stoi(input.substr(input.find(" ")));
    inputv.push_back(temp);
  }

  //Calculating all 3 values and printing
  int result1 = fifo(frames, inputv);
  cout << "FIFO: " << result1 << endl;

  int result2 = lru(frames, inputv);
  cout << "LRU: " << result2 << endl;

  int result3 = opt(frames, inputv);
  cout << "OPT: " << result3 << endl;
  
  in.close();
  return 0;
}

bool mem_compare(mem m1, mem m2) {
  if(m1.pid == m2.pid && m1.page == m2.page)
    return true;
  else
    return false;
}

int fifo(int frames, vector<mem> input) {
  deque<mem> table;
  int pf = 1; //the first is always a page fault
  bool found;
  table.push_back(input[0]);

  for (size_t i=1; i< input.size(); i++) {
    found = false;
    for(size_t j=0; j<table.size() && !found; j++) {
      if (mem_compare(input[i], table[j])){
	found = true;
      }
    }
    if (!found) {
      pf++;
      if(table.size() == (size_t)frames) 
	table.pop_front();
      table.push_back(input[i]);      
      }
    
  }
  
  return pf;
}

int lru(int frames, vector<mem> input) {
  deque<mem> table;
  int pf = 1;
  bool found;
  table.push_back(input[0]);

  for (size_t i=1; i< input.size(); i++) {
    found = false;
    for(size_t j=0; j<table.size() && !found; j++) {
      if (mem_compare(input[i], table[j])){
	found = true;
	table.push_back(table[j]);
	table.erase(table.begin() + j); //prevents duplicates
      }
    }
    if (!found) {
      pf++;
      if(table.size() == (size_t)frames) 
	table.pop_front();
      table.push_back(input[i]);      
    }
    
  }
  
  return pf;
}

int opt(int frames, vector<mem> input) {
  deque<mem> table;
  int pf = 1;
  bool found;
  table.push_back(input[0]);

  for (size_t i=1; i< input.size(); i++) {
    found = false;
    for(size_t j=0; j<table.size() && !found; j++) {
      if (mem_compare(input[i], table[j])){
	found = true;
	  
      }
    }
    if (!found) {
      pf++;
      
      if(table.size() == (size_t)frames) {
	deque<mem> new_table;
	for (size_t k=i; k < input.size()
	       && new_table.size() < (size_t)(frames-1); k++)
	  for(size_t j=0; j<table.size(); j++) 
	    if (mem_compare(input[k], table[j])){
	      new_table.push_back(table[j]);
	      table.erase(table.begin() + j);
	    }
	table = new_table;
      }
      table.push_back(input[i]);
    }
    
  }
  
  return pf;
}
