struct Foo {};

int main() {
  int i;
  volatile int vi;

  //int *p = &vi; //same as const
  volatile int *vp = &i;

  //int &r = vi;  //same as const
  volatile int &vr = i;

  const Foo cf, cf1;
  Foo f(cf); //OK
  f = cf1; //OK
  const Foo cf2(f);

  volatile Foo vf, vf1;
  //In summary, sythesized-ctor's parameter is not "volatile Foo&"
  //Foo f1(vf); //error: no matching function for call to 'Foo::Foo(volatile Foo&)'
  //f1 = vf1; //error: no match for 'operator=' (operand types are 'Foo' and 'volatile Foo')
  //volatile Foo vf3(vf); //NOK

  volatile Foo vf2(f); //OK, since synthesize ctr's parameter is "Foo&/const Foo&"
  return 0;
}  
