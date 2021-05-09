#include <stdio.h>
#include <string.h>

#define CODEC8 8
#define CODEC8EXT 142

#define CODEC_ID_SIZE 1
#define NUMBER_OF_DATA1_SIZE 1
#define NUMBER_OF_DATA2_SIZE 1
#define CRC16_SIZE 4
//TCP
#define PREAMBLE_SIZE 4
#define DATA_FIELD_LENGTH_SIZE 4
//UDP
#define LENGTH_SIZE 2
#define PACKET_ID_SIZE 2
#define PACKET_TYPE_SIZE 1
#define AVL_PACKET_ID_SIZE 1
#define IMEI_LENGTH_SIZE 2

#define TIMESTAMP_SIZE 8
#define PRIORITY_SIZE 1
#define LONGITUDE_SIZE 4
#define LATITUDE_SIZE 4
#define ALTITUDE_SIZE 2
#define ANGLE_SIZE 2
#define SATELLITES_SIZE 1
#define SPEED_SIZE 2

#define MAX_IO_IN_CLASS 10

char inputData[2500];
char protocol[4];

struct AVL {
    //GPS
    unsigned long long timestamp;
    int priority;
    int longitude;
    int latitude;
    int altitude;
    int angle;
    int satellites;
    int speed;
    //I/O
    int enentID;
    int totalElementCount;
    int elementCount1b;
    int elementCount2b;
    int elementCount4b;
    int elementCount8b;
    int elementCountXb;
    int elements1b[MAX_IO_IN_CLASS][2];
    int elements2b[MAX_IO_IN_CLASS][2];
    int elements4b[MAX_IO_IN_CLASS][2];
    int elements8b[MAX_IO_IN_CLASS][2];
    int elementsXb[MAX_IO_IN_CLASS][2];
};

struct Codec {
    char protocol[4];
    //UPD
    int totalLength;
    int packetID;
    int packetType;
    int AVLpacketID;
    int imeiLength;
    unsigned long long imei;
    //TCP
    int preamble;
    int dataFieldLength;
    //Common
    int CodecID;
    int NumberOfData1;
    struct AVL AVLdata;
    int NumberOfData2;
    int CRC16;
};

