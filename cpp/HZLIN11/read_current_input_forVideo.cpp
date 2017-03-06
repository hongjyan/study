#include <stdio.h>
#include <stdlib.h>

static char *buffer = NULL;
static int buffer_ind = 0;
static int buffer_size = 0;
char buf[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ,15};



short int read_current_input()
{
	int oddFlag = 0;
	unsigned char oddBytes[2] = { 0, 0 };
	
	// if we have finished processing the last frame, read a new one
	if (buffer_ind + 1 == buffer_size) {
		oddFlag = 1;
		oddBytes[0] = buffer[buffer_ind];
	}
	if((buffer_size - buffer_ind) <= 1) 
	{
		buffer_ind = oddFlag;
	     	/*
		//if(!mDecoder->getFrame(buffer_size, (void* &)buffer))
		if(!mDecoder->getOriginalFrame(buffer_size, (void* &)buffer))
		{
			mDone = true;
			if (oddFlag) {
				oddBytes[1] = 0;
				return *(reinterpret_cast<short int*>(oddBytes));
			}
			return 0;
		}
                */
                buffer_size = sizeof(buf);
                buffer = &buf[0];
		if (oddFlag) {
			oddBytes[1] = buffer[0];
			return *(reinterpret_cast<short int*>(oddBytes));
		}
	}

	short int res = *(reinterpret_cast<int16_t*>(buffer+buffer_ind));
	buffer_ind+=2;
	return res;
}



int main() {
    int i = 0;
    short int data = 0;
    int length = 3*sizeof(buf);

    while (i <= length) {
        data = read_current_input();
        i++;
        printf("%04hx ", data);
    }
    printf("\n");
}
