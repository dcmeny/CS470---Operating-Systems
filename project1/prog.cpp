//Dalton Meny
//CS 470
//Project 1

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <list>
#include <stdlib.h>

using namespace std;


int main() {
  string input;
  bool should_run = true;

  //vector of input vectors
  vector<vector<string> > history;

  while (should_run) {
    cout << "osh>";
    fflush(stdout);

    bool good_input = true;

    //Reading input
    vector<string> args;
    string arg;
    getline(cin, input);
    istringstream iss(input);
    while(iss >> arg){
      args.push_back(arg);
    }

    //Putting command in history
    history.push_back(args);

    //Changing args if a ! command is used
    if(args[0].at(0) == '!') {
      history.pop_back();
      if (args[0].at(1) == '!') {
	if (history.empty()) {
	  cout << "No commands in history" << endl;
	  good_input = false;
	}
	else
	  args = history.back();
      }
      else {
	int num = atoi((args[0].substr(1)).c_str());
	if ((size_t)num <= history.size() && num != 0){
	  args = history[num-1];
	}
	else {
	  cout << "No such command in history" << endl;
	  good_input = false;
	}
      }
    }

    if (args[0] == "exit")
      should_run = false;

    //Printing last 10 commands in history
    else if(args[0] == "history"){
      history.pop_back();
      if (history.empty())
	cout << "No commands in history" << endl;
      else {
	int stop;
	if (history.size() < 10)
	  stop = 0;
	else
	  stop = history.size()-10;
	  
	for (int i=history.size()-1; i>=stop; i--) {
	  cout << i+1;
	  for(size_t j=0; j<history[i].size();j++)
	    cout << " " << history[i][j];
	  cout << endl;
	}
      }
    }

    else if (good_input){

      pid_t pid;
      pid = fork();
    
      if (pid == 0) {
	if ((args.back() == "&")){
	  pid_t pid2;
	  pid2 = fork();
	  if (pid2 != 0)
	    exit(0);
	  else {
	    char *args2[args.size()];
	    for (size_t i=0; i<args.size()-1; i++)
	      args2[i] = const_cast<char*>(args[i].c_str());
	    args2[args.size()] = NULL;
	    execvp(args2[0], args2);
	  }
	}
	else{
	  char *args2[args.size()+1];
	  for (size_t i=0; i<args.size(); i++)
	    args2[i] = const_cast<char*>(args[i].c_str());
	  args2[args.size()] = NULL;
	  execvp(args2[0], args2);
	}

      }

      else if (pid > 0)
	wait(NULL);
    }
  }

  return 0;

}
