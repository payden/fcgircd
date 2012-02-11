/* 
 * File:   fcgircd.h
 * Author: payden
 *
 * Created on February 11, 2012, 5:40 PM
 */

#ifndef FCGIRCD_H
#define	FCGIRCD_H
#include <libmemcached/memcached.h>

#ifdef	__cplusplus
extern "C" {
#endif

    void init_memcached(memcached_st *mem);


#ifdef	__cplusplus
}
#endif

#endif	/* FCGIRCD_H */

