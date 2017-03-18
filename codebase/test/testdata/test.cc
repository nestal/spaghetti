int global_var = 0;

class Base4
{
public:
	Base4() = default;
};

class Base
{
public:
	virtual ~Base() = default;
	virtual void Func() = 0;
};

template <typename BaseType>
class RecursiveBase : public BaseType, public Base4
{
public:
	virtual void SomeFunc()
	{
		// prevent optimizer to remove the function
		global_var++;
	}
};

class Base2
{
public:
	Base2() = default;
};

class Base3
{
public:
	Base3() = default;
};

class Derived : public RecursiveBase<Base>, public Base2, public Base3
{
public:
	Derived() = default;

	void Func() override {}
};

int main()
{
	return 0;
}
