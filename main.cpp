/****************************************************************
 * Project: VFTable
 * File: main.cpp
 * Create Date: 2020/04/08
 * Author: Gao Jiongjiong
 * Descript: Research virtual function table.
****************************************************************/

#include <iomanip>
#include <iostream>

using namespace std;

#ifdef _WIN64
typedef long long POINTER;
#else // _WIN64
typedef long POINTER;
#endif // _WIN64

// B1 has no virtual member functions
class B1
{
	POINTER b1;
public:
	B1() : b1(0) {}
};

// B2 has two virtual member functions fb and fb2
class B2
{
	POINTER b2;
public:
	B2() : b2(0) {}
	virtual void fb() { cout << "B2.fb" << endl; }
	virtual void fb2() { cout << "B2.fb2" << endl; }
};

// B3 has two virtual member functions fb and fb3
class B3
{
	POINTER b3;
public:
	B3() : b3(0) {}
	virtual void fb() { cout << "B3.fb" << endl; }
	virtual void fb3() { cout << "B3.fb3" << endl; }
};

// D1 derived from B2 and has no self virtual member functions
class D1 : public B2
{
	POINTER d1;
public:
	D1() : d1(0) {}
};

// D2 derived from B2 and has one self virtual member function fb
class D2 : public B2
{
	POINTER d2;
public:
	D2() : d2(0) {}
	virtual void fb() override { cout << "D2.fb" << endl; }
};

// D3 derived from B2 and has two self virtual member functions fb and fc
class D3 : public B2
{
	POINTER d3;
public:
	D3() : d3(0) {}
	virtual void fb() override { cout << "D3.fb" << endl; }
	virtual void fc() { cout << "D3.fc" << endl; }
};

// D4 derived from B1 and has one self virtual member function fc
class D4 : public B1
{
	POINTER d4;
public:
	D4() : d4(0) {}
	virtual void fc() { cout << "D4.fc" << endl; }
};

// D11 derived from B2 and B3 and has no self virtual member functions
class D11 : public B2, public B3
{
	POINTER d11;
public:
	D11() : d11(0) {}
};

// D12 derived from B2 and B3 and has one self virtual member function fb
class D12 : public B2, public B3
{
	POINTER d12;
public:
	D12() : d12(0) {}
	virtual void fb() override { cout << "D12.fb" << endl; }
};

// D13 derived from B2 and B3 and has two self virtual member functions fb and fc
class D13 : public B2, public B3
{
	POINTER d13;
public:
	D13() : d13(0) {}
	virtual void fb() override { cout << "D13.fb" << endl; }
	virtual void fc() { cout << "D13.fc" << endl; }
};

// D14 derived from B1, B2 and B3 and has two self virtual member functions fb and fc
class D14 : public B1, public B2, public B3
{
	POINTER d14;
public:
	D14() : d14(0) {}
	virtual void fb() override { cout << "D14.fb" << endl; }
	virtual void fc() { cout << "D14.fc" << endl; }
};

//void PrintPointer(POINTER* pointer)
//{
//	cout << "0x" << setfill('0') << setw(sizeof(POINTER) * 2) << pointer;
//}

#define PRINT_POINTER(pointer) "0x" << setfill('0') << setw(sizeof(POINTER) * 2) << hex << pointer

typedef void (*FUNC)();

inline void PrintVFTable(POINTER* table)
{
	if (!table) return;
	for (int i = 0; (table[i] & 0xFFFFFFFF) != 0; i++)
	//for (int i = 0; table[i] != 0; i++)
	{
		cout << i << ": " << PRINT_POINTER(table[i]) << " ";
		FUNC func = (FUNC)table[i];
		func();
	}
}

//inline void PrintVFTable(POINTER* table, const char* name)
//{
//	cout << "Begin Print VFTable of " << name << ": " << PRINT_POINTER(table) << endl;
//	PrintVFTable(table);
//	cout << "End Print VFTable" << endl << endl;
//}

#define TEST(Pointer, Target) \
	cout << "Begin Print VFTable of " << Target << ": " << PRINT_POINTER(Pointer) << endl; \
	PrintVFTable((POINTER*)(*(POINTER*)Pointer)); \
	cout << "End Print VFTable" << endl << endl;

#define TEST_CLASS(Class, Name) \
	Class Name; \
	TEST(&Name, #Class)

#define TEST_POINTER(Pointer, Class, Name) \
	Pointer = &Name; \
	TEST(Pointer, #Pointer" of "#Class)

#define TEST_SINGLE(Pointer, Class, Name) \
	Class Name; \
	TEST_POINTER(Pointer, Class, Name)

#define TEST_TUPLE(Pointer1, Pointer2, Class, Name) \
	Class Name; \
	TEST_POINTER(Pointer1, Class, Name) \
	TEST_POINTER(Pointer2, Class, Name)

#define TEST_TRIPLE(Pointer1, Pointer2, Pointer3, Class, Name) \
	Class Name; \
	TEST_POINTER(Pointer1, Class, Name) \
	TEST_POINTER(Pointer2, Class, Name) \
	TEST_POINTER(Pointer3, Class, Name)

int main()
{
	B1* pb1;
	B2* pb2;
	B3* pb3;

	// B2
	TEST_CLASS(B2, b2);

	// SINGLE
	TEST_SINGLE(pb2, D1, d1);
	TEST_SINGLE(pb2, D2, d2);
	TEST_SINGLE(pb2, D3, d3);
	TEST_CLASS(D4, d4);
	TEST_POINTER(pb1, D4, d4);

	// TUPLE
	TEST_TUPLE(pb2, pb3, D11, d11);
	TEST_TUPLE(pb2, pb3, D12, d12);
	TEST_TUPLE(pb2, pb3, D13, d13);

	// TRIPLE
	TEST_TRIPLE(pb1, pb2, pb3, D14, d14);
}