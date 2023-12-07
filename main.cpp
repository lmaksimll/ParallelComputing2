#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>
#include <math.h>
#include <atomic>
#include <mutex>

using namespace std;

void array_initialization(int size, vector<int>& number_mass)
{
//    random_device rd();                           // non-deterministic generator
    mt19937 gen(13);                                // to seed mersenne twister.
    uniform_int_distribution<> dist(0,50000000);    // distribute results between 1 and 6 inclusive.

    for(int i=0;i < size;i++)
    {
        number_mass[i] = dist(gen);
    }
}

void array_search(int size, vector<int>& number_mass, int start, int end, int& unique_elements)
{
    bool check = false;

    if(end > size)
    {
        end = size;
    }

    for (int j = start; j < end; j++)
    {
        check = false;

        //Если элемент первый в отрезке
        if(j == start)
        {
            //Если элемент первый в отрезке и списке
            if(j == 0)
            {
                if(number_mass[j] == number_mass[j+1])
                {
                    check = true;
                }
            }
            else{
                if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
        }

        //Если элемент последний в отрезке
        else if(j == end-1)
        {
            //Если элемент последний в отрезке и списке
            if(j == size)
            {
                if(number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
            else{
                if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
        }

        //Если элемент посередине
        else
        {
            if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
            {
                check = true;
            }
        }

        if(!check)
        {
            unique_elements++;
        }
    }
}

void array_search_mutex(int size, vector<int>& number_mass, int start, int end, int& unique_elements,mutex& unique_mutex)
{
    bool check = false;

    if(end > size)
    {
        end = size;
    }

    for (int j = start; j < end; j++)
    {
        check = false;

        //Если элемент первый в отрезке
        if(j == start)
        {
            //Если элемент первый в отрезке и списке
            if(j == 0)
            {
                if(number_mass[j] == number_mass[j+1])
                {
                    check = true;
                }
            }
            else{
                if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
        }

        //Если элемент последний в отрезке
        else if(j == end-1)
        {
            //Если элемент последний в отрезке и списке
            if(j == size)
            {
                if(number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
            else{
                if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
        }

        //Если элемент посередине
        else
        {
            if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
            {
                check = true;
            }
        }

        if(!check)
        {
            unique_mutex.lock();
                unique_elements++;
            unique_mutex.unlock();
        }
    }
}

void array_search_atomic(int size, vector<int>& number_mass, int start, int end, atomic<int>& unique_elements)
{
    bool check = false;

    if(end > size)
    {
        end = size;
    }

    for (int j = start; j < end; j++)
    {
        check = false;

        //Если элемент первый в отрезке
        if(j == start)
        {
            //Если элемент первый в отрезке и списке
            if(j == 0)
            {
                if(number_mass[j] == number_mass[j+1])
                {
                    check = true;
                }
            }
            else{
                if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
        }

        //Если элемент последний в отрезке
        else if(j == end-1)
        {
            //Если элемент последний в отрезке и списке
            if(j == size)
            {
                if(number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
            else{
                if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
                {
                    check = true;
                }
            }
        }

        //Если элемент посередине
        else
        {
            if(number_mass[j] == number_mass[j+1] || number_mass[j] == number_mass[j-1])
            {
                check = true;
            }
        }

        if(!check)
        {
            unique_elements++;
        }
    }
}

void res_main_individual(int count, int size, vector<int>& number_mass)
{
    printf_s("Work on %d thread\n",count);

    //Время начала работы
    clock_t tStart = clock();

    int res = 0;
    int unique_elements[count];
    vector <thread> thread_vec;

    if(count > 0 && count < 20)
    {
        //int step = ceil((float)size / (float)count);
        int step = (size+count-1) / count;
        int start = 0;
        int end = step;

        for (int j = 0; j < count; ++j) {
           unique_elements[j] = 0;
        }

        //Создание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.push_back(thread(array_search,size,ref(number_mass),start,end,ref(unique_elements[i])));

            //array_search(size,(number_mass),start,end,(unique_elements[i]));

            start += step;
            end += step;
        }

        //Ожидание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.at(i).join();
        }

        //Сумирование результата
        for (int i = 0; i < count; ++i) {
            res +=unique_elements[i];
        }
    }

    //Вывод результата
    printf_s("---Individual variables---\n");
    printf_s("Number of unique elements: %d\n",res);
    printf_s("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void res_main_general(int count, int size, vector<int>& number_mass)
{
    //Время начала работы
    clock_t tStart = clock();

    int unique_elements = 0;
    vector <thread> thread_vec;

    if(count > 0 && count < 20)
    {
        //int step = ceil((float)size / (float)count);
        int step = (size+count-1) / count;
        int start = 0;
        int end = step;

        //Создание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.push_back(thread(array_search,size,ref(number_mass),start,end,ref(unique_elements)));

            //array_search(size,(number_mass),start,end,(unique_elements[i]));

            start += step;
            end += step;
        }

        //Ожидание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.at(i).join();
        }
    }

    //Вывод результата
    printf_s("---General variables---\n");
    printf_s("Number of unique elements: %d\n",unique_elements);
    printf_s("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void res_main_atomic(int count, int size, vector<int>& number_mass)
{
    //Время начала работы
    clock_t tStart = clock();

    atomic<int> unique_elements = 0;
    vector <thread> thread_vec;

    if(count > 0 && count < 20)
    {
        //int step = ceil((float)size / (float)count);
        int step = (size+count-1) / count;
        int start = 0;
        int end = step;

        //Создание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.push_back(thread(array_search_atomic,size,ref(number_mass),start,end,ref(unique_elements)));

            //array_search(size,(number_mass),start,end,(unique_elements[i]));

            start += step;
            end += step;
        }

        //Ожидание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.at(i).join();
        }
    }

    //Вывод результата
    printf_s("---Using atomic---\n");
    printf_s("Number of unique elements: %d\n",(int)unique_elements);
    printf_s("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void res_main_mutex(int count, int size, vector<int>& number_mass)
{
    //Время начала работы
    clock_t tStart = clock();

    int unique_elements = 0;
    mutex unique_mutex;
    vector <thread> thread_vec;

    if(count > 0 && count < 20)
    {
        //int step = ceil((float)size / (float)count);
        int step = (size+count-1) / count;
        int start = 0;
        int end = step;

        //Создание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.push_back(thread(array_search_mutex,size,ref(number_mass),start,end,ref(unique_elements),ref(unique_mutex)));

            //array_search(size,(number_mass),start,end,(unique_elements[i]));

            start += step;
            end += step;
        }

        //Ожидание потоков
        for(int i=0;i < count;i++)
        {
            thread_vec.at(i).join();
        }
    }

    //Вывод результата
    printf_s("---Using mutex---\n");
    printf_s("Number of unique elements: %d\n",unique_elements);
    printf_s("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf_s("-----------------------------------\n");
}

int main()
{
    //Инициализация массива
    int size = 200000000;
    vector<int>number_mass(size);
    array_initialization(size, number_mass);

    //Сортировка массива
    sort(number_mass.begin(),number_mass.end());

    //Вызов потоков
    for (int count = 1; count < 16; ++count)
    {
        res_main_individual(count,size,number_mass);
        res_main_general(count,size,number_mass);
        res_main_atomic(count,size,number_mass);
        res_main_mutex(count,size,number_mass);
    }

    return 0;
}
