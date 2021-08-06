#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/time.h>

using namespace std;

struct Shifts {
    char id;
    int length; // lt
    vector<char> shifts_prohibited; // Rt
};
struct Employee {
    char id[2];
    vector<int> maxShift; // mit
    int maxTotalMinutes; // ci
    int minTotalMinutes; // bi
    int maxConsecutivesShift; // gi
    int minConsecutivesShift; // fi
    int minConsecutivesDays; // fi
    int maxWeekend; // ai
    vector<int> daysOff; // Ni
};

// Penalties
struct Qidt {
    char idEmployee[2];
    char idShift;
    int indexDay;
    int weight;
};
struct Pidt {
    char idEmployee[2];
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


struct Assignment {
    int indexDay;
    char shift;
};
struct Conflicts {
    int idAssign;
    vector <Assignment> conflicts;
};

// Decision Variable
struct Xidt {
    char idEmployee[2];
    vector <Assignment> horizon;
};

// Parameters
vector<struct Employee> I;
int h;
// int D;
// int W;
vector<struct Shifts> T;
vector<struct Qidt> penaltyQ;
vector<struct Pidt> penaltyP;
vector<struct SUVidt> penaltySUV;

vector<struct Xidt> varXidt;
vector<struct Conflicts> conflics;

int countConsectiveDays = 0;
int countMinutsAssigned = 0;
int countFreeConsectiveDays = 0;
int weekend = 0;
int okDay = 1;
int flag = 0;
struct Assignment lastAssign;



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
                    strcpy(employee.id, token);
                    // employee.id = *token;

                    // Get S
                    token = strtok(NULL, ",");
                    char *ee = token;
                    // employee.maxShift ="";
                    // printf("%s\n",ff);
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
                    employee.minConsecutivesDays = atoi(token);
                    token = strtok(NULL, ",");
                    employee.maxWeekend = atoi(token);
                    //printf("%d\n", employee.maxWeekend);

                    char *ff = strtok(ee, "|");
                    //ff = strtok(NULL, ",");
                    while( ff != NULL ) {
                        employee.maxShift.push_back(atoi(ff+2));
                        ff = strtok(NULL, ",");
                    }

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
                    strcpy(penalty.idEmployee, token);
                    
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
                    strcpy(penalty.idEmployee, token);
                    
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



void solve( ) {
    
    // TODO: implement F.O

    // TODO:
    // 1. add pos to Xidt
    // 0. Check days_off
    // 2. check all constrains and push conflicts
    // 3. while conflics > 0 sort conflicts and goto
    // 4. die
    // binary?

    vector<struct SUVidt> new_vec(penaltySUV.size());
    copy(penaltySUV.begin(), penaltySUV.end(), new_vec.begin());

    for( int i=0; i<I.size(); i++) {
        struct Xidt var;
        struct Employee employee = I[i];
        strcpy(var.idEmployee, employee.id);
        // var.idEmployee = employee.id;
        int countTypeShift[T.size()];

        for(int ii=0;ii<T.size();ii++) {
            countTypeShift[ii]=0;
        }


        for( int d=0; d<h; d++ ) {
            // select and set value
            struct Assignment horizon;


            // Get last T[t]

            for( int t=0; t<T.size(); t++) {
                // select and set value
                horizon.indexDay = d;
                horizon.shift = T[t].id;
                var.horizon.push_back(horizon);

                //sum params
                countMinutsAssigned += T[t].length;
                countConsectiveDays++;
                countTypeShift[t]++;

                if( d%7 == 0  || d%6 == 0) {
                    weekend++;
                }

                // checking cover
                // buscar

                int break_ = 0;


                // for( int jj=0; jj<new_vec.size(); jj++ ) {
                //     if( new_vec[jj].idShift == T[t].id && new_vec[jj].indexDay == d ) {
                //         if( new_vec[jj].requirement == 0 ) {
                //             break_ = 1;
                //             break;
                //         }
                //         new_vec[jj].requirement--;
                //     }
                // }




                

                // // First loop
                // if(flag == 0) {
                //     // soft
                //     for( int j=0; j<penaltyP.size(); j++ ){
                //         if( strcmp(penaltyP[j].idEmployee, employee.id) == 0 && penaltyP[j].idShift==T[t].id && penaltyP[j].indexDay == d ) {
                //             break_ = 1;
                //             break;
                //         }
                //     }
                // }




                if( break_ == 1) {
                    var.horizon.pop_back();
                    countConsectiveDays--;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    countFreeConsectiveDays = 0;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }

                    continue;
                }

                // CONSTRAINS

                if( okDay == 1 ){
                    var.horizon.pop_back();
                    countConsectiveDays--;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    countFreeConsectiveDays = 0;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }

                    continue;
                }
                if( countFreeConsectiveDays > 0 && countFreeConsectiveDays < employee.minConsecutivesDays ) {
                    var.horizon.pop_back();
                    countConsectiveDays = 0;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }

                    continue;
                }
                


                if( countTypeShift[t] > employee.maxShift[t] ) {
                    var.horizon.pop_back();
                    countConsectiveDays--;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }

                    continue;
                }
                // dayoff
                if( find(employee.daysOff.begin(),employee.daysOff.end(), d ) != employee.daysOff.end() ) {
                    var.horizon.pop_back();
                    countConsectiveDays--;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }
                    countFreeConsectiveDays++;

