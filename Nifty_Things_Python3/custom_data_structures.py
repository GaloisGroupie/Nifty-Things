""" Updatable priority queue that does insertions and updates in log(n) time.
    The underlying data structure for the queue is an array which we create a 
    binary heap out of. A dictionary is also used for constant time look up
    for the updates. The dictionary key is an element we put inside the 
    priority queue and the value is its current index in the list.

    **** Important ****
    The class only works on things which have a properly defined __cmp__()
    method as it uses it to decide the heap structure. Each time an element
    is updated, the priority queue MUST be told that they element was updated
    using the update_key() method. If this is not done, the properties heap
    structure may not be held and the priority queue will be incorrect """

class updatable_priority_queue():

    # Create a list to serve as our heap and a dictionary
    # for fast look up time. The dictionary has the item we 
    # wish to update as its key and its index as the value
    def __init__(self):
        self._heap = list()
        self._key_index_dict = dict()

    def size(self):
        return len(self._heap)

    #Put item into queue
    def put(self,key):
        if key in self._key_index_dict:
            return

        self._heap.append(key)
        self._key_index_dict[key] = (len(self._heap)-1)

        self._bubble_up(key)
    
    # Swaps two elements inside the queue. Order and property of the
    # queue may not be maintained. Only other update we make is to 
    # update the index value we have of them in our dictionary
    def _swap_elements(self,elmt1,elmt2):
        # Get indices of elements
        elmt1_index = self._key_index_dict[elmt1]
        elmt2_index = self._key_index_dict[elmt2]

        # Swap elements
        temp_ref = elmt1
        self._heap[elmt1_index] = elmt2
        self._heap[elmt2_index] = elmt1

        # Update index dictionary
        self._key_index_dict[elmt1] = elmt2_index
        self._key_index_dict[elmt2] = elmt1_index

    # Swaps the position of an element and its parent if the element is
    # 'lighter' than its parent. We do this recursively as many times as
    # we need until the parent is no longer heavier than the element
    def _bubble_up(self,key):
        if not key in self._key_index_dict:
            return

        key_index = self._key_index_dict[key]

        # Our key is now the root. Can't bubble up anymore. Make sure properly
        # preserved so see if we need to trickle down
        if key_index == 0:
            return    
 
        parent_index = (key_index-1)//2
        parent = self._heap[parent_index]

        if key.__cmp__(parent) < 0:
        
            self._swap_elements(parent,key)
       
            # Bubble up as far as possible through recursive calls 
            self._bubble_up(key)
         
    # Swaps position of element and its parent if element is 
    # 'heavier' than its parent. We do this recursively as many times as
    # we need until the parent is no longer heavier than the element
    def _trickle_down(self,key):
        if not key in self._key_index_dict:
            return

        key_index = self._key_index_dict[key]

        # Our key can't trickle down anymore
        if ((2*key_index)+1) >= len(self._heap):
            return

        # Decide which child is 'lighter' since we want to maintain
        # heap order and to do this, the lighter one must be the one we swap with
        child1_index = (2*key_index)+1
        child2_index = (2*key_index)+2

        # If there's only the left child, then we just have to compare with that.
        # Otherwise, we proceed in deciding who is the 'lighter' child
        if child2_index == len(self._heap):
            swap_child = self._heap[child1_index]
        else:
            child1 = self._heap[child1_index]
            child2 = self._heap[child2_index]
            
            if child1.__cmp__(child2) < 0:
                swap_child = child1
            else:
                swap_child = child2

        # If the child is lighter than the parent(key), then
        # we swap the two elements
        if swap_child.__cmp__(key) < 0:
            self._swap_elements(key,swap_child)
            self._trickle_down(key)

    # We lazily check update the position of the element. It's okay.
    # it's only 2 * log(n)
    def update_key(self,key):
        self._trickle_down(key)
        self._bubble_up(key)

    def is_empty(self):
        return len(self._heap) == 0

    # Returns current minimum element in the heap while preserving heap
    # properties
    def pop(self):
        if self.is_empty():
            return None
            print(self.size())
        else:
            min_elmt = self._heap[0]
            farthest_elmt = self._heap[-1]

            self._swap_elements(min_elmt,farthest_elmt)

            # Delete element from dictionary and list
            del self._heap[-1]
            del self._key_index_dict[min_elmt]

            # Fix heap structure
            self._trickle_down(farthest_elmt)
            
            return min_elmt

    # Swap with the 'last' element so that we maintain our heap property of 
    # having our binary tree be complete
    def delete_key(key):
        if not key in self._key_index_dict:
            return

        last_element = self._heap[-1]
        self._swap_elements(key,last_element)

        self.update_key(key)

        del self._heap[-1]
        del self._key_index_dict[last_element]
