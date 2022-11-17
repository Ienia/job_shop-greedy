#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string.h>

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

main(){
    fstream data;
    data.open("przyklad.txt", fstream::in);
    if(data.is_open()){
        int nr_of_jobs, nr_of_machines;
        data >> nr_of_jobs >> nr_of_machines;
        int i, j;

        //initialization of dynamic array (containing our data)
        int **table = new int*[nr_of_jobs];
        for(i = 0; i < nr_of_jobs; i++)
            table[i] = new int[nr_of_machines*2];
        for(i = 0; i < nr_of_jobs; i++)
            for(j = 0; j < 2*nr_of_machines; j++)
                data >> table[i][j];
        cout<<"DANE:"<<endl;
        write_out_table_2D(nr_of_jobs, 2*nr_of_machines, table);

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

        //greedy
        int tasks_done = 0, machine, machine_compare, time, time_compare;
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
        cout<<endl<<"ROZWIAZANIE:"<<endl;
        write_out_table_2D(nr_of_jobs, nr_of_machines, solution);
        cout<<endl<<"KTORY TASK ROZPATRUJEMY:"<<endl;
        write_our_table_1D(nr_of_jobs, tasks);
        cout<<endl<<"JAK DLUGO MASZYNY BEDA ZAJETE:"<<endl;
        write_our_table_1D(nr_of_machines, times);
        cout<<endl<<"TASKS DONE ALL: "<<tasks_done<<endl;

        int previous_time, previous_time_all, machine_time;

        //next loops
        while(tasks_done < 62)
        {
            for(i = 0; i < nr_of_jobs; i++)
            {
                if(tasks[i]==9) break;
                machine = table[i][tasks[i]*2];
                time = table[i][tasks[i]*2+1];
                can_I_do_it = 1;
                for(j = 0; j < nr_of_jobs; j++)
                {
                    machine_compare = table[j][tasks[j]*2];
                    time_compare = table[j][tasks[j]*2+1];
                    if((machine == machine_compare && i!=j && time > time_compare) || tasks[j]==9)
                    {
                        can_I_do_it = 0;
                        break;
                    }
                }
                if(can_I_do_it)
                    {
                        if(tasks[i]>0)
                        {
                            cout<<endl<<"Job nr "<<i<<" "<<machine<<" "<<time<<endl;
                            machine_time = times[machine];
                            cout<<"machine time "<<machine_time<<endl;
                            previous_time = table[i][tasks[i]*2-1];
                            cout<<"previous time "<<previous_time<<endl;
                            previous_time_all = solution[i][tasks[i]-1];
                            cout<<"previous in solution "<<previous_time_all<<endl;
                            if(previous_time + previous_time_all > times[machine])
                            {
                                solution[i][tasks[i]] = previous_time_all + previous_time;
                                cout<<endl<< "wartosc przypisana (1) = "<< previous_time_all + previous_time<<endl;
                            }
                            else
                            {
                                solution[i][tasks[i]] = times[machine];
                                cout<<endl<< "wartosc przypisana (2) = "<< times[machine] <<endl;
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
            cout<<endl<<"ROZWIAZANIE:"<<endl;
            write_out_table_2D(nr_of_jobs, nr_of_machines, solution);
            cout<<endl<<"KTORY TASK ROZPATRUJEMY:"<<endl;
            write_our_table_1D(nr_of_jobs, tasks);
            cout<<endl<<"JAK DLUGO MASZYNY BEDA ZAJETE:"<<endl;
            write_our_table_1D(nr_of_machines, times);
            cout<<endl<<"TASKS DONE ALL: "<<tasks_done<<endl;
        }
    }
    else
        cout << "file is not good" << endl;
    data.close();
}