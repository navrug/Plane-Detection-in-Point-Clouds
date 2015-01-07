#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <memory>
#include <map>

template <typename Key, typename Value>
class UnionFind
{
    struct Cell
    {
        Cell(const Value& v) :
            value(v) {}
        
        std::shared_ptr<Cell> parent;
        Value value;
    };
    
public:
    UnionFind()
    {
    }
    
    void append(const Key& key, const Value& value)
    {
        cells[key] = std::make_shared<Cell>(value);
    }

    Value at(const Key& key)
    {
        std::shared_ptr<Cell> c = find(key);
        if (c)
            return c->value;
        return Value();
    }
    
    void set(const Key& key, const Value& value)
    {
        std::shared_ptr<Cell> c = find(key);
        if (c)
            c->value = value;
    }
    
    void merge(const Key& k1, const Key& k2)
    {
        std::shared_ptr<Cell> root1 = find(k1);
        std::shared_ptr<Cell> root2 = find(k2);
        
        if (root1 != root2)
            root1->parent = root2;
    }
    
private:
    std::shared_ptr<Cell> find(const Key& key)
    {
        auto found = cells.find(key);
        if (found == cells.end())
            return std::shared_ptr<Cell>();
        std::shared_ptr<Cell> c = found->second;
        
        std::shared_ptr<Cell> root = c;
        while (root->parent)
            root = root->parent;
        
        while (c != root)
        {
            std::shared_ptr<Cell> p = c->parent;
            c->parent = root;
            c = p;
        }
        
        return root;
    }
    
    std::map<Key, std::shared_ptr<Cell>> cells;
};

#endif // UNION_FIND_H

