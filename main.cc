#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>

using namespace std;

void write_out_table_2D(int nr_of_rows, int nr_of_columns, int **tab)
{
    int i, j;
    for(i = 0; i < nr_of_rows; i++)
    {
        for(j = 0; j < nr_of_columns; j++)
            cout << tab[i][j] << " ";
        cout << endl;
    }
}

void write_our_table_1D(int n, int *tab)
{
    int i;
    for(i = 0; i < n; i++)
        cout<< tab[i] << endl;
}

bool can_I(int nr_of_jobs, int nr_of_machines, int *tasks, int **table, int machine, int time, int i)
{
    int j;
    for(j = 0; j < nr_of_jobs; j++)
    {
        if(tasks[j]<nr_of_machines)
        {
            int machine_compare = table[j][tasks[j]*2];
            int time_compare = table[j][tasks[j]*2+1];
            if(machine == machine_compare && i!=j && time > time_compare)
            {
                return 0;
            }
        }
    }
    return 1;
}

main()
{
    fstream data;
    data.open("przyklad.txt", fstream::in);
    if(data.is_open()){
        int max_jobs, nr_of_jobs, nr_of_machines;
        data >> max_jobs >> nr_of_machines;
        cout << "Insert number of jobs you wanna do. ";
        do
        {
            cout << "Max is " << max_jobs << endl;
            cin >> nr_of_jobs;
        } while (nr_of_jobs > max_jobs);
        
        int i, j;
        //initialization of dynamic array (containing our data)
        int **table = new int*[nr_of_jobs];
        for(i = 0; i < nr_of_jobs; i++)
            table[i] = new int[nr_of_machines*2];
        for(i = 0; i < nr_of_jobs; i++)
            for(j = 0; j < 2*nr_of_machines; j++)
                data >> table[i][j];
        //cout<<"DANE:"<<endl;
        //write_out_table_2D(nr_of_jobs, 2*nr_of_machines, table);

        //initialization of dynamic array (containing our solution)
        int **solution = new int*[nr_of_jobs];
        for(i = 0; i < nr_of_jobs; i++)
        {
            solution[i] = new int[nr_of_machines];
            for(j = 0; j < nr_of_machines; j++)
            {
                solution[i][j] = -1;
            }
        }

        //the table containing what number of task we've done in each job
        int *tasks = new int[nr_of_jobs];
        int *agg = new int[nr_of_jobs];
        for(i = 0; i < nr_of_jobs; i++)
        {
            tasks[i] = 0;
            agg[i] = 0;
        }

        //the table containing what are the times of machines being used
        int *times = new int[nr_of_machines];
        for(i = 0; i < nr_of_machines; i++)
            times[i] = 0;

        //start measuring time
        struct timeval begin, end;
        gettimeofday(&begin, 0);

        //greedy
        int tasks_done = 0, machine, time;
        bool can_I_do_it;

        //first loop
        for(i = 0; i < nr_of_jobs; i++)
        {
            can_I_do_it = 1;
            for(j = 0; j < nr_of_jobs; j++)
                if(table[i][0] == table[j][0] && i!=j && table[i][1] > table[j][1])
                {
                    can_I_do_it = 0;
                    break;
                }
            if(can_I_do_it)
            {
                solution[i][0] = 0;
                times[table[i][0]] = table[i][1];
                tasks[i]++;
                tasks_done++;
            }
        }
        for(i = 0; i < nr_of_jobs; i++)
                agg[i] = tasks[i];

        int previous_time, previous_time_all;

        //next loops
        while(tasks_done < nr_of_jobs*nr_of_machines)
        {
            for(i = 0; i < nr_of_jobs; i++)
            {
                machine = table[i][tasks[i]*2];
                time = table[i][tasks[i]*2+1];
                if(tasks[i]<nr_of_machines && can_I(nr_of_jobs, nr_of_machines, tasks, table, machine, time, i))
                {
                    if(tasks[i]>0)
                    {
                        //cout<<"machine time "<<machine_time<<endl;
                        previous_time = table[i][tasks[i]*2-1];
                        //cout<<"previous time "<<previous_time<<endl;
                        previous_time_all = solution[i][tasks[i]-1];
                        //cout<<"previous in solution "<<previous_time_all<<endl;
                        if(previous_time + previous_time_all > times[machine])
                        {
                            solution[i][tasks[i]] = previous_time_all + previous_time;
                            //cout<<endl<< "wartosc przypisana (1) = "<< previous_time_all + previous_time<<endl;
                        }
                        else
                        {
                            solution[i][tasks[i]] = times[machine];
                            //cout<<endl<< "wartosc przypisana (2) = "<< times[machine] <<endl;
                        }
                    }    
                    else 
                    solution[i][tasks[i]] = times[machine];
                    times[machine] = solution[i][tasks[i]]+ time;
                    agg[i]++;
                    tasks_done++;
                }                    
            }
            for(i = 0; i < nr_of_jobs; i++)
                tasks[i] = agg[i];
        }
        int max_time=0;
        for(i = 0; i < nr_of_machines; i++)
            if(max_time<times[i]) max_time = times[i];
        cout<<max_time<<endl;
        write_out_table_2D(nr_of_jobs, nr_of_machines, solution);

        //stop measuring time and calculate elapsed time
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds *1e-6;
        printf("Time measured: %.3f seconds. \n", elapsed);
    }
    else
        cout << "file is not good" << endl;
    data.close();
}