void printOutput(struct Codec Data) {
    int ioElementSize;
    if(Data.CodecID == CODEC8) {
        ioElementSize = 1;
    } else {
        ioElementSize = 2;
    }

    printf("\n%-30s %-10s %-20s %-20s\n", "Name", "Size", "Value", "Hex Value");

    if(strcmp(Data.protocol, "TCP") == 0) {
        printf("%-30s %-10s\n", "TCP AVL Data Packet", "var");
        printf("%-30s %-10d %-20d %-20.*X\n", "  Preamble", PREAMBLE_SIZE, Data.preamble, PREAMBLE_SIZE*2, Data.preamble);
        printf("%-30s %-10d %-20d %-20.*X\n", "  AVL Data Length", DATA_FIELD_LENGTH_SIZE, Data.dataFieldLength, DATA_FIELD_LENGTH_SIZE*2, Data.dataFieldLength);
    } else {
        printf("%-30s %-10s\n", "UDP AVL Data Packet", "var");
        printf("%-30s %-10d %-20d %-20.*X\n", "  Length", LENGTH_SIZE, Data.totalLength, LENGTH_SIZE*2, Data.totalLength);
        printf("%-30s %-10d %-20d %-20.*X\n", "  Packet ID", PACKET_ID_SIZE, Data.packetID, PACKET_ID_SIZE*2, Data.packetID);
        printf("%-30s %-10d %-20d %-20.*X\n", "  Packet Type", PACKET_TYPE_SIZE, Data.packetType, PACKET_TYPE_SIZE*2, Data.packetType);
        printf("%-30s %-10d %-20d %-20.*X\n", "  AVL packet ID", AVL_PACKET_ID_SIZE, Data.AVLpacketID, AVL_PACKET_ID_SIZE*2, Data.AVLpacketID);
        printf("%-30s %-10d %-20d %-20.*X\n", "  Imei length", IMEI_LENGTH_SIZE, Data.imeiLength, IMEI_LENGTH_SIZE*2, Data.imeiLength);
        printf("%-30s %-10d %-20d %-20.*X\n", "  Imei", Data.imeiLength, Data.imei, Data.imeiLength*2, Data.imei);
    }

    printf("%-30s %-10s\n", "  Data", "var");
    printf("%-30s %-10d %-20d %-20.*X\n", "   Codec ID", CODEC_ID_SIZE, Data.CodecID, CODEC_ID_SIZE*2, Data.CodecID);
    printf("%-30s %-10d %-20d %-20.*X\n", "   AVL Data Count", NUMBER_OF_DATA1_SIZE, Data.NumberOfData1, NUMBER_OF_DATA1_SIZE*2, Data.NumberOfData1);

    //AVL Data
    printf("%-30s %-10s\n", "   AVL Data", "var");
    printf("%-30s %-10d %-20d %-20.*X\n", "    Timestamp", TIMESTAMP_SIZE, Data.AVLdata.timestamp, TIMESTAMP_SIZE*2, Data.AVLdata.timestamp);
    printf("%-30s %-10d %-20d %-20.*X\n", "    Priority", PRIORITY_SIZE, Data.AVLdata.priority, PRIORITY_SIZE*2, Data.AVLdata.priority);
    //GPS element
    printf("%-30s %-10d\n", "    GPS Element", LONGITUDE_SIZE + LATITUDE_SIZE + ALTITUDE_SIZE + ANGLE_SIZE + SATELLITES_SIZE + SPEED_SIZE);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Longitude", LONGITUDE_SIZE, Data.AVLdata.longitude, LONGITUDE_SIZE*2, Data.AVLdata.longitude);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Latitude", LATITUDE_SIZE, Data.AVLdata.latitude, LATITUDE_SIZE*2, Data.AVLdata.latitude);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Altitude", ALTITUDE_SIZE, Data.AVLdata.altitude, ALTITUDE_SIZE*2, Data.AVLdata.altitude);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Angle", ANGLE_SIZE, Data.AVLdata.angle, ANGLE_SIZE*2, Data.AVLdata.angle);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Satellites", SATELLITES_SIZE, Data.AVLdata.satellites, SATELLITES_SIZE*2, Data.AVLdata.satellites);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Speed", SPEED_SIZE, Data.AVLdata.satellites, SPEED_SIZE*2, Data.AVLdata.satellites);
    //IO element
    printf("%-30s %-10s\n", "    I/O Element", "var");
    printf("%-30s %-10d %-20d %-20.*X\n", "     Event ID", ioElementSize, Data.AVLdata.enentID, ioElementSize*2, Data.AVLdata.enentID);
    printf("%-30s %-10d %-20d %-20.*X\n", "     Element count", ioElementSize, Data.AVLdata.totalElementCount, ioElementSize*2, Data.AVLdata.totalElementCount);
    //1b element
    printf("%-30s %-10d %-20d %-20.*X\n", "     1b Element count", ioElementSize, Data.AVLdata.elementCount1b, ioElementSize*2, Data.AVLdata.elementCount1b);
    for(int i = 0; i < Data.AVLdata.elementCount1b; i++) {
        printf("%-30s %-10d %-20d %-20.*X\n", "     ID", ioElementSize, Data.AVLdata.elements1b[i][0], ioElementSize*2, Data.AVLdata.elements1b[i][0]);
        printf("%-30s %-10d %-20d %-20.2X\n", "     Value", 1, Data.AVLdata.elements1b[i][1], Data.AVLdata.elements1b[i][1]);
    }
    //2b element
    printf("%-30s %-10d %-20d %-20.*X\n", "     2b Element count", ioElementSize, Data.AVLdata.elementCount2b, ioElementSize*2, Data.AVLdata.elementCount2b);
    for(int i = 0; i < Data.AVLdata.elementCount2b; i++) {
        printf("%-30s %-10d %-20d %-20.*X\n", "     ID", ioElementSize, Data.AVLdata.elements2b[i][0], ioElementSize*2, Data.AVLdata.elements2b[i][0]);
        printf("%-30s %-10d %-20d %-20.4X\n", "     Value", 2, Data.AVLdata.elements2b[i][1], Data.AVLdata.elements2b[i][1]);
    }
    //4b element
    printf("%-30s %-10d %-20d %-20.*X\n", "     4b Element count", ioElementSize, Data.AVLdata.elementCount4b, ioElementSize*2, Data.AVLdata.elementCount4b);
    for(int i = 0; i < Data.AVLdata.elementCount4b; i++) {
        printf("%-30s %-10d %-20d %-20.*X\n", "     ID", ioElementSize, Data.AVLdata.elements4b[i][0], ioElementSize*2, Data.AVLdata.elements4b[i][0]);
        printf("%-30s %-10d %-20d %-20.8X\n", "     Value", 4, Data.AVLdata.elements4b[i][1], Data.AVLdata.elements4b[i][1]);
    }
    //8b element
    printf("%-30s %-10d %-20d %-20.*X\n", "     8b Element count", ioElementSize, Data.AVLdata.elementCount8b, ioElementSize*2, Data.AVLdata.elementCount8b);
    for(int i = 0; i < Data.AVLdata.elementCount8b; i++) {
        printf("%-30s %-10d %-20d %-20.*X\n", "     ID", ioElementSize, Data.AVLdata.elements8b[i][0], ioElementSize*2, Data.AVLdata.elements8b[i][0]);
        printf("%-30s %-10d %-20d %-20.16X\n", "     Value", 8, Data.AVLdata.elements8b[i][1], Data.AVLdata.elements8b[i][1]);
    }
    //Xb element
    if(Data.CodecID == CODEC8EXT) {
        printf("%-30s %-10d %-20d %-20.*X\n", "     Xb Element count", ioElementSize, Data.AVLdata.elementCountXb, ioElementSize*2, Data.AVLdata.elementCountXb);
        for(int i = 0; i < Data.AVLdata.elementCountXb; i++) {
            printf("%-30s %-10d %-20d %-20.*X\n", "     ID", ioElementSize, Data.AVLdata.elementsXb[i][0], ioElementSize*2, Data.AVLdata.elementsXb[i][0]);
            printf("%-30s %-10d %-20d %-20.34X\n", "     Value", 17, Data.AVLdata.elementsXb[i][1], Data.AVLdata.elementsXb[i][1]);
        }
    }

    printf("%-30s %-10d %-20d %-20.*X\n", "   AVL Data Count", NUMBER_OF_DATA2_SIZE, Data.NumberOfData2, NUMBER_OF_DATA2_SIZE*2, Data.NumberOfData2);

    //CRC
    if(strcmp(Data.protocol, "TCP") == 0) {
        printf("%-30s %-10d %-20d %-20.*X\n", "  Crc", CRC16_SIZE, Data.CRC16, CRC16_SIZE*2, Data.CRC16);
    }
}

