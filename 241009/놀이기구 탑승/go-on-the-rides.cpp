#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define X first
#define Y second

int N;
int board[21][21]; // 교실 자리 배열
int like[401][4];  // 각 학생의 좋아하는 친구 목록
int dx[4] = { -1, 1, 0, 0 }; // 상하좌우 이동
int dy[4] = { 0, 0, -1, 1 };

int calculate_satisfaction(int x, int y, int student) {
    int count = 0;
    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
        for (int i = 0; i < 4; i++) {
            if (board[nx][ny] == like[student][i]) {
                count++;
                break;
            }
        }
    }
    if (count == 0) return 0;
    if (count == 1) return 1;
    if (count == 2) return 10;
    if (count == 3) return 100;
    return 1000;
}

pair<int, int> find_position(int student) {
    int max_like_count = -1;
    int max_empty_count = -1;
    pair<int, int> best_position = { N, N }; // 초기값은 큰 값으로 설정

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (board[x][y] != 0) continue; // 이미 자리가 차 있는 경우
            int like_count = 0;
            int empty_count = 0;

            for (int dir = 0; dir < 4; dir++) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;

                if (board[nx][ny] == 0) empty_count++;
                for (int i = 0; i < 4; i++) {
                    if (board[nx][ny] == like[student][i]) {
                        like_count++;
                        break;
                    }
                }
            }

            // 우선순위에 따라 자리 선택
            if (like_count > max_like_count ||
                (like_count == max_like_count && empty_count > max_empty_count) ||
                (like_count == max_like_count && empty_count == max_empty_count && best_position > make_pair(x, y))) {
                max_like_count = like_count;
                max_empty_count = empty_count;
                best_position = { x, y };
            }
        }
    }
    return best_position;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> N;

    vector<int> students(N * N);
    for (int i = 0; i < N * N; i++) {
        int student;
        cin >> student;
        students[i] = student;
        for (int j = 0; j < 4; j++) {
            cin >> like[student][j];
        }
    }

    // 학생 자리를 배정
    for (int student : students) {
        pair<int, int> position = find_position(student);
        board[position.X][position.Y] = student;
    }

    // 만족도 계산
    int total_satisfaction = 0;
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            total_satisfaction += calculate_satisfaction(x, y, board[x][y]);
        }
    }

    cout << total_satisfaction;
    return 0;
}