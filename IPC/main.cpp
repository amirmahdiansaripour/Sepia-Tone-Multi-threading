#include "def.h"


int main(int argc, char *argv[]){
    mkfifo(DECREASE_FIFO, 0777);
    string test_address = argv[1];
    //cout << test_address << '\n';
    int num_of_users = count_files(COUNT_INST);
    vector<string> addresses = assign_address(test_address, num_of_users);
    string got_from_reduce = send_to_childs(addresses, num_of_users);
    map<string, int>HM;
    seperate_data(got_from_reduce, HM);
    send_to_csv_file(HM);
    return 0;
}