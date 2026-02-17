#include <iostream>
#include <vector>
#include <thread>
#include <mutex>


/*
 CONSIDERACIÓN A TENER EN CUENTA:
 He notado que el enunciado dice que el balance final debe ser 1000 - (10 * 50) = 500, pero creería que  esa fórmula
 correspondería a solo retiros de 50, no a la secuencia de depósito y retiro que noto en el código de ejemplo.

 Por tanto, intuyo que es probable que haya un error en el enunciado al escribir la expresión espresada.
 */

// Mutex global para proteger operaciones
std::mutex operationMutex;

// Clase Account
class Account {
private:
    std::string id;
    double balance;

public:
    Account(std::string id, double initialBalance): id(id), balance(initialBalance) {}

    double getBalance() const { return balance; }
    
    void deposit(double amount) { balance += amount; }
    void withdraw(double amount) { balance -= amount; }
};

// Clase Transaction
class Transaction {
private:
    Account* account;
    double amount;

public:
    Transaction(Account* account, double amount): account(account), amount(amount) {}
};

// Singleton - TransactionManager
class TransactionManager {
private:
    static TransactionManager* instance;

    TransactionManager() {}

public:
    // Obtener instancia única (le agregué mutex para asegurar thread-safe)
    static TransactionManager& getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(operationMutex);
            if (instance == nullptr) {
                instance = new TransactionManager();
            }
        }
        return *instance;
    }

    // Operación atómica de depósito
    void deposit(Account* account, double amount) {
        std::lock_guard<std::mutex> lock(operationMutex);
        account->deposit(amount);
    }

    // Operación atómica de retiro
    void withdraw(Account* account, double amount) {
        std::lock_guard<std::mutex> lock(operationMutex);
        account->withdraw(amount);
    }

    // Eliminar copia y movimiento
    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;
};

// Inicialización de miembros estático
TransactionManager* TransactionManager::instance = nullptr;


int main() {
    auto& manager = TransactionManager::getInstance();
    Account account("A1", 1000);
    
    std::vector<std::thread> threads;
    
    // Creación de 10 threads que realizan operaciones
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            manager.deposit(&account, 100);
            manager.withdraw(&account, 50);
        });
    }
    
    // Esperamos a que terminen todos los threads
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Balance final: " << account.getBalance() << std::endl;
    std::cout << "Direccion manager: " << &TransactionManager::getInstance() << std::endl;
    
    return 0;
}