                    continue;
                }


                if( countConsectiveDays > employee.maxConsecutivesShift ) {
                    var.horizon.pop_back();
                    countConsectiveDays--;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }

                    continue;
                }

                if( countMinutsAssigned > employee.maxTotalMinutes ) {
                    var.horizon.pop_back();
                    countConsectiveDays--;
                    countMinutsAssigned -= T[t].length;
                    countTypeShift[t]--;
                    if( d%7 == 0  || d%6 == 0) {
                        weekend--;
                    }

                    continue;
                }

                struct Assignment target =  var.horizon[d-1];
                // check last day assigned
                if( target.indexDay == d-1 ){
                    char tmp = target.shift;
                    if( find( T[t].shifts_prohibited.begin(),T[t].shifts_prohibited.end(), tmp ) != T[t].shifts_prohibited.end() ) {
                        var.horizon.pop_back();
                        countConsectiveDays--;
                        countMinutsAssigned -= T[t].length;
                        countTypeShift[t]--;
                        if( d%7 == 0  || d%6 == 0) {
                            weekend--;
                        }
                        continue;
                    }
                }


                lastAssign = horizon;
                okDay = 1;
                
            }

            //next day




            // if( flag == 0 ){
            //     flag = 1;
            //     --d;
            // }
            if( okDay == 0 ){
                countFreeConsectiveDays++;
                countConsectiveDays = 0;
            }else{
                countFreeConsectiveDays = 0;
                countConsectiveDays++;
            }
            okDay = 0;

            // Verify d ok?





        }
        // save all horizon of employee i
        varXidt.push_back(var);
        countConsectiveDays = 0;
        countFreeConsectiveDays =0;
        countMinutsAssigned = 0;
        okDay =0;

    }







/*     for( int d=0; d<h; d++ ) {
        struct Xidt var;

        for( int t=0; t<T.size(); t++) {

            for( int i=0; i<I.size(); i++) {
                struct Employee employee = I[i];
                var.idEmployee = employee.id;

                struct Assignment horizon;
                horizon.indexDay = d;
                horizon.shift = T[t].id;
                var.horizon.push_back(horizon);

                // TODO; check constrains, if employee is ok to (d,t).

                if( find(employee.daysOff.begin(),employee.daysOff.end(), d ) != employee.daysOff.end() ) {
                    // next employee
                    var.pop_back();
                    continue;
                }
                int sum = 0;

                for( int kk=0; kk<varXidt.size(); kk++ ) {
                    // TODO: verify maxShift
                    if( varXidt[i].horizon[kk].shift == T[t] ){
                        sum++;
                    }
                }

                if( sum > employee.maxShift){
                    // next employee
                    var.pop_back();
                    continue;
                }



            }

        }
    }
 */





}





void printResult() {
    for( int i=0; i<varXidt.size(); i++ ) {
        string horizon = "";
        //printf("%s\n___\n", varXidt[i].idEmployee);
        for( int j=0; j<varXidt[i].horizon.size(); j++){
            horizon += "(" +to_string(varXidt[i].horizon[j].indexDay)+","+varXidt[i].horizon[j].shift+") ";
            //printf("(%d,%c)\n", varXidt[i].horizon[j].indexDay,varXidt[i].horizon[j].shift);
        }
        printf("%s: %s\n", varXidt[i].idEmployee, horizon.c_str());


    }
}




int main( int argc, char *argv[] ) {

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    if( argc > 1) {
        FILE *fp;
        fp = fopen(argv[1], "r");
        // cout<<"Iniciando lectura de la instancia...\n";
        readFile(fp);
        solve( );
        printResult();
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;
        printf("Time measured: %.8f seconds.\n", elapsed);
        
        // cout<<"Lectura de la instancia finalizada.\n";
        // cout<<"Iniciando calculos ....\n";

        fclose(fp);
    } else {
        printf("Revise los parámetros de inicialización.");
        return 1;
    }   
    return 0;

}