unsigned long getHex(int place, int length) {
    int temp[40];
    memset(temp, 0, sizeof(temp));
    strncat(temp, &inputData[place*2], length*2);
    unsigned long hex = (unsigned long)strtoul(temp, NULL, 16);
    return hex;
}

struct Codec sortToStruct() {
    struct Codec Data;
    int ioElementLength;
    int place = 0;
    int length;

    //Set protocol
    strcpy(Data.protocol, protocol);

    if(strcmp(Data.protocol, "TCP") == 0) {
        //Preamble
        length = PREAMBLE_SIZE;
        Data.preamble = 0;

        //Data Field Length
        place += length;
        length = DATA_FIELD_LENGTH_SIZE;
        Data.dataFieldLength = getHex(place, length);

        //CRC
        int placeTmp = place + Data.dataFieldLength + 4;
        length = CRC16_SIZE;
        Data.CRC16 = getHex(placeTmp, length);
    } else {
        //Length
        length = LENGTH_SIZE;
        Data.totalLength = getHex(place, length);

        //Packet ID
        place += length;
        length = PACKET_ID_SIZE;
        Data.packetID = getHex(place, length);

        //Packet Type
        place += length;
        length = PACKET_TYPE_SIZE;
        Data.packetType = getHex(place, length);

        //AVL packet ID
        place += length;
        length = AVL_PACKET_ID_SIZE;
        Data.AVLpacketID = getHex(place, length);

        //IMEI length
        place += length;
        length = IMEI_LENGTH_SIZE;
        Data.imeiLength = getHex(place, length);

        //IMEI
        place += length;
        length = Data.imeiLength;
        Data.imei = getHex(place, length);
    }

    //Codec ID
    place += length;
    length = CODEC_ID_SIZE;
    Data.CodecID = getHex(place, length);
    if(Data.CodecID == CODEC8) {
        ioElementLength = 1;
    } else {
        ioElementLength = 2;
    }

    //AVL Data Count 1
    int placeTmp = place + length;
    length = NUMBER_OF_DATA1_SIZE;
    Data.NumberOfData1 = getHex(placeTmp, length);

    //AVL Data Count 2
    if(strcmp(Data.protocol, "TCP") == 0) {
        placeTmp = place + Data.dataFieldLength - 1;
        length = NUMBER_OF_DATA2_SIZE;
        Data.NumberOfData2 = getHex(placeTmp, length);
    } else {
        placeTmp = Data.totalLength + 1;
        length = NUMBER_OF_DATA2_SIZE;
        Data.NumberOfData2 = getHex(placeTmp, length);
    }


