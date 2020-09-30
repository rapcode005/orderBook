// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <boost/algorithm/string.hpp>

using namespace std;

typedef struct orderBook {
    int ID;
    string countID;
    string action;
    double price;
    int stock;
};

class OrderBookJob {
public:
    orderBook curOrderBookItem;
    int hasSell() {
        int indexSell = -1;
        //list<orderBook>::iterator it = m_sell.begin();
        const int &buyStock = curOrderBookItem.stock;
        auto iterS = find_if(m_sell.begin(), m_sell.end(),
            [=, &indexSell](orderBook tsS) { //Check if price has sell.
                indexSell++;
                return tsS.price == curOrderBookItem.price;
            }
        );

        if (iterS != m_sell.end()) { 
            return indexSell;
        }
        else
            return -1;
    }
    void editSellItem(const int& index) {
        list<orderBook>::iterator sellI = m_sell.begin();
        advance(sellI, index);
        const int& curSt = sellI->stock;
        sellI->stock = curSt + curOrderBookItem.stock;
    }
    void addNewSellItem() {
        sellID++;
        curOrderBookItem.ID = sellID;
        m_sell.push_back(curOrderBookItem);
    }

    int hasBuy() {
        int indexBuy = -1;
        const int& buyStock = curOrderBookItem.stock;
        auto iterS = find_if(m_buy.begin(), m_buy.end(),
            [=, &indexBuy](orderBook ts) { //Check if price has sell.
                indexBuy++;
                return ts.price == curOrderBookItem.price;
            }
        );

        if (iterS != m_buy.end()) { 
            return indexBuy;
        }
        else
            return -1;
    }
    void editBuyItem(const int& index) {
        list<orderBook>::iterator buyI = m_buy.begin();
        advance(buyI, index);
        const int& curSt = buyI->stock;
        buyI->stock = curSt + curOrderBookItem.stock;
    }
    void addNewBuyItem() {
        buyID++;
        curOrderBookItem.ID = buyID;
        m_buy.push_back(curOrderBookItem);
    }

    void transaction() {
        const int& inBuy = hasBuy();
        const int& inSell = hasSell();
        if (inSell >= 0 && inBuy >= 0) {
            list<orderBook>::iterator itB = m_buy.begin();
            list<orderBook>::iterator itS = m_sell.begin();

            advance(itB, inBuy);
            advance(itS, inSell);

            const double& stockBuy = itB->stock;
            const double& stockSell = itS->stock;
            const double& total = stockBuy - stockSell;

            if (total == 0) {
                m_buy.erase(itB);
                m_sell.erase(itS);
            }
            else if (total > 0) {
                m_sell.erase(itS);
                itB->stock = total;
            }
            else {
                m_buy.erase(itB);
                itS->stock = -1 * (total);
            }
        }
    }

    void showRecords() {
        for (list<orderBook>::iterator i = m_buy.begin(); i != m_buy.end(); ++i) {
            cout << i->ID << "|" << i->countID << "|" << i->action << "|" << i->price << "|" << i->stock << "\n";
        }
        for (list<orderBook>::iterator i = m_sell.begin(); i != m_sell.end(); ++i) {
            cout << i->ID << "|" << i->countID << "|" << i->action << "|" << i->price << "|" << i->stock << "\n";
        }
        cout << "\n\n";
    }
private:
    list<orderBook> m_buy, m_sell;
    int buyID = 0, sellID = 0;
};

int main()
{
    fstream readFile;
    readFile.open("test-orders.log", ios::in);
    vector<string> results;

    OrderBookJob obJob;

    if (readFile.is_open()) {
        string tp;
        while (getline(readFile, tp)) {
            boost::split(results, tp, [](char c) {return c == '|'; });
            obJob.curOrderBookItem.countID = results[0];
            obJob.curOrderBookItem.action = results[1];
            obJob.curOrderBookItem.price = stod(results[2]);
            stringstream strStock(results[3]);
            int stock = 0;
            strStock >> stock;
            obJob.curOrderBookItem.stock = stock;
            if (obJob.curOrderBookItem.action == "B") {
                const int& indBuy = obJob.hasBuy();
                if (indBuy >= 0) {
                    obJob.editBuyItem(indBuy);
                }
                else {
                    obJob.addNewBuyItem();
                }
            }
            else {
                const int& indSell = obJob.hasSell();
                if (indSell >= 0) {
                    obJob.editSellItem(indSell);
                }
                else {
                    obJob.addNewSellItem();
                }
            }

            cout << "Before Transaction\n";
            obJob.showRecords();

            obJob.transaction();

            cout << "After Transaction\n";
            obJob.showRecords();
        }
        readFile.close();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
