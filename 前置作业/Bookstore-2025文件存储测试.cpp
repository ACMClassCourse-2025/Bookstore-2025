//
// Created by HTY on 2025/12/5.
//
#include<iostream>
#include<fstream>
#include<string>
#include<array>
#include<algorithm>
#include<cstring>
#include<map>
#include<vector>
template <class T1,class T2,int block_size = 1>
class Database {
private:
    std::fstream file;
    std::fstream index_file;
    //std::map<T1,std::vector<T2>> buffer;
    int node_number = 0;
    struct Content {
        T1 index;
        T2 value;
    };
    struct Node {
        T1 max_key,min_key;
        int body_size;
        long long next_node = -1;
        long long content_pos;
    };
    static bool cmp (const Content &c1,const Content &c2) {
        if (c1.index < c2.index) return true;
        else if (c1.index > c2.index) return false;
        else {
            return c1.value < c2.value;// 升序排序
        }
    }
public:
    void initialize() {
        index_file.open("index.txt",std::ios::in | std::ios::binary);
        if (!index_file.good()) index_file.open("index.txt",std::ios::out);
        else {
            index_file.seekg(0,std::ios::end);
            node_number = index_file.tellg() / sizeof(Node);
        }
        file.open("map.txt",std::ios::in | std::ios::binary);
        if (!file.good()) file.open("map.txt",std::ios::out);
        index_file.close();
        file.close();
    }
    void Insert(T1 index,T2 value) {
        index_file.open("index.txt",std::ios::binary | std::ios::in | std::ios::out);
        file.open("map.txt",std::ios::binary | std::ios::in | std::ios::out);
        index_file.seekg(0,std::ios::beg);
        Content cur;
        cur.index = index;
        cur.value = value;
        Node tmp,next_tmp;
        bool flag = false;
        //std::cerr << node_number << "\n";
        for (int i = 0; i < node_number; i++){
            index_file.read(reinterpret_cast<char *>(&tmp),sizeof(Node));
            if (tmp.next_node != -1) {
                long long cur_pos = index_file.tellg();
                index_file.seekg(tmp.next_node);
                index_file.read(reinterpret_cast<char *>(&next_tmp),sizeof(Node));
                index_file.seekg(cur_pos);
            }
            // 考虑一个index被裂到不同块中的情况
            if (tmp.body_size >= 1 && cur.index <= tmp.max_key) {
                file.seekg(tmp.content_pos);
                Content prev[block_size + 1];
                file.read(reinterpret_cast<char *>(prev),sizeof(Content) * tmp.body_size);
                file.seekp(tmp.content_pos);
                // 相同的键值对不重复插入
                bool repeat = false;
                for (int j = 0; j < tmp.body_size; j++) {
                    if (prev[j].index == cur.index && prev[j].value == cur.value) {
                        repeat = true;
                        break;
                    }
                }
                if (repeat) {
                    flag = true;
                    break;
                }

                if (tmp.next_node != -1 && cur.index == next_tmp.min_key && cur.value > prev[tmp.body_size - 1].value) {
                    index_file.seekg(tmp.next_node);
                    continue;
                }

                if (cur.index < tmp.min_key) tmp.min_key = cur.index;
                //std::cerr << cur.value << " " << tmp.body_size << "\n";
                /*prev[tmp.body_size] = cur;
                std::sort(prev,prev + tmp.body_size + 1,cmp);*/
                int id = 0;
                if (cmp(cur,prev[0])) id = -1;
                else {
                    for (; id < tmp.body_size - 1; id++) {
                        if (cmp(prev[id],cur) && cmp(cur,prev[id + 1])) {
                        break;
                        }
                    }
                }

                for (int k = tmp.body_size - 1; k >= id + 1; k--) {
                    prev[k + 1] = prev[k];
                }
                prev[id + 1] = cur;
                tmp.body_size++;
                //std::cerr << tmp.body_size << "\n";
                file.write(reinterpret_cast<char *>(prev),sizeof(Content) * tmp.body_size);

                index_file.seekp(-sizeof(Node),std::ios::cur);
                index_file.write(reinterpret_cast<char *>(&tmp),sizeof(Node));
                if (tmp.body_size == block_size) {
                    index_file.seekp(-sizeof(Node),std::ios::cur);
                    SplitBlock(tmp,index_file.tellp());
                }
                flag = true;
                break;
            }
            if (tmp.next_node == -1) break;
            index_file.seekg(tmp.next_node,std::ios::beg);
        }

        if (!flag) {
            if (node_number > 0) {
                long long cur_pos = index_file.tellg();
                index_file.seekp(0,std::ios::end);
                tmp.next_node = index_file.tellp();
                index_file.seekp(cur_pos - sizeof(Node));
                index_file.write(reinterpret_cast<char *>(&tmp),sizeof(Node));
            }
            node_number++;
            tmp.max_key = tmp.min_key = index;
            tmp.body_size = 1;
            tmp.next_node = -1;
            index_file.seekp(0,std::ios::end);
            tmp.content_pos = index_file.tellp() / sizeof(Node) * sizeof(Content) * block_size;
            //std::cerr << tmp.content_pos << "\n";
            index_file.write(reinterpret_cast<char *>(&tmp),sizeof(Node));
            //file.open("map.txt",std::ios::binary | std::ios::out | std::ios::in);
            file.seekp(tmp.content_pos);
            file.write(reinterpret_cast<char *>(&cur),sizeof(Content));
        }
        index_file.close();
        file.close();
    }
    void Delete(T1 index,T2 value){
        index_file.open("index.txt",std::ios::binary | std::ios::in | std::ios::out);
        file.open("map.txt",std::ios::binary | std::ios::in | std::ios::out);
        Content cur;
        cur.index = index;
        cur.value = value;
        Node tmp,pre_tmp;
        long long pre_pos = 0;
        for (int j = 0; j < node_number; j++) {
            index_file.read(reinterpret_cast<char *>(&tmp),sizeof(Node));
            if (cur.index < tmp.min_key) break;
            if (cur.index >= tmp.min_key && cur.index <= tmp.max_key) {
                //file.open("map.txt",std::ios::binary | std::ios::out | std::ios::in);
                Content prev[block_size + 1];
                file.seekg(tmp.content_pos);
                file.read(reinterpret_cast<char *>(prev),sizeof(Content) * tmp.body_size);
                int i = 0;
                for (; i < tmp.body_size; i++) {
                    //std::cerr << prev[i].value << "\n";
                    if (prev[i].index == cur.index && prev[i].value == cur.value) {
                        break;
                    }
                }

                if (i < tmp.body_size) {
                    for (;i < tmp.body_size; i++) {
                        prev[i] = prev[i + 1];
                    }
                    tmp.body_size--;
                    /*if (tmp.body_size == 0 && pre_pos != 0) {
                        pre_tmp.next_node = tmp.next_node;
                        index_file.seekp(pre_pos - sizeof(Node));
                        index_file.write(reinterpret_cast<char *>(&pre_tmp),sizeof(Node));
                    }*/
                    if (tmp.max_key == cur.index) tmp.max_key = prev[tmp.body_size - 1].index;
                    if (tmp.min_key == cur.index) tmp.min_key = prev[0].index;

                    file.seekp(tmp.content_pos);
                    file.write(reinterpret_cast<char *>(prev),sizeof(Content) * tmp.body_size);
                    index_file.seekp(-sizeof(Node),std::ios::cur);
                    index_file.write(reinterpret_cast<char *>(&tmp),sizeof(Node));
                }
                //file.close();
            }
            if (tmp.next_node == -1) break;
            pre_tmp = tmp;
            pre_pos = index_file.tellg();
            index_file.seekg(tmp.next_node,std::ios::beg);
        }
        index_file.close();
        file.close();
    }
    // 要考虑一个index被拆到不同块的情况
    void Find(T1 index) {
        index_file.open("index.txt",std::ios::binary | std::ios::in);
        file.open("map.txt",std::ios::binary | std::ios::in);
        index_file.seekg(0,std::ios::beg);
        Node tmp;
        bool has_value = false;
        //std::cerr << node_number << std::endl;
        for (int j = 0; j < node_number; j++) {
            index_file.read(reinterpret_cast<char *>(&tmp),sizeof(Node));
            //std::cerr << tmp.content_pos << "\n";
            if (index < tmp.min_key) break;
            if (index >= tmp.min_key && index <= tmp.max_key) {
                //file.open("map.txt",std::ios::binary | std::ios::in);
                file.seekg(tmp.content_pos);
                Content cur[block_size + 1];
                file.read(reinterpret_cast<char *>(cur),sizeof(Content) * tmp.body_size);

                for (int i = 0; i < tmp.body_size; i++) {
                    //std::cerr << cur[i].value << "\n";
                    if (cur[i].index == index) {
                        has_value = true;
                        std::cout << cur[i].value << " ";
                    }
                }
                //file.close();
            }
            if (tmp.next_node == -1) break;
            index_file.seekg(tmp.next_node,std::ios::beg);
        }
        if (!has_value) {
            std::cout << "null";
        }
        std::cout << "\n";
        index_file.close();
        file.close();
    }

