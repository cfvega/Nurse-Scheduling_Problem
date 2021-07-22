#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// BORRAR
// #include <sys/time.h>
//END BORRAR


using namespace std;

struct Shifts {
    char id;
    int length;
    vector<char> shifts_prohibited;
};
struct Employee {
    char id;
    char *maxShift;
    int maxTotalMinutes;
    int minTotalMinutes;
    int maxConsecutivesShift;
    int minConsecutivesShift;
    int maxWeekend;
    vector<int> daysOff;
};
struct Qidt {
    char idEmployee;
    char idShift;
    int indexDay;
    int weight;
};
struct Pidt {
    char idEmployee;
    char idShift;
    int indexDay;
    int weight;
};
struct SUVidt {
    char idShift;
    int indexDay;
    int requirement; // Sidt
    int weightFU; // Uidt
    int weightFO; // Vidt
};


// Parameters
vector<struct Employee> I;
int h;
int D;
int W;
vector<struct Shifts> T;
vector<struct Qidt> penaltyQ;
vector<struct Pidt> penaltyP;
vector<struct SUVidt> penaltySUV;




vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

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
                h = atoi(line);
                continue;
            }
            // -...
            if( strcmp(line, "SECTION_SHIFTS") == 0 ) {

                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                if( line == NULL)
                    continue;
                while( line != NULL ) {
                    if(line[0] == '#') {
                        getline(&line, &len, fp);
                        line = strtok(line, "\r\n");
                        continue;
                    }
                    // TODO: Read and analize params

                    struct Shifts shift;
                    // Get ShiftID
                    char *token = strtok(line, ",");
                    shift.id = *token;

                    // Get Length
                    token = strtok(NULL, ",");
                    shift.length = *token;

                    // Get Shifts Prohibited
                    token = strtok(NULL, ",");
                    char *ee = strtok(token, "|");
                    while( ee != NULL )  {
                        shift.shifts_prohibited.push_back(*ee);
                        ee = strtok(NULL, "|");
                    }
                    

                    // Add to T
                    T.push_back(shift);
                    
                    // next line
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");

                free(ee);
                }
                continue;
            }
            if( strcmp(line, "SECTION_STAFF") == 0 ) {
                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                if( line == NULL)
                    continue;
                while( line != NULL ) {
                    if(line[0] == '#') {
                        getline(&line, &len, fp);
                        line = strtok(line, "\r\n");
                        continue;
                    }
                    // TODO: Read and analize params

                    struct Employee employee;

                    // Get EmployeeID
                    char *token = strtok(line, ",");
                    employee.id = *token;

                    // Get S
                    token = strtok(NULL, ",");
                    employee.maxShift = token;
                    // char *ee = strtok(token, "|");
                    // while( ee != NULL )  {
                    //     printf("%\n",*ee);
                    //     ee = strtok(NULL, "|");
                    // }
                    token = strtok(NULL, ",");
                    employee.maxTotalMinutes = atoi(token);
                    token = strtok(NULL, ",");
                    employee.minTotalMinutes = atoi(token);
                    token = strtok(NULL, ",");
                    employee.maxConsecutivesShift = atoi(token);
                    token = strtok(NULL, ",");
                    employee.minConsecutivesShift = atoi(token);
                    token = strtok(NULL, ",");
                    employee.maxWeekend = atoi(token);
                    // printf("%d\n", employee.maxWeekend);
                    // Add to I
                    I.push_back(employee);
                    
                    // next line
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");
                    
                }
                continue;
            }
            if( strcmp(line, "SECTION_DAYS_OFF") == 0 ) {

                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                if( line == NULL)
                    continue;
                int i = 0;
                while( line != NULL ) {
                    if(line[0] == '#') {
                        getline(&line, &len, fp);
                        line = strtok(line, "\r\n");
                        continue;
                    }
                    // TODO: Read and analize params

                    // Get EmployeeID
                    char *token = strtok(line, ",");
                    token = strtok(NULL, ",");
                    while( token != NULL ) {
                        I[i].daysOff.push_back(atoi(token));
                        token = strtok(NULL, ",");
                    }
                    // next line
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");
                    i++;
                }
                continue;
            }

            if( strcmp(line, "SECTION_SHIFT_ON_REQUESTS") == 0 ) {

                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                if( line == NULL)
                    continue;
                while( line != NULL ) {
                    if(line[0] == '#') {
                        getline(&line, &len, fp);
                        line = strtok(line, "\r\n");
                        continue;
                    }
                    // TODO: Read and analize params

                    struct Qidt penalty;
                    
                    // get EmployeeID
                    char *token = strtok(line, ",");
                    penalty.idEmployee = *token;
                    
                    // get Day
                    token = strtok(NULL, ",");
                    penalty.indexDay = atoi(token);

                    // get ShiftID
                    token = strtok(NULL, ",");
                    penalty.idShift = *token;

                    // get weight
                    token = strtok(NULL, ",");
                    penalty.weight = atoi(token);


                    penaltyQ.push_back(penalty);
                    
                    // next line
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");
                }
                continue;
            }

            if( strcmp(line, "SECTION_SHIFT_OFF_REQUESTS") == 0 ) {

                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                if( line == NULL)
                    continue;
                while( line != NULL ) {
                    if(line[0] == '#') {
                        getline(&line, &len, fp);
                        line = strtok(line, "\r\n");
                        continue;
                    }
                    // TODO: Read and analize params

                   struct Pidt penalty;
                    
                    // get EmployeeID
                    char *token = strtok(line, ",");
                    penalty.idEmployee = *token;
                    
                    // get Day
                    token = strtok(NULL, ",");
                    penalty.indexDay = atoi(token);

                    // get ShiftID
                    token = strtok(NULL, ",");
                    penalty.idShift = *token;

                    // get weight
                    token = strtok(NULL, ",");
                    penalty.weight = atoi(token);

                    penaltyP.push_back(penalty);
                    
                    // next line
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");

                }
                continue;
            }

            if( strcmp(line, "SECTION_COVER") == 0 ) {

                getline(&line, &len, fp);
                line = strtok(line, "\r\n");
                if( line == NULL)
                    continue;
                while( line != NULL ) {
                    if(line[0] == '#') {
                        getline(&line, &len, fp);
                        line = strtok(line, "\r\n");
                        continue;
                    }
                    // TODO: Read and analize params

                    struct SUVidt penalty;
                    
                    // get Day
                    char *token = strtok(line, ",");
                    penalty.indexDay = atoi(token);

                    // get ShiftID
                    token = strtok(NULL, ",");
                    penalty.idShift = *token;

                    // get EmployeeID
                    token = strtok(NULL, ",");
                    penalty.requirement = atoi(token);

                    // get weight
                    token = strtok(NULL, ",");
                    penalty.weightFU = atoi(token);

                    // get weight
                    token = strtok(NULL, ",");
                    penalty.weightFO = atoi(token);

                    // Add to SUV
                    penaltySUV.push_back(penalty);
                    
                    // next line
                    getline(&line, &len, fp);
                    line = strtok(line, "\r\n");

                }
                continue;
            }
        }
    }
    
    if(line)
        free(line);
}


int main( int argc, char *argv[] ) {

    // BORRAR
    // struct timeval begin, end;
    // gettimeofday(&begin, 0);
    //END BORRAR

    if( argc == 3) {
        FILE *fp;
        fp = fopen(argv[1], "r");
        cout<<"Iniciando lectura de la instancia...\n";
        readFile(fp);
        
        // gettimeofday(&end, 0);
        // long seconds = end.tv_sec - begin.tv_sec;
        // long microseconds = end.tv_usec - begin.tv_usec;
        // double elapsed = seconds + microseconds*1e-6;
        // printf("Time measured: %.3f seconds.\n", elapsed);
        
        cout<<"Lectura de la instancia finalizada.\n";
        fclose(fp);

    } else {
        printf("Revise los parámetros de inicialización.");
        return 1;
    }   
    return 0;

}