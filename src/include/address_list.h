#pragma once

class AdressList {
   private:
    struct AdressListNode {
        void* nextNode = nullptr;
    };

   public:
    void AddAddress(void* address);

    void* GetAddress();

    size_t GetListSize() const;

    void Clear();

    size_t m_ListSize = 0;
    AdressListNode* m_HeadNode = nullptr;
};
