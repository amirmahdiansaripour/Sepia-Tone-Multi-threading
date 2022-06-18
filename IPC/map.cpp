#include "def.h"

int main(int argc, char* argv[]){
    int read_side = stoi(argv[1]);
    char address[ADDRESS_LEN];
    read(read_side, address, ADDRESS_LEN);
    //cout << "adress : " << address << '\n';
    string read = read_data(address);
    cout << "MAP from MAIN (pipe): " << read << "\n\n\n";
    vector<string> words = seperate_by_comma(read);
    map <string , int> HM;
    find_frequency(HM, words);
    string freq = pack_result(HM);
    send_to_reduce_process(freq);
    return 0;
}