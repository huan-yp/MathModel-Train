#include <gen_route.h>

using namespace std;

const int MAX_SEE = 10;

vector<Node> find_virtual_points(Plain &plain){
    int n=plain.rows(), m=plain.cols();
    vector<vector<bool>> cover(n, vector<bool>(m, 0));
    vector<Node> virtual_points;

    for(int i = 0; i < n; i++){
        int tar=-1;
        for(int j = 0; j < m; j++){
            if(cover[i][j]){
                continue;
            }
            tar = j;
            break;
        }
        if(tar == -1){
            continue;
        }

        auto calc_cover_counts = [&](int x, int y, bool cv=false){
            int cnt = 0;
            for(int expdx = - MAX_W * 2; expdx < MAX_W * 2; expdx ++)
            for(int expdy = - MAX_W * 2; expdy < MAX_W * 2; expdy ++){
                double distance = sqrt(expdx * expdx + expdy * expdy) * 0.5;
                int i=x+expdx, j=y+expdy;
                if(i < 0 || i >= n || j < 0 || j >= m){
                    continue;
                }
                if(distance > plain.a[i][j].w){
                    continue;
                }

                cnt += !cover[i][j];
                cover[i][j] = cover[i][j] | cv;
            }
            return cnt;
        };

        auto find_max_cover_point = [&](int x, int y){
            int max_cover = 0, max_cover_x = -1, max_cover_y = -1;
            for(int dx = 0; dx < MAX_W * 2; dx++)
            for(int dy = 0; dy < MAX_W * 2; dy++){
                int i=x+dx, j=y+dy;
                double distance = sqrt(dx * dx + dy * dy) * 0.5;
                if(i >= n || j >= m){
                    continue;
                }
                if(distance > plain.a[i][j].w){
                    continue;
                }
                int cnt = calc_cover_counts(i, j);
                if(cnt > max_cover){
                    max_cover = cnt;
                    max_cover_x = i;
                    max_cover_y = j;
                }
            }
            return make_pair(max_cover_x, max_cover_y);
        };

        auto [x, y] = find_max_cover_point(i, tar);
        calc_cover_counts(x, y, true);
        virtual_points.push_back(Node(x, y, 0, 0, 0));
    }
    return virtual_points;
}

vector<pair<Node, Node>> find_matchs(vector<Node> points, Plain &plain){
    int n=plain.rows(), m=plain.cols();
    vector<pair<Node, Node>> matchs;
    vector<vector<bool>> cover(n, vector<bool>(m, 0));

    for(auto &point : points){
        cover[point.x][point.y] = true;
    }

    auto get_nearest_point = [&](int x, int y){
        int min_dist = 1e9, min_x = -1, min_y = -1;
        for(int dx = 0; dx < MAX_W * 2; dx++)
        for(int dy = 0; dy < MAX_W * 2; dy++){
            int i = x + dx, j = y + dy;
            double distance = sqrt(dx * dx + dy * dy) * 0.5;
            if(i >= n || j >= m){
                continue;
            }
            if(distance > min_dist){
                continue;
            }
            min_dist = distance, min_x = i, min_y = j;
        }
        return make_pair(min_x, min_y);
    };
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(!cover[i][j]){
                continue;
            }
            auto [x, y] = get_nearest_point(i, j);
            if(x == -1 || y == -1){
                cover[i][j] = true;
                matchs.push_back(make_pair(Node(i, j), Node(i, j)));
                continue;
            }
            cover[x][y] = false, cover[i][j] = false;
            matchs.push_back(make_pair(Node(i, j), Node(x, y)));
        }
    }
}

vector<vector<bool>> find_coverage(Plain &plain, vector<Node> points){
    int n=plain.rows(), m=plain.cols();
    vector<vector<bool>> cover(n, vector<bool>(m, 0));
    for(auto point:points){
        int x = point.x, y = point.y;
        for(int expdx=-MAX_SEE; expdx<MAX_SEE; expdx++)
        for(int expdy=-MAX_SEE; expdy<MAX_SEE; expdy++){
            int i=x+expdx, j=y+expdy;
            if(i < 0 || i >= n || j < 0 || j >= m){
                continue;
            }
            double dis = sqrt(expdx * expdx + expdy * expdy) * 0.5;
            cover[i][j] = cover[i][j] | (dis + 1 < plain.a[i][j].c * 3 + 2);
        }
    }
    return cover;
}

int main(){
    string path; cin >> path; ifstream in(path);
    Plain plain; plain.read(in);

    auto result = find_virtual_points(plain);
    auto matchs = find_matchs(result, plain);

    auto avaliable_points = vector<Node>(0);
    for(auto &match : matchs){
        auto points = gen_route_access(plain, match.first, match.second);
        avaliable_points.insert(avaliable_points.end(), points.begin(), points.end());
    }

    auto coverage = find_coverage(plain, avaliable_points);

    return 0;
}