    void SplitBlock(Node target,long long cur_pos) {
        Node new_node;
        node_number++;
        //index_file.open("index.txt",std::ios::binary | std::ios::in | std::ios::out);
        index_file.seekg(0,std::ios::end);
        new_node.next_node = target.next_node;
        target.next_node = index_file.tellg();
        new_node.body_size = target.body_size = block_size / 2;

        Content origin_content[block_size + 1],new_content[block_size + 1];
        //file.open("map.txt",std::ios::binary | std::ios::in | std::ios::out);
        file.seekg(target.content_pos);
        file.read(reinterpret_cast<char *>(origin_content),sizeof(Content) * block_size);
        new_node.max_key = target.max_key;
        target.max_key = origin_content[block_size / 2 - 1].index;
        new_node.min_key = origin_content[block_size / 2].index;

        index_file.seekp(0,std::ios::end);
        new_node.content_pos = (index_file.tellp() / sizeof(Node)) * sizeof(Content) * block_size;

        index_file.seekp(0,std::ios::end);
        index_file.write(reinterpret_cast<char *>(&new_node),sizeof(Node));
        index_file.seekp(cur_pos);
        index_file.write(reinterpret_cast<char *>(&target),sizeof(Node));

        for (int i = 0; i < block_size / 2; i++) {
            new_content[i] = origin_content[i + block_size / 2];
            //std::cerr << new_content[i].value << "\n";
        }
        file.seekp(new_node.content_pos);
        file.write(reinterpret_cast<char *>(new_content),sizeof(Content) * block_size);
    }
};
int main() {
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(nullptr);
    int n;
    Database<std::array<char,65>,int,4> db;
    db.initialize();
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string op;
        char name[65];
        std::memset(name,0,65);
        std::cin >> op;
        if (op == "insert") {
            std::array<char,65> index;
            int value;
            std::cin >> name;
            std::cin >> value;
            int j = 0;
            for (; j < 64; j++) index[j] = name[j];
            db.Insert(index,value);
        }
        else if (op == "delete") {
            std::array<char,65> index;
            int value;
            std::cin >> name;
            int j = 0;
            for (; j < 64; j++) index[j] = name[j];
            std::cin >> value;
            db.Delete(index,value);
        }
        else if (op == "find") {
            std::array<char,65> index;
            std::cin >> name;
            int j = 0;
            for (; j < 64; j++) index[j] = name[j];
            db.Find(index);
        }
    }
    return 0;
}