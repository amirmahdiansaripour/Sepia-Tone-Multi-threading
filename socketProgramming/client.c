#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#define STDIN 0

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

#define USERS_FOR_PROJECT 3
#define ANSWER_TIME 60

bool broad_cast = true;
bool allow_to_ask = true;

char buffer[255];
char statement[255];
char question[255];
char first_ans[255]; 
char second_ans[255];
char send_file[1024];
char answer_turn[3];

int ask_turn = 1;
int my_turn;


void ask_and_answer(char* p, int mode){
    if(mode == 0){
        strcat(buffer, "Question from person ");
    }
    else{
        strcat(buffer, "Answer from person ");
    }
    strcat(buffer, p);
    strcat(buffer, " : ");
    return;
}
void initialize(){
    bzero(buffer, 255);
    bzero(statement, 255);
    bzero(question, 255);
    return;
}
void manage_answers(){
    bzero(statement, 255);
    int index = 23;
    while(buffer[index] != '\0'){
        char t[2] = {buffer[index], '\0'};
        strcat(statement,t);
        index++;
    }
    //printf("statement : %s\n", statement);
    if(first_ans[0] == '0' || strlen(first_ans) == 0){
        bzero(first_ans, 255);
        //printf("first is empty \n");
        strcat(first_ans, statement);
    }
    else if(second_ans[0] == '0' || strlen(second_ans) == 0){
        bzero(second_ans, 255);
        //printf("second is empty \n");
        strcat(second_ans, statement);
    }
}

bool qustion_is_answered(){
    if(answer_turn[0] == '1' && answer_turn[1] == '1' && answer_turn[2] == '1'){
        return true;
    }
    return false;
}

void alarm_handler(int sig){
    bzero(buffer, 255);
    strcat(buffer, "Answer from person ");
    char p[2] = {my_turn + '0', '\0'};
    strcat(buffer, p);
    strcat(buffer, " : ");
    strcat(buffer, "Not enough time!!\n");
}

int udp_port, udp_sockfd;
struct sockaddr_in bc_adr_sendto, bc_adr_recvfrom;
int sockfd, port;

void find_UDP_port(){
    //printf("llllllll\n");
    int index = 0;
    for(int i = 0; i < strlen(buffer); i++){
        if(buffer[i] == ':'){
            index = i + 2;
            break; 
        }
    }
    char p[4] = {buffer[index], buffer[index + 1], buffer[index + 2], buffer[index + 3]};
    //printf("udp_port : %s\n", port);
    udp_port = atoi(p);
    return;
}

bool check_turn(){
    if(answer_turn[my_turn - 1] == '1') return false;
    for(int i = 0; i < my_turn - 1; i++){
        if(answer_turn[i] == '0')
            return false;
    }
    return true;
}

void ack_next_person(){
    bzero(question, 225);
    strcat(question, "It's turn of person ");
    char p[2] = {ask_turn + '1', '\0'};
    strcat(question, p);
    strcat(question, " to ask question\n");
    sendto(udp_sockfd, question, strlen(question), 0,(struct sockaddr *)&bc_adr_sendto, sizeof(struct sockaddr_in));
    return;
}

void end_of_question(){
    bzero(question, 255);
    bzero(statement, 255);
    bzero(send_file, 255);
    bool a = (first_ans[0] == 'N' || (first_ans[0] == 'I' && first_ans[1] == ' ') || strlen(first_ans) == 0) ? false : true;       
    int index = 23;
    //printf("buffer[index] : %c\n", buffer[index]);
    while(buffer[index] != '\0'){
        char t[2] = {buffer[index], '\0'};
        strcat(statement,t);
        index++;
    }
    //printf("statement : %s\n", statement);
    bool b = (statement[0] == 'N' || (statement[0] == 'I' && statement[1] == ' ') || strlen(statement) == 0) ? false : true;
    if(!a && !b){ // Nobody answered
        strcat(question, "My question wasn't answered!! \n");
    }
    else{   //Best answer
        strcat(question, "The best answer was : ");
        strcat(send_file, "The best answer was : ");
        bool first_added = false;
        if(a == true){
            first_added = true;
            strcat(question, first_ans);
            strcat(send_file, first_ans);

        }
        if(b == true) {
            if(a)
                strcat(send_file, "Another answer :");
            strcat(send_file, statement);
            strcat(send_file, "\n");
            if(!first_added)
                strcat(question, statement);
        }
        write(sockfd, send_file, strlen(send_file));
    }
    strcat(question, "\n");
    sendto(udp_sockfd, question, strlen(question), 0,(struct sockaddr *)&bc_adr_sendto, sizeof(struct sockaddr_in));
    ack_next_person();
    return;
}

