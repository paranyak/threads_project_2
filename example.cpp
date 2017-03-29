//
// Created by natasha on 28.03.17.
//

#include <iostream>
#include <math.h>
#include <assert.h>
#include <mutex>
#include <thread>
using namespace std;

mutex mx;
float allS = 0;
int m = 5;


float summing(float x1, float x2)
{
    float firstS = 0;
    float secondS= 0;
    float generalS;
    for (int i = 1; i <= m; ++i)
    {
        firstS += i * cos((i + 1) * x1 + 1);
        secondS += i * cos((i + 1) * x2 + 1);
    }

    generalS = -(firstS * secondS);

    cout <<"x1  " <<x1<< ", x2  " << x2 << " , sum : " <<generalS<< endl;
    return generalS;
}

void *integration (float xStart, float xEnd, float yStart, float yEnd, float stepX , float stepY){

    //початок і кінець інтегрування, крок між х і-тим та х і-1
    //дельта це stepX/Y
    //з шапочкою це 2 стартових + крок

    mx.lock();
    float sum = 0;
    while(xStart <= xEnd){
        float yS = yStart;
        while(yS <= yEnd){
            sum += summing((2 * xStart + stepX)/2, (2*yS + stepY)/2)*stepX*stepY;
            yS += stepY;
        }
        xStart += stepX;
    }
    allS += sum;
    mx.unlock();
}


int main(){
    int N = 2;                  //кількість потоків
    thread my_thread[N];        //замінити
    long id;
    //long starting = 0;
    long xS = 0;        //зчитувати з файлу   початки-кінці інтегрування
    long yS = 0;
    long xE = 10;
    long yE = 10;
    float stepX = (xE - xS)/N;      //приблизна межа для кожного інтегралу
    float stepY = (yE - yS)/N;

    //потоки
    for(id = 0; id < N; id++){
        float xEnd = xS + stepX;        //кінець для локальної межі
        //cout << stepX;
        float stepingX = stepX/1000;    //крок між і-тим та і-1-шим
        float yEnd = yS + stepY;
        float stepingY = stepY/1000;
        if (xEnd > xE){
            xEnd = xE;
        }if(yEnd > yE){
            yEnd = yE;
        }
       // cout << xEnd << xS << yEnd <<yS <<endl;
        cout << stepingX<< stepingY<<endl;
        my_thread[id] =  thread(integration, xS, xEnd, yS, yEnd, stepingX, stepingY);
        xS += stepX + stepingX;     //наступний початок - це крок між межами для потоків + крок для локальних меж
        yS += stepY + stepingY;
    }
    for(id = 0; id < N; id++){
        my_thread[id].join();
    }
    cout<< allS;
}