    //AVL Data
    //Timestamp
    place += NUMBER_OF_DATA1_SIZE + 1;
    length = TIMESTAMP_SIZE;
    Data.AVLdata.timestamp = strtoul(inputData, NULL, 16);

    //Priority
    place += length;
    length = PRIORITY_SIZE;
    Data.AVLdata.priority = getHex(place, length);

    //GPS
    //Longitude
    place += length;
    length = LONGITUDE_SIZE;
    Data.AVLdata.longitude = getHex(place, length);
    //Latitude
    place += length;
    length = LATITUDE_SIZE;
    Data.AVLdata.latitude = getHex(place, length);
    //Altitude
    place += length;
    length = ALTITUDE_SIZE;
    Data.AVLdata.altitude = getHex(place, length);
    //Angle
    place += length;
    length = ANGLE_SIZE;
    Data.AVLdata.angle = getHex(place, length);
    //Satellites
    place += length;
    length = SATELLITES_SIZE;
    Data.AVLdata.satellites = getHex(place, length);
    //Speed
    place += length;
    length = SPEED_SIZE;
    Data.AVLdata.speed = getHex(place, length);

    //IO
    //Event ID
    place += length;
    length = ioElementLength;
    Data.AVLdata.enentID = getHex(place, length);
    //Element count
    place += length;
    length = ioElementLength;
    Data.AVLdata.totalElementCount = getHex(place, length);
    //1b Elements
    place += length;
    length = ioElementLength;
    Data.AVLdata.elementCount1b = getHex(place, length);
    for(int i = 0; i < Data.AVLdata.elementCount1b; i++) {
        place += length;
        length = ioElementLength;
        Data.AVLdata.elements1b[i][0] = getHex(place, length);
        place += length;
        length = 1;
        Data.AVLdata.elements1b[i][1] = getHex(place, length);
    }
    //2b Elements
    place += length;
    length = ioElementLength;
    Data.AVLdata.elementCount2b = getHex(place, length);
    for(int i = 0; i < Data.AVLdata.elementCount2b; i++) {
        place += length;
        length = ioElementLength;
        Data.AVLdata.elements2b[i][0] = getHex(place, length);
        place += length;
        length = 2;
        Data.AVLdata.elements2b[i][1] = getHex(place, length);
    }
    //4b Elements
    place += length;
    length = ioElementLength;
    Data.AVLdata.elementCount4b = getHex(place, length);
    for(int i = 0; i < Data.AVLdata.elementCount4b; i++) {
        place += length;
        length = ioElementLength;
        Data.AVLdata.elements4b[i][0] = getHex(place, length);
        place += length;
        length = 4;
        Data.AVLdata.elements4b[i][1] = getHex(place, length);
    }
    //8b Elements
    place += length;
    length = ioElementLength;
    Data.AVLdata.elementCount8b = getHex(place, length);
    for(int i = 0; i < Data.AVLdata.elementCount8b; i++) {
        place += length;
        length = ioElementLength;
        Data.AVLdata.elements8b[i][0] = getHex(place, length);
        place += length;
        length = 8;
        Data.AVLdata.elements8b[i][1] = getHex(place, length);
    }
    //Xb Elmenets
    if(Data.CodecID == CODEC8EXT) {
        place += length;
        length = ioElementLength;
        Data.AVLdata.elementCountXb = getHex(place, length);
        for(int i = 0; i < Data.AVLdata.elementCountXb; i++) {
            place += length;
            length = ioElementLength;
            Data.AVLdata.elementsXb[i][0] = getHex(place, length);
            place += length;
            length = 17;
            Data.AVLdata.elementsXb[i][1] = getHex(place, length);
        }
    }

    if(Data.NumberOfData1 != Data.NumberOfData2) {
        printf("Invalid data!\n");
    }

    return Data;
}

void setProtocol() {
    while(strcmp(protocol, "TCP") != 0 && strcmp(protocol, "UDP") != 0 ) {
        printf("Enter protocol (TCP or UDP):\n");
        scanf("%s", &protocol);

        if(strcmp(protocol, "TCP") != 0 && strcmp(protocol, "UDP") != 0 ) {
            printf("Invalid protocol selected. Available TCP or UDP protocols.\n");
        } else {
            printf("%s protocol selected\n", &protocol);
        }
    }
}

void getData() {
    printf("Enter data:\n");
    scanf("%s", &inputData);
}

int main(){
    setProtocol();
    getData();

    //Sort to Structure
    struct Codec Data = sortToStruct();

    printOutput(Data);

    return 0;
}
