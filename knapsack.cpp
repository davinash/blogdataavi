#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

struct item {
	int value;
	int capacity;
};	

class Knapsack {
public:
	Knapsack(int knapsack_size, int item_size);
	~Knapsack();
	void add_items(int value, int capacity);
	int  solve();
	void get_items_selected(std::vector<item>& resultItems);

	friend std::ostream &operator <<( std::ostream&, const Knapsack& );
private:
	void init_knapsack_table();
	std::vector<item> m_items;
	int m_knapsack_size;
	int m_item_size;
	// 2D matrix to store intermediate 
	// result of the knapsack calculation.
	std::vector<std::vector<int> > m_knapsack_table;
	// 2D matrix to store the intermediate
	// result for which item is selected
	// Later this matrix is used to get which items
	// are selected for optimal calculation.
	std::vector<std::vector<int> > m_selection_table;
protected:
};

Knapsack::Knapsack(int knapsack_size, int item_size):
                   m_knapsack_size(knapsack_size),
				   m_item_size(item_size),
				   m_knapsack_table(item_size + 1,std::vector<int>(knapsack_size + 1)),
				   m_selection_table(item_size + 1,std::vector<int>(knapsack_size + 1)){
	m_items.reserve(m_item_size);
}
void Knapsack::add_items(int value, int capacity) {
	item t;
	t.value = value;
	t.capacity = capacity;
	m_items.push_back(t);
}
int Knapsack::solve() {
	// Initialize the first row in both the
	// tables, these values are used as default
	// as if no items are selected and no capacity
	// is available.
	// This is the default case for bottom-up approach.
	for ( int i = 0; i < m_knapsack_size + 1 ; i++) {
		m_knapsack_table [0][i]  = 0;
		m_selection_table[0][i]  = 0;
	}
	int row = 1;
	for ( std::vector<item>::iterator itemIterator = m_items.begin();
		                              itemIterator != m_items.end();
		                              ++itemIterator) {
		item currentItem = *itemIterator;
		int col = 0; // col is capacity available.
		while ( col < m_knapsack_size + 1 ) {
			//1. Check if item can be fit by it's own.
			if ( currentItem.capacity > col ) {
				//2. Get the solution for the already solved
				//   knapsack problem.
				m_knapsack_table[row][col] = m_knapsack_table[row - 1][col];
				// Update the selection table as we are not able to accomodate this item
				// eventually we are not selecting this ite.
				m_selection_table[row][col] = 0;
			} else {
				// We are now considering the item.
				int capacity_remaining = col - currentItem.capacity;
				int new_value  = currentItem.value + m_knapsack_table[row - 1][capacity_remaining];
				int prev_value = m_knapsack_table[row - 1][col];
				if ( prev_value >= new_value) {
					// There is no gain here to consider this item.
					m_knapsack_table[row][col] = m_knapsack_table[row - 1][col];
					m_selection_table[row][col] = 0;
				} else {
					// Add this item into the knapsack.
					m_knapsack_table[row][col]  = new_value;
					// Update the selection table as we are considering
					// this item.
					m_selection_table[row][col] = 1;
				}
			}
			col++;
		}
		row++;
	}
	return m_knapsack_table[m_item_size][m_knapsack_size];
}

void Knapsack::get_items_selected(std::vector<item>& resultItems) {
	int row = m_item_size;
	int col = m_knapsack_size;
	int cap = m_knapsack_size;
	while ( cap > 0 ) {
		if ( m_selection_table[row][col] == 1) {
			resultItems.push_back(m_items[row - 1]);
			cap = cap - m_items[row - 1].capacity;
			col = cap;
		} 			
		row = row - 1;
	}
}

std::ostream &operator << (std::ostream &out, const Knapsack &knp ) {
	out << std::endl;
	out << "SOLUTION MATRIX" << std::endl << std::endl;
	out << std::setw(15) << "Capacity |";
	for ( int i = 0; i <= knp.m_knapsack_size; i++) {
		out << std::setw(5) << i ;
	}
	out << std::endl;
	out << std::endl;

	int row = 0;
	out << std::setw(15) << "NONE |";
	int col = 0;
	while ( col < knp.m_knapsack_size + 1 ) {
		out << std::setw(5) << knp.m_knapsack_table[row][col];
		col++;
	}
	out << std::endl;
	row++;
	for ( std::vector<item>::const_iterator itemIterator = (knp.m_items).begin();
		                              itemIterator != knp.m_items.end();
		                              ++itemIterator) {
		out << "(V:" 
			<< std::setw(2) 
			<< itemIterator->value 
			<< ", " 
			<< "W:" 
			<< itemIterator->capacity 
			<< ")" 
				<< std::setw(4) 
			<< "|" ;
		col = 0;
		while ( col < knp.m_knapsack_size + 1 ) {
			out << std::setw(5) << knp.m_knapsack_table[row][col];
			col++;
		}
		row++;
		out << std::endl;
	}

	out << std::endl;
	out << "SELECTION MATRIX" << std::endl << std::endl;
	out << std::setw(15) << "Capacity |";
	for ( int i = 0; i <= knp.m_knapsack_size; i++) {
		out << std::setw(5) << i ;
	}
	out << std::endl;
	out << std::endl;

	row = 0;
	out << std::setw(15) << "NONE |";
	col = 0;
	while ( col < knp.m_knapsack_size + 1 ) {
		out << std::setw(5) << knp.m_knapsack_table[row][col];
		col++;
	}
	out << std::endl;
	row++;
	for ( std::vector<item>::const_iterator itemIterator = (knp.m_items).begin();
		                              itemIterator != knp.m_items.end();
		                              ++itemIterator) {
		out << "(V:" 
			<< std::setw(2) 
			<< itemIterator->value 
			<< ", " 
			<< "W:" 
			<< itemIterator->capacity 
			<< ")" 
			<< std::setw(4) 
			<< "|" ;
		col = 0;
		while ( col < knp.m_knapsack_size + 1 ) {
			out << std::setw(5) << knp.m_selection_table[row][col];
			col++;
		}
		row++;
		out << std::endl;
	}
	return out;
}

Knapsack::~Knapsack() {
}

int main ( int argc, char **argv) {
	Knapsack knp(18,7);
	knp.add_items(12,4 );
	knp.add_items(10,6 );
	knp.add_items(8 ,5 );
	knp.add_items(11,7 );
	knp.add_items(14,3 );
	knp.add_items(7 ,1 );
	knp.add_items(9 ,6 );
	int solution  = knp.solve();
	std::cout << knp;
	std::vector<item> resultItems;
	knp.get_items_selected(resultItems);

	std::cout << "SOLUTION" << std::endl;
	std::cout << '\t' << "Value : " << solution << std::endl;
	std::cout << '\t' << "Items : " << std::endl;
	for ( std::vector<item>::iterator itr = resultItems.begin();
								      itr != resultItems.end();
									  ++itr) {
		std::cout	<< '\t' << '\t' << "(V:" 
					<< std::setw(2) 
					<< itr->value 
					<< ", " 
					<< "W:" 
					<< itr->capacity 
					<< ")" 
					<< std::endl;
	}
	return 0;
}
