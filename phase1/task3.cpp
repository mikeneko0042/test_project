#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>

using namespace std;

int W, H, N;
// 店舗のグリッド。障害物（通れない場所）を true とする
vector<vector<bool>> is_obstacle;

// 上下左右への移動を表す配列（x方向、y方向）
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

// 指定したスタート地点から、グリッド上のすべてのマスへの最短距離を計算する関数（BFS）
vector<vector<int>> get_distances(int start_x, int start_y) {
    // 距離を -1（未到達）で初期化
    vector<vector<int>> dist(W, vector<int>(H, -1));
    queue<pair<int, int>> q;

    dist[start_x][start_y] = 0;
    q.push({start_x, start_y});

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        // 上下左右の4方向を調べる
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            // グリッドの範囲内かチェック
            if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
                // 障害物がなく、まだ訪れていないマスなら進む
                if (!is_obstacle[nx][ny] && dist[nx][ny] == -1) {
                    dist[nx][ny] = dist[cx][cy] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
    return dist;
}

int main() {
    // 入出力の高速化
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // グリッドの幅W、高さH、商品数Nの読み込み
    if (!(cin >> W >> H >> N)) return 0;

    is_obstacle.assign(W, vector<bool>(H, false));

    // 1. 固定の障害物（壁）を設定する
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            // y=0 の行は、入口(1,0) と 出口(W-2,0) 以外は侵入不可 [cite: 91]
            if (y == 0 && x != 1 && x != W - 2) {
                is_obstacle[x][y] = true;
            }
            // 上部の両角 (0,H-1) と (W-1,H-1) は侵入不可 [cite: 91]
            if (y == H - 1 && (x == 0 || x == W - 1)) {
                is_obstacle[x][y] = true;
            }
        }
    }

    // 商品名から「商品を取るべき座標（ターゲット座標）」を引くためのマップ
    map<string, pair<int, int>> product_targets;

    // 2. 商品と棚の配置（障害物）を設定する
    for (int i = 0; i < N; ++i) {
        int x, y;
        string s;
        char d;
        cin >> x >> y >> s >> d;

        // 商品が置かれている棚自体は障害物になる 
        is_obstacle[x][y] = true;

        // 商品を取るための隣接座標を計算 [cite: 88]
        int tx = x, ty = y;
        if (d == 'E') tx += 1;
        else if (d == 'W') tx -= 1;
        else if (d == 'N') ty += 1;
        else if (d == 'S') ty -= 1;

        product_targets[s] = {tx, ty};
    }

    // 3. 入口と出口からの全マスの最短距離をあらかじめ計算しておく（計算量削減の工夫）
    auto dist_from_ent = get_distances(1, 0);       // 入口(1,0) からの距離 [cite: 90]
    auto dist_from_ext = get_distances(W - 2, 0);   // 出口(W-2,0) からの距離 [cite: 90]

    int Q;
    cin >> Q;

    // 4. 顧客ごとのルート距離を計算して出力
    for (int i = 0; i < Q; ++i) {
        int m;
        string p;
        cin >> m >> p; // 今回の課題では m は常に 1 [cite: 149]

        // 目的の商品のターゲット座標を取得
        auto [tx, ty] = product_targets[p];

        // 総移動距離 ＝ (入口からターゲットまでの距離) ＋ (ターゲットから出口までの距離)
        int total_dist = dist_from_ent[tx][ty] + dist_from_ext[tx][ty];
        cout << total_dist << "\n";
    }

    return 0;
}
