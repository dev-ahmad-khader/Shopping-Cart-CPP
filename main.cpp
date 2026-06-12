
#include <iostream>
#include <string>
using namespace std;

// ===========================
//        (ANSI Colors)
// ===========================
#define RESET   "\033[0m"
#define RED     "\033[31m"     
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"     
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"     
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"


struct Phone {
    string model;
    float price;
    Phone* next;
};

class ReturnStack {
private:
    Phone* topPtr;
public:
    ReturnStack() { topPtr = NULL; }

    void push(string model, float price) {
        Phone* newPhone = new Phone;
        newPhone->model = model;
        newPhone->price = price;
        newPhone->next = topPtr;
        topPtr = newPhone;
    }

    Phone* pop() {
        if (topPtr == NULL) return NULL;
        Phone* temp = topPtr;
        topPtr = topPtr->next;
        return temp;
    }

    bool isEmpty() { return topPtr == NULL; }
};


class ShoppingCart {
private:
    Phone* head;
    ReturnStack undoStack;
    float discountRate; 

public:
    ShoppingCart() {
        head = NULL;
        discountRate = 0.0;
    }

    void insertPhone(string model, float price) {
        Phone* newPhone = new Phone;
        newPhone->model = model;
        newPhone->price = price;
        newPhone->next = head;
        head = newPhone;
        cout << GREEN << BOLD << "\n[+] Added " << model << " to your cart." << RESET << "\n";
    }

    void deletePhone(string targetModel) {
        if (head == NULL) {
            cout << RED << "\n[!] Cart is already empty." << RESET << "\n";
            return;
        }

        Phone* current = head;
        Phone* prev = NULL;

        while (current != NULL && current->model != targetModel) {
            prev = current;
            current = current->next;
        }

        if (current == NULL) {
            cout << RED << "\n[!] Phone not found in the cart." << RESET << "\n";
            return;
        }

        undoStack.push(current->model, current->price);

        if (prev == NULL) {
            head = current->next;
        }
        else {
            prev->next = current->next;
        }

        cout << YELLOW << "\n[-] Removed " << current->model << " from the cart." << RESET << "\n";
        delete current;
    }

    void undoDelete() {
        Phone* recovered = undoStack.pop();
        if (recovered == NULL) {
            cout << RED << "\n[!] No recently deleted items to restore." << RESET << "\n";
        }
        else {
            insertPhone(recovered->model, recovered->price);
            cout << BLUE << "[⏪] Restored from Return Stack." << RESET << "\n";
            delete recovered;
        }
    }

    void applyPromoCode(string code) {
        if (code == "ibtisam") {
            discountRate = 0.5; // خصم 50
            cout << GREEN << BOLD << "\n[$$$] Promo Code Accepted! 50% OFF applied to your cart." << RESET << "\n";
        }
        else if (code == "Elinor") {
            discountRate = 0.99; // خصم 99
            cout << GREEN << BOLD << "\n[$$$] Promo Code Accepted! 99% OFF applied to your cart." << RESET << "\n";
        }
        else {
            cout << RED << "\n[!] Invalid Promo Code." << RESET << "\n";
        }
    }

    void displayCart() {
        if (head == NULL) {
            cout << RED << "\n[!] Your Shopping Cart is empty." << RESET << "\n";
            return;
        }
        cout << BLUE << BOLD << "\n--- YOUR SHOPPING CART ---\n" << RESET;
        Phone* temp = head;
        float total = 0;
        int count = 1;
        while (temp != NULL) {
            cout << count << ". " << temp->model << " - $" << temp->price << endl;
            total += temp->price;
            temp = temp->next;
            count++;
        }
        cout << BLUE << "--------------------------\n" << RESET;

        if (discountRate > 0) {
            float discountAmount = total * discountRate;
            float finalTotal = total - discountAmount;
            cout << "Subtotal: $" << total << "\n";
            cout << GREEN <<

"Discount ("<<discountRate*100<<"%): -$" << discountAmount << RESET << "\n";
            cout << CYAN << BOLD << "Final Total: $" << finalTotal << RESET << "\n";
        }
        else {
            cout << CYAN << BOLD << "Total Price: $" << total << RESET << "\n";
        }
    }
};



int main() {
    ShoppingCart cart;
    int choice;
    string inputStr;
    float price;

    while (true) {
        cout << MAGENTA << BOLD << "\n======================================" << RESET;
        cout << CYAN << BOLD << "\n       * Ahmad's MOBILE Store * " << RESET;
        cout << MAGENTA << BOLD << "\n======================================" << RESET;

        cout << YELLOW << "\n--- SHOPPING CART ---" << RESET;
        cout << "\n[1] Insert Phone to Cart";
        cout << "\n[2] Delete Phone from Cart";
        cout << "\n[3] Undo Last Delete (Restore)";
        cout << "\n[4] Apply Promo Code";
        cout << "\n[5] Display Cart & Total";
        cout << "\n[0] Exit System";

        cout << MAGENTA << BOLD << "\n======================================" << RESET;
        cout << BOLD << "\nSelect an option: " << RESET;
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Phone Model: "; cin >> inputStr;
            cout << "Enter Price ($): "; cin >> price;
            cart.insertPhone(inputStr, price);
            break;
        case 2:
            cout << "Enter Phone Model to Delete: "; cin >> inputStr;
            cart.deletePhone(inputStr);
            break;
        case 3:
            cart.undoDelete();
            break;
        case 4:
            cout << "Enter Promo Code: "; cin >> inputStr;
            cart.applyPromoCode(inputStr);
            break;
        case 5:
            cart.displayCart();
            break;
        case 0:
            cout << CYAN << BOLD << "\nExiting System. Goodbye!\n" << RESET;
            return 0;
        default:
            cout << RED << "\n[!] Invalid choice.\n" << RESET;
        }
    }
}