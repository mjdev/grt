//
//  GestureRecognitionPipeline.m
//  grt
//
//  Created by mjahnen on 25/11/15.
//  Copyright © 2015 jahnen. All rights reserved.
//

#ifdef __cplusplus
#include "grt.h"
#endif

#import "GestureRecognitionPipeline.h"

@interface GestureRecognitionPipeline()
@property GRT::GestureRecognitionPipeline *instance;
@end

@implementation GestureRecognitionPipeline

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.instance = new GRT::GestureRecognitionPipeline;
    }
    return self;
}

- (void)dealloc
{
    delete self.instance;
}

- (BOOL)load:(NSString *) path
{
    return self.instance->load(std::string(path.UTF8String));
}

@end
