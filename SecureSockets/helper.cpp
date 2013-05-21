#include "helper.h"

int min(int first, int second){
	if(first < second){
		return first;
	}
	return second;
}


/* Keeps transmitting a buffer until the buffer is transmitted */
int tcp_send(const int& socket, const void* buffer, const int& buffSize){
	/* @numSent - the number of bytes sent in one shot
	 * @totalBytesSent - the cumulative number of bytes sent
	 */
	int numSent = 0, totalBytesSent = 0;
	
	/* Keep transmitting until all is transmitted */
	while(totalBytesSent != buffSize){
		/* Transmit the buffer */
		if((numSent = write(socket, (char*)buffer + totalBytesSent, buffSize - totalBytesSent)) < 0){
			return numSent; /* Something went wrong while transmitting */
		}
		totalBytesSent += numSent; /* Update the total number of bytes sent */
	}	
	return totalBytesSent; /* Successful transmission */
}


/* Keeps receiving a buffer until all bytes are received */
int tcp_recv(const int& socket, void* buffer, const int& buffSize){
	/* @numRecv - the number of bytes received in one shot
	 * @totalBytesRecv - the cumulative number of bytes received
	 */
	int numRecv = 0, totalBytesRecv = 0;
	
	/* Keep transmitting until all is transmitted */
	while(totalBytesRecv != buffSize){
		/* Transmit the buffer */
		if((numRecv = read(socket, (char*)buffer + totalBytesRecv, buffSize - totalBytesRecv)) < 0){
			return numRecv; /* Something went wrong while receiving */
		}
		totalBytesRecv += numRecv; /* Update the total number of bytes receiving */
	}
	return totalBytesRecv; /* Successful reception */
}

/* Send size over socket */
int tcp_send_size(const int& socket, const int& size){
	char size_buff[SIZE_MSG_SIZE]; /* The buffer to store the stringified size */
	
	/* Conver int to string and get length*/
	if(snprintf(size_buff, SIZE_MSG_SIZE, "%d", size) < 0){
		return -1;
	}
	
	/* Send the size */
	return tcp_send(socket, size_buff, SIZE_MSG_SIZE);	
}

/* Receive size message */
int tcp_recv_size(const int& socket){
	char size_buff[SIZE_MSG_SIZE + 1]; /* The buffer to store the size */
	int numRecv = -1; /* The number of bytes received */
			
	/* Receive the size */
	if((numRecv = tcp_recv(socket, size_buff, SIZE_MSG_SIZE)) < 0){
		return numRecv;	/* Something went wrong */
	}
	size_buff[numRecv] = '\0'; /* NULL terminate the received size */
	return atoi(size_buff); /* Convert the string to integer, and we are done */
}

#include "Helper.h"

int LetterToInt(string letter){
    if(letter == "A"){
        return 0;
    }else if(letter == "B"){
        return 1;
    }else if(letter == "C"){
        return 2;
    }else if(letter == "D"){
        return 3;
    }else if(letter == "E"){
        return 4;
    }else if(letter == "F"){
        return 5;
    }else if(letter == "G"){
        return 6;
    }else if(letter == "H"){
        return 7;
    }else if(letter == "I"){
        return 8;
    }else if(letter == "J"){
        return 9;
    }else if(letter == "K"){
        return 10;
    }else if(letter == "L"){
        return 11;
    }else if(letter == "M"){
        return 12;
    }else if(letter == "N"){
        return 13;
    }else if(letter == "O"){
        return 14;
    }else if(letter == "P"){
        return 15;
    }else if(letter == "Q"){
        return 16;
    }else if(letter == "R"){
        return 17;
    }else if(letter == "S"){
        return 18;
    }else if(letter == "T"){
        return 19;
    }else if(letter == "U"){
        return 20;
    }else if(letter == "V"){
        return 21;
    }else if(letter == "W"){
        return 22;
    }else if(letter == "X"){
        return 23;
    }else if(letter == "Y"){
        return 24;
    }else if(letter == "Z"){
        return 25;
    }
    return 0;
}

string IntToLetter(int intValue){
    switch (intValue) {
        case 0:
            return "A";
        case 1:
            return "B";
        case 2:
            return "C";
        case 3:
            return "D";
        case 4:
            return "E";
        case 5:
            return "F";
        case 6:
            return "G";
        case 7:
            return "H";
        case 8:
            return "I";
        case 9:
            return "J";
        case 10:
            return "K";
        case 11:
            return "L";
        case 12:
            return "M";
        case 13:
            return "N";
        case 14:
            return "O";
        case 15:
            return "P";
        case 16:
            return "Q";
        case 17:
            return "R";
        case 18:
            return "S";
        case 19:
            return "T";
        case 20:
            return "U";
        case 21:
            return "V";
        case 22:
            return "W";
        case 23:
            return "X";
        case 24:
            return "Y";
        case 25:
            return "Z";
        default:
            return "ERROR";
    }
}


