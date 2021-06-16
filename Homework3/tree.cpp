int countContains(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0) {
		return 1;
	}
	if (n1 <= 0) {
		return 0;
	}

	int n = 0;

	if (a1[n1 - 1] == a2[n2 - 1]) {
		n+=countContains(a1, n1 - 1, a2, n2 - 1);
	}
	n += countContains(a1, n1 - 1, a2, n2);

	return n;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

void separate(double a[], int n, double separator,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;


	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < separator)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > separator)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 0 || n == 1) {
		return;
	}
	int fng, fl;
	separate(a, n, a[0], fng, fl);
	order(a, fng + 1);
	order(a + fl, n - fl);
}