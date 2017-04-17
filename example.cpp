//
// Created by natasha on 28.03.17.
//

#include <iostream>
#include <math.h>
#include <assert.h>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>
#include "timing.cpp"

using namespace std;

mutex mx;
double allS = 0;
int m = 5;


double summing(double x1, double x2)
{
    double firstS = 0;
    double secondS= 0;
    double generalS = 0;
    for (int i = 1; i <= m; ++i)
    {

        firstS += i * cos((i + 1) * x1 + 1);
        secondS += i * cos((i + 1) * x2 + 1);
    }
    generalS += -(firstS) * (secondS);
    //cout <<"x1  " <<x1<< ", x2  " << x2 << " , sum : " <<generalS<< "\n" <<endl;
    return generalS;
}

void *integration (double xStart, double xEnd, double yStart, double yEnd, double stepX , double stepY){

    //початок і кінець інтегрування, крок між х і-тим та х і-1
    //дельта це stepX/Y
    //з шапочкою це 2 стартових + крок



    double sum = 0;
    while(xStart <= xEnd){
        double yS = yStart;
        while(yS <= yEnd){
            sum += summing((2 * xStart + stepX)/2, (2*yS + stepY)/2 )*stepX*stepY;
            //cout << xStart << "<-- XXXXXXX  " << yS << "<-- YYYYYYYY"<< endl;
            yS += stepY;
        }
        xStart += stepX;
    }
    mx.lock();
    allS += sum;
    mx.unlock();
}


int main(){
    auto stage1_start_time = get_current_time_fenced();

    ifstream myfile;
    vector<string> words;
    string word;


    myfile.open("read.txt");    //файл з усма конфігураціями
    if (!myfile.is_open()) {
        cerr << "Error" << endl;
    }
    while (myfile >> word) {        //додаємо рядок без =
        words.push_back(word);
    }

    long xS, yS, xE, yE;
    int N = 2;
    double abs, rel;
    for (int i = 0; i < words.size(); ++i) {
        //cout << words[i] << ' ';
        if (words[i].find("xStart=") != std::string::npos) {
            size_t pos = words[i].find("xStart=");
             xS  = atoi(words[i].substr(pos + 7).c_str());
        }
        if (words[i].find("yStart=") != std::string::npos) {
            size_t pos = words[i].find("yStart=");
            yS  = atoi(words[i].substr(pos + 7).c_str());
        }
        if (words[i].find("xEnd=") != std::string::npos) {
            size_t pos = words[i].find("xEnd=");
            xE = atoi(words[i].substr(pos + 5).c_str());
        }
        if (words[i].find("yEnd=") != std::string::npos) {
            size_t pos = words[i].find("yEnd=");
            yE = atoi(words[i].substr(pos + 5).c_str());
        }
       if (words[i].find("m=") != std::string::npos) {
            size_t pos = words[i].find("m=");
            m = atoi(words[i].substr(pos + 2).c_str());
        }
        if (words[i].find("threads=") != std::string::npos) {
            size_t pos = words[i].find("threads=");
            N = atoi(words[i].substr(pos + 8).c_str());
        }
        if (words[i].find("absolute=") != std::string::npos) {
            size_t pos = words[i].find("absolute=");
            abs = atoi(words[i].substr(pos + 9).c_str());
        }
        if (words[i].find("relative=") != std::string::npos) {
            size_t pos = words[i].find("relative=");
            rel = atoi(words[i].substr(pos + 9).c_str());
        }



    }

    thread my_thread[N];
    long id;
    double stepX = (xE - xS)/N;      // межа для кожного інтегралу
    double stepY = (yE - yS)/N;

    //потоки
    for(id = 0; id < N; id++){
        double stepingX = 1/(stepX*1000);    //крок між і-тим та і-1-шим
        double xEnd = xS +stepX*(id+1) + stepingX*id;        //кінець для локальної межі
        //cout<< "!!!!!!! step" << stepingX;
        double stepingY = 1/(stepY*1000);
        double yEnd = yS +stepY*(id+1) + stepingY*id;
        if (xEnd > xE){
            xEnd = xE;
        }if(yEnd > yE){
            yEnd = yE;
        }
        my_thread[id] =  thread(integration, xS + stepX*id + stepingX*id, xEnd, yS +stepY*(id) + stepingY*id, yEnd, stepingX, stepingY);

    }
    for (auto& th : my_thread) th.join();
    auto finish_time = get_current_time_fenced();
    auto all_time = finish_time - stage1_start_time;
    cout << "ALL time: " << to_us(all_time) << endl;

    fstream log;
    log.open("result.txt", fstream::app);
    std::chrono::duration<double, std::milli> all_ms = all_time;
    log << "Result: " << allS<< " ";
    log << all_ms.count();
    log <<"\n";
    log.close();
    cout<< "Result : "<<allS << "\n";
    cout << "Absolute and relative values (observational error): " << abs << " " << rel << "\n";
    if((abs/(allS + abs)) <= rel || (abs/(allS - abs)) <= rel){
        cout << "All done right) integral is without observational error";
    }
}




