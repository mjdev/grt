//
//  VectorDouble.h
//  grt
//
//  Created by M J on 09/12/15.
//  Copyright © 2015 jahnen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface VectorDouble : NSObject

- (void)pushBack:(double)value;

#ifdef __cplusplus
- (GRT::VectorDouble)cppInstance;
#endif

@end
