#ifndef PERARR_H_
#define PERARR_H_

void CaddEle(int *arr, int size, int elem) {
	int *curr = new int[size];
	for (int i = 0; i < size; i++) {
		curr[i] = arr[i];
	}
	
	delete[] arr;

	arr = new int[size + 1];
	for (int i = 0; i < size; i++) {
		arr[i] = curr[i];
	}
	arr[size] = elem;

	delete[] curr;
}

char Cpop(int *arr, int size, int position) {
	if (size <= 0 || position < 0 || position >= size) {
		return '\0';
	}

	int *curr = new int[size - 1];
	for (int i = 0; i < position; i++) {
		curr[i] = arr[i];
	}
	for (int i = position + 1; i < size; i++) {
		curr[i - 1] = arr[i];
	}
	
	int removed_elem = arr[position];
	
	delete[] arr;
	arr = curr;

	return removed_elem;
}


#endif