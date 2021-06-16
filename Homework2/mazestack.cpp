#include <string>
#include <stack>

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

	stack<Coord> coordStack;
	Coord curr(sr, sc);
	coordStack.push(curr);
	int r = sr, c = sc;
	maze[r][c] = ',';
	while (!coordStack.empty()) {
		curr = coordStack.top();
		coordStack.pop();
		r = curr.r();
		c = curr.c();

		if (r == er && c == ec) {
			return true;
		}

		if (maze[r][c + 1] == '.') {
			Coord next(r, c + 1);
			maze[r][c + 1] = ',';
			coordStack.push(next);
		}
		if (maze[r+1][c] == '.') {
			Coord next(r+1, c);
			maze[r+1][c] = ',';
			coordStack.push(next);
		}
		if (maze[r][c - 1] == '.') {
			Coord next(r, c - 1);
			maze[r][c - 1] = ',';
			coordStack.push(next);
		}
		if (maze[r - 1][c] == '.') {
			Coord next(r - 1, c);
			maze[r-1][c] = ',';
			coordStack.push(next);
		}
	}
	return false;
}