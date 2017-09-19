#include <cstdio>
void swap(int* array, int i, int j) {
    array[i] = array[i]^array[j];
    array[j] = array[i]^array[j];
    array[i] = array[i]^array[j];
}

void printArray(int *array, int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void insertSort(int* array, int size) {
    int i;
    for (i=1; i<size; i++) {
        if (array[i-1] > array[i]) swap(array, i, i-1);
        int j = i-1;
        while (j>0 && array[j-1]>array[j]) {
            swap(array, j, j-1);
            j--;
        }
    }
}

void standardInsertSort(int* array, int size) {
    for (int i=1; i<size; i++) {
        int element = array[i];
        int j = i-1;
        while(j>0 && array[j]>element) {
            array[j+1] = array[j];
            j--;
        }
        array[j+1] = element;
    }
}

void recursiveStandardInsertSort(int* array, int size) {
    if (size > 1) {
        recursiveStandardInsertSort(array, size-1);
        int element = array[size-1];
        int i = size-2;
        while (i>0 && array[i]>element) {
            array[i+1] = array[i];
            i--;
        }
        array[i+1] = element;
    }
}


//seems bubbleSort is reverse direction of insertSort, but do not know why it said that insertSort is efficient than bubbleSort
void BubbleSort(int* array, int size) {
    int n = size;
    while (n>0) {
        for (int j=0; j<n-1; j++) {
            if (array[j] > array[j+1]) swap(array, j, j+1);
        }
        --n;
    }
} 

void reverseBubbleSort(int* array, int size) {
    int n = 0;
    while (n<size) {
        for (int j=size-1; j>n; j--) {
             if (array[j] < array[j-1]) swap(array, j, j-1);
        }
        n++;
    }
}

void SelectionSort(int* array, int size) {
    int iMin = 0, j;
    for (int i=0; i<size; i++) {
        iMin = i;
        for (j=i+1; j<size; j++) {
            if (array[iMin] > array[j]) iMin = j;
        }
        if (i != iMin) {
            swap(array, i, iMin);
        }
    }
}


//Merge sort
void Merge(int* arrayA, int beg, int mid, int end, int* arrayB) {
    int i = beg;
    int j = mid; 
    for (int k=beg; k<end; k++) {
        if (i<mid && (j>=end || arrayA[i]<arrayA[j]) ) {
            arrayB[k] = arrayA[i];
            i++;
        } else {
            arrayB[k] = arrayA[j];
            j++;
        }
    }
}
void copyArray(int* src, int* dst, int size) {
    for (int i=0; i<size; i++) {
        dst[i] = src[i];
    }
}
int min(int a, int b) {
    return a<b ? a:b;
}
//TopDown
void TopDownSplitMerge(int* arrayA, int beg, int end, int* arrayB) {
    if (end-beg <= 1) return;

    int mid = (beg+end)/2;
    TopDownSplitMerge(arrayB, beg, mid, arrayA); //arrayA is the temporary array, merge arrayA--->arrayB below.
    TopDownSplitMerge(arrayB, mid, end, arrayA);
    Merge(arrayA, beg, mid, end, arrayB);
}
void MergeSort_TopDown(int* array, int size) {
    int tempArray[size] = {0};
    copyArray(array, tempArray, size);
    TopDownSplitMerge(tempArray, 0, size, array);
}
//BottomUp
void MergeSort_BottomUp(int* A, int size) {
    int B[size];
    for (int width=1; width<size; width*=2) {
        for (int beg=0; beg<=size; beg+=2*width) {
            Merge(A, beg, min(beg+width, size), min(beg+2*width, size), B);
        }
        copyArray(B, A, size);
    }
}

//HeapSort
int getLeftChild(int index) {
    return 2*index + 1;
}
int getParent(int index) {
    if (0 == index) return -1;
    return (index-1)/2;
}
//repair the heap whose new added leaf:end maybe the largest of it branch, assuming the heaps rooted at start is valid .
void siftUp(int A[], int start, int end) {
    int child = end;
    while (child > start) {
        int parent = getParent(child);
        if (A[child] > A[parent]) {
            swap(A, child, parent);
            child = parent;
        } else {
            return;
        }
    }
}                        

void heaptifyUp(int A[], int size) {
    int end = 1; //must start siftUp from index:1 to make sure heaps before end is a valid heap
    while (end < size) {
        siftUp(A, 0, end);
        end++;
    }
}

void HeapSortUp(int A[], int size) {
    if (size==1) return;
    heaptifyUp(A, size);
    //after Heaptify, the Heap created from array A was already been set up.
    swap(A, 0, size-1);
    HeapSortUp(A, size-1);  //O is bigger than HeapSort.
}
//repair the heap whose root element maybe not the largest, assuming the heaps rooted at its children are valid.
//this function can not get the maximum element from a branch, otherwise, function:Heaptify is needless.
void siftDown(int A[], int root, int end) {
    int iswap, leftChild;
    while ((leftChild=getLeftChild(root)) <= end) {
        iswap = root;
        if (A[leftChild] > A[root]) {
            iswap = leftChild;
        } 
        if ((leftChild+1 <= end) && (A[iswap] < A[leftChild+1])) {
            iswap = leftChild+1;
        }

        if (iswap == root) {
            return;
        } else {
            swap(A, iswap, root);
            root = iswap;
        }
    }
}
//set up a heap from array A
void Heaptify(int A[], int size) {
    if (size<=1) return;
    int start = (size-2)/2; //the last element in a 0-based array is at index size-1, the parent of the last element is size-1-1/2, the later -1 in case the last element is rightChild.
    while (start>=0) {
        siftDown(A, start, size-1);
        start--;
    }
}

void HeapSort(int A[], int size) {
    Heaptify(A, size);
    //after Heaptify, the Heap created from array A was already been set up.

    int i = size-1;
    while (i>0) {
        swap(A, 0, i); //swap the largest element(the index 0) to the end. After this swap, only the first element of the new heap(whoes element are A[0]...A[i-2]) was in wrong place , so can use siftDown(A, 0, i-1) to repair it.
        siftDown(A, 0, i-1);
        i--;
    }
}
void HeapSort2(int A[], int size) {
    if (size==1) return;
    Heaptify(A, size);
    //after Heaptify, the Heap created from array A was already been set up.

    swap(A, 0, size-1); //swap the largest element(the index 0) to the end. previous Action will destroy heap who has size-1 element. 
    HeapSort2(A, size-1);
}


//Quick Sort
void quickSort(int A[], int size) {
    if (size<=1) return;
    int pivot = size-1;
    int i=0; 
    while (i<pivot) {
        if (A[i] > A[pivot]) {
            int temp = A[i];
            A[i] = A[pivot-1];
            A[pivot-1] = A[pivot];
            A[pivot] = temp;
            pivot--;
        } else {
            i++;
        }
    }
    quickSort(A, pivot);
    if (size-1 != pivot) quickSort(A+pivot+1, size-1-pivot);
}

int main() {
    printf("standardInsertSort\n");
    int array0[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    standardInsertSort(array0, sizeof(array0)/sizeof(int));
    printArray(array0, sizeof(array0)/sizeof(int));
    printf("My insertSort\n");
    int array[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    insertSort(array, sizeof(array)/sizeof(int));
    printArray(array, sizeof(array)/sizeof(int));
    int array00[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    printf("recursiveStandardInsertSort\n");
    recursiveStandardInsertSort(array00, sizeof(array00)/sizeof(int));
    printArray(array00, sizeof(array00)/sizeof(int));

    printf("BubbleSort\n");
    int array1[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    BubbleSort(array1, sizeof(array1)/sizeof(int));
    printArray(array1, sizeof(array1)/sizeof(int));
    printf("reverseBubbleSort\n");
    int array11[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    reverseBubbleSort(array11, sizeof(array11)/sizeof(int));
    printArray(array11, sizeof(array11)/sizeof(int));

    printf("SelectionSort\n");
    int array2[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    SelectionSort(array2, sizeof(array2)/sizeof(int));
    printArray(array2, sizeof(array2)/sizeof(int));

    printf("MergeSort_TopDown\n");
    int array3[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    MergeSort_TopDown(array3, sizeof(array3)/sizeof(int));
    printArray(array3, sizeof(array3)/sizeof(int));
    printf("MergeSort_BottomUp\n");
    int array33[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    MergeSort_BottomUp(array33, sizeof(array33)/sizeof(int));
    printArray(array33, sizeof(array33)/sizeof(int));

    printf("HeapSort\n");
    int array4[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    HeapSort(array4, sizeof(array4)/sizeof(int));
    printArray(array4, sizeof(array4)/sizeof(int));
    printf("HeapSort2\n");
    int array44[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    HeapSort2(array44, sizeof(array44)/sizeof(int));
    printArray(array44, sizeof(array44)/sizeof(int));
    printf("HeapSortUp\n");
    int array444[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    HeapSortUp(array444, sizeof(array444)/sizeof(int));
    printArray(array444, sizeof(array444)/sizeof(int));

    printf("QickSort\n");
    int array5[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 23, 854,384};
    quickSort(array5, sizeof(array5)/sizeof(int));
    printArray(array5, sizeof(array5)/sizeof(int));

    return 0;
}    
