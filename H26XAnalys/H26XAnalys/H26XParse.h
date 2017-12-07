//
//  H26XParse.h
//  H26XAnalys
//
//  Created by lunli on 2017/12/7.
//  Copyright © 2017年 lunli. All rights reserved.
//

#ifndef H26XParse_h
#define H26XParse_h

#include <stdio.h>
#include <stdbool.h>


typedef struct h26X_File_Object {
    unsigned char path[512];
    FILE     *fp;
    uint32_t file_size;
    uint8_t  *file_data;
    uint32_t  cur_pos;
    uint32_t  nalu_index;   //start from 0
} h26X_Object;

typedef struct h26X_nal_unit_t {
    uint8_t     nal_unit_type;
    uint8_t     startCodeSize;
    uint32_t    startPos;
    uint32_t    endPos;
    uint32_t    dataSize;
    uint8_t     *data;
} h26X_nal_unit_t;

#ifdef __cplusplus
extern "C" {
#endif
    
    void h26X_object_free(h26X_Object * obj);
    
    void h26X_nal_unit_free(h26X_nal_unit_t * nalu);

    
    h26X_Object *h26X_object_from_file(char *path);
    
    h26X_nal_unit_t *h26X_read_nal_unit(h26X_Object * obj);
    
    void print_hex_memory(void *mem, int size);
    
    void dump_h26X_nal_unit(h26X_nal_unit_t *nu);

    bool compareByteArray(void *byteArr1, void *byteArr2, int size);

    
#ifdef __cplusplus
}
#endif


#endif /* H26XParse_h */
