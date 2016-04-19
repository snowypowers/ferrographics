#ifndef HASHNODE_H
#define HASHNODE_H

class HashNode {
public:
    HashNode( int key,  int value) :
    key(key), value(value), next(NULL) {
    };

    int getKey() const {
        return key;
    };

    int getValue() const {
        return value;
    };

    void setValue(int value) {
        HashNode::value = value;
    };

    HashNode *getNext() const {
        return next;
    };

	HashNode* getLast() {
		HashNode* node = this;
		while (node->next != NULL) {
			node = node->next;
		}
		return node;
	};

    void setNext(HashNode *next) {
        HashNode::next = next;
    };

private:
    // key-value pair
    int key;
    int value;
    // next bucket with the same key
    HashNode *next;
};

#endif