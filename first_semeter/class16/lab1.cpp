/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
// class LinkedList {
// private:
//     ListNode* head;
// public:
//     LinkedList() : head(nullptr) {}
//     ListNode* createLinkedList(const std::string& input);
//     ListNode* swapNodes(ListNode* head, int k);
//     void printLinkedList(ListNode* head);
//     ~LinkedList();
// };
// LinkedList::~LinkedList() {
//    ListNode* current = head;
//    while (current != nullptr) {
//        ListNode* next = current->next;
//        delete current;
//        current = next;
//    }
// }
ListNode* LinkedList::createLinkedList(const std::string& input) {
	std::istringstream iss(input);
  	int val;
    ListNode* head = nullptr;
    ListNode* current = nullptr;

while (iss >> val) {
    ListNode* newNode = new ListNode(val);
    if (!head) {
        head = newNode; // 头节点
        current = head;
    } else {
        current->next = newNode; // 链接到当前链表
        current = current->next;
    }
}
return head;

}

ListNode* LinkedList::swapNodes(ListNode* head, int k) {
 if (!head) return head;

    ListNode* first = head;
    ListNode* second = head;
    ListNode* temp = head;
    int len = 0;
    // 计算链表的长度
    while (temp != nullptr) {
        len++;
        temp = temp->next;
    }

    if (k > len) return head;

    for (int i = 1; i < k; i++) {
        first = first->next;
    }

    for (int i = 1; i < len - k + 1; i++) {
        second = second->next;
    }

    int tempValue = first->val;
    first->val = second->val;
    second->val = tempValue;

    return head;
}


void LinkedList::printLinkedList(ListNode* head) {
 	ListNode* current = head;
    while (current != nullptr) {
        std::cout << current->val;
        if (current->next != nullptr) std::cout << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

int main() {
  LinkedList list;
    int k;
    std::string first_line;
    std::getline(std::cin, first_line);

    ListNode* head = list.createLinkedList(first_line); 
    std::cin >> k;
    ListNode* newhead = list.swapNodes(head, k); 

    list.printLinkedList(newhead);

    return 0;
}

