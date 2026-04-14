#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// 2つの商品のペアを扱いやすくするために型を定義
typedef pair<string, string> ProductPair;

// 比較関数：第一条件を購入回数の降順、第二条件をペアの辞書順（昇順）とする
bool comparePairs(const pair<ProductPair, int>& a, const pair<ProductPair, int>& b) {
    // 1. 購入回数が異なる場合は、回数の多い順（降順）
    if (a.second != b.second) {
        return a.second > b.second;
    }
    // 2. 購入回数が同じ場合、ペアの1つ目の商品名で比較（辞書順）
    if (a.first.first != b.first.first) {
        return a.first.first < b.first.first;
    }
    // 3. 1つ目の商品名も同じ場合、ペアの2つ目の商品名で比較（辞書順）
    return a.first.second < b.first.second;
}

int main() {
    // 入出力の高速化
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // 顧客数Nの読み込み
    if (!(cin >> n)) return 0;

    // 「商品のペア」をキー、「同時購入回数」を値とするマップ
    map<ProductPair, int> pair_counts;

    // 1. 購入履歴の読み込みとペアの集計
    for (int i = 0; i < n; ++i) {
        int m;
        cin >> m;
        
        // 1人の顧客が購入した商品を一時的にベクターに保存
        vector<string> products(m);
        for (int j = 0; j < m; ++j) {
            cin >> products[j];
        }

        // ★ポイント1：ペア内の順序をアルファベット順に固定するため、事前にソートする
        sort(products.begin(), products.end());

        // ★ポイント2：購入した商品から考えられる「すべての2つの組み合わせ」を生成
        for (int j = 0; j < m; ++j) {
            for (int k = j + 1; k < m; ++k) {
                // 事前にソートしているため、必ず products[j] < products[k] となる
                ProductPair p = {products[j], products[k]};
                pair_counts[p]++; // ペアの購入回数をカウントアップ
            }
        }
    }

    // 2. ソートのためにマップのデータをベクターに移す
    vector<pair<ProductPair, int>> sorted_pairs(pair_counts.begin(), pair_counts.end());

    // 3. カスタム比較関数を用いてベクターをソート
    sort(sorted_pairs.begin(), sorted_pairs.end(), comparePairs);

    int q;
    // クエリ数Qの読み込み
    cin >> q;

    // 4. クエリの処理と出力
    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;
        
        // 配列のインデックス調整 (a-1 から b-1 まで)
        for (int j = a - 1; j < b; ++j) {
            // 購入回数 ペアの1つ目の商品 ペアの2つ目の商品 を出力
            cout << sorted_pairs[j].second << " " 
                 << sorted_pairs[j].first.first << " " 
                 << sorted_pairs[j].first.second << "\n";
        }
    }

    return 0;
}
