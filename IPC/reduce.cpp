
#include "def.h"

int main(int argc, char* argv[]){
    //cout << "hello\n";
    vector<string> unorganized;
    int counter = 0;
    char got_from_map[ADDRESS_LEN];
    //cout << "argv[1] " << argv[1] << '\n';
    int num_of_users = stoi(argv[1]);
    while(counter < num_of_users){
        int fd = open(DECREASE_FIFO ,O_RDONLY);
        //cout << "while loop\n";
        read(fd, got_from_map, ADDRESS_LEN);
        unorganized.push_back(got_from_map);
        cout << "REDUCE from MAP (fifo): "<< got_from_map << "\n\n\n";
        close(fd);
        counter++;  
        //cout << "counter : " << counter << '\n';  
    }

    map<string, int>HM;

    for(int i = 0; i < unorganized.size(); i++)
        seperate_data(unorganized[i], HM);
    
    string r = pack_result(HM);
    //cout << "r : " << r.c_str() << '\n';
    int write_side = stoi(argv[2]);
    int read_side = stoi(argv[3]);
    close(read_side);
    write(write_side, r.c_str(), ADDRESS_LEN);
    close(write_side);
    return 0;
}
