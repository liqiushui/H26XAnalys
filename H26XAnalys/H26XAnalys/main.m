//
//  main.m
//  H26XAnalys
//
//  Created by lunli on 2017/12/7.
//  Copyright © 2017年 lunli. All rights reserved.
//

#include "H26XParse.h"
#include "H26XLog.h"
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    
    for (int i = 0; i < argc; i++) {
        eprintf("%s", argv[i]);
    }

    if(!(argc != 2 || argc != 4))
    {
        eprintf("Usage : H26XAnalys [h265 file path] [-l] [limitcount]");
        return -1;
    }
    else
    {
//        h26X_Object *obj = h26X_object_from_file("/Users/lunli/Downloads/log 19/dump_1512554836.h265");
        
        int limit = INTMAX_MAX;
        
        if(argc == 4)
        {
            limit = atoi(argv[3]);
        }
        
        h26X_Object *obj = h26X_object_from_file((char *)argv[1]);
        
        h26X_nal_unit_t *nu = NULL;
        do {
            
            h26X_nal_unit_free(nu);
            
            nu = h26X_read_nal_unit(obj);
            
            printf("nalu[%d] ", obj->nalu_index);
            dump_h26X_nal_unit(nu);
            
        } while (nu && obj->nalu_index < limit);
        
        h26X_object_free(obj);
    }
    
    

    

    return 0;
}
