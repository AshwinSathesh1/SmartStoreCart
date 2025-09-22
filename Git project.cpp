#include <iostream>
#include <string>
using namespace std;

//Product Base Class
class Product {
protected:
    int id;
    string cust;
    string productType;

public:
    Product(int i, string c, string type) : id(i), cust(c), productType(type) {}
};

//Base Discountable Class
class Discountable {
protected:
    float discountPercent;

public:
	//Parameterized Constructor to get discount value
    Discountable(float disc = 0) {
    discountPercent = disc;
}
    //apply discount
    void applyDiscount(float percent) {
        discountPercent = percent;
        cout << "\nApplied " << percent << "% discount";
    }
	
    float getDiscountPercent() const {
        return discountPercent;
    }
};


class Item : public Product {
protected:
    string itemName;
    float price;
    int quantity;

    static string bookCatalog[9];
    static int bookPrices[9];
    static string groceryCatalog[7];
    static int groceryPrices[7];
    
	//Check Prices
    float lookupPrice(string type, string item) {
        if (type == "Books" || type == "books") {
            for (int k = 0; k < 9; ++k)
                if (item == bookCatalog[k]) return bookPrices[k];
        } else if (type == "Grocery" || type == "grocery") {
            for (int k = 0; k < 7; ++k)
                if (item == groceryCatalog[k]) return groceryPrices[k];
        }
        return -1;
    }

public:
    Item(int i, string c, string type, string item, int q) : Product(i, c, type) {
        itemName = item;
        quantity = q;
        price = lookupPrice(productType, itemName);
    }
     //function to show details
    void showDetails() const {
        if (price < 0) {
            cout << "\nProduct not available\n";
            return;
        }
        cout << "\nCustomer Name : " << cust;
        cout << "\nProduct Type  : " << productType;
        cout << "\nItem Selected : " << itemName;
        cout << "\nQuantity      : " << quantity;
        cout << "\nPrice per item: $" << price;
    }
      //Get Subtotal
    float getSubtotal() const {
        return (quantity > 0 && price > 0) ? price * quantity : 0;
    }
       // Function to show catalog
    static void showCatalog(string type) {
        if (type == "Books" || type == "books") {
            cout << "\nAvailable Books:\n";
            for (int i = 0; i < 9; ++i)
                cout << "- " << bookCatalog[i] << " ($" << bookPrices[i] << ")\n";
        } else if (type == "Grocery" || type == "grocery") {
            cout << "\nAvailable Grocery Items:\n";
            for (int i = 0; i < 7; ++i)
                cout << "- " << groceryCatalog[i] << " ($" << groceryPrices[i] << ")\n";
        }
    }
};

//Discounted item class derived from item and discountable
class DiscountedItem : public Item, public Discountable {
public:
    DiscountedItem(int i, string c, string type, string item, int q, float disc = 0)
        : Item(i, c, type, item, q), Discountable(disc) {}

    DiscountedItem() : Item(0, "Guest", "Books", "", 0), Discountable(0) {}

    float getFinalPrice() const {
        float subtotal = getSubtotal();
        float discountAmount = (subtotal * getDiscountPercent()) / 100;
        return subtotal - discountAmount;
    }
};


class Cart {
private:
    DiscountedItem items[20];
    int count;
    float discountPercent;

public:
    Cart() : count(0), discountPercent(0) {}
    
	//Overloaded += operator to add items into cart
    void operator+=(DiscountedItem& item) {
        if (count < 20)
            items[count++] = item;
        else
            cout << "\nCart is full.";
    }
		//to apply discount
    void operator-() {
        discountPercent = 10;
        cout << "\nCart-wide 10% discount applied.";
    }

    void showCart() const {
        if (count == 0) {
            cout << "\nCart is empty.\n";
            return;
        }

        cout << "\n--- Cart Contents ---\n";
        for (int i = 0; i < count; ++i) {
            items[i].showDetails();
            cout << "\nSubtotal: $" << items[i].getSubtotal();
            cout << "\nFinal (after item discount): $" << items[i].getFinalPrice() << "\n\n";
        }
    }
   //Functio to calculate original total
    float getOriginalTotal() const {
        float total = 0;
        for (int i = 0; i < count; ++i)
            total += items[i].getFinalPrice();
        return total;
    }
    //function to calculate total after discount
    float getFinalTotal() const {
        float total = getOriginalTotal();
        float discountAmount = (total * discountPercent) / 100;
        return total - discountAmount;
    }
};

     //Defining static arrays
string Item::bookCatalog[9] = {
    "Geronimo Stilton", "Harry Potter", "Wings of Fire",
    "The Alchemist", "Atomic Habits", "The Psychology of Money",
    "Rich Dad Poor Dad", "The Silent Patient", "I Am Malala"
};
int Item::bookPrices[9] = {100, 250, 300, 200, 180, 289, 349, 375, 254};

string Item::groceryCatalog[7] = {
    "Tata Salt", "Aashirvaad Atta", "Amul Milk",
    "Fortune Sunflower Oil", "India Gate Basmati Rice", "Maggi Noodles", "Detergent"
};
int Item::groceryPrices[7] = {27, 250, 30, 150, 130, 70, 225};

int main() {
    int id, quantity;
    string customerName, productType, itemName;
    Cart myCart;

    cout << " Welcome to the Store\n";
    cout << "------------------------\n";

    cout << "Enter customer ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter customer name: ";
    getline(cin, customerName);
     //Start item Entry loop
    char more;
    do {
        cout << "\nEnter product type (Books/Grocery): ";
        getline(cin, productType);

        Item::showCatalog(productType);

        cout << "\nEnter item name: ";
        getline(cin, itemName);

        cout << "Enter quantity: ";
        cin >> quantity;
        cin.ignore();

        DiscountedItem dprod(id, customerName, productType, itemName, quantity);
        dprod.showDetails();

        cout << "\nApply 10% discount to this item? (y/n): ";
        char apply;
        cin >> apply;
        cin.ignore();
        if (apply == 'y' || apply == 'Y') {
            dprod.applyDiscount(10);
        }

        myCart += dprod;

        cout << "\nAdd another item? (y/n): ";
        cin >> more;
        cin.ignore();

    } while (more == 'y' || more == 'Y');

    cout << "\n============================";
    cout << "\nFinal Cart Summary for " << customerName;
    cout << "\n============================";
    myCart.showCart();

    cout << "\nApply 10% discount to entire cart? (y/n): ";
    char applyCartDiscount;
    cin >> applyCartDiscount;
    if (applyCartDiscount == 'y' || applyCartDiscount == 'Y') {
        -myCart;
    }

    cout << "\nOriginal Cart Value : $" << myCart.getOriginalTotal();
    cout << "\nFinal Amount to Pay : $" << myCart.getFinalTotal() << "\n";

    return 0;
}
