#include <string>
#include <iostream>

using std::cin;
using std::cout;
using std::cerr;
using std::string;

struct Sales_data {
	string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

int main() {
	Sales_data data1, data2;
	double price;
	cin >> data1.bookNo >> data1.units_sold >> price;
	data1.revenue = price * data1.units_sold;
	cin >> data2.bookNo >> data2.units_sold >> price;
	data2.revenue = price * data2.units_sold;

	if (data1.bookNo == data2.bookNo) {
		unsigned totalSold = data1.units_sold + data2.units_sold;
		double totalRevenue = data1.revenue + data2.revenue;
		cout << data1.bookNo << " " << totalSold
			<< " " << totalRevenue << " ";
		if (totalSold != 0)
			cout << totalRevenue / totalSold << std::endl;
		else
			cout << "(no sales)" << std::endl;

		return 0;
	}
	else {
		cerr << "Data must refer to the same ISBN" << std::endl;
		return -1;
	}
}