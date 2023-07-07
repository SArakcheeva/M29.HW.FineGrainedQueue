#include "FineGrainedQueue.h"


void FineGrainedQueue::insertIntoMiddle(int value, int pos)
{
    // создаем новый узел 
    Node* newNode = new Node{ value, nullptr, new std::mutex };
    newNode->node_mutex->lock();

    int currPos = 0;

    queue_mutex->lock();     // теперь наш head нельзя будет поменять

    Node* current = this->head;  // Указатель на текущий элемент
    current->node_mutex->lock(); // Блокируем текущий элемент

    // разлочиваем общий для структуры мьютекс (Сейчас head нельзя поменять, потому что мы залочили конкретно его на себя)
    queue_mutex->unlock();

    //проверка, не является ли последним элементом
    if (current->next == nullptr) {
        current->next = newNode;
    }
    else {
        // в цикле идем по списку, пока список не кончится, или пока не дойдем до позиции
        while (currPos < pos && current->next != nullptr)
        {
            Node* next = current->next;
            current->node_mutex->unlock();  // Разблокируем текущий элемент

            current = next;
            if (current) { // проверили и только потом залочили
                current->node_mutex->lock();
            }

            currPos++;
        }

        // меняем указатель на следующий узел на указатель на новый узел
        Node* temp = current->next;
        current->next = newNode;

        // связываем список обратно, меняем указатель на узел, следующий после нового узла,
        // на указатель на узел, следующий за current
        newNode->next = temp;
    }

    // Разблокируем текущий элемент и новый узел
    current->node_mutex->unlock();
    newNode->node_mutex->unlock();
}