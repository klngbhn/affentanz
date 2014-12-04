#pragma once

#import <UIKit/UIKit.h>

class iOSInputSystem;

@interface InputGestureView : UIView
{
    iOSInputSystem* inputSystem;
}
@property (assign) iOSInputSystem* inputSystem;

@end

