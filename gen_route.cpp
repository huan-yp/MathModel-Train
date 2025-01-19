#include "gen_route.h"

double spiral_length(double a, double b, double alpha, double beta){
    auto F = [&](double theta) { 
        return (b * b * theta + a * b) / (2 * b * b) * sqrt(pow((b * theta + a), 2) + b * b) + \
            b / 2 * log(b * theta + a + sqrt(pow((b * theta + a), 2) + b * b));
    };
    return F(beta) - F(alpha);
}

vector<int> FindCircle(float a1, float b1, float a2, float b2, int n1, int n2);

vector<Node> gen_route_access(const Plain &plain, Node start, Node end)
{
    vector<Node> vec; 
    vec.clear();
    double pi = 3.1415926;
    // 此处是算两个螺线的极坐标表示
    auto [startx, starty] = start.center();
    auto [endx, endy] = end.center();
    float a1 = 0, a2 = sqrt((pow(startx - endx, 2) + pow(starty - endy, 2)));
    int rowsize = plain.rows(), colsize = plain.cols();
    //float b1 = start.c / (2 * pi);
    //float b2 = end.c / (2 * pi);
    float b1 = 1 / (2 * pi);
    float b2 = b1;
    // b1: pho = a + b * theta
    
    // 对于螺线找合适的结束点，第一个螺线结束点为（x1，y1），第二个螺线结束点为（x2，y2）
    float l1 = 0;
    int n1 = 0;
    while (l1 <= 15)
    {
        float beta1 = 2 * pi * (n1 + 1); 
        float fi1 = atan2((a1 + b1 * beta1) / b1, b1);
        l1 = 0.5 * (tan(fi1) / cos(fi1) + log(abs(tan(fi1) + 1 / cos(fi1))));
        n1++;
    }
    // 算出目前已经经过的距离
    float beta1 = 2 * pi * (n1);
    float fi1 = atan2((a1 + b1 * beta1) / b1, b1);
    l1 = 0.5 * (tan(fi1) / cos(fi1) + log(abs(tan(fi1) + 1 / cos(fi1))));
    float l2 = 0;
    int n2 = 0;
    while (l2 <= 15)
    {
        float beta2 = 2 * pi * (n2 + 1) + pi;
        float fi2 = atan2((a2 + b2 * beta2) / b2, b2);
        l2 = 0.5 * (tan(fi2) / cos(fi2) + log(abs(tan(fi2) + 1 / cos(fi2))));
        n2++;
    }
    float beta2 = 2 * pi * (n2) + pi;
    float fi2 = atan2((a2 + b2 * beta2) / b2, b2);
    l2 = 0.5 * (tan(fi2) / cos(fi2) + log(abs(tan(fi2) + 1 / cos(fi2))));
    // 根据两点之间的距离判断到底可否联通：如果一圈联通不了可以少一圈联通（迭代）
    float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    float alpha1 = 0, alpha2;
    if (a2 > 30){
        return vec;
    }
    else if(a2==30){
        n1 = 0, n2 = 0;
        alpha1 = 0, alpha2 = 0;
    }
    else{
        vector<int> vec1;
        vec1 = FindCircle(a1, b1, a2, b2, n1, n2);
        if (vec1.size() == 0){
            n1 = 0, n2 = 0;
        }
        n1 = vec1[0], n2 = vec1[1];
        alpha1 = 2 * pi * (n1);
        if (n2 == 0){
            alpha2 = 0;
        }
        else{
            alpha2 = 2 * pi * n2 + pi;
        }
    }
    // 根据两点之间的距离判断如何取曲线对应的点
    // 先判断螺线上的点
    for(double i = 0 ; i <= alpha1 ; i=i+0.01)
    {
        //极坐标转原坐标
        float x = (a1 + b1 * i) * cos(i);
        x1 = x + startx;
        float y = (a1 + b1 * i) * sin(i);
        y1 = y + starty;
        //对应plain上的点
        int n = (x1 - 0.25) / 0.5;
        int m = (y1 - 0.25) / 0.5;
        if(n <= rowsize && m <= colsize)
        {
            vec.push_back(plain.a[n][m]);
        }
    } 
    for(double i = 0 ; i <= alpha2 ; i = i + 0.01){
        // 极坐标转原坐标
        float x = (a2 + b2 * i) * cos(i);
        x1 = x + endx;
        float y = (a2 + b2 * i) * sin(i);
        y1 = y + endy;
        // 对应plain上的点
        int n = (x1 - 0.25) / 0.5;
        int m = (y1 - 0.25) / 0.5;
        if(n <= rowsize && m <= colsize){
            vec.push_back(plain.a[n][m]);
        }   
    }
    // 判断非螺线上的点

    x1 = (a1 + b1 * alpha1) * cos(alpha1)+startx;
    beta1 = 2 * pi * (n1);
    fi1 = atan2((a1 + b1 * beta1) / b1, b1);
    l1 = 0.5 * (tan(fi1) / cos(fi1) + log(abs(tan(fi1) + 1 / cos(fi1))));
    y1 = starty;

    x2 = (a2 + b2 * alpha2) * cos(alpha2)+endx;
    beta2 = 2 * pi * (n2 + 1) + pi;
    fi2 = atan2((a2 + b2 * beta2) / b2, b2);
    l2 = 0.5 * (tan(fi2) / cos(fi2) + log(abs(tan(fi2) + 1 / cos(fi2))));
    y2 = endy;
    float d = 30 - l1 - l2;
    float r = (x2 - x1)/2/pi;
    if (d >= 2*pi*r){
        for(double theta = 0; theta<=2*pi; theta = theta + 0.1){
            float x = x1 + r*(theta-sin(theta));
            float y = y + r * (1-cos(theta));
            int n = (x1 - 0.25) / 0.5;
            int m = (y1 - 0.25) / 0.5;
            if(n <= rowsize && m <= colsize){
                vec.push_back(plain.a[n][m]);
            }
        }
    }
    else{
        for(double i = 0; i<=10; i = i + 0.2){
            float x = x1 + i * (x2-x1)/10; 
            float y = y1 + i * (y2-y1)/10;
            int n = (x1 - 0.25) / 0.5;
            int m = (y1 - 0.25) / 0.5;
            if(n <= rowsize && m <= colsize){
                vec.push_back(plain.a[n][m]);
            }
        }
    }
    // 进行查重去除
    return vec;
}
// 这个函数用来迭代求解应该转几圈
vector<int> FindCircle(float a1, float b1, float a2, float b2, int n1, int n2)
{
    double pi = 3.1415926;
    vector<int> vec1;
    vec1.clear();
    float r1 = 2 * pi * (n1) * b1 + a1;
    float x1 = r1 * cos(2 * pi * (n1));
    float beta1 = 2 * pi * (n1);
    float fi1 = atan2((a1 + b1 * beta1) / b1, b1);
    float l1 = 0.5 * (tan(fi1) / cos(fi1) + log(abs(tan(fi1) + 1 / cos(fi1))));

    float r2 =(2 * pi * n2 + pi) * b2 + a2;
    float x2 = r2 * cos(2 * pi * (n2) + pi);
    float beta2 = 2 * pi * (n2 + 1) + pi;
    float fi2 = atan2((a2 + b2 * beta2) / b2, b2);
    float l2 = 0.5 * (tan(fi2) / cos(fi2) + log(abs(tan(fi2) + 1 / cos(fi2))));

    float d = x2 - x1;
    float l = 30 - l2 - l1;
    if (d > l)
    {
        vec1.push_back(n1);
        vec1.push_back(n2);
        return vec1;
    }
    else if(n1==0&&n2==0){
        return vec1;
    }
    else{
        if ((rand() % 2 == 0 || n2 == 0) && n1 != 0){
            n1--;
            vec1=FindCircle(a1, b1, a2, b2, n1, n2);
            return vec1;
        }
        else
        {
            n2--;
            vec1=FindCircle(a1, b1, a2, b2, n1, n2);
            return vec1;
        }
    }  
}