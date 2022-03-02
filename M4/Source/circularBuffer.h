/* Ryan Colon
 * 02.15.22
 * Circular Buffer library for use in embedded systems
 * Modeled after standard c circular buffer library Embedded Artistry article
 */ 

#ifndef CIRCULARBUFFER_H
    #define CIRCULARBUFFER_H

    #include <assert.h>
    #include <stddef.h>

    //This struct is the actual circular buffer
    typedef struct st_circBuff{
        char* pc_buffer;
	    size_t u_head;
	    size_t u_tail;
	    size_t u_max;
	    bool b_full;
    }st_circBuff;

    //"Empties" the buffer
    void fn_circBuffReset(st_circBuff* pst_me){
        assert(pst_me);

	    pst_me->u_head = 0;
	    pst_me->u_tail = 0;
	    pst_me->b_full = false;
    }
    
    //Checks if the buffer is empty
    bool fn_circBuffEmpty(st_circBuff* pst_me){
        assert(pst_me);

	    return (!pst_me->b_full && (pst_me->u_head == pst_me->u_tail));
    }

    //Initialize the buffer struct, user must provide the structure and buffer to maintain static allocation
    void fn_circBuffInit(st_circBuff* pst_me, char* pc_buff, size_t u_size){
        assert(pst_me && pc_buff && u_size);
    
        pst_me->pc_buffer = pc_buff;
        pst_me->u_max = u_size;
        fn_circBuffReset(pst_me);

        assert(fn_circBuffEmpty(pst_me));
    }

    //Checks if the buffer is full
    bool fn_circBuffFull(st_circBuff* pst_me){
        assert(pst_me);

	return pst_me->b_full;
    }

    //Returns the maximum capacity of the buffer
    size_t fn_circBuffCapacity(st_circBuff* pst_me){
        assert(pst_me);

	return pst_me->u_max;
    }

    //Returns the current amount of data in the buffer in (number of data types) NOT BITS/BYTES
    size_t fn_circBuffSize(st_circBuff* pst_me){
        assert(pst_me);

	size_t u_size = pst_me->u_max;

	if(!pst_me->b_full){
		if(pst_me->u_head >= pst_me->u_tail)
			u_size = pst_me->u_head - pst_me->u_tail;
		else
			u_size = pst_me->u_max + pst_me->u_head - pst_me->u_tail;
	}
	
	return u_size;
    }

    //Called by Put, moves the head and tail appropriately
    static void fn_advancePointer(st_circBuff* pst_me){
        assert(pst_me);

	if(pst_me->b_full){
            if(++(pst_me->u_tail) == pst_me->u_max)
	        pst_me->u_tail = 0;
	}

	if(++(pst_me->u_head) == pst_me->u_max)
	    pst_me->u_head = 0;

	pst_me->b_full = (pst_me->u_head == pst_me->u_tail);
    }

    //Called by Get, moves the tail appropriately
    static void fn_retreatPointer(st_circBuff* pst_me){
        assert(pst_me);

	pst_me->b_full = false;

	if(++(pst_me->u_tail) == pst_me->u_max)
		pst_me->u_tail = 0;

    }

    //Puts data in the buffer
    void fn_circBuffPut(st_circBuff* pst_me, char c_data){
        assert(pst_me && pst_me->pc_buffer);

	pst_me->pc_buffer[pst_me->u_head] = c_data;

	fn_advancePointer(pst_me);
    }

    //Removes data from the buffer, returns pass/fail (need to supply storage for data in parameters)
    bool fn_circBuffGet(st_circBuff* pst_me, char* pc_data){
        //assert(pst_me && pc_data && pst_me->pc_buffer);
        assert(pst_me);
        assert(pc_data);
        assert(pst_me->pc_buffer);

	bool r = false;

	if(!fn_circBuffEmpty(pst_me)){
        *pc_data = pst_me->pc_buffer[pst_me->u_tail];
	    fn_retreatPointer(pst_me);

	   r = true; 
	}

        return r;
    }

    bool fn_circBuffGetMostRecent(st_circBuff* pst_me, char* pc_data){
        assert(pst_me && pc_data && pst_me->pc_buffer);

        bool r = false;

        if(!fn_circBuffEmpty(pst_me)){
                if(pst_me->u_head == 0){
                    pst_me->u_head = (pst_me->u_max) - 1;
                *pc_data = pst_me->pc_buffer[pst_me->u_head];
                r = true;	
            }   
            else{
                *pc_data = pst_me->pc_buffer[--(pst_me->u_head)];
            r = true;
            }
        }

        return r;
    }
#endif