void connect_UDP(){
    udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sockfd < 0)
        error("ERROR opening socket\n");
    
    int broadcast_en = 1, opt_en = 1, reuse_adr_en = 1;
    setsockopt(udp_sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_en, sizeof(broadcast_en)); 
    setsockopt(udp_sockfd, SOL_SOCKET, SO_REUSEPORT, &opt_en, sizeof(opt_en));
    setsockopt(udp_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_adr_en, sizeof(reuse_adr_en));
    
    bc_adr_recvfrom.sin_family = AF_INET;
    bc_adr_recvfrom.sin_addr.s_addr = htonl(INADDR_ANY);
    bc_adr_recvfrom.sin_port = htons(udp_port);

    bc_adr_sendto.sin_family = AF_INET;
    bc_adr_sendto.sin_addr.s_addr = htonl(INADDR_BROADCAST); 
    bc_adr_sendto.sin_port = htons(udp_port);

    
    if (bind(udp_sockfd, (struct sockaddr *) &bc_adr_recvfrom, sizeof(bc_adr_recvfrom)) < 0) 
        error("ERROR on binding");
    
}

void send_question_to_server(){
    bzero(send_file, 1024);
    strcat(send_file, "Question : ");
    strcat(send_file, question);
    write(sockfd, send_file, strlen(send_file));
}

// void observe_turn(int t){
//     bzero(buffer, 1024);
//     strcat(buffer, "It's not your turn person ");
//     char p[3] = {t + '0', '\n','\0'};
//     strcat(buffer, p);
//     return;
// }

void init_answer_turn(){
    for(int i = 0; i < 3; i++)
        answer_turn[i] = '0';
}

void update_answer_turn(){
    for(int i = 0; i < USERS_FOR_PROJECT; i++){
        if(answer_turn[i] != '1'){
            answer_turn[i] = '1';
            break;
        }
    }
    return;
}

bool question_answered = false;

void pass_process(char* p){
    strcat(buffer, "I don't know the answer of question ");
    strcat(buffer, p);
    strcat(buffer, "\n");
    return;               
}

