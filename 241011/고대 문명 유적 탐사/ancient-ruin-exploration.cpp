#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#define N_large 5
#define N_small 3

int board[N_large][N_large]; // 고대 문명 격자
int k, m; // 탐사 횟수 K, 벽면에 적힌 유물 조각의 개수 M
queue<int> numQ; // 벽면 숫자 큐
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };

// 회전된 결과를 적용한 임시 보드를 반환하는 함수
void Rotate(int sy, int sx, int angle) {
    int tmp[N_small][N_small];
    for (int i = 0; i < N_small; ++i)
        for (int j = 0; j < N_small; ++j)
            tmp[i][j] = board[sy + i][sx + j];

    // 각도에 따라 회전 수행
    if (angle == 90) {
        for (int i = 0; i < N_small; ++i)
            for (int j = 0; j < N_small; ++j)
                board[sy + j][sx + 2 - i] = tmp[i][j];
    }
    else if (angle == 180) {
        for (int i = 0; i < N_small; ++i)
            for (int j = 0; j < N_small; ++j)
                board[sy + 2 - i][sx + 2 - j] = tmp[i][j];
    }
    else if (angle == 270) {
        for (int i = 0; i < N_small; ++i)
            for (int j = 0; j < N_small; ++j)
                board[sy + 2 - j][sx + i] = tmp[i][j];
    }
}

// BFS로 유물 조각들을 탐색하여 점수를 계산하는 함수
int CalScore() {
    bool visited[N_large][N_large] = { false };
    int totalScore = 0;

    for (int i = 0; i < N_large; ++i) {
        for (int j = 0; j < N_large; ++j) {
            if (visited[i][j] || board[i][j] == 0) continue;

            queue<pair<int, int>> q;
            vector<pair<int, int>> trace;
            q.push({ i, j });
            visited[i][j] = true;
            trace.push_back({ i, j });

            while (!q.empty()) {
                pair<int, int> cur = q.front();
                int y = cur.first;
                int x = cur.second;
                q.pop();

                for (int dir = 0; dir < 4; ++dir) {
                    int ny = y + dy[dir], nx = x + dx[dir];
                    if (ny < 0 || ny >= N_large || nx < 0 || nx >= N_large) continue;
                    if (visited[ny][nx] || board[ny][nx] != board[y][x]) continue;

                    visited[ny][nx] = true;
                    q.push({ ny, nx });
                    trace.push_back({ ny, nx });
                }
            }

            // 3개 이상 연결된 유물 조각이면 점수에 반영
            if (trace.size() >= 3) {
                totalScore += trace.size();
                for (auto cur : trace) {
                    int y = cur.first;
                    int x = cur.second;
                    board[y][x] = 0; // 유물 조각을 제거
                }

            }
        }
    }

    return totalScore;
}

// 0인 자리에 벽면 숫자를 채워 넣는 함수
void Fill() {
    for (int j = 0; j < N_large; ++j) {
        for (int i = N_large - 1; i >= 0; --i) {
            if (board[i][j] == 0 && !numQ.empty()) {
                board[i][j] = numQ.front();
                numQ.pop();
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> k >> m;

    for (int i = 0; i < N_large; ++i)
        for (int j = 0; j < N_large; ++j)
            cin >> board[i][j];

    for (int i = 0; i < m; ++i) {
        int num;
        cin >> num;
        numQ.push(num);
    }

    // K번의 탐사 수행
    while (k--) {
        int maxScore = 0, bestAngle = 0, bestY = 0, bestX = 0;

        // 모든 가능한 회전 수행 및 점수 계산
        for (int angle : {90, 180, 270}) {
            for (int sy = 0; sy <= N_large - N_small; ++sy) {
                for (int sx = 0; sx <= N_large - N_small; ++sx) {
                    // 보드 상태 저장
                    int original[N_large][N_large];
                    for (int i = 0; i < N_large; ++i)
                        for (int j = 0; j < N_large; ++j)
                            original[i][j] = board[i][j];

                    // 회전 및 점수 계산
                    Rotate(sy, sx, angle);
                    int score = CalScore();

                    // 최대 점수 갱신
                    if (score > maxScore ||
                        (score == maxScore && angle < bestAngle) ||
                        (score == maxScore && angle == bestAngle && (sx < bestX || (sx == bestX && sy < bestY)))) {
                        maxScore = score;
                        bestAngle = angle;
                        bestY = sy;
                        bestX = sx;
                    }

                    // 보드 상태 복원
                    for (int i = 0; i < N_large; ++i)
                        for (int j = 0; j < N_large; ++j)
                            board[i][j] = original[i][j];
                }
            }
        }

        // 더 이상 점수를 얻을 수 없으면 종료
        if (maxScore == 0) break;

        // 최적의 회전 수행
        Rotate(bestY, bestX, bestAngle);
        int turnScore = CalScore();

        // 연쇄적으로 유물을 획득할 수 있는지 반복
        while (turnScore > 0) {
            Fill();
            turnScore = CalScore();
            maxScore += turnScore;
        }

        cout << maxScore << " ";
    }

    return 0;
}