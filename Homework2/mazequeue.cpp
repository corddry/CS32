#include <string>
#include <queue>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {

	queue<Coord> coordQueue;
	Coord curr(sr, sc);
	coordQueue.push(curr);
	int r = sr, c = sc;
	maze[r][c] = ',';
	while (!coordQueue.empty()) {
		curr = coordQueue.front();
		coordQueue.pop();
		r = curr.r();
		c = curr.c();

		if (r == er && c == ec) {
			return true;
		}

		if (maze[r][c + 1] == '.') {
			Coord next(r, c + 1);
			maze[r][c + 1] = ',';
			coordQueue.push(next);
		}
		if (maze[r + 1][c] == '.') {
			Coord next(r + 1, c);
			maze[r + 1][c] = ',';
			coordQueue.push(next);
		}
		if (maze[r][c - 1] == '.') {
			Coord next(r, c - 1);
			maze[r][c - 1] = ',';
			coordQueue.push(next);
		}
		if (maze[r - 1][c] == '.') {
			Coord next(r - 1, c);
			maze[r - 1][c] = ',';
			coordQueue.push(next);
		}
	}
	return false;
}

#include <iostream>
int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X.XXX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 4, 3, 1, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}