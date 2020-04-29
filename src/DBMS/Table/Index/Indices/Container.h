#ifndef DATABASE_CONTAINER_H
#define DATABASE_CONTAINER_H

namespace idx {
template <typename T>
concept Streamable = requires (std::ostream & os, T const & t) { os << t; };

template <Streamable U, typename V>
struct Container {
private:
    std::size_t small_kids = 0;
    std::size_t big_kids = 0;

public:
    Container <U, V> * parent;
    Container <U, V> * smaller = nullptr;
    Container <U, V> * bigger  = nullptr;

    bool isSmaller;

    U const key;
    V val;

    Container (U key, V val, Container * parent = nullptr) :
            key {std::move (key)},
            val {std::move (val)},
            parent {parent}
    {
        if (parent) isSmaller = key < parent->key;
    }
    ~Container() {
        delete smaller; smaller = nullptr;
        delete bigger;  bigger  = nullptr;
    }

    void destroy() {
        parent = smaller = bigger = nullptr;
        delete this;
    }

    static bool append (U key, V val, Container <U, V> * * node) {
        Container <U, V> * deref = * node;
        if (!deref) return * node = new Container <U, V> (std::move (key), std::move (val));
        if (key == deref->key) {
            LOG (WARNING) << "Key " << key << " already exists - did you want to insert only?";
            return false;
        }
        bool isSmaller = key < deref->key;
        bool success = append (key, val, isSmaller ? & deref->smaller : & deref->bigger);
        (isSmaller ? deref->small_kids : deref->big_kids) += success;
        return success;
    }

    static bool remove (U key, Container <U, V> * * node) {
        Container <U, V> * deref = * node;
        if (!deref) {
            LOG (WARNING) << "The element you want to remove does not exist (" << key << ")!";
            return false;
        }
        if (key == deref->key) {
            * node = (deref->bigger ?: deref->smaller);
            if (!deref->smaller || !deref->bigger) return true;
            Container <U, V> * trav = deref->bigger;
            while (trav->smaller) trav = trav->smaller;
            trav->smaller = deref->smaller;
            deref->destroy();
            return true;
        }
        bool isSmaller = key < deref->key;
        bool success = remove (key, isSmaller ? & deref->smaller : & deref->bigger);
        (isSmaller ? deref->small_kids : deref->big_kids) -= success;
        return success;
    }

    static Container <U, V> * select (U const & key, Container <U, V> * const * node) {
        Container <U, V> * deref = * node;
        if (!deref) return nullptr;
        if (key == deref->key) return deref;
        return select (key, (key < deref->key) ? & deref->smaller : & deref->bigger);
    }

    static std::stringstream & dump (std::stringstream & ss, Container <U, V> const * node, void (print) (std::stringstream & ss, U const & key, V const & val)) {
        if (!node) return ss;
        if (node->smaller) dump (ss, node->smaller, print);
        print (ss, node->key, node->val);
        if (node->bigger) dump (ss, node->bigger, print);
        return ss;
    }

    std::ostream & operator << (std::ostream & os) const {
        return os << str();
    }
    std::string str () const {
        std::stringstream ss;
        if (smaller) ss << smaller->str();
        ss << '\t' << key;
        if (bigger) ss << bigger->str();
        return ss.str();
    }
};
};

#endif //DATABASE_CONTAINER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */