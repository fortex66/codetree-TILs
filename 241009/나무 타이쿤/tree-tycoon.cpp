#include <iostream>
#include <vector>
using namespace std;
#define X first
#define Y second

int n, m;
int board[16][16];
int dx[9] = { 0,0,-1,-1,-1,0,1,1,1 };
int dy[9] = { 0,1,1,0,-1,-1,-1,0,1 };
vector<pair<int, int> > drug_area;
vector<vector<bool>> used; // 영양제를 투입한 위치 기록

// p만큼 영양제 d방향 이동
void move_drug(int d, int p) {
    for (int i = 0; i < (int)drug_area.size(); i++) {
        int nx = (drug_area[i].X + dx[d] * p + n) % n;
        int ny = (drug_area[i].Y + dy[d] * p + n) % n;
        drug_area[i] = { nx, ny };
    }
}

void use_drug() {
    used = vector<vector<bool>>(n, vector<bool>(n, false));
    for (int i = 0; i < (int)drug_area.size(); i++) {
        int x = drug_area[i].X;
        int y = drug_area[i].Y;
        board[x][y]++;
        used[x][y] = true; // 영양제를 투입한 위치를 기록
    }
}

// 영양제 위치에서 대각선 탐색
void drug_effet() {
    for (int i = 0; i < (int)drug_area.size(); i++) {
        int x = drug_area[i].X;
        int y = drug_area[i].Y;
        int count = 0;

        for (int dir = 2; dir < 9; dir += 2) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n ) continue;
            if (board[nx][ny] > 0) count++;
        }
        board[x][y] += count;
    }
}

// 컷팅후 약물 놓기
void cutting() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!used[i][j] && board[i][j] >= 2) {
                board[i][j] -= 2;
                drug_area.push_back({ i,j });
            }
        }
    }
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

    // 초기 영양제 위치 설정
    drug_area.push_back({ n - 2,0 });
    drug_area.push_back({ n - 2,1 });
    drug_area.push_back({ n-1 ,0 });
    drug_area.push_back({ n-1 ,1 });

    // 년도별 반복
    while (m--) {
        int d, p;
        cin >> d >> p;
        move_drug(d, p); // 영양제 이동
        use_drug();// 영양제 주입
        drug_effet(); // 리브로수 높이 증가
        drug_area.clear();
        cutting(); // 리브로수 자르고 영약 놓기
        
    }

    int score = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            score += board[i][j];
        }
    }

    cout << score;

    return 0;
}