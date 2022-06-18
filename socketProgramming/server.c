#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>

#define MAX_GROUP_SIZE 3
#define MAX_MAJOR_SIZE 10
#define ELECTRICAL 1 
#define MECHANICAL 2
#define CIVIL      3
#define COMPUTER   0

void error(const char *err){
    perror(err);
    exit(1);
}

char buff[255];
struct person{
    char field[20];
};

struct group{
    int members_fd[MAX_GROUP_SIZE];
    int number_of_members;
    bool full;
};

struct group majors[4][MAX_MAJOR_SIZE];


void initialize_majors(){
    int c1, c2;
    for(c1 = 0; c1 < 4; c1++){
        for(int c2 = 0; c2 < MAX_GROUP_SIZE; c2++){
            majors[c1][c2].number_of_members = 0;
            majors[c1][c2].full = false;
        }
    }
}

struct person clients[255];

char topics[4][20] = {"computer", "electrical", "mechanical", "civil"};

int setupServer(int port) {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    listen(server_fd, 10);

    return server_fd;
}

int acceptClient(int server_fd) {
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);
    return client_fd;
}

int num_of_clients = 0;


void send_turn_and_group(int major_index, int max_index, int client_fd){
    char turn[2] = {majors[major_index][max_index].number_of_members + '0', '\0'};
    //printf("%s \n", turn);
    //char group_index[2] = {major_index + '0', '\n'};
    bzero(buff, 255);
    strcat(buff, "You're number ");
    strcat(buff, turn);
    strcat(buff, " in ");
    strcat(buff, topics[major_index]);
    strcat(buff, " major for asking question!\n\0");
    printf("%s", buff); 
    write(client_fd, buff, strlen(buff));
    return;
}

void handle_new_group(int major_index, int max_index){
    for(int i = 0; i < MAX_GROUP_SIZE - 1; i++){
        int client_fd = majors[major_index][max_index].members_fd[i];
        //printf("Your client fd is: %d\n", client_fd);
        memset(buff, 0, 255);
        strcat(buff, "Connect to group with port : ");
        char port[7] = {'8', '0', max_index + '0', major_index + '0','.', '\n','\0'};
        strcat(buff, port);
        strcat(buff, "Ask in turn. Your turn is : ");
        char turn[2] = {i + '1','\n'};
        strcat(buff, turn);
        write(client_fd, buff, strlen(buff));
    }
    int client_fd = majors[major_index][max_index].members_fd[MAX_GROUP_SIZE - 1];
    memset(buff, 0, 255);
    strcat(buff, "Connect to group with port : ");
    char port[7] = {'8', '0', max_index + '0', major_index + '0','.', '\n','\0'};
    strcat(buff, port);
    write(client_fd, buff, strlen(buff));
    memset(buff, 0, 255);
    strcat(buff, "Ask in turn. Your turn is : ");
    char turn[2] = {MAX_GROUP_SIZE - 1 + '1','\n'};
    strcat(buff, turn);
    write(client_fd, buff, strlen(buff));
    return;
}

int find_first_empty_group(int major_index){
    int max_index = 0;
    for(int i = 0; i < MAX_MAJOR_SIZE; i++){
        if(!majors[major_index][i].full){
            max_index = i;
            break;
        }
    }
    return max_index;
}

int handle_client_request(int client_fd, int major_index){
    
    int max_index = find_first_empty_group(major_index);

    for(int i = max_index; i < MAX_MAJOR_SIZE; i++){
        if((majors[major_index][i].number_of_members > majors[major_index][max_index].number_of_members) && (!majors[major_index][i].full) )
            max_index = i;
    }

    bzero(clients[client_fd].field, 20);
    strcat(clients[client_fd].field, topics[major_index]);
    
    int current_number_of_members_in_group = majors[major_index][max_index].number_of_members;
    
    majors[major_index][max_index].members_fd[current_number_of_members_in_group] = client_fd;
   
    majors[major_index][max_index].number_of_members++;
    
    send_turn_and_group(major_index, max_index, client_fd);
    bzero(buff, 255);
    if(majors[major_index][max_index].number_of_members == MAX_GROUP_SIZE){
        majors[major_index][max_index].full = true;
        return max_index;
    }
    return -1;
}

int choose_major(){
    int major_index = -1;
    if(buff[0] == 'c' && buff[1] == 'o')
        major_index = 0;
    if(buff[0] == 'e' && buff[1] == 'l')
        major_index = 1;
    if(buff[0] == 'm' && buff[1] == 'e')
        major_index = 2;
    if(buff[0] == 'c' && buff[1] == 'i')
        major_index = 3;
    return major_index;
}

int fd_comp, fd_elec, fd_mechanic, fd_civil;

void add_to_file(int client_fd){
    char* major = clients[client_fd].field;
    int sz;
    if(major[0] == 'c' && major[1] == 'o'){
        sz = write(fd_comp, buff, strlen(buff));
    }
    else if(major[0] == 'e' && major[1] == 'l'){
        sz = write(fd_elec, buff, strlen(buff));
    }
    else if(major[0] == 'm' && major[1] == 'e'){
        sz = write(fd_mechanic, buff, strlen(buff));
    }
    else if(major[0] == 'c' && major[1] == 'i'){
        sz = write(fd_civil, buff, strlen(buff));
    }
    if(sz < 0){
        error("ERROR : Couldn't write\n");
    }
}


int main(int argc, char *argv[]){
    initialize_majors();
    fd_comp = open("comp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fd_elec = open("elec.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fd_mechanic = open("mechanic.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fd_civil = open("civil.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);       
    int port = atoi(argv[1]);
    int server_fd, client_fd, max_fd;
 
    if (fd_comp < 0 || fd_elec < 0 || fd_mechanic < 0 || fd_civil < 0)
        error("Couldn't open file\n");


    server_fd = setupServer(port);
    max_fd = server_fd;

    fd_set master_set, current_set;
    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    write(1, "Server is running\n", 18);
    printf("Engineering Majors: Computer, Electrical, Mechanical, Civil \n");

    while (1) {
        current_set = master_set;
        select(max_fd + 1, &current_set, NULL, NULL, NULL);// waiting for change on current_set

        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &current_set)) {    // some thing changed in current_set
                memset(buff, 0, 255);
                if (i == server_fd) {  // new clinet connected
                    client_fd = acceptClient(server_fd);
                    FD_SET(client_fd, &master_set);
                    if (client_fd > max_fd)
                        max_fd = client_fd;
                    printf("\n\n\nNew client connected. fd = %d\n", client_fd);
                    printf("Total number of clients: %d\n", ++num_of_clients);
                }
                
                else { // client sending msg
                    int bytes_received;
                    //bzero(buff, 255);
                    bytes_received = recv(i , buff, 255, 0);
                    
                    if (bytes_received == 0) { // EOF
                        printf("client fd = %d closed\n", i);
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }

                    printf("\n\n\nClient %d: %s\n", i, buff);

                    if(buff[0] == 'Q' || buff[0] == 'T'){
                        add_to_file(i);
                    }


                    int major_index = choose_major();
                    if(major_index != -1){
                        int index = handle_client_request(i, major_index); 
                        if(index != -1){
                            bzero(buff, 255);
                            handle_new_group(major_index, index);
                        }
                    }
                    //memset(buff, 0, 255);
                }
            }
        }

    }



}