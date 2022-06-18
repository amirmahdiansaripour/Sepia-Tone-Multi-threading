#include "def.h"

string pack_result(const map<string, int>& AB){
    string result;
    for(auto i : AB){
        string curr;
        curr += i.first;
        curr.push_back(COMMA);
        curr += to_string(i.second);
        curr.push_back(FOR_SLASH);
        result += curr;
    }
    result.push_back(BACK_SLASH);
    //cout << "MAP : " << result << '\n';
    return result;
}

void seperate_data(const string& packed, map<string, int>& HM){
    int counter = 0;
    string word, frequency;
    bool word_started = true, freq_started = false;
    while(true){
        if(packed[counter] == FOR_SLASH){
            HM[word] += stoi(frequency);
            word.clear();
            frequency.clear();
            word_started = true;
            freq_started = false;
            counter++;
            continue;
        }
        else if(packed[counter] == COMMA){
            word_started = false;
            freq_started = true;
            counter++;
            continue;
        }
        else if(packed[counter] == BACK_SLASH) break;
        
        if(word_started)
            word.push_back(packed[counter]);
        else if(freq_started)
            frequency.push_back(packed[counter]);

        counter++;
    }
}

void error(const char *msg){
    perror(msg);
    exit(FAILURE);
}

string send_to_childs(const vector<string>& addresses, const int& num_of_users){
    int pipe1[2];
    char got_from_reduce[ADDRESS_LEN];
    if(pipe(pipe1) < 0)
        error(PIPE_ERROR);
    
    int pid = fork();
    if(pid == 0){
        string p1 = to_string(pipe1[WRITE]);
        char* send = (char*)p1.c_str();
        string p2 = to_string(pipe1[READ]);
        char* rec = (char*)p2.c_str();
        string p3 = to_string(num_of_users);
        char* num = (char*)p3.c_str();
        //cout << "num : " << num << '\n';
        char* arg[] = {"./reduce.out", num, send, rec, NULL};
        execv("./reduce.out", arg);
    }
    else if(pid > 0){
        int counter = 0;
        
        for (int i = 0 ; i < num_of_users; i++){
            int pipe2[2];
            if (pipe(pipe2) == -1)
                error(PIPE_ERROR);
            

            int pid = fork();
            if (pid < 0)
                error(FORK_ERROR);

            if (pid == 0){     //child process
                string port = to_string(pipe2[READ]);   
                char* read_side = (char*) port.c_str();
                char* args[] = {"./map.out", read_side, NULL}; 
                execv("./map.out", args);
            }
            else{      //parent process
                close(pipe2[READ]);
                write(pipe2[WRITE], addresses[i].c_str(), ADDRESS_LEN);
                close(pipe2[WRITE]);
                wait(NULL);  
            }         
        }  
        wait(NULL);
        close(pipe1[WRITE]);
        read(pipe1[READ], got_from_reduce, sizeof(got_from_reduce));
        close(pipe1[READ]);
        cout << MAIN_FROM_REDUCE << got_from_reduce << "\n\n\n"; 
    }
    
    return got_from_reduce;
}

vector<string> assign_address(const string& addr, const int& num_of_users){
    string offset = FIRST_FILE_OFFSET;
    vector<string> addresses;
    addresses.emplace_back(addr + offset);
    
    for(int i = 1; i < num_of_users; i++){
        string s = addresses[i - 1];
        addresses.emplace_back(s);
        addresses[i][10] += 1;
    }
    return addresses;
}

void send_to_csv_file(const map<string , int>& HM){
    ofstream file;
    file.open(OUTPUT_FILE);
    for(auto i : HM){
        file << i.first << " : " << i.second << "\n";
    }
    file.close();
}

int count_files(string cmd){
    string data;
    FILE * stream;
    const int max_buffer = 10;
    char buffer[max_buffer];
    
    stream = popen(cmd.c_str(), READ_MODE);
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    }
    pclose(stream);
    return stoi(data);
}

vector<string> seperate_by_comma(const string& sentence){
    string new_word;
    int counter = 0;
    vector<string> words;
    while(sentence[counter] != '\0'){
        if(sentence[counter] == COMMA){
            words.emplace_back(new_word);
            new_word.clear();
        }
        else{
            new_word.push_back(sentence[counter]);
        }
        counter++;
    }
    words.emplace_back(new_word);
    return words;
}

void find_frequency(map<string, int>& HM, const vector<string>& words){
    for(int i = 0; i < words.size(); i++)
        HM[words[i]]++;
}

string read_data(const string& detailed_addr){
    fstream file;
    file.open(detailed_addr);
    string read;
    getline(file, read);
    //cout << "read : " << read << '\n';
    file.close();
    return read;
}

void send_to_reduce_process(const string& freq){
    //cout << "freq : " << freq << '\n';
    int fd = open(DECREASE_FIFO, O_WRONLY);
    write(fd, freq.c_str(), ADDRESS_LEN);
    close(fd);
}



