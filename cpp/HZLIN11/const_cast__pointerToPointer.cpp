//[Note: if a program could assign a pointer of type T** to a pointer of type const T** (that is, if line //1 below was allowed), a program could inadvertently modify a const object (as it is done on line //2). For example,

int main() {
  const char c = 'c';
  char* pc;
  const char** pcc = &pc; //1: not allowed
  *pcc = &c;
  *pc = 'C'; //2: modifies a const object
}
