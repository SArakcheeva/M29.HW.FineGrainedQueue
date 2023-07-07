#include "FineGrainedQueue.h"


void FineGrainedQueue::insertIntoMiddle(int value, int pos)
{
    // ������� ����� ���� 
    Node* newNode = new Node{ value, nullptr, new std::mutex };
    int currPos = 0;

    queue_mutex->lock();     // ������ ��� head ������ ����� ��������

    Node* current = this->head;  // ��������� �� ������� �������
    current->node_mutex->lock(); // ��������� ������� �������

    // ����������� ����� ��� ��������� ������� (������ head ������ ��������, ������ ��� �� �������� ��������� ��� �� ����)
    queue_mutex->unlock();


    // � ����� ���� �� ������, ���� ������ �� ��������, ��� ���� �� ������ �� �������
    while (currPos < pos && current->next != nullptr)
    {
        Node* next = current->next;
        current->node_mutex->unlock();  // ������������ ������� �������
        
        current = next;    
        if (current) { // ��������� � ������ ����� ��������
            current->node_mutex->lock();
        }

        currPos++;
    }


    // ������ ��������� �� ��������� ���� �� ��������� �� ����� ����
    if (current->next != nullptr) {
        Node* temp = current->next;
        current->next = newNode;

        // ��������� ������ �������, ������ ��������� �� ����, ��������� ����� ������ ����,
        // �� ��������� �� ����, ��������� �� current
        newNode->next = temp;
    }
    else { //��������, �� �������� �� ��������� ���������
        current->next = newNode;
    }


    // ������������ ������� ������� � ����� ����
    current->node_mutex->unlock();
}