#include <cassert>
#include "address_list.h"

void AdressList::AddAddress(void* address) {
    // We are using the data we want to store as the memory
    // we will use to construct the node
    assert(address != nullptr);
    AdressListNode* newNode = new (address) AdressListNode();
    newNode->nextNode = m_HeadNode;
    
    m_HeadNode = newNode;
    
    m_ListSize += 1;
}

void* AdressList::GetAddress() {
    if (m_HeadNode == nullptr) {
        return nullptr;
    } else {
        void* tmp = m_HeadNode;
        m_HeadNode = reinterpret_cast<AdressListNode*>(m_HeadNode->nextNode);
        
        m_ListSize -= 1;

        assert(m_ListSize > 0 || m_HeadNode == nullptr);
        return tmp;
    }
}

size_t AdressList::GetListSize() const { return m_ListSize; }

void AdressList::Clear() {
    m_ListSize = 0;
    m_HeadNode = nullptr;
}

