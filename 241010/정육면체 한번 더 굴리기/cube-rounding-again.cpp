#include <iostream>
#include <queue>
using namespace std;
#define X first
#define Y second

int n, m, x, y, ans;
int board[21][21];
int dice[7] = { 0, 5, 6, 2, 1, 4, 3 };
int dir = 1;
int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1};

// 상: 0,  우: 1, 하: 2, 좌: 3
void dice_dir() {
    // 시계 방향 90도 회전
    if (board[x][y] < dice[2]) {
        dir = (dir + 1) % 4;
    }
    // 반시계 방향 90도 회전
    else if (board[x][y] > dice[2]) {
        dir = (dir + 3) % 4;
    }
    // 숫자가 같다면 그대로
}

// dir로 주사위 굴리기
void dice_roll() {
    // 상: 0,  우: 1, 하: 2, 좌: 3
    if (dir == 0) {
        int tmp = dice[1];
        dice[1] = dice[4];
        dice[4] = dice[3];
        dice[3] = dice[2];
        dice[2] = tmp;
    }
    else if (dir == 1) {
        int tmp = dice[2];
        dice[2] = dice[6];
        dice[6] = dice[4];
        dice[4] = dice[5];
        dice[5] = tmp;
    }
    else if (dir == 2) {
        int tmp = dice[1];
        dice[1] = dice[2];
        dice[2] = dice[3];
        dice[3] = dice[4];
        dice[4] = tmp;
    }
    else {
        int tmp = dice[2];
        dice[2] = dice[5];
        dice[5] = dice[4];
        dice[4] = dice[6];
        dice[6] = tmp;
    }
}

// 반사 처리
void reflection() {
    dir = (dir + 2) % 4; // 방향 전환
    dice_roll(); // 주사위 굴리기
    x += dx[dir];
    y += dy[dir];
}

// 보드 좌표 이동
void move_board() {
    int nx = x + dx[dir];
    int ny = y + dy[dir];

    // 반사 처리
    if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
        reflection();
    }
    else {
        x = nx;
        y = ny;
    }
}

// 보드 bfs 및 점수 계산
int dice_score() {
    queue<pair<int, int> > Q;
    bool visited[21][21] = { false };
    Q.push({ x,y });
    visited[x][y] = true;
    int count = 1;
    while (!Q.empty()) {
        auto cur = Q.front(); Q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = cur.X + dx[i];
            int ny = cur.Y + dy[i];
            // 범위 검사 및 방문검사
            if (nx < 0 || ny < 0 || nx >= n || ny >= n || visited[nx][ny]) continue;
            // 현재 보드의 숫자와 다르면 패스
            if (board[nx][ny] != board[cur.X][cur.Y]) continue;
            visited[nx][ny] = true;
            Q.push({ nx,ny });
            count++;
        }
    }
    int score = board[x][y] * count;
    return score;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> m;

    // 보드 입력 받기
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }

    while (m--) {
        dice_roll(); // 주사위 굴리기
        move_board(); // 보드 좌표 이동
        int score = dice_score(); // 탐색 및 점수
        dice_dir(); // 주사위의 진행방향 구하기
        ans += score;
    }
    cout << ans;
    return 0;
}