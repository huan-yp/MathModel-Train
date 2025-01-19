#include "gen_route.h"
#include "gen_route.cpp"
int main()
{
    Plain plain;
    auto in=ifstream("data.in");
    plain.read(in);
    Node start = plain.a[0][0];
    Node end = plain.a[2][2];
    vector<Node> vec;
    vec = gen_route_access(plain, start, end);
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i].x << " " << vec[i].y << endl;
    }
}