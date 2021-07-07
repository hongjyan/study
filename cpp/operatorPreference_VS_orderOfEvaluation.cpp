//1.（）不是操作符， 只是起到group的作用， 表示括号内的东西要单独拉出来计算，而不是计算顺序。
//2. 操作符的优先级不决定表达式的某部分的计算顺序，也是相当于()起到的group作用
//3. 表达式操作符间的计算顺序取决于具体操作符， 如逗号操作符要从左到右顺序执行。而+操作符计算先后顺序并不能保证。
//      这里有点奇怪的是： 那么结合性用在什么地方？+操作符是右结合不是意味着右边操作数先计算吗？

#include <iostream>
#include <vector>

int main()
{
    int n = 1;
    int m = (++n, (std::cout << "n = " << n << '\n', ++n, 2*n));
    std::cout << "m = " << (++m, m) << '\n';

    //https://stackoverflow.com/questions/51246601/comma-operator-suppress-the-precedence-of-parentheses-in-c
    int x=0, y=0, z=0;
    x = ( y = 3, ( z = ++y + 2 ) + 5 );
    std::cout << x << std::endl;
    x=0, y=0, z=0;
    x = ( z = ++y + 2 ) + y; //括号为3， 第二个y值不定，如其先被evaluate，那么其值为0， 否则为1.
    std::cout << x << std::endl;

    


    //逗号运算符始终从左到右顺序执行， 故结果始终是6, 2， 45， 12
    std::vector<int> v3;
    (v3.push_back(6), (v3.push_back(2), (v3.push_back(45), v3.push_back(12))));
    for(auto i : v3) std::cout << i << ' ';
    std::cout << std::endl;
    std::vector<int> v4;
    (((v4.push_back(6), v4.push_back(2)), v4.push_back(45)), v4.push_back(12));
    for (auto i : v4) std::cout << i << ' ';
    std::cout << std::endl;

}
 