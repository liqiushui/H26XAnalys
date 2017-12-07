//
//  H26XParse.c
//  H26XAnalys
//
//  Created by lunli on 2017/12/7.
//  Copyright © 2017年 lunli. All rights reserved.
//

#include "H26XParse.h"
#include "H26XLog.h"
#include <string.h>
#include <stdlib.h>


void h26X_object_free(h26X_Object * obj)
{
    if(obj->file_data)
    {
        free(obj->file_data);
        obj->file_data = NULL;
    }
    
    if(obj->fp)
    {
        fclose(obj->fp);
        obj->fp = NULL;
    }
    
    memset(obj, 0, sizeof(h26X_Object));
}


void h26X_nal_unit_free(h26X_nal_unit_t * nalu)
{
    if(nalu)
    {
        if(nalu->data)
        {
            free(nalu->data);
            nalu->data = NULL;
        }
        
        memset(nalu, 0, sizeof(h26X_nal_unit_t));
    }
}


h26X_Object *h26X_object_from_file(char *path)
{
    if(!path || strlen((const char *)path) == 0)
    {
        eprintf("path is invalid");
        return NULL;
    }
    
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        eprintf("read file : %s failed", path);
        return NULL;
    }
    
    h26X_Object *obj = (h26X_Object *)malloc(sizeof(h26X_Object));
    memset(obj, 0, sizeof(h26X_Object));
    strcpy((char *)obj->path, (char *)path);
    
    fseek(fp , 0 , SEEK_END);
    obj->file_size = (uint32_t)ftell(fp);
    rewind(fp);
    
    obj->file_data = (uint8_t *)malloc(obj->file_size);
    obj->fp = fp;
    
    size_t result = fread(obj->file_data, 1, obj->file_size, fp);
    
    if(!result)
    {
        eprintf("read file : %s failed", path);
        h26X_object_free(obj);
        return NULL;
    }
    
    return obj;
}

bool compareByteArray(void *byteArr1, void *byteArr2, int size)
{
    uint8_t *p1 = (uint8_t *)byteArr1;
    uint8_t *p2 = (uint8_t *)byteArr2;
    for (int i = 0; i < size; i++) {
        if(p1[i] != p2[i])
            return false;
    }
    return true;
}


h26X_nal_unit_t *h26X_read_nal_unit(h26X_Object * obj)
{

    h26X_nal_unit_t *nu = (h26X_nal_unit_t *)malloc(sizeof(h26X_nal_unit_t));
    memset(nu, 0, sizeof(h26X_nal_unit_t));
    unsigned char startCode1[4] = {0x00, 0x00, 0x00, 0x01};
    unsigned char startCode2[3] = {0x00, 0x00, 0x01};
    
    if(compareByteArray(startCode1, &obj->file_data[obj->cur_pos], 4))
    {
        nu->startCodeSize = 4;
        obj->cur_pos += 4;
        nu->startPos = obj->cur_pos;
    }
    else if(compareByteArray(startCode2, &obj->file_data[obj->cur_pos], 3))
    {
        nu->startCodeSize = 3;
        obj->cur_pos += 3;
        nu->startPos = obj->cur_pos;
    }
    else
    {
        eprintf("can't find nalu start code");
        free(nu);
        return NULL;
    }
    
    while (!compareByteArray(&obj->file_data[obj->cur_pos], startCode2, 3) && obj->cur_pos < obj->file_size) {
        obj->cur_pos++;
    }
    
    if(obj->cur_pos < obj->file_size)
    {
        nu->endPos = obj->cur_pos - 1;
        nu->dataSize = nu->endPos - nu->startPos;
        nu->data = malloc(nu->dataSize);
        memcpy(nu->data, &obj->file_data[nu->startPos], nu->dataSize);
        nu->nal_unit_type = nu->data[0];
        obj->nalu_index++;
    }
    else
    {
        h26X_nal_unit_free(nu);
        nu = NULL;
    }

    return nu;
}

void print_hex_memory(void *mem, int size) {
    int i;
    unsigned char *p = (unsigned char *)mem;
    for (i=0;i<size;i++) {
        printf("0x%02x ", p[i]);
        if (((i + 1)%16==0) && i)
            printf("\n");
    }
    printf("\n");
}

void dump_h26X_nal_unit(h26X_nal_unit_t *nu)
{
    if(nu)
    {
        printf("NAL unit hex content(without start code){\n");
        printf("  nal_unit_type: %d\n", nu->nal_unit_type);
        printf("  startCodeSize: %d\n", nu->startCodeSize);
        printf("  startPos: %d\n", nu->startPos);
        printf("  endPos: %d\n", nu->endPos);
        printf("  datasize: %d\n", nu->dataSize);
        print_hex_memory(nu->data, nu->dataSize);
        printf("  }\n\n");
    }
}
