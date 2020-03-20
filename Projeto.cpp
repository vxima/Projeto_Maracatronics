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
#include<thread>
#include <iostream>

//constantes para a variaçao de movimento, porta e endereço do servidor
#define delta 5
#define PORT 8889
#define IP "127.0.0.1"

using namespace std;

// variaveis das juntas 
int joint1=0;
int joint2=0;
int joint3=0;
int joint4=0;
int joint5=0;
int joint6=0;

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

//thread que vai movimentar as juntas de acordo com o que foi pressionado
void keyp(){
    int key;
    while(1){
        key = getch();
        // junta 1
        if(key == 'q'){
            joint1 += delta;
        }
        else if(key == 'a'){
            joint1 -= delta;
        }
        // junta 2
        if(key == 'w'){
            joint2 += delta;
        }
        else if(key == 's'){
            joint2 -= delta;
        }
        // junta 3
        if(key == 'e'){
            joint3 += delta;
        }
        else if(key == 'd'){
            joint3 -= delta;
        }
        // junta 4
        if(key == 'r'){
            joint4 += delta;
        }
        else if(key == 'f'){
            joint4 -= delta;
        }
        //junta 5
        if(key == 't'){
            joint5 += delta;
        }
        else if(key == 'g'){
            joint5 -= delta;
        }
        //junta 6
        if(key == 'y'){
            joint6 += delta;
        }
        else if(key == 'h'){
            joint6 -= delta;
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
        cout << "Erro bind" < endl;
        exit(1);
    }

    //listen 
    if(listen(server_fd , 1) <0){
        cout << "Erro na escuta" << endl;
        exit(1);
    }

    //conexao
    while(true){
        cout << "esperando conexao" << endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
            cout << "Erro em aceitar a conexao" << endl; 
            exit(1); 
        } 
        int movement=-;
        valread = read( new_socket , buffer, 1024); //valor lido do buffer 
        while(valread){
            string message;
            for(int i=0 ; i < 1024 , i++){
                message += buffer[i];// colocando as mensgens do buffer na string 
            }
            
            //if e elses para as mensagens 
            if(message == "CONNECTION_OPEN"){
                strcpy(parametro , "CONNECTION_OPEN;;");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_NUM_ARMS"){
                strcpy(parametro , "GET_NUM_ARMS;1;");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_ARM_CODNAME;1"){
                strcpy(parametro , "GET_ARM_CODNAME;1;NS_00101");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_ARM_AXES;1"){
                strcpy(parametro , "GET_ARM_AXES;1;111111----");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_ARM_AUX;1"){
                strcpy(parametro , "GET_ARM_AUX;1;----------");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_ARM_BASE;1"){
                strcpy(parametro , "GET_ARM_BASE;1;0;0;0;0;0;0");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_TOOL_RMT;1"){
                strcpy(parametro , "GET_TOOL_RMT;1;False");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_ARM_ALL_FRAMES;1"){
                strcpy(parametro , "GET_ARM_ALL_FRAMES;1;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");
                send(new_socket , parametro , strlen(parametro) , 0);
            }
            else if(message == "GET_ALL_JNT"){
                strcpy(parametro , "GET_ALL_JNT;");
                char auxiliar[3];
                //auxiliar para converter o valor da junta em string para depois concatenar com o parametro
                // faço isso para todas as 6 juntas 
                sprintf(auxiliar , "%d;" , joint1);
                strcat(parametro , auxiliar);
                sprintf(auxiliar , "%d;" , joint2);
                strcat(parametro , auxiliar);
                sprintf(auxiliar , "%d;" , joint3);
                strcat(parametro , auxiliar);
                sprintf(auxiliar , "%d;" , joint4);
                strcat(parametro , auxiliar);
                sprintf(auxiliar , "%d;" , joint5);
                strcat(parametro , auxiliar);
                sprintf(auxiliar , "%d;" , joint6);
                strcat(parametro , auxiliar);
                //no final concateno com os outros 0
                strcat(parametro  , ";0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");

                

                send(new_socket , parametro , strlen(parametro) , 0);
                movement++;
            }
            else if(message == "GET_IR_TYPES"){
                strcpy(parametro , "GET_IR_TYPES;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0");
                send(new_socket , parametro , strlen(parametro) , 0);
                
            }
            valread = read( new_socket , buffer, 1024);
        }
        cout << "Conexao encerrada" << endl;
        
    }
    return 0;
}