int main(int argc, char *argv[]){
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 2) {
       error("ERROR : more info is needed! \n");
       exit(EXIT_FAILURE);
    }
    port = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR : opening socket\n");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR: no such host\n");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR : can't connect\n");


    fd_set current_sockets;


    while (1){
        FD_ZERO(&current_sockets);
        FD_SET(sockfd, &current_sockets); 
        FD_SET(STDIN, &current_sockets);  

        if (select(sockfd + 1, &current_sockets, NULL, NULL, NULL) < 0)
            perror("ERROR on select");
        
        if (FD_ISSET(sockfd, &current_sockets)){ //something from server
            
                bzero(buffer, 255);
                int n = read(sockfd, buffer, 255);
                if (n < 0)
                    error("ERROR : client could'nt read from server\n");
                
                printf("MESSAGE FROM SERVER: %s\n\n", buffer);
                //printf("d: %d\n", strlen(buffer));
                if(buffer[0] == 'Y'){
                    //printf("hiiiiiiii\n");
                    char p[2] = {buffer[14], '\0'};
                    int t = atoi(p);
                    my_turn = t;
                    //printf("My turn : %d\n", my_turn); 
                }
                else if((buffer[0] == 'C' && buffer[1] == 'o') || (buffer[0] == 'A' && buffer[1] == 's') || strlen(buffer) > 65){
                    find_UDP_port();
                    //FD_CLR(sockfd, &current_sockets);
                    //FD_CLR(STDIN, &current_sockets);
                    break;
                }
        }

        if (FD_ISSET(STDIN, &current_sockets)){  //something from you to server
            bzero(buffer, 255);
            fgets(buffer, 255, stdin);
            int n = write(sockfd, buffer, strlen(buffer));
            if(n < 0)
                error("ERROR: Couldn't send to server\n");
        }

    }


    connect_UDP();
    
    while (1){
        signal(SIGALRM, alarm_handler);
        siginterrupt(SIGALRM, 1);
        FD_ZERO(&current_sockets);
        FD_SET(udp_sockfd, &current_sockets);
        FD_SET(STDIN, &current_sockets);  //standard input 

        int res = select(udp_sockfd + 1, &current_sockets, NULL, NULL, NULL);
        
        if (FD_ISSET(STDIN, &current_sockets)){ // something from you to other persons
            initialize();
            
            char p[2] = {ask_turn + '0', '\0'};
            char p2[2] = {my_turn + '0', '\0'};

            fgets(statement, 255, stdin);

            bool q_state = (statement[0] == 'Q' || statement[0] == 'q'); 
            bool a_state = ((statement[0] == 'A' || statement[0] == 'a'));
            if(q_state){//question
                if(ask_turn == my_turn && allow_to_ask){
                    //printf("hiiii\n");
                    ask_and_answer(p2, 0);
                    fgets(question, 255, stdin);
                    strcat(buffer, question);
                    send_question_to_server();
                    sendto(udp_sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&bc_adr_sendto, sizeof(struct sockaddr_in)); 
                }
                else{
                    //printf("jijiji\n");
                    strcat(buffer, "It's not your turn person ");
                    strcat(buffer, p2);
                    strcat(buffer, "\n");
                    sendto(udp_sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&bc_adr_sendto, sizeof(struct sockaddr_in)); 
                }
                
            }

            else if(a_state){// answer
                if(check_turn()){
                    //printf("hiiiii\n");
                    ask_and_answer(p2, 1);
                    alarm(ANSWER_TIME);
                    fgets(question, 255, stdin);
                    if(question[0] == 'p' && question[1] == 'a' && question[2] == 's' && question[3] == 's' && strlen(question) == 5){
                        pass_process(p);
                    }
                    else
                        strcat(buffer, question);
                    sendto(udp_sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&bc_adr_sendto, sizeof(struct sockaddr_in));
                    alarm(0);
                }
                else{
                    //printf("jijiji\n");
                    strcat(buffer, "It's not your turn person ");
                    strcat(buffer, p2);
                    strcat(buffer, "\n");
                    sendto(udp_sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&bc_adr_sendto, sizeof(struct sockaddr_in)); 
                }
            }

            if(first_ans[0] != '0' && strlen(first_ans) != 0 && a_state == true){
                    //printf("\nhiiiiiiiiiiiii\n");
                    end_of_question();
            }
        }

        if (FD_ISSET(udp_sockfd, &current_sockets)){ //something broadcasted on udp_sockfd
            bzero(buffer, 255);
            socklen_t bc_adr_len = sizeof(bc_adr_recvfrom);
            recvfrom(udp_sockfd, buffer, 255, 0, (struct sockaddr *)&bc_adr_recvfrom, &bc_adr_len);
            printf("BROADCASTED MESSAGE: %s\n", buffer);
            if(buffer[0] == 'Q'){   //Question is broadcasted
                init_answer_turn();
                bzero(first_ans, 255);
                bzero(second_ans, 255);
                char t[2] = {buffer[21], '\n'};
                int a = atoi(t) - 1;
                answer_turn[a] = '1';
                allow_to_ask = false;
                //printf("answer_turn : %s\n", answer_turn);
            }
            if(buffer[0] == 'A'){   //Answer is broadcasted
                manage_answers();
                update_answer_turn();
                //printf("answer_turn : %s\n", answer_turn);
                if(qustion_is_answered()){
                    allow_to_ask = true;
                    ask_turn++;
                    printf("Question completed\n");
                    if(ask_turn == USERS_FOR_PROJECT + 1){  // End of service
                        bool a = (first_ans[0] == 'N' || (first_ans[0] == 'I' && first_ans[1] == ' ') || strlen(first_ans) == 0) ? false : true;       
                        bool b = (second_ans[0] == 'N' || (second_ans[0] == 'I' && second_ans[1] == ' ') || strlen(second_ans) == 0) ? false : true;
                        if(!a && !b) // Nobody answered
                            printf("My question wasn't answered!! \n");
                        else{   //Best answer
                            printf("The best answer was : ");
                            bool first_added = false;
                            if(a == true){
                                first_added = true;
                                printf("%s", first_ans);
                            }
                            if(b == true) {
                                if(!first_added)
                                    printf("%s\n", second_ans);
                            }
                        }
                        printf("Group is done\n");
                        ask_turn = 0;
                        break;
                    }
                }
            }
            fflush(stdout);
        }

    }
    
    return 0;
}