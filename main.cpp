#include <iostream>
#include <random>
#include <ctime>
#include <iomanip>


struct CurrencyPair {
    std::string name;
    double rate;
};


struct Balance {
    double rub = 0.0;
    double usd = 0.0;
    double eur = 0.0;
    double usdt = 0.0;
    double btc = 0.0;
};


void adjustRate(CurrencyPair &pair) {
    std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<double> distribution(-0.05, 0.05);
    double change = distribution(generator);
    pair.rate += pair.rate * change;
    std::cout << "Новый курс " << pair.name << ": " << std::fixed << std::setprecision(6) << pair.rate << std::endl;
}

bool buyCurrency(Balance &user, Balance &terminal, CurrencyPair &pair, std::string buy_currency, std::string sell_currency, double amount) {
    double rate = pair.rate;
    double amount_to_sell = amount * rate;

    if (sell_currency == "RUB" && user.rub < amount_to_sell) {
        std::cout << "Недостаточно RUB на счете пользователя." << std::endl;
        return false;
    } else if (sell_currency == "USD" && user.usd < amount_to_sell) {
        std::cout << "Недостаточно USD на счете пользователя." << std::endl;
        return false;
    }


    if (buy_currency == "USD" && terminal.usd < amount) {
        std::cout << "Недостаточно USD на счете терминала." << std::endl;
        return false;
    } else if (buy_currency == "EUR" && terminal.eur < amount) {
        std::cout << "Недостаточно EUR на счете терминала." << std::endl;
        return false;
    } else if (buy_currency == "USDT" && terminal.usdt < amount) {
        std::cout << "Недостаточно USDT на счете терминала." << std::endl;
        return false;
    } else if (buy_currency == "BTC" && terminal.btc < amount) {
        std::cout << "Недостаточно BTC на счете терминала." << std::endl;
        return false;
    }

    if (sell_currency == "RUB") {
        user.rub -= amount_to_sell;
    } else if (sell_currency == "USD") {
        user.usd -= amount_to_sell;
    }

    if (buy_currency == "USD") {
        user.usd += amount;
        terminal.usd -= amount;
    } else if (buy_currency == "EUR") {
        user.eur += amount;
        terminal.eur -= amount;
    } else if (buy_currency == "USDT") {
        user.usdt += amount;
        terminal.usdt -= amount;
    } else if (buy_currency == "BTC") {
        user.btc += amount;
        terminal.btc -= amount;
    }

    std::cout << "Покупка завершена успешно!" << std::endl;
    return true;
}


bool sellCurrency(Balance &user, Balance &terminal, CurrencyPair &pair, std::string sell_currency, std::string buy_currency, double amount) {
    double rate = pair.rate;
    double amount_to_buy = amount * rate;


    if (sell_currency == "USD" && user.usd < amount) {
        std::cout << "Недостаточно USD на счете пользователя." << std::endl;
        return false;
    } else if (sell_currency == "EUR" && user.eur < amount) {
        std::cout << "Недостаточно EUR на счете пользователя." << std::endl;
        return false;
    } else if (sell_currency == "USDT" && user.usdt < amount) {
        std::cout << "Недостаточно USDT на счете пользователя." << std::endl;
        return false;
    } else if (sell_currency == "BTC" && user.btc < amount) {
        std::cout << "Недостаточно BTC на счете пользователя." << std::endl;
        return false;
    }

    if (buy_currency == "RUB" && terminal.rub < amount_to_buy) {
        std::cout << "Недостаточно RUB на счете терминала." << std::endl;
        return false;
    }

    if (sell_currency == "USD") {
        user.usd -= amount;
    } else if (sell_currency == "EUR") {
        user.eur -= amount;
    } else if (sell_currency == "USDT") {
        user.usdt -= amount;
    } else if (sell_currency == "BTC") {
        user.btc -= amount;
    }

    if (buy_currency == "RUB") {
        user.rub += amount_to_buy;
        terminal.rub -= amount_to_buy;
    }

    std::cout << "Продажа завершена успешно!" << std::endl;
    return true;
}

