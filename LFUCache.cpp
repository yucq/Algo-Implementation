class LFUCache {
private:
    struct CacheNode{
        int key;
        int val;
        int freq;
        CacheNode(int x, int y) : key(x), val(y), freq(0) {}
    };
    
    list<list<CacheNode>> data_{};
    unordered_map<int, list<CacheNode>::iterator> n_map{};
    unordered_map<int, list<list<CacheNode>>::iterator> l_map{};
    int capacity_;
    int size_;
    
    void helper(int key){
        auto it = n_map[key];
        int freq = it->freq;
        ++it->freq;
        auto l_it = l_map[freq];
        if(next(l_it)==data_.end() || next(l_it)->begin()->freq != freq+1){
            data_.insert(next(l_it), list<CacheNode>());
            l_map[freq+1]=next(l_it);
        }
        next(l_it)->splice(next(l_it)->begin(), *l_it, it);
        n_map[key] = next(l_it)->begin();
        if(l_it->size()==0){
            l_map.erase(freq);
            data_.erase(l_it);
        }
    }
    
public:
    LFUCache(int capacity) : capacity_(capacity), size_(0) {
        
    }
    
    int get(int key) {
        if(n_map.find(key)==n_map.end())
            return -1;
        helper(key);
        return n_map[key]->val;
    }
    
    void put(int key, int value) {
        if(capacity_==0)
            return;
        if(n_map.find(key)!=n_map.end()){
            helper(key);
            n_map[key]->val = value;
            return;
        }
        if(size_ == capacity_){
            auto to_delete = data_.begin()->back();
            int k = to_delete.key, f = to_delete.freq;
            n_map.erase(k);
            data_.begin()->pop_back();
            if(data_.begin()->size() == 0){
                l_map.erase(f);
                data_.pop_front();
            }
        }
        if(data_.size() == 0 || data_.begin()->front().freq != 0){
            data_.push_front(list<CacheNode>());
            l_map[0] = data_.begin();
        }
        data_.begin()->push_front(CacheNode(key, value));
        n_map[key] = data_.begin()->begin();
        if(size_ < capacity_)
            ++size_;
    }
};
