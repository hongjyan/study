class Abstract {
public:
	//virtual Abstract() = 0;
	virtual ~Abstract() = 0;
};


class Concrete : public Abstract {
public:
	Concrete() {};
	~Concrete() {};
};

int main() {
	Concrete concrete;
	return 0;
}
