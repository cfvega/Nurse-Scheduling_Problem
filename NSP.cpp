#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Parameters
int I;
int h;
int D;
int W;

struct Shifts {
    char id;
    int length;
    vector<char> shifts_prohibited;
};

vector<struct Shifts> T;


void readFile( FILE *fp ) {
    char *line = NULL;
    size_t len = 0;

    if (fp == NULL)
        exit(EXIT_FAILURE);

    while( getline(&line, &len, fp) != -1 ) {
        line = strtok(line, "\r\n");
        if( line == NULL)
            continue;
        if( line[0] != '#' ) {
            // OK
            if( strcmp(line, "SECTION_HORIZON") == 0 ) {
                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                while( line != NULL && line[0] == '#' ) {
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");
                }
                I = atoi(line);
                continue;
            }
            // -...
            // if( strcmp(line, "SECTION_SHIFTS") == 0 ) {
            //     int nturn = 0;

            //     getline(&line, &len, fp);
            //     line = strtok(line, "\r\n");
            //     if( line == NULL)
            //         continue;
            //     while( line != NULL ) {
            //         if(line[0] == '#') {
            //             getline(&line, &len, fp);
            //             line = strtok(line, "\r\n");
            //             continue;
            //         }
            //         // TODO: Read and analize params

            //         struct Shifts shift;

            //         // Get ShiftID
            //         char *token = strtok(line, ",");
            //         shift.id = *token;
            //         // strcpy( shift.id, token);

            //         // Get Length
            //         token = strtok(NULL, ",");
            //         printf("%s\n",token);
            //         // strcpy( shift.length, token);
            //         shift.length = *token;

            //         // Get Shifts Prohibited
            //         token = strtok(NULL, ",");
            //         printf("%s\n",token);

            //         T[nturn] = shift;
            //         nturn++;



            //         // next line
            //         getline(&line, &len, fp);
            //         line = strtok(line, "\r\n");

            //     }
            //     printf("--> %c\n", T[3].id);
            //     continue;
            // }


        }
    }
    
    if(line)
        free(line);
}


int main( int argc, char *argv[] ) {

    if( argc == 3) {
        FILE *fp;
        fp = fopen(argv[1], "r");
        readFile(fp);
        fclose(fp);


    } else {
        printf("Revise los parámetros de inicialización.");
        return 1;
    }   
    return 0;

}