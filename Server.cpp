#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>

using namespace std;


int main() {

    //create socket
    int listening= socket(AF_INET,SOCK_STREAM,0);

    //3 onwaord outup if success, automatic increment, lsof

    if (listening<=0){
        cout<<"port not opened"<<endl;
        return -1;
    }

    //bind the socket (picking random IP) (port=3456)
    sockaddr_in hint;
    hint.sin_family=AF_INET;
    //Convert little endian to big endian HOT to NOT
    hint.sin_port=htons(54000); //ntohs!
    //run on any IP
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    //bind now
    if (bind(listening, (sockaddr*)&hint, sizeof(hint))==-1) {

        cout<< "cannot bind to  IP/PORT";
        return -2;

    }

    //mark socket for listening
    //queue size generate, how many pending connections, timeout, max queue size, 
    //if zero immediate disconnect
    if (listen(listening, 10)==-1) {

        cout<<"can't listen";
        return -3;

    }

    //accept a call 
    //wiats in clocking state, make new thread, 
    sockaddr_in client;
    socklen_t clientsize=sizeof(client);
    char host[NI_MAXHOST];
    char svc [NI_MAXSERV];

    int clientsocket=accept(listening, (sockaddr*)&client, &clientsize);

    if (clientsocket == -1) {

        cout<<"problem with client connecting";
        return -4;
    }
    //must take multiple connections
    //must not close 
    close(listening);

    //clean garbage

    memset(host, 0, NI_MAXHOST);
    memset(svc,0,NI_MAXSERV);

    int result=getnameinfo((sockaddr*)&client,sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result) {

        cout<<"host connected on"<<svc <<endl;

    }

    else {

        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout<<host<< "connected on "<<ntohs(client.sin_port)<<endl;

    }

    //display message
    char buf[4096];

    while(true) {

        //clear buffer
        memset(buf, 0, 4096);

        //wait for a message
        int bytesrecv= recv(clientsocket, buf, 4096, 0);
        if (bytesrecv == -1) {

            cout<< "there was a connection issue"<<endl;
            break;

        }

        if (bytesrecv==0){

            cout<<"The client disconnected"<< endl;
            break;
        }
        

        //display
        cout<<"\033[1;32m"<< "Recieved: " <<"\033[1;31m"<< string(buf, 0, bytesrecv)<<endl;


    }

    //close socket
    close(clientsocket);

    return 0;

}

//write client : 
//new client thread only for that client
//one main thread only for accept - blocking
//asynchronus comm with timeout, all clients single thread, file descriptor non blocking
//after client, go to while loop to accept second client
//new client new thread

//when client close socket, disconnect thread/kill


// TCP/IP->connection oriented, needs an assigned endpoints

// Server
// socket()-> bind()->listen ()->accept() -> read() /write()->close


// client
// socket()->connect()->read/write->close

// structures used? sockaddr

// network vyte order -> big endian
// linux -> intel -> little edian
// convert little endian to big endian -> host order to network order
