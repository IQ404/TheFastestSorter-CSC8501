#include <iostream>
#include <random>
#include <chrono>

using namespace std;

// Pigeonhole Sort
void pigenhole_sort(int* arr, int arr_size, int upper_bound)
{
	int* pigeon_holes = new int[upper_bound + 1]{ 0 };

	for (int i = 0; i < arr_size; i++) {
		pigeon_holes[arr[i]] += 1;
	}

	for (int i = 1, counter = 0; i < upper_bound + 1; i++) {
		for (int y = 0; y < pigeon_holes[i]; y++, counter++) {
			arr[counter] = i;
		}
	}
}

// Merge Sort

void merge(int* arr, int* aux_arr, int head, int mid, int tail)
{
	int left_size = mid - head + 1;
	int right_size = tail - mid;
	int merged_size = left_size + right_size;

	int tracker_left = 0;
	int tracker_right = 0;
	int tracker_merged = head;



	while (tracker_left < left_size && tracker_right < right_size)
	{
		if (arr[head + tracker_left] <= arr[mid + 1 + tracker_right])
		{
			aux_arr[tracker_merged] = arr[head + tracker_left];
			tracker_left++;
		}
		else
		{
			aux_arr[tracker_merged] = arr[mid + 1 + tracker_right];
			tracker_right++;
		}
		tracker_merged++;
	}

	while (tracker_left < left_size)
	{
		aux_arr[tracker_merged] = arr[head + tracker_left];
		tracker_left++;
		tracker_merged++;
	}

	while (tracker_right < right_size)
	{
		aux_arr[tracker_merged] = arr[mid + 1 + tracker_right];
		tracker_right++;
		tracker_merged++;
	}

	for (int i = 0; i < merged_size; ++i, --tail)
	{
		arr[tail] = aux_arr[tail];
	}
}

void merge_sort(int* arr, int* aux_arr, int head, int tail)
{
	if (head < tail)
	{
		int mid = head + (tail - head) / 2;
		merge_sort(arr, aux_arr, head, mid);
		merge_sort(arr, aux_arr, mid + 1, tail);
		merge(arr, aux_arr, head, mid, tail);
	}
}

void merge_sort_starter(int* arr, int head, int tail)
{
	int* aux_arr = new int[tail - head + 1];
	merge_sort(arr, aux_arr, head, tail);
}

// Insertion Sort
void insertion_sort(int* arr, int head_index, int tail_index)
{
	if (head_index == tail_index) return;
	for (int pos = head_index + 1; pos <= tail_index; pos++)
	{
		int t = arr[pos];
		int index = pos;
		for (; index > 0 && t < arr[index - 1]; index--)
		{
			arr[index] = arr[index - 1];
		}
		arr[index] = t;
	}
}

// auxiliaries
void int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// QuickSort (with Insertion-Sort optimisation for small subarray)

int median_3(int* arr, int head_index, int tail_index)
{
	int mid_index = head_index + (tail_index - head_index) / 2;
	if (arr[mid_index] < arr[head_index]) int_swap(&arr[head_index], &arr[mid_index]);
	if (arr[tail_index] < arr[head_index]) int_swap(&arr[head_index], &arr[tail_index]);
	if (arr[tail_index] < arr[mid_index]) int_swap(&arr[mid_index], &arr[tail_index]);

	// hide the pivot
	int_swap(&arr[mid_index], &arr[tail_index - 1]);  // tail is now larger than or equal to the pivot
	return arr[tail_index - 1];
}

void quick_sort(int* arr, int head_index, int tail_index)
{
	if (head_index + 20 <= tail_index)
	{
		//int pivot = arr[head_index];
		int pivot = median_3(arr, head_index, tail_index);

		int i = head_index;
		int j = tail_index - 1;
		while (true)
		{
			while (arr[++i] < pivot) {}  // head is already smaller than or equal to the pivot
			while (arr[--j] > pivot) {}  // the initial j is at the pivot
			if (i < j) int_swap(&arr[i], &arr[j]);
			else break;
		}
		int_swap(&arr[i], &arr[tail_index - 1]);
		quick_sort(arr, head_index, i - 1);
		quick_sort(arr, i + 1, tail_index);
	}
	else insertion_sort(arr, head_index, tail_index);
}

int main()
{
	// randomness
	mt19937 gen;
	uint32_t seed_num = time(nullptr);
	gen.seed(seed_num);
	int upper_bound = 100;
	uniform_int_distribution<uint32_t> interval(1, upper_bound);

	// original array
	const int arr_size = 1000;
	int arr[arr_size];
	for (int i = 0; i < arr_size; i++)
	{
		arr[i] = interval(gen);
	}

	//----------------------------------------------------------------

	auto start = chrono::system_clock::now();

	// sort
	//pigenhole_sort(arr, arr_size, upper_bound);
	//merge_sort_starter(arr, 0, arr_size - 1);
	//insertion_sort(arr, 0, arr_size - 1);
	quick_sort(arr, 0, arr_size - 1);

	auto end = chrono::system_clock::now();
	auto elapsed = end - start;

	//----------------------------------------------------------------

	// print
	for (int i = 0; i < arr_size; i++)
	{
		cout << arr[i] << '\n';
	}
	cout << "----------------------------\n";
	cout << "Computational time: " << elapsed.count() << '\n';
}