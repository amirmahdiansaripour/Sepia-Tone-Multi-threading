#ifndef _DEF_H_
#define _DEF_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FAILURE 0
#define WHICH_CSV_FILE 10
#define READ 0
#define COUNT_INST "ls ./testcases | wc -l"
#define WRITE 1
#define ADDRESS_LEN 400
#define DECREASE_FIFO "decrease_fifo"
#define PIPE_ERROR "ERROR : couldn't open pipe\n"
#define FORK_ERROR "ERROR : could'nt run fork\n"
#define BACK_SLASH '\\'
#define FOR_SLASH '/'
#define COMMA ','
#define READ_MODE "r"
#define OUTPUT_FILE "output.csv"
#define FIRST_FILE_OFFSET "/1.csv"
#define MAIN_FROM_REDUCE "MAIN form REDUCE (pipe): "

using namespace std;

string pack_result(const map<string, int>& AB);

void seperate_data(const string& packed, map<string, int>& HM);

void error(const char *msg);

string send_to_childs(const vector<string>& addresses, const int& num_of_users);

vector<string> assign_address(const string& addr, const int& num_of_users);

void send_to_csv_file(const map<string , int>& HM);

int count_files(string cmd);

void find_frequency(map<string, int>& HM, const vector<string>& words);

string read_data(const string& detailed_addr);

void send_to_reduce_process(const string& freq);

vector<string> seperate_by_comma(const string& sentence);
#endif