#include <iostream>
#include <random>
#include <memory>

namespace Random {
	unsigned long const seed = 3452;
	std::mt19937 generator;
	int generateRandom(int low = -100000, int high = 100000) {
		std::uniform_int_distribution<int> dist(low, high);
		return dist(generator);
	}
	int * generateRandomArr(int size, int low = -100000, int high = 100000) {
		generator.seed(seed);
		int*randNiz = new int[size];
		for (int i = 0; i < size; i++)
			randNiz[i] = generateRandom(low, high);
		return randNiz;
	}
}

namespace Sort {
	void adjust(int arr[], int i, int n) {
		int j = 2 * i + 1; // j je levi sin od i
		while (j < n) {
			if (j < (n - 1) && arr[j] < arr[j + 1]) //selektuj vece dete
				j = j + 1;
			if (arr[(j - 1) / 2] < arr[j]) {//otac manji od klinca?
				std::swap(arr[(j - 1) / 2], arr[j]); //zameni ih
				j = 2 * j + 1;  //nastavi zabavu
			}
			else
				return;
		}
	}
	void heapsort(int arr[], int n) {
		for (int i = (n / 2 - 1); i >= 0; i--) //od pretposlednjeg reda navise
			adjust(arr, i, n);
		for (int i = (n - 1); i >= 0; i--) {
			std::swap(arr[0], arr[i]);
			adjust(arr, 0, i);
		}
	}

	void insertionsort(int arr[], int n) {
		for (int i = 1; i < n; i++) {
			int j = i - 1;
			while (j >= 0 && arr[j + 1]<arr[j]) {
				std::swap(arr[j + 1], arr[j]);
				j--;
			}
		}
	}

	void shellsort(int arr[], int n, std::initializer_list<int> h = { 7,4 }) {
		for (int gap : h) {
			for (int i = gap; i < n; i += gap) {
				int j = i - gap;
				while (j >= 0 && arr[j + gap] < arr[j]) {
					std::swap(arr[j + gap], arr[j]);
					j -= gap;
				}
			}
		}
	}

	int partition(int arr[], int i, int j) {
		int pivot = (i + j) / 2;
		while (i<j) {
			while (arr[i] <= arr[pivot] && i<j)
				i++;
			while (arr[j]>arr[pivot])
				j--;
			if (i<j) {
				std::swap(arr[i], arr[j]);
				pivot = i == pivot ? j : j == pivot ? i : pivot;
			}
		}
		std::swap(arr[j], arr[pivot]);
		return j;
	}

	void quicksort(int arr[], int low, int high) {
		if (low<high) {
			int j = partition(arr, low, high);
			quicksort(arr, low, j - 1);
			quicksort(arr, j + 1, high);
		}
	}
}

void print(int arr[], int n) {
	for (int i = 0; i < n; i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
}

int main() {
	const int SIZE = 20;
	const int LOW = -100;
	const int HIGH = 100;

	int *arr = Random::generateRandomArr(SIZE, LOW, HIGH);

	print(arr, SIZE);
	Sort::quicksort(arr, 0, SIZE - 1);
	print(arr, SIZE);

	delete[] arr;
	system("pause");
	return 0;
}