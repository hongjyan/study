#include <cstdio>
void swap(int* array, int i, int j) {
    if (i==j) return;
    array[i] = array[i]^array[j];
    array[j] = array[i]^array[j];
    array[i] = array[i]^array[j];
    // int temp = array[i];
    // array[i] = array[j];
    // array[j] = temp;
}

void printArray(int *array, int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void insertSort(int* array, int size) {
    int i;
    // 将数组的第一个元素视为已排序序列。然后遍历随后元素
    for (i=1; i<size; i++) {
        // 此处if时为了i=1时， 下面两次j-1， 将导致数组的index变为-1. 所以此处为了比较只有两个元素的数组。
        if (array[i-1] > array[i]) swap(array, i, i-1);
        int j = i-1;
        // 比较array[j]与已排序好的array[0...j-1]所有元素
        while (j>0 && array[j-1]>array[j]) {
            swap(array, j, j-1);
            j--;
        }
    }
}

void standardInsertSort(int* array, int size) {
    // 将数组的第一个元素视为已排序序列。然后遍历随后元素
    // array[0...i-1]是已排序序列，接下来寻找array[i]的插入位置
    for (int i=1; i<size; i++) {
        int element = array[i]; // 暂存array[i]
        int j = i-1;
        // 比较已排序的array[0...i-1]里所有元素与array[i]大小， 如果后者小，那么就要
        // 腾出一个位子给array[i]插入。 怎么腾呢？ 就是将大于array[i]的所有元素向右
        // 移动一个位置， 即下面的array[j+1] = array[j]
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
// 第一个和第二个比较， 谁高谁向右， 第二个和第三个比较，谁高谁右移。以此类推， 第一次循环最高到最右。第二次循环第二高到最右。 
void BubbleSort(int* array, int size) {
    int n = size;
    while (n>0) {
        // 从最左边开始， 第N高结束，因为右边的n...size-1已是有序最高的了
        for (int j=0; j<n-1; j++) {
            if (array[j] > array[j+1]) swap(array, j, j+1);
        }
        --n;
    }
} 

// 反向冒泡， 最矮冒泡到最左边
void reverseBubbleSort(int* array, int size) {
    int n = 0;
    while (n<size) {
        // 从最右边开始，第N矮结束， 因为左边的0...n已经是有序最矮的了
        for (int j=size-1; j>n; j--) {
             if (array[j] < array[j-1]) swap(array, j, j-1);
        }
        n++;
    }
}

// 假定一个最矮，然后将队列中所有人和当前得到的最矮比较高矮，那么遍历完成就会得到真正最矮
// 第二次遍历就会得到真正第二矮。以此类推
void SelectionSort(int* array, int size) {
    int iMin = 0, j;
    // 先假设第i个人最矮。当然这个i从0开始，且从0开始遍历，那么第一次的最矮就是全队列最矮的
    // 第二次从1开始遍历，那么第二次最矮的就是第二矮的
    for (int i=0; i<size; i++) {
        iMin = i;
        // 比较i以后的所有元素即array[j与array[i]高矮, 将更矮的index更新到最矮号牌上
        for (j=i+1; j<size; j++) {
            if (array[iMin] > array[j]) iMin = j;
        }
        // 如果最矮号码牌上index与i不一致， 那么swap。
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
    //while循环结束后， pivot左边得元素都比pivot小， pivot右边元素都比pivot大。
    while (i<pivot) {
        if (A[i] > A[pivot]) {
            swap(A, i, pivot-1);
            swap(A, pivot-1, pivot);
            pivot--;
        } else {
            i++;
        }
    }
    // 递归排序pivot左边元素
    quickSort(A, pivot);
    // 如果pivot不是最后一个元素， 就有必要递归排序pivot右边元素。
    if (size-1 != pivot) quickSort(A+pivot+1, size-1-pivot);
}


// BST. Binary search tree sort
struct Node {
    int key;
    Node* left;
    Node* right;
};

Node* newNode(int key) {
    Node* node = new Node;
    node->key = key;
    node->left = node->right = nullptr;
    return node;
}

Node* insert(Node* root, int key) {
    if (nullptr == root) {
        return newNode(key);
    }
    if (root->key > key) {
        root->left = insert(root->left, key);
    }
    else if (root->key < key) {
        root->right = insert(root->right, key);
    }
    return root;
}

Node* createBinaryTree(int* arr, int n) {
    if (nullptr == arr) return nullptr;

    Node* root = nullptr;
    for (int i=0; i!=n; ++i) {
        root = insert(root, arr[i]);
    }
    return root;
}

void destructBinaryTree(Node* root) {
    if (nullptr != root->left) destructBinaryTree(root->left);
    if (nullptr != root->right) destructBinaryTree(root->right);
    if (nullptr != root) delete root;
}

void storeSorted(Node* root, int arr[], int& i) {
    if (nullptr != root) {
        storeSorted(root->left, arr, i);
        arr[i++] = root->key;
        storeSorted(root->right, arr, i);
    }
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

    printf("QuickSort\n");
    int array5[] = {1, 2323, 434, 543534,65, 7,3,88983, 8453,213 ,23445, 343658, 213, 23, 854,384};
    quickSort(array5, sizeof(array5)/sizeof(int));
    printArray(array5, sizeof(array5)/sizeof(int));

    printf("binaryTreeSort\n");
    int array6[] = {1, 2323, 434, 543534, 65, 7,3,88983, 8453, 213, 23445, 343658, 213, 23, 854,384};
    int size = sizeof(array6)/sizeof(int);
    printArray(array6, size);
    Node* root = createBinaryTree(array6, size);
    int i = 0;
    storeSorted(root, array6, i);
    printArray(array6, i);
    destructBinaryTree(root);

    return 0;
}    
