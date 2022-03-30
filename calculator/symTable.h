#pragma once
#include <QMap>
#include "ast.h"

template <typename Symbol_t>
class SymTable_t
{
public:
    using Item_t = QMap<QString, Symbol_t>;
    using iterator = typename Item_t::iterator;
    using const_iterator = typename Item_t::const_iterator;
    using size_type = typename Item_t::size_type;
    using difference_type = typename Item_t::difference_type;
    // using reverse_iterator = typename Item_t::reverse_iterator;
    // using const_reverse_iterator = typename Item_t::const_reverse_iterator;
protected:
    Item_t table;

public:
    SymTable_t() {}
    ~SymTable_t() {}

    inline iterator begin() { return this->table.begin(); }
    inline iterator end() { return this->table.end(); }

    inline const_iterator begin() const { return this->table.begin(); }
    inline const_iterator end() const { return this->table.end(); }

    inline size_type size() { return this->table.size(); }
    inline size_type len() { return this->table.size(); }

    bool insert(const QString& symName, const Symbol_t sym)
    {
        if (this->table.find(symName) != this->table.end())
            return false;
        this->table.insert(symName, sym);
        return true;
    }

    bool remove(const QString& symName)
    {
        auto it = this->table.find(symName);
        if (it != this->table.end())
        {
            this->table.erase(it);
            return true;
        }
        return false;
    }

    bool search(const QString& symName, Symbol_t& sym) const
    {
        auto it = this->table.find(symName);
        if (it != this->table.end())
        {
            sym = it.value();
            return true;
        }
        return false;
    }
};

// class SigSymTable_t :public SymTable_t<QListWidgetItem*>
// {
// private:

// protected:

// public:
//     SigSymTable_t() {}
//     ~SigSymTable_t()
//     {
//         for (auto it : this->table)
//             delete it;
//     }
// };

using FunSymTable_t = SymTable_t<ASTFunctionCall_t::calFunc_t>;
using SigSymTable_t = SymTable_t<QListWidgetItem*>;

extern SigSymTable_t SigSymTable;
extern FunSymTable_t FunSymTable;
