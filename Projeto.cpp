#include<bits/stdc++.h>
//biblitecas de Socket
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
//bibliotecas pra getch
#include<termios.h>
#include<unistd.h>
//bibliotecas de C
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
//bibliteca pra thread
#include<chrono>
#include<thread>
#include <iostream>

//constantes para a variaçao de movimento, porta e endereço do servidor
#define delta 5
#define PORT 8889
#define IP "127.0.0.1"

using namespace std;



// variaveis da juntas em um array

int joint[6] = {};

//variaveis auxiliares
char buffer[1024]= {0};
char parametro[1024] = {0};

//procurei na internet o getch para o linux 
int getch( ) {
  struct termios oldt, newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}


//funcoes extras 

void Giro360(){
    cout << "Giro 360 graus" << endl;
     for(int i = 0 ; i < 360 ; i ++){
        joint[0] = (joint[0] + 1)%360;
        this_thread::sleep_for(chrono::milliseconds(5));//funcao que faz a thread dormir para ter um delay na movimentacao
    }
}
void Reset(){
    cout << "Reiniciando" << endl;
    for(int i = 0 ; i < 6 ; i++){
        while(1){
            if(joint[i] == 0){
                break;
            }
            else{
                joint[i] = (joint[i] + 1)%360;
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        }
    }
}

void Acenar(){
    cout << "Giro 180 graus para acenar" << endl;
    for(int i = 0 ; i < 90 ; i++){
        joint[1] = (joint[1] + 1)%360;
        this_thread::sleep_for(chrono::milliseconds(10));
    } 
    for(int i = 0 ; i < 180 ; i++){
        joint[1] = (joint[1] - 1)%360;
        this_thread::sleep_for(chrono::milliseconds(10));
    } 
    for(int i = 0 ; i < 90 ; i++){
        joint[1] = (joint[1] + 1)%360;
        this_thread::sleep_for(chrono::milliseconds(10));
    } 

}
//thread que vai movimentar as juntas de acordo com o que foi pressionado
void keyp(){
    int key;
    while(1){
        key = getch();
        // junta 1
        if(key == 'q'){
            joint[0] = (joint[0] + delta)%360;
        }
        else if(key == 'a'){
            joint[0] = (joint[0] - delta)%360;
        }
        // junta 2
        else if(key == 'w'){
            joint[1] = (joint[1] + delta)%360;
        }
        else if(key == 's'){
            joint[1] = (joint[1] - delta)%360;
        }
        // junta 3
        else if(key == 'e'){
            joint[2] = (joint[2] + delta)%360;
        }
        else if(key == 'd'){
            joint[2] = (joint[2] - delta)%360;
        }
        // junta 4
        else if(key == 'r'){
            joint[3] = (joint[3] + delta)%360;
        }
        else if(key == 'f'){
            joint[3]= (joint[3] - delta)%360;
        }
        //junta 5
        else if(key == 't'){
            joint[4] = (joint[4] + delta)%360;
        }
        else if(key == 'g'){
            joint[4] = (joint[4] - delta)%360;
        }
        //junta 6
        else if(key == 'y'){
            joint[5] = (joint[5] + delta)%360;
        }
        else if(key == 'h'){
            joint[5] = (joint[5] - delta)%360;
        }

        //funcao para girar 360 graus
        else if (key == 'z'){
            Giro360();
        }

        //funcao pra acenar
        else if(key == 'x'){
            Acenar();
        }

        //funcao pra resetar
        else if(key == 'c'){
            Reset();
        }


        //apertou ESC para sair 
        else if(key == 27){
            exit(1);
        }
    }
}


int main(){
    //chamando a thread
    thread first(keyp);
    first.detach();//executa em paralelo


    int server_fd , new_socket , valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    
    //criando o socket 
    server_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (server_fd == 0){
        cout << "Erro ao criar o socket" << endl;
        exit(1);
    }

    //forçando socket para a porta 8889
    if(setsockopt(server_fd , SOL_SOCKET , SO_REUSEADDR | SO_REUSEPORT , &opt , sizeof(opt))){
        cout << "Setsockopt"<< endl;
        exit(1);
    }

    //conectar a porta 8889 e IP 127.0.0.1

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(PORT);

    //bind o ip e a porta 

    if(bind(server_fd , (struct sockaddr *) &address , sizeof(address))<0){
        cout << "Erro bind" << endl;
        exit(1);
    }

    //listen 
    

    //conexao
    while(true){
        if(listen(server_fd , 1) >= 0){
            //cout << "sucesso na escuta" << endl;
            
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))>= 0) { 
                //cout << "Sucesso em aceitar a conexao" << endl; 
                
            
                int movement=0;
                valread = read( new_socket , buffer, 1024); //valor lido do buffer
                cout << valread << endl; 
                while(valread){
                    //cout << "entrou no while do valread" << endl;
                    string message;
                    for(int i=0 ; i < sizeof(buffer) / sizeof(char) ; i++){
                        message += buffer[i];// colocando as mensgens do buffer na string
                        if( buffer[i] == '>'){
                            break;
                        }
                        
                    }
                    //cout << "passou de carregar a message" << endl << message << endl;
                    //if e elses para as mensagens 
                    if(message == "<CONNECTION_OPEN>"){
                        //cout << "entrou no connection_open" << endl;
                        strcpy(parametro , "CONNECTION_OPEN;;");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_NUM_ARMS>"){
                        //cout << "entrou no get_num_arms" << endl;
                        strcpy(parametro , "GET_NUM_ARMS;1;");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_ARM_CODNAME;1>"){
                        strcpy(parametro , "GET_ARM_CODNAME;1;NS_00101");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_ARM_AXES;1>"){
                        strcpy(parametro , "GET_ARM_AXES;1;111111----");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_ARM_AUX;1>"){
                        strcpy(parametro , "GET_ARM_AUX;1;----------");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_ARM_BASE;1>"){
                        strcpy(parametro , "GET_ARM_BASE;1;0;0;0;0;0;0");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_TOOL_RMT;1>"){
                        strcpy(parametro , "GET_TOOL_RMT;1;False");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_ARM_ALL_FRAMES;1>"){
                        strcpy(parametro , "GET_ARM_ALL_FRAMES;1;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_AUX_BASE;1>"){
                        strcpy(parametro , "GET_AUX_BASE;1;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");
                        send(new_socket , parametro , strlen(parametro) , 0);
                    }
                    else if(message == "<GET_ALL_JNT>"){
                        strcpy(parametro , "GET_ALL_JNT;");
                        char auxiliar[3];
                        //auxiliar para converter o valor da junta em string para depois concatenar com o parametro
                        // faço isso para todas as 6 juntas 
                        

                        for(int i = 0 ; i < 6 ;i++){
                            sprintf(auxiliar , "%d;" , joint[i]);
                            strcat(parametro , auxiliar);
                        }
                        //no final concateno com os outros 0
                        strcat(parametro  , ";0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");

                        

                        send(new_socket , parametro , strlen(parametro) , 0);
                        movement++;
                    }
                    else if(message == "<GET_IR_TYPES>"){
                        //cout << "chegou ate o type" << endl;
                        strcpy(parametro , "GET_IR_TYPES;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");
                        send(new_socket , parametro , strlen(parametro) , 0);
                        
                    }
                    valread = read( new_socket , buffer, 1024);
                }
                cout << "Conexao encerrada" << endl;
            }
        }
    }
    return 0;
}

