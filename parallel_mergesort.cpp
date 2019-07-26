#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<pthread.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <typeinfo>

const int TESTNUM=1;
const int ARRAYNUM=1000000;
const int THREADNUM=4;

void print_vector(const std::vector<int> &array){
    int size=array.size();
    std::cout<<"print_vector:";
    for (int i = 0; i <size; i++)
    std::cout<<array[i]<<" ";
    std::cout<<"\n";
}
void print_vectorse(const std::vector<int> &array,int s,int e){
    std::cout<<"print_vector:";
    for (int i = s; i <= e; i++)
    std::cout<<array[i]<<" ";
    std::cout<<"\n";
}

void print_vector_vector(const std::vector<std::vector<int> > &array)
{
    int size=array.size();
    std::cout<<"print_v_v\n";
    for (int i = 0; i < size; ++i)
    {
        print_vector(array[i]);
    }
    std::cout<<"print_v_v end\n";
}

std::vector<int> merge(const std::vector<int> &array1,const std::vector<int>&array2)
{
    std::vector<int>result;
    int i,j=0,k=0;
    int len1=array1.size(),len2=array2.size();
    for (i=0; i<len1+len2; i++)
    {
        if (len2<=k||(j<len1&&array1[j]<array2[k]))
        {
            result.push_back(array1[j]);j++;
        }
        else{
            result.push_back(array2[k]);k++;
        }
    }
    return result;
}

void single_mergesort(std::vector<int> &array,int start,int end)
{

    if (start<end)
    {
        int q=(start+end)/2;
        single_mergesort(array, start, q);
        single_mergesort(array, q+1, end);
        std::vector<int> s,e,m;
        for (int i = start; i <= q; ++i) s.push_back(array[i]);
        for (int i=q+1;i <= end ; ++i)   e.push_back(array[i]);
        m=merge(s,e);
        for (int i = start; i <= end; ++i)array[i]=m[i-start];
    }
}
class param
{
    public:
    std::vector<int> *array;
    int start,end;
    param(std::vector<int> *a,int s,int e){
        array=a;
        start=s;
        end=e;
    }
    
};
void *_parallel_mergesort_2t(void* p)
{
    param t=*(param *)p;
    single_mergesort(*t.array,t.start,t.end);
    return NULL;
}

void parallel_mergesort_2t(std::vector<int> &array,int start,int end)
{
    int q=(start+end)/2;
    param lp(&array,start,q);
    param rp(&array,q+1,end);
    pthread_t lside,rside;
    pthread_create(&lside, NULL, _parallel_mergesort_2t, (void*)&lp);
    pthread_create(&rside, NULL, _parallel_mergesort_2t, (void*)&rp);
    pthread_join(lside, NULL);
    pthread_join(rside, NULL);
    std::vector<int> s,e,m;
    for (int i = start; i <= q; ++i) s.push_back(array[i]);
    for (int i=q+1;i <= end ; ++i)   e.push_back(array[i]);
    m=merge(s,e);
    for (int i = start; i <= end; ++i)array[i]=m[i-start];
    
}
void *_parallel_mergesort_4t(void *p)
{
    param t=*(param *)p;
    parallel_mergesort_2t(*t.array,t.start, t.end);
    return NULL;
}
void parallel_mergesort_4t(std::vector<int> &array,int start,int end)
{
    if (end-start<8)
    {
        parallel_mergesort_2t(array,start,end);
    }
    else
    {
        int q=(start+end)/2;
        param lp(&array,start,q);
        param rp(&array,q+1,end);
        pthread_t lside,rside;
        pthread_create(&lside, NULL, _parallel_mergesort_4t, (void*)&lp);
        pthread_create(&rside, NULL, _parallel_mergesort_4t, (void*)&rp);
        pthread_join(lside, NULL);
        pthread_join(rside, NULL);
        std::vector<int> s,e,m;
        for (int i = start; i <= q; ++i) s.push_back(array[i]);
        for (int i=q+1;i <= end ; ++i)   e.push_back(array[i]);
        m=merge(s,e);
        for (int i = start; i <= end; ++i)array[i]=m[i-start];
    }
}
void *_parallel_mergesort_8t(void *p)
{
    param t=*(param *)p;
    parallel_mergesort_4t(*t.array,t.start, t.end);
    return NULL;
}
void parallel_mergesort_8t(std::vector<int> &array,int start,int end)
{
    if (end-start<16)
    {
        parallel_mergesort_2t(array,start,end);
    }
    else
    {
        int q=(start+end)/2;
        param lp(&array,start,q);
        param rp(&array,q+1,end);
        pthread_t lside,rside;
        pthread_create(&lside, NULL, _parallel_mergesort_8t, (void*)&lp);
        pthread_create(&rside, NULL, _parallel_mergesort_8t, (void*)&rp);
        pthread_join(lside, NULL);
        pthread_join(rside, NULL);
        std::vector<int> s,e,m;
        for (int i = start; i <= q; ++i) s.push_back(array[i]);
        for (int i=q+1;i <= end ; ++i)   e.push_back(array[i]);
        m=merge(s,e);
        for (int i = start; i <= end; ++i)array[i]=m[i-start];
    }
}
double gettimeofday_sec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

bool check(std::vector<int>array){
    int t=array[0];
    for (int j = 1; j < ARRAYNUM; ++j)
    {
        if (array[j]<t)
        {
            return false;
        }
        t=array[j];
    }
    return true;
}

int main(int argc, char *argv[])
{
    std::vector<std::vector<int> > array1(TESTNUM,std::vector<int>(ARRAYNUM)),array2;
    srand((unsigned int) time(NULL));
    for (int i = 0; i < TESTNUM; ++i)
    for (int j = 0; j < ARRAYNUM; j++)
    array1[i][j]=rand()%TESTNUM;
    
    double t1, t2;
    array2=array1;
    t1 = gettimeofday_sec();
    
    for (int i = 0; i < TESTNUM; ++i)
    {
        single_mergesort(array2[i],0,ARRAYNUM-1);
        if (!check(array2[i])){
            std::cout<<"error! (single thread)\n";
            exit(1);
        }
    }
    t2 = gettimeofday_sec();
    printf("mergesort\t%f\n", t2 - t1);
    
    array2=array1;
    t1 = gettimeofday_sec();
    for (int i = 0; i < TESTNUM; ++i)
    {
        parallel_mergesort_2t(array2[i],0,ARRAYNUM-1);
        if (!check(array2[i])){
            std::cout<<"error! (parallel)\n";
            exit(1);
        }
    }
    t2 = gettimeofday_sec();
    printf("mergesort 2t\t%f\n", t2 - t1);
    
    array2=array1;
    t1 = gettimeofday_sec();
    for (int i = 0; i < TESTNUM; ++i)
    {
        parallel_mergesort_4t(array2[i],0,ARRAYNUM-1);
        if (!check(array2[i])){
            std::cout<<"error! (parallel)\n";
            exit(1);
        }
    }
    t2 = gettimeofday_sec();
    printf("mergesort 4t\t%f\n", t2 - t1);
    array2=array1;
    t1 = gettimeofday_sec();
    for (int i = 0; i < TESTNUM; ++i)
    {
        parallel_mergesort_8t(array2[i],0,ARRAYNUM-1);
        if (!check(array2[i])){
            std::cout<<"error! (parallel)\n";
            exit(1);
        }
    }
    t2 = gettimeofday_sec();
    printf("mergesort 8t\t%f\n", t2 - t1);
    return 0;
}