void showBalances(const Balance &user, const Balance &terminal) {
    std::cout << "\nБаланс пользователя: RUB: " << user.rub << ", USD: " << user.usd << ", EUR: " << user.eur
              << ", USDT: " << user.usdt << ", BTC: " << user.btc << std::endl;
    std::cout << "Баланс терминала: RUB: " << terminal.rub << ", USD: " << terminal.usd << ", EUR: " << terminal.eur
              << ", USDT: " << terminal.usdt << ", BTC: " << terminal.btc << std::endl;
}

int main() {
    setlocale(LC_ALL, "Rus");
    CurrencyPair rub_usd = {"RUB/USD", 90.0};
    CurrencyPair rub_eur = {"RUB/EUR", 100.0};
    CurrencyPair usd_eur = {"USD/EUR", 1.1};
    CurrencyPair usd_usdt = {"USD/USDT", 1.0};
    CurrencyPair usd_btc = {"USD/BTC", 40000.0};


    Balance user = {1000000, 0, 0, 0, 0};
    Balance terminal = {10000, 1000, 1000, 1000, 1.5};

    int choice;
    double amount;
    int operation_choice;


    while (true) {
        showBalances(user, terminal);


        std::cout << "\nВыберите валютную пару для операции:\n"
                  << "1. RUB/USD\n2. RUB/EUR\n3. USD/EUR\n4. USD/USDT\n5. USD/BTC\n6. Выйти\nВаш выбор: ";
        std::cin >> choice;

        if (choice == 6) {
            std::cout << "Завершение работы." << std::endl;
            break;
        }

        std::cout << "Выберите операцию:\n1. Купить\n2. Продать\nВаш выбор: ";
        std::cin >> operation_choice;

        std::cout << "Введите сумму для операции: ";
        std::cin >> amount;

        switch (choice) {
            case 1:
                if (operation_choice == 1) {
                    if (buyCurrency(user, terminal, rub_usd, "USD", "RUB", amount)) adjustRate(rub_usd);
                } else if (operation_choice == 2) {
                    if (sellCurrency(user, terminal, rub_usd, "USD", "RUB", amount)) adjustRate(rub_usd);
                }
                break;
            case 2:
                if (operation_choice == 1) {
                    if (buyCurrency(user, terminal, rub_eur, "EUR", "RUB", amount)) adjustRate(rub_eur);
                } else if (operation_choice == 2) {
                    if (sellCurrency(user, terminal, rub_eur, "EUR", "RUB", amount)) adjustRate(rub_eur);
                }
                break;
            case 3:
                if (operation_choice == 1) {
                    if (buyCurrency(user, terminal, usd_eur, "EUR", "USD", amount)) adjustRate(usd_eur);
                } else if (operation_choice == 2) {
                    if (sellCurrency(user, terminal, usd_eur, "EUR", "USD", amount)) adjustRate(usd_eur);
                }
                break;
            case 4:
                if (operation_choice == 1) {
                    if (buyCurrency(user, terminal, usd_usdt, "USDT", "USD", amount)) adjustRate(usd_usdt);
                } else if (operation_choice == 2) {
                    if (sellCurrency(user, terminal, usd_usdt, "USDT", "USD", amount)) adjustRate(usd_usdt);
                }
                break;
            case 5:
                if (operation_choice == 1) {
                    if (buyCurrency(user, terminal, usd_btc, "BTC", "USD", amount)) adjustRate(usd_btc);
                } else if (operation_choice == 2) {
                    if (sellCurrency(user, terminal, usd_btc, "BTC", "USD", amount)) adjustRate(usd_btc);
                }
                break;
            default:
                std::cout << "Неверный выбор!" << std::endl;
        }
    }

    return 0;
}
