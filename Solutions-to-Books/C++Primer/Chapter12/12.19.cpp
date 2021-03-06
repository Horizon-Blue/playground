// Exercise 12.19
// Define your own version of StrBlobPtr and update your StrBlob class with the
// appropriate friend declaration and begin and end members.
// Xiaoyan Wang 8/4/2016
#ifndef EX_12_19_CPP
#define EX_12_19_CPP // I usd .cpp instead of .h just for consistency of the names
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <exception>
// #include <iostream> // use for testing purpose
using std::vector;
using std::string;

class StrBlobPtr;

// class StrBlob is mostly copied from 12.02.cpp
class StrBlob {
	friend StrBlobPtr;
public:
	typedef vector<string>::size_type size_type;

	// constructors
	StrBlob(): data(std::make_shared<vector<string>>()) { }
	StrBlob(std::initializer_list<string> il): data(std::make_shared<vector<string>>(il)) { }

	size_type size() const { return data -> size(); }
	bool empty() const { return data -> empty(); }
	// NEW: begin and end operations that return pointer to itself
	StrBlobPtr begin();
	StrBlobPtr end();
	
	// add and remove elements
	void push_back(const string &t) { data -> push_back(t); }
	void pop_back();

	// element access
	string& front();
	string& back();
	const string& front() const;
	const string& back() const;

private:
	std::shared_ptr<vector<string>> data;
	// throw msg if data[i] isn't valid
	void check(size_type i, const string &msg) const {
		if (i >= data -> size())
			throw std::out_of_range(msg);
	}
};

class StrBlobPtr {
public:
	// constructors
	StrBlobPtr(): curr(0) { }
	StrBlobPtr(StrBlob &sb, std::size_t sz = 0): dataptr(sb.data), curr(sz) { }

	string& deref() const;
	StrBlobPtr& incr();

	friend bool isequal(const StrBlobPtr &sbptr1, const StrBlobPtr &sbptr2);

private:
	// helper function
	std::shared_ptr<vector<string>> check(std::size_t pos, const string &msg) const;
	// data
	std::weak_ptr<vector<string>> dataptr;
	std::size_t curr;
};


void StrBlob::pop_back() {
	check(0, "pop_back on empty StrBlob");
	data -> pop_back();
}

string& StrBlob::front() {
	check(0, "front on empty StrBlob");
	return data -> front();
}

string& StrBlob::back() {
	check(0, "back on empty StrBlob");
	return data -> back();
}

const string& StrBlob::front() const {
	check(0, "front on empty StrBlob");
	return data -> front();
}

const string& StrBlob::back() const {
	check(0, "back on empty StrBlob");
	return data -> back();
}

inline StrBlobPtr StrBlob::begin() {
	return StrBlobPtr(*this);
}

inline StrBlobPtr StrBlob::end() {
	return StrBlobPtr(*this, data->size());
}

string& StrBlobPtr::deref() const {
	auto p = check(curr, "dereference past end");
	return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr() {
	auto p = check(curr, "increment past end of StrBlobPtr");
	++curr;
	return *this;
}

std::shared_ptr<vector<string>> StrBlobPtr::check(std::size_t pos, const string &msg) const {
	// test to see whether the vector pointed by the weak_ptr is still around
	auto ptr = dataptr.lock();
	if (!ptr)
		throw std::runtime_error("unbound StrBlobPtr");
	if (pos >= ptr->size())
		throw std::out_of_range(msg);
	return ptr;
}

bool isequal(const StrBlobPtr &sbptr1, const StrBlobPtr &sbptr2) {
	auto ptr1 = sbptr1.dataptr.lock(), ptr2 = sbptr2.dataptr.lock();
	if(ptr1 == ptr2)
		return (!ptr1 || sbptr1.curr == sbptr2.curr);
	else
		return false;
}

#endif

// // testing program
// int main() {
// 	StrBlob sb({"How", "are", "you", "today?"});
// 	StrBlobPtr sbptr(sb);
// 	while(true) {
// 		std::cout << sbptr.deref() << std::endl;
// 		sbptr.incr();
// 	}
// 	return 0;
// }