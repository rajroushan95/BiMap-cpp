#ifndef _CUSTOM_BIMAP_H_
#define _CUSTOM_BIMAP_H_

#include <set>
#include <stdexcept>


template <typename K, typename V>
class BiMap
{
  private:
    class Key;
    class Value
    {
      public:
        V item;
        Key<K,V> *key;
        Value(V item){
            this->item = item;
        }
    };

    class Key
    {
      public:
        K item;
        Value<K,V> *value;
        Key(K item){
            this->item = item;
        }
    };

    template <typename C>
    class comparision
    {
      public:
        bool operator()(const C *lhs, const C *rhs)
        {
            return lhs->item < rhs->item;
        }
    };

    std::set<Key<K,V> *, comparision<Key<K,V>>> keys; // check for set of objects in cpp. how to keep unique
    std::set<Value<K,V> *, comparision<Value<K,V>>> values;

  public:
    void put(K key, V value)
    {
        Key<K,V> *k = new Key<K,V>(key);
        Value<K,V> *v = new Value<K,V>(value);
        if(keys.find(k) != keys.end()){
            delete k;
            delete v;
            return;
        }
        if(values.find(v) != values.end()){
            delete k;
            delete v;
            return;
        }
        k->value = v;
        v->key = k;
        keys.insert(k);
        values.insert(v);
    }

    V getValueForKey(K key)
    {
        Key<K,V> *k = new Key<K,V>(key);
        auto it = keys.find(k);
        if(it == keys.end()){
            delete k;
            throw std::invalid_argument("Key not present in map");
        }
        delete k;
        return (*it)->value->item;
    }

    K getKeyForValue(V value)
    {
        Value<K,V> *v = new Value<K,V>(value);
        auto it = values.find(v);
        if(it == values.end()){
            delete v;
            throw std::invalid_argument("Key not present in map");
        }
        delete v;
        return (*it)->key->item;
    }

    class iterator
    {
        private:
        typename std::set<Key<K,V>*>::iterator it;
        public:
        iterator(std::set<Key<K,V>*>::iterator it){
            this->it = it;
        }

        K first(){
            return (*it)->item;
        }

        V second(){
            return (*it)->value->item;
        }

        bool operator!= (const iterator& eit){
            return this->it != eit.it;
        }

        iterator operator++(){
            iterator old = *this;
            this->it++;
            return old;
        }

        iterator operator++(int){
            this->it++;
            return *this;
        }
    };

    iterator begin(){
        return iterator(keys.begin());
    }

    iterator end(){
        return iterator(keys.end());
    }
};


#endif