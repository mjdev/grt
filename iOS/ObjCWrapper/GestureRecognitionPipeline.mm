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

class NSLogStream: public std::streambuf
{
public:
    NSLogStream(std::ostream& stream) :
    orgStream(stream)
    {
        // Swap the the old buffer in ostream with this buffer.
        orgBuf = orgStream.rdbuf(this);
    }
    
    ~NSLogStream(){
        orgStream.rdbuf(orgBuf); // Restore old buffer
    }
    
protected:
    int_type overflow(int_type c)
    {
        if(!traits_type::eq_int_type(c, traits_type::eof()))
        {
            char_type const t = traits_type::to_char_type(c);
            this->xsputn(&t, 1);
        }
        return !traits_type::eof();
    }
    
    int sync()
    {
        return 0;
    }
    
    virtual streamsize xsputn(const char *msg, streamsize count){
        std::string s(msg,count);
        NSLog(@"GRT cout: %@", @(s.c_str()));
        return count;
    }
    
private:
    std::streambuf *orgBuf;
    std::ostream& orgStream;
};

@interface GestureRecognitionPipeline()
@property GRT::GestureRecognitionPipeline *instance;
@property NSLogStream *nsLogStream;
@end

@implementation GestureRecognitionPipeline

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.instance = new GRT::GestureRecognitionPipeline;
        
        // Redirect cout to NSLog
        self.nsLogStream = new NSLogStream(std::cout);
    }
    return self;
}

- (void)dealloc
{
    delete self.instance;
    delete self.nsLogStream;
}

- (BOOL)load:(NSString *) path
{
    BOOL result = self.instance->load(std::string(path.UTF8String));
    
    if (result) {
        cout << "GRT config";
        cout << self.instance->getModelAsString();
        cout << "GRT info: " << self.instance->getInfo();
    }
    
    return result;
}

- (NSUInteger)predictedClassLabel
{
    return self.instance->getPredictedClassLabel();
}

- (BOOL)predict:(VectorDouble *) inputVector
{
    return self.instance->predict(*[inputVector cppInstance]);
}